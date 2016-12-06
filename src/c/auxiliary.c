/**
  * Includes the various functions that interact with the application logic of the metronome
  */

#include "auxiliary.h"
#include "meter_arm.h"
#include "main.h"

/**
 * Updates the bpm by the amount specified
 */
void update_bpm(int amount) {
    int new_bpm = bpm + amount;
    if(new_bpm < 1 || new_bpm > 300)
        return;
    bpm = new_bpm;
    text_layer_set_text(bpm_text_layer, int_to_str(bpm));
    text_layer_set_text(tempo_text_layer, get_tempo_marking(bpm));
}

/**
 * Convert bpm to time, for scheduling timers for vibrations
 */
uint32_t convert_bpm(int beats) {
    // Minute to milliseconds
    int minute = 1000 * 60;
    double converted = (1 / (double)beats) * minute;
    return (uint32_t)converted;
}

/**
 * Converts an integer to a char array/string
 */
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

/**
 * Gets the tempo marking given the bpm
 */
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

/**
 * Changes the tempo marking to either the next or previous based on the dir given, either 1 or -1
 */
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

/**
 * Toggles the colors between the foreground and background colors
 * @param toggle - a pointer to a boolean, 0 to reset
 */
void toggle_colors(int *toggle) {
    GColor fg_color_con = GColorFromHEX(fg_color);
    GColor bg_color_con = GColorFromHEX(bg_color);
    if(*toggle) {
        window_set_background_color(window, fg_color_con);
        text_layer_set_background_color(bpm_text_layer, fg_color_con);
        text_layer_set_text_color(bpm_text_layer, bg_color_con);
        text_layer_set_background_color(tempo_text_layer, fg_color_con);
        text_layer_set_text_color(tempo_text_layer, bg_color_con);
        
        meter_color = bg_color;
    }
    // Value of 0 resets the colors
    else {
        window_set_background_color(window, bg_color_con);
        text_layer_set_background_color(bpm_text_layer, bg_color_con);
        text_layer_set_text_color(bpm_text_layer, fg_color_con);
        text_layer_set_background_color(tempo_text_layer, bg_color_con);
        text_layer_set_text_color(tempo_text_layer, fg_color_con);
        
        meter_color = fg_color;
    }
    // Update the color of the metronome arm
    layer_mark_dirty(s_path_layer);
}

/**
 * Either shows or hides the meter arm, based on the message key "meter_arm"
 */
void toggle_meter_arm() {
    GRect bounds = layer_get_bounds(window_get_root_layer(window));
    // Centers the text on the round watches, looks weird if off center
    int action_bar_w = ACTION_BAR_WIDTH==30 ? ACTION_BAR_WIDTH : 0;
    
    if(meter_arm) {
        layer_set_hidden(s_path_layer, false);
        
        // Move to make room for the meter arm
        layer_set_frame((Layer *)bpm_text_layer, GRect(0, 0, bounds.size.w - action_bar_w, 45));
        text_layer_set_font(bpm_text_layer, fonts_get_system_font(FONT_KEY_BITHAM_34_MEDIUM_NUMBERS));
        
        layer_set_frame((Layer *)tempo_text_layer, GRect(0, 40, bounds.size.w - action_bar_w, 40));
        text_layer_set_font(tempo_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
    }
    else {
        layer_set_hidden(s_path_layer, true);
        
        // Center the frames
        layer_set_frame((Layer *)bpm_text_layer, GRect(0, bounds.size.h/2 - 25, bounds.size.w - action_bar_w, 45));
        text_layer_set_font(bpm_text_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_MEDIUM_NUMBERS));

        layer_set_frame((Layer *)tempo_text_layer, GRect(0, bounds.size.h/2 + 35, bounds.size.w - action_bar_w, 45));
        text_layer_set_font(tempo_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
    }
}