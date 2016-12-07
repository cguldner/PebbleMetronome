#include <pebble.h>


Window *window;

// The actual value of the metronome
TextLayer *bpm_text_layer,
// What tempo group the bpm falls under
          *tempo_text_layer;

ActionBarLayer *prim_action_bar, *aux_action_bar;

// Variables set by the Clay configurator
//int fg_color, bg_color, bpm;
//bool flashing, meter_arm;

// Define our settings struct
typedef struct ClaySettings {
    int fg_color, bg_color, bpm;
    bool flashing, meter_arm;
    uint32_t vibe_pat[1];
} ClaySettings;
ClaySettings settings;

void update_bpm(int amount);

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

void prv_inbox_received_handler(DictionaryIterator *iter, void *context);
void prv_save_settings();
void prv_load_settings();

void init(void);
void deinit(void);

int main(void);