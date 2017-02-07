#ifndef MAIN_H
#define MAIN_H

#include <pebble.h>
#include "auxiliary.h"
#include "meter_arm.h"

Window *window;

Layer *window_layer;

// The actual value of the metronome
TextLayer *bpm_text_layer,
// What tempo group the bpm falls under
          *tempo_text_layer;

ActionBarLayer *prim_action_bar, *aux_action_bar;

// Define our settings struct
typedef struct ClaySettings {
    int bpm, fg_color, bg_color;
    bool flashing, meter_arm, vibrate;
    uint32_t vibe_pat[1];
} ClaySettings;
ClaySettings settings;

void metro_loop_handler(void *data);

void click_config_provider(void *context);
void aux_click_config_provider(void *context);

void up_bpm_click_handler(ClickRecognizerRef recognizer, void *context);
void up_tempo_click_handler(ClickRecognizerRef recognizer, void *context);

void select_play_click_handler(ClickRecognizerRef recognizer, void *context);
void select_meter_click_handler(ClickRecognizerRef recognizer, void *context);
void select_long_click_handler(ClickRecognizerRef recognizer, void *context);

void down_bpm_click_handler(ClickRecognizerRef recognizer, void *context);
void down_tempo_click_handler(ClickRecognizerRef recognizer, void *context);

void window_load(Window *window);
void window_unload(Window *window);

/**
 * App message handler to receive data from Clay preferences
 */
void prv_inbox_received_handler(DictionaryIterator *iter, void *context);
void prv_save_settings();
void prv_load_settings();

void init(void);
void deinit(void);

int main(void);

#endif