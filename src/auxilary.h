#include <pebble.h>

Window *window;

// The actual value of the metronome
TextLayer *bpm_text_layer,
// What tempo group the bpm falls under
          *tempo_text_layer;

ActionBarLayer *prim_action_bar, *aux_action_bar;

// Variables set by the Clay configurator
int fg_color, bg_color;
bool flashing;

int bpm;


uint32_t convert_bpm(int beats);

char * int_to_str(int num);

char * get_tempo_marking(int bpm);
int change_tempo_marking(int dir);

void toggle_colors(int *toggle);