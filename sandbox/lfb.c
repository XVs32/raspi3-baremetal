/*
 * Copyright (C) 2018 bzt (bztsrc@github)
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */

#include "uart.h"
#include "mbox.h"
#include "homer.h"
#include "delays.h"
#include "ascii_pixel.h"
#include <stdint.h>

#define SCREEN_WIDTH    1920
#define SCREEN_HEIGHT   1080


unsigned int width, height, pitch, isrgb;   /* dimensions and channel order */
unsigned char *lfb;                         /* raw frame buffer address */

/**
 * Set screen resolution to 1920x1080
 */
void lfb_init()
{
    /* newer qemu segfaults if we don't wait here a bit */
    wait_msec(100000);

    mbox[0] = 35*4;
    mbox[1] = MBOX_REQUEST;

    mbox[2] = MBOX_TAG_FRAMEBUFFER_SET_PHYSICAL_WIDTH_HEIGHT;  //set phy wh
    mbox[3] = 8;
    mbox[4] = 0;
    mbox[5] = SCREEN_WIDTH;         //FrameBufferInfo.width
    mbox[6] = SCREEN_HEIGHT;        //FrameBufferInfo.height

    mbox[7] = MBOX_TAG_FRAMEBUFFER_SET_VIRTUAL_WIDTH_HEIGHT;  //set virt wh
    mbox[8] = 8;
    mbox[9] = 0;
    mbox[10] = SCREEN_WIDTH;        //FrameBufferInfo.virtual_width
    mbox[11] = SCREEN_HEIGHT;       //FrameBufferInfo.virtual_height

    mbox[12] = MBOX_TAG_FRAMEBUFFER_GET_VIRTUAL_OFFSET; //set virt offset
    mbox[13] = 8;
    mbox[14] = 0;
    mbox[15] = 0;           //FrameBufferInfo.x_offset
    mbox[16] = 0;           //FrameBufferInfo.y.offset

    mbox[17] = MBOX_TAG_FRAMEBUFFER_SET_DEPTH; //set depth
    mbox[18] = 4;
    mbox[19] = 0;
    mbox[20] = 32;          //FrameBufferInfo.depth

    mbox[21] = MBOX_TAG_FRAMEBUFFER_SET_PIXEL_ORDER; //set pixel order
    mbox[22] = 4;
    mbox[23] = 0;
    mbox[24] = 1;           //RGB, not BGR preferably

    mbox[25] = MBOX_TAG_FRAMEBUFFER_ALLOCATE; //get framebuffer, gets alignment on request
    mbox[26] = 8;
    mbox[27] = 0;
    mbox[28] = SCREEN_WIDTH * 4;        //FrameBufferInfo.pointer
    mbox[29] = 0;           //FrameBufferInfo.size

    mbox[30] = MBOX_TAG_FRAMEBUFFER_GET_PITCH; //get pitch
    mbox[31] = 4;
    mbox[32] = 0;
    mbox[33] = 0;           //FrameBufferInfo.pitch

    mbox[34] = MBOX_TAG_PROPERTY_END;

    //this might not return exactly what we asked for, could be
    //the closest supported resolution instead
    if(mbox_call(MBOX_CH_PROP) && mbox[20]==32 && mbox[28]!=0) {
        mbox[28]&=0x3FFFFFFF;   //convert GPU address to ARM address
        width=mbox[5];          //get actual physical width
        height=mbox[6];         //get actual physical height
        pitch=mbox[33];         //get number of bytes per line
        isrgb=mbox[24];         //get the actual channel order
        lfb=(void*)((unsigned long)mbox[28]);
    } else {
        uart_puts("Unable to set screen resolution to 1024x768x32\n");
    }
    
}

/**
 * Show a picture
 */
