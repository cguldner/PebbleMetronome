#ifndef ARM_H
#define ARM_H

#include "main.h"
#include "SmallMaths.h"

/**
  * Creating the meter arm
  */
const GPathInfo METER_ARM_POINTS;
Layer *s_path_layer;
GPath *s_meter_path;
int s_path_angle;
int meter_color;

void create_meter_arm(void);

/**
 * This is the layer update callback which is called on render updates
 * e.g. when the layer is marked "dirty"
 */
void path_layer_update_callback(Layer *layer, GContext *ctx);

/**
 * Updates the angle by the given amount, circling around
 * if the angle would end up being greater than 360
 * @param angle - How much to increase the angle by
 */
int path_angle_add(int angle);

/**
 * Return the bounds that the metronome arm can travel
 */
int get_angle_bounds(void);

/**
  * Animating the meter arm
  */
Animation *forward_animate, *backward_animate;
AnimationImplementation forward_impl, backward_impl;

/**
 * Animate the meter arm
 */
void reset_animation(void);
void forward_animate_update(Animation *animation, const AnimationProgress progress);
void backward_animate_update(Animation *animation, const AnimationProgress progress);

/**
 * Sets up and executes the animation in the given direction for the duration
 * @param direction True is forward, false is backwards
 * @param duration How long the animation should be
 */
void animate_meter_arm(bool toggle, int duration);

/**
 * Either shows or hides the meter arm, based on the message key "meter_arm"
 * This also changes the size and location of the bpm and tempo info, making
 * it larger if the arm is not visible, and smaller if it is visible
 */
void toggle_meter_arm_visibility(void);

#endif