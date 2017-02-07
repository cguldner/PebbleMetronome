#include "auxiliary.h"


void update_bpm(int amount) {
    int new_bpm = settings.bpm + amount;
    if(new_bpm < 1 || new_bpm > 300)
        return;
    settings.bpm = new_bpm;
    text_layer_set_text(bpm_text_layer, int_to_str(settings.bpm));
    text_layer_set_text(tempo_text_layer, get_tempo_marking());
}

uint32_t convert_bpm_to_time() {
    // Minute to milliseconds
    int minute = 1000 * 60;
    double converted = (1 / (double)settings.bpm) * minute;
    return (uint32_t)converted;
}

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

char * get_tempo_marking(void) {
    char buff[20] = {};
    char * tempo = buff;
    
    switch(settings.bpm) {
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
            APP_LOG(APP_LOG_LEVEL_ERROR, "invalid bpm of %d", settings.bpm);
            break;
    }
    return tempo;
}

/**
 * Updates the bpm to the closest tempo marking 
 * @param dir - 1 for higher, -1 for lower
 */
static int change_tempo_marking(int dir) {
    if(dir != 1 && dir != -1) {
        APP_LOG(APP_LOG_LEVEL_ERROR, "invalid direction of %d", dir);
        return -1;
    }
    int new_bpm;
    switch(settings.bpm) {
        case 1 ... 20:
            new_bpm = dir==1 ? 21 : 1;
            break;
        case 21 ... 40:
            new_bpm = dir==1 ? 41 : 1;
            break;
        case 41 ... 60:
            new_bpm = dir==1 ? 61 : 21;
            break;
        case 61 ... 66:
            new_bpm = dir==1 ? 67 : 41;
            break;
        case 67 ... 76:
            new_bpm = dir==1 ? 77 : 61;
            break;
        case 77 ... 108:
            new_bpm = dir==1 ? 109 : 67;
            break;
        case 109 ... 120:
            new_bpm = dir==1 ? 121 : 77;
            break;
        case 121 ... 140:
            new_bpm = dir==1 ? 141 : 109;
            break;
        case 141 ... 168:
            new_bpm = dir==1 ? 169 : 121;
            break;
        case 169 ... 200:
            new_bpm = dir==1 ? 201 : 141;
            break;
        case 201 ... 300:
            new_bpm = dir==1 ? 300 : 169;
            break;
        default:
            new_bpm = 0;
            APP_LOG(APP_LOG_LEVEL_ERROR, "invalid bpm of %d", settings.bpm);
            break;
    }
    return new_bpm;
}

int increment_tempo_marking() {
    return change_tempo_marking(1);
} 

int decrement_tempo_marking() {
    return change_tempo_marking(-1);
}

void toggle_colors(bool toggle) {
    GColor fg_color_con = GColorFromHEX(settings.fg_color);
    GColor bg_color_con = GColorFromHEX(settings.bg_color);
    if(toggle) {
        window_set_background_color(window, fg_color_con);
        text_layer_set_background_color(bpm_text_layer, fg_color_con);
        text_layer_set_text_color(bpm_text_layer, bg_color_con);
        text_layer_set_background_color(tempo_text_layer, fg_color_con);
        text_layer_set_text_color(tempo_text_layer, bg_color_con);
        
        meter_color = settings.bg_color;
    }
    // Value of 0 resets the colors
    else {
        window_set_background_color(window, bg_color_con);
        text_layer_set_background_color(bpm_text_layer, bg_color_con);
        text_layer_set_text_color(bpm_text_layer, fg_color_con);
        text_layer_set_background_color(tempo_text_layer, bg_color_con);
        text_layer_set_text_color(tempo_text_layer, fg_color_con);
        
        meter_color = settings.fg_color;
    }
    // Update the color of the metronome arm
    layer_mark_dirty(s_path_layer);
}