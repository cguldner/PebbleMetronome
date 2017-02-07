#ifndef AUX_H
#define AUX_H

#include <pebble.h>
#include "main.h"

/**
 * Updates the bpm by the amount specified
 * @param amount How much to increment the bpm by
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
 * Wrapper function that updates the bpm to the nearest higher tempo marking
 */
int increment_tempo_marking(void);

/**
 * Wrapper function that updates the bpm to the nearest lower tempo marking
 */
int decrement_tempo_marking(void);

/**
 * Toggles the colors between the foreground and background colors
 * @param toggle A pointer to a boolean, 0 to reset
 */
void toggle_colors(bool toggle);

#endif