void lfb_showpicture()
{
    int x,y;
    unsigned char *ptr=lfb;
    char *data=homer_data, pixel[4];

    for(y=0;y<homer_height;y++) {
        for(x=0;x<homer_width;x++) {
            HEADER_PIXEL(data, pixel);
            // the image is in RGB. So if we have an RGB framebuffer, we can copy the pixels
            // directly, but for BGR we must swap R (pixel[0]) and B (pixel[2]) channels.
            *((unsigned int*)ptr)=isrgb ? *((unsigned int *)&pixel) : (unsigned int)(pixel[0]<<16 | pixel[1]<<8 | pixel[2]);
            ptr+=4;
        }
        ptr+=pitch-homer_width*4;
    }
}

void lfb_showchar(int x, int y, char c){
    unsigned char *ptr=lfb;
    char pixel[4];

    //move to offset
    ptr += x*ASCII_PIXEL_WIDTH*CHAR_SCALE*4;
    ptr += y*ASCII_PIXEL_HEIGHT*CHAR_SCALE*pitch;
    for(y=0;y<ASCII_PIXEL_HEIGHT*CHAR_SCALE;y++) {
        for(x=0;x<ASCII_PIXEL_WIDTH*CHAR_SCALE;x++) {
            FONT(c,x/CHAR_SCALE,y/CHAR_SCALE);
            //FONT(c,x,y);

            // the image is in RGB. So if we have an RGB framebuffer, we can copy the pixels
            // directly, but for BGR we must swap R (pixel[0]) and B (pixel[2]) channels.
            *((unsigned int*)ptr)=isrgb ? *((unsigned int *)&pixel) : (unsigned int)(pixel[0]<<16 | pixel[1]<<8 | pixel[2]);
            ptr+=4;
        }
        ptr+=pitch-ASCII_PIXEL_WIDTH*CHAR_SCALE*4;
    }
}

unsigned int lfb_maxchar(){
    return SCREEN_WIDTH/(ASCII_PIXEL_WIDTH*CHAR_SCALE);
}

unsigned int lfb_maxline(){
    return SCREEN_HEIGHT/(ASCII_PIXEL_HEIGHT*CHAR_SCALE);
}

/**
 * Shift the screen, drop pixels outside of screen
 */
void lfb_shift(int offset_x, int offset_y){

    uint32_t *ptr=lfb;
    int x,y;

    if(offset_x > 0){
        for(y=0;y<SCREEN_HEIGHT;y++) {
            for(x=SCREEN_WIDTH-1;x>=offset_x;x--) {
                ptr[x + y*SCREEN_WIDTH + 0] = ptr[x + y*SCREEN_WIDTH + 0 - offset_x];
            }
            for(;x>=0;x--) {
                ptr[x + y*SCREEN_WIDTH + 0] = 0;
            }
        }
    }
    else if(offset_x < 0){
        for(y=0;y<SCREEN_HEIGHT;y++) {
            for(x=0;x<(SCREEN_WIDTH + offset_x);x++) {
                ptr[x + y*SCREEN_WIDTH + 0] = ptr[x + y*SCREEN_WIDTH + 0 - offset_x];
            }
            for(;x<SCREEN_WIDTH;x++) {
                ptr[x + y*SCREEN_WIDTH + 0] = 0;
            }
        }
    }

    if(offset_y > 0){
        for(y=SCREEN_HEIGHT-1;y>=offset_y;y--) {
            for(x=0;x<SCREEN_WIDTH;x++) {
                ptr[x + y*SCREEN_WIDTH + 0] = ptr[x + (y-offset_y)*SCREEN_WIDTH + 0];
            }
        }
        for(;y>=0;y--) {
            for(x=0;x<SCREEN_WIDTH;x++) {
                ptr[x + y*SCREEN_WIDTH + 0] = 0;
            }
        }
    }
    else if(offset_y < 0){
        for(y=0;y<SCREEN_HEIGHT+offset_y;y++) {
            for(x=0;x<SCREEN_WIDTH;x++) {
                ptr[x + y*SCREEN_WIDTH + 0] = ptr[x + (y-offset_y)*SCREEN_WIDTH + 0];
            }
        }
        for(;y<SCREEN_HEIGHT;y++) {
            for(x=0;x<SCREEN_WIDTH;x++) {
                ptr[x + y*SCREEN_WIDTH + 0] = 0;
            }
        }
    }
    return;
}