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
#include "ascii_pixel.h"
#include "lfb.h"
#include "sd.h"
#include "print.h"

// the end of bss segment from linker
extern unsigned char _end;

void main()
{
    // set up serial console and linear frame buffer
    uart_init();
    lfb_init();

    // display a pixmap
    //lfb_showpicture();

    int flag = sd_init();
    if(flag == SD_OK){
        /*
        lfb_showchar(0,11,'S');
        lfb_showchar(1,11,'D');
        lfb_showchar(2,11,' ');
        lfb_showchar(3,11,'O');
        lfb_showchar(4,11,'K');

        lfb_shift(4*(ASCII_PIXEL_WIDTH*CHAR_SCALE),0);

        print_dec(1234567890123456789);
        print_dec(1234567890123456789);
        print_dec(1234567890123456789);
        print_dec(1234567890123456789);
        print_dec(1234567890123456789);
        print_dec(12345);
        print_dec(-789101);
        */

        printf("Hello world  ");
        printf("%d %d ",1, 2);
        printf("%s","LOLL");
        printf("Hello world  ");
        

        // read the master boot record after our bss segment
        if(sd_readblock(0,&_end,1)) {
            // dump it to serial console
            //uart_dump(&_end);
        }
    }
    else if(flag == SD_TIMEOUT){
        lfb_showchar(0,11,'S');
        lfb_showchar(1,11,'D');
        lfb_showchar(2,11,' ');
        lfb_showchar(3,11,'T');
        lfb_showchar(4,11,'L');
        lfb_showchar(5,11,'E');
    }
    else{
        lfb_showchar(0,11,'S');
        lfb_showchar(1,11,'D');
        lfb_showchar(2,11,' ');
        lfb_showchar(3,11,'F');
        lfb_showchar(4,11,'A');
        lfb_showchar(5,11,'I');
        lfb_showchar(6,11,'L');
    }

    // echo everything back
    while(1) {
        uart_send(uart_getc());
    }
}
