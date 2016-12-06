#include <pebble.h>

void update_bpm(int amount);

uint32_t convert_bpm(int beats);

char * int_to_str(int num);

char * get_tempo_marking(int bpm);
int change_tempo_marking(int dir);

void toggle_colors(int *toggle);
void toggle_meter_arm();