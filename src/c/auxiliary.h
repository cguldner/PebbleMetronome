#ifndef AUX_H
#define AUX_H

#include <pebble.h>
#include "main.h"

/**
 * Updates the bpm by the amount specified
 * @param amount - how much to increment the bpm by
 */
void update_bpm(int amount);

/**
 * Convert bpm to an actual time value, for scheduling timers for vibrations
 */
uint32_t convert_bpm_to_time(void);

/**
 * Converts an integer to a char array/string
 */
char * int_to_str(int num);

/**
 * Gets the tempo marking (Allegro, Andante, etc) based on the current bpm
 */
char * get_tempo_marking(void);

/**
 * Updates the bpm to the closest tempo marking 
 * @param dir - 1 for higher, -1 for lower
 */
int change_tempo_marking(int dir);

/**
 * Toggles the colors between the foreground and background colors
 * @param toggle - a pointer to a boolean, 0 to reset
 */
void toggle_colors(bool toggle);

#endif