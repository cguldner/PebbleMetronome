#include "auxilary.h"


uint32_t convert_bpm(int beats) {
    // Minute to milliseconds
    int minute = 1000 * 60;
    double converted = (1 / (double)beats) * minute;
    return (uint32_t)converted;
}

// Converts an integer to a char array/string
char * int_to_str(int num) {
    static char buff[3] = {};
    int i = 0, temp_num = num, length = 0;
    char * string = buff;
    if (num >= 0) {
        // count how many characters in the number
        while (temp_num) {
            temp_num /= 10;
            length++;
        }
        // assign the number to the buffer starting at the end of the
        // number and going to the begining since we are doing the
        // integer to character conversion on the last number in the
        // sequence
        for (i = 0; i < length; i++) {
            buff[(length - 1) - i] = '0' + (num % 10);
            num /= 10;
        }
        buff[i] = '\0'; // can't forget the null byte to properly end our string
    } else
        return "Unsupported Number";
    return string;
}

char * get_tempo_marking(int bpm) {
    char buff[20] = {};
    char * tempo = buff;
    
    switch(bpm) {
        case 1 ... 20:
            tempo = "Larghissimo";
            break;
        case 21 ... 40:
            tempo = "Grave";
            break;
        case 41 ... 60:
            tempo = "Largo";
            break;
        case 61 ... 66:
            tempo = "Larghetto";
            break;
        case 67 ... 76:
            tempo = "Adagio";
            break;
        case 77 ... 108:
            tempo = "Andante";
            break;
        case 109 ... 120:
            tempo = "Moderato";
            break;
        case 121 ... 140:
            tempo = "Allegro";
            break;
        case 141 ... 168:
            tempo = "Vivace";
            break;
        case 169 ... 200:
            tempo = "Presto";
            break;
        case 201 ... 300:
            tempo = "Prestissimo";
            break;
        default:
            tempo = "Not valid";
            break;
    }
    return tempo;
}

// Changes the tempo marking to either the next or previous based on the dir given, either 1 or -1
int change_tempo_marking(int dir) {    
    switch(bpm) {
        case 1 ... 20:
            bpm = dir==1 ? 21 : 1;
            break;
        case 21 ... 40:
            bpm = dir==1 ? 41 : 1;
            break;
        case 41 ... 60:
            bpm = dir==1 ? 61 : 21;
            break;
        case 61 ... 66:
            bpm = dir==1 ? 67 : 41;
            break;
        case 67 ... 76:
            bpm = dir==1 ? 77 : 61;
            break;
        case 77 ... 108:
            bpm = dir==1 ? 109 : 67;
            break;
        case 109 ... 120:
            bpm = dir==1 ? 121 : 77;
            break;
        case 121 ... 140:
            bpm = dir==1 ? 141 : 109;
            break;
        case 141 ... 168:
            bpm = dir==1 ? 169 : 121;
            break;
        case 169 ... 200:
            bpm = dir==1 ? 201 : 141;
            break;
        case 201 ... 300:
            bpm = dir==1 ? 300 : 169;
            break;
        default:
            bpm = 0;
            break;
    }
    return bpm;
}

// Toggles the colors on the screen
void toggle_colors(int *toggle) {
    GColor fg_color_con = GColorFromHEX(fg_color);
    GColor bg_color_con = GColorFromHEX(bg_color);
    if(*toggle) {
        window_set_background_color(window, bg_color_con);
        text_layer_set_background_color(bpm_text_layer, bg_color_con);
        text_layer_set_text_color(bpm_text_layer, fg_color_con);
        text_layer_set_background_color(tempo_text_layer, bg_color_con);
        text_layer_set_text_color(tempo_text_layer, fg_color_con);
    }
    // Value of 0 resets the colors
    else {
        window_set_background_color(window, fg_color_con);
        text_layer_set_background_color(bpm_text_layer, fg_color_con);
        text_layer_set_text_color(bpm_text_layer, bg_color_con);
        text_layer_set_background_color(tempo_text_layer, fg_color_con);
        text_layer_set_text_color(tempo_text_layer, bg_color_con);
    }
}