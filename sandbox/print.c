
#include "ascii_pixel.h"
#include "lfb.h"
#include "print.h"


unsigned int cursor_x=0;
unsigned int cursor_y=0;

void next_char_pos(){
    cursor_x++;
    if(cursor_x==lfb_maxchar()){
        cursor_x = 0;
        cursor_y++;
    }
    if(cursor_y==lfb_maxline()){
        cursor_y = lfb_maxline() - 1;
        lfb_shift(0, -ASCII_PIXEL_HEIGHT*CHAR_SCALE);
    }
}

/**
 * print string
 */
uint32_t print_str(char *a){
    uint32_t count = 0;
    uint32_t i;
    for(i=0;a[i]!='\0';i++){
        lfb_showchar(cursor_x, cursor_y, a[i]);
        next_char_pos();
        count++;
    }
    return count;
}

/**
 * print decimal
 */
uint32_t print_dec(int64_t a){
    uint32_t count = 0;
    char a_s[20];
    if(a == 0){
        lfb_showchar(cursor_x, cursor_y, '0');
        next_char_pos();
        count++;
    }
    else{
        if(a<0){
            lfb_showchar(cursor_x, cursor_y, '-');
            next_char_pos();
            count++;
            a *= -1;
        }
        int32_t i;
        for(i=0;a!=0;i++){
            a_s[i] = a%10;
            a/=10;
        }
        for(i--;i>=0;i--){
            lfb_showchar(cursor_x, cursor_y, a_s[i] + '0');
            next_char_pos();
            count++;
        }
    }
    return count;
}


/**
 * minimal printf implementation
 */
unsigned int vprintf(char* fmt, __builtin_va_list args){
    uint32_t count = 0;
    long int arg;
    int len, sign, i;
    char *p, tmpstr[19];

    // failsafes
    if(fmt==(void*)0) {
        return 0;
    }

    // main loop
    arg = 0;
    while(*fmt) {
        // argument access
        if(*fmt=='%') {
            fmt++;
            // literal %
            if(*fmt=='%') {
                goto put;
            }
            len=0;
            // size modifier
            while(*fmt>='0' && *fmt<='9') {
                len *= 10;
                len += *fmt-'0';
                fmt++;
            }
            // skip long modifier
            if(*fmt=='l') {
                fmt++;
            }
            // character
            if(*fmt=='c') {
                arg = __builtin_va_arg(args, int);
                goto put;
                
            } else
            // decimal number
            if(*fmt=='d') {
                arg = __builtin_va_arg(args, int);

                count += print_dec(arg);

                // padding, only space
                if(len>0 && len<18) {
                    while(i>18-len) {
                        count += print_str(" ");
                    }
                }
            } else
            // hex number
            if(*fmt=='x') {

                /* !!!! NOT CONVERTED TO XVs VERSION YET !!!! */
                arg = __builtin_va_arg(args, long int);
                // convert to string
                i=16;
                tmpstr[i]=0;
                do {
                    char n=arg & 0xf;
                    // 0-9 => '0'-'9', 10-15 => 'A'-'F'
                    tmpstr[--i]=n+(n>9?0x37:0x30);
                    arg>>=4;
                } while(arg!=0 && i>0);
                // padding, only leading zeros
                if(len>0 && len<=16) {
                    while(i>16-len) {
                        tmpstr[--i]='0';
                    }
                }
                p=&tmpstr[i];
                if(p==(void*)0) {
                    p="(null)";
                }
            }
            else if(*fmt=='s') {// string
                p = __builtin_va_arg(args, char*);
                count += print_str((char*)p);
            }
        } else {
put:        
            lfb_showchar(cursor_x, cursor_y, *fmt);
            next_char_pos();
            count++;
        }
        fmt++;
    }
    // number of bytes written
    return count;
}

/**
 * minimal printf implementation
 */
unsigned int printf(char* fmt, ...){
    __builtin_va_list args;
    __builtin_va_start(args, fmt);
    return vprintf(fmt,args);
}
