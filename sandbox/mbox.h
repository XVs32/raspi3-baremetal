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

/* a properly aligned buffer */
extern volatile unsigned int mbox[36];

#define MBOX_REQUEST    0

/* channels */
#define MBOX_CH_POWER   0
#define MBOX_CH_FB      1
#define MBOX_CH_VUART   2
#define MBOX_CH_VCHIQ   3
#define MBOX_CH_LEDS    4
#define MBOX_CH_BTNS    5
#define MBOX_CH_TOUCH   6
#define MBOX_CH_COUNT   7
#define MBOX_CH_PROP    8

/* tags */
#define MBOX_TAG_PROPERTY_END                             0
#define MBOX_TAG_GET_FIRMWARE_REVISION                    0x00000001
#define MBOX_TAG_SET_CURSOR_INFO                          0x00008010
#define MBOX_TAG_SET_CURSOR_STATE                         0x00008011
#define MBOX_TAG_GET_BOARD_MODEL                          0x00010001
#define MBOX_TAG_GET_BOARD_REVISION                       0x00010002
#define MBOX_TAG_GET_BOARD_MAC_ADDRESS                    0x00010003
#define MBOX_TAG_GET_BOARD_SERIAL                         0x00010004
#define MBOX_TAG_GET_ARM_MEMORY                           0x00010005
#define MBOX_TAG_GET_VC_MEMORY                            0x00010006
#define MBOX_TAG_GET_CLOCKS                               0x00010007
#define MBOX_TAG_GET_POWER_STATE                          0x00020001
#define MBOX_TAG_GET_TIMING                               0x00020002
#define MBOX_TAG_SET_POWER_STATE                          0x00028001
#define MBOX_TAG_GET_CLOCK_STATE                          0x00030001
#define MBOX_TAG_GET_CLOCK_RATE                           0x00030002
#define MBOX_TAG_GET_VOLTAGE                              0x00030003
#define MBOX_TAG_GET_MAX_CLOCK_RATE                       0x00030004
#define MBOX_TAG_GET_MAX_VOLTAGE                          0x00030005
#define MBOX_TAG_GET_TEMPERATURE                          0x00030006
#define MBOX_TAG_GET_MIN_CLOCK_RATE                       0x00030007
#define MBOX_TAG_GET_MIN_VOLTAGE                          0x00030008
#define MBOX_TAG_GET_TURBO                                0x00030009
#define MBOX_TAG_GET_MAX_TEMPERATURE                      0x0003000a
#define MBOX_TAG_GET_STC                                  0x0003000b
#define MBOX_TAG_ALLOCATE_MEMORY                          0x0003000c
#define MBOX_TAG_LOCK_MEMORY                              0x0003000d
#define MBOX_TAG_UNLOCK_MEMORY                            0x0003000e
#define MBOX_TAG_RELEASE_MEMORY                           0x0003000f
#define MBOX_TAG_EXECUTE_CODE                             0x00030010
#define MBOX_TAG_EXECUTE_QPU                              0x00030011
#define MBOX_TAG_SET_ENABLE_QPU                           0x00030012
#define MBOX_TAG_GET_DISPMANX_RESOURCE_MEM_HANDLE         0x00030014
#define MBOX_TAG_GET_EDID_BLOCK                           0x00030020
#define MBOX_TAG_GET_CUSTOMER_OTP                         0x00030021
#define MBOX_TAG_GET_DOMAIN_STATE                         0x00030030
#define MBOX_TAG_GET_THROTTLED                            0x00030046
#define MBOX_TAG_SET_CLOCK_STATE                          0x00038001
#define MBOX_TAG_SET_CLOCK_RATE                           0x00038002
#define MBOX_TAG_SET_VOLTAGE                              0x00038003
#define MBOX_TAG_SET_TURBO                                0x00038009
#define MBOX_TAG_SET_CUSTOMER_OTP                         0x00038021
#define MBOX_TAG_SET_DOMAIN_STATE                         0x00038030
#define MBOX_TAG_GET_GPIO_STATE                           0x00030041
#define MBOX_TAG_SET_GPIO_STATE                           0x00038041
#define MBOX_TAG_SET_SDHOST_CLOCK                         0x00038042
#define MBOX_TAG_GET_GPIO_CONFIG                          0x00030043
#define MBOX_TAG_SET_GPIO_CONFIG                          0x00038043
#define MBOX_TAG_GET_PERIPH_REG                           0x00030045
#define MBOX_TAG_SET_PERIPH_REG                           0x00038045

/* Dispmanx TAGS */
#define MBOX_TAG_FRAMEBUFFER_ALLOCATE                     0x00040001
#define MBOX_TAG_FRAMEBUFFER_BLANK                        0x00040002
#define MBOX_TAG_FRAMEBUFFER_GET_PHYSICAL_WIDTH_HEIGHT    0x00040003
#define MBOX_TAG_FRAMEBUFFER_GET_VIRTUAL_WIDTH_HEIGHT     0x00040004
#define MBOX_TAG_FRAMEBUFFER_GET_DEPTH                    0x00040005
#define MBOX_TAG_FRAMEBUFFER_GET_PIXEL_ORDER              0x00040006
#define MBOX_TAG_FRAMEBUFFER_GET_ALPHA_MODE               0x00040007
#define MBOX_TAG_FRAMEBUFFER_GET_PITCH                    0x00040008
#define MBOX_TAG_FRAMEBUFFER_GET_VIRTUAL_OFFSET           0x00040009
#define MBOX_TAG_FRAMEBUFFER_GET_OVERSCAN                 0x0004000a
#define MBOX_TAG_FRAMEBUFFER_GET_PALETTE                  0x0004000b
#define MBOX_TAG_FRAMEBUFFER_GET_TOUCHBUF                 0x0004000f
#define MBOX_TAG_FRAMEBUFFER_GET_GPIOVIRTBUF              0x00040010
#define MBOX_TAG_FRAMEBUFFER_RELEASE                      0x00048001
#define MBOX_TAG_FRAMEBUFFER_TEST_PHYSICAL_WIDTH_HEIGHT   0x00044003
#define MBOX_TAG_FRAMEBUFFER_TEST_VIRTUAL_WIDTH_HEIGHT    0x00044004
#define MBOX_TAG_FRAMEBUFFER_TEST_DEPTH                   0x00044005
#define MBOX_TAG_FRAMEBUFFER_TEST_PIXEL_ORDER             0x00044006
#define MBOX_TAG_FRAMEBUFFER_TEST_ALPHA_MODE              0x00044007
#define MBOX_TAG_FRAMEBUFFER_TEST_VIRTUAL_OFFSET          0x00044009
#define MBOX_TAG_FRAMEBUFFER_TEST_OVERSCAN                0x0004400a
#define MBOX_TAG_FRAMEBUFFER_TEST_PALETTE                 0x0004400b
#define MBOX_TAG_FRAMEBUFFER_TEST_VSYNC                   0x0004400e
#define MBOX_TAG_FRAMEBUFFER_SET_PHYSICAL_WIDTH_HEIGHT    0x00048003
#define MBOX_TAG_FRAMEBUFFER_SET_VIRTUAL_WIDTH_HEIGHT     0x00048004
#define MBOX_TAG_FRAMEBUFFER_SET_DEPTH                    0x00048005
#define MBOX_TAG_FRAMEBUFFER_SET_PIXEL_ORDER              0x00048006
#define MBOX_TAG_FRAMEBUFFER_SET_ALPHA_MODE               0x00048007
#define MBOX_TAG_FRAMEBUFFER_SET_VIRTUAL_OFFSET           0x00048009
#define MBOX_TAG_FRAMEBUFFER_SET_OVERSCAN                 0x0004800a
#define MBOX_TAG_FRAMEBUFFER_SET_PALETTE                  0x0004800b
#define MBOX_TAG_FRAMEBUFFER_SET_TOUCHBUF                 0x0004801f
#define MBOX_TAG_FRAMEBUFFER_SET_GPIOVIRTBUF              0x00048020
#define MBOX_TAG_FRAMEBUFFER_SET_VSYNC                    0x0004800e
#define MBOX_TAG_FRAMEBUFFER_SET_BACKLIGHT                0x0004800f

#define MBOX_TAG_VCHIQ_INIT                               0x00048010

#define MBOX_TAG_GET_COMMAND_LINE                         0x00050001
#define MBOX_TAG_GET_DMA_CHANNELS                         0x00060001


int mbox_call(unsigned char ch);
