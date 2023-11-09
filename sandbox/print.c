
#include "ascii_pixel.h"
#include "lfb.h"
#include "print.h"


unsigned int cursor_x=0;
unsigned int cursor_y=0;
/**
 * print decimal
 */
void print_dec(int64_t a){
    char a_s[20];
    if(a == 0){
        lfb_showchar(cursor_x, cursor_y, '0');
        next_char_pos();
    }
    else{
        if(a<0){
            lfb_showchar(cursor_x, cursor_y, '-');
            next_char_pos();
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
        }
    }
    return;
}

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