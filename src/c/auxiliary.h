#ifndef AUX_H
#define AUX_H

#include <pebble.h>

uint32_t convert_bpm(int beats);

char * int_to_str(int num);

char * get_tempo_marking(int bpm);
int change_tempo_marking(int bpm, int dir);

#endif