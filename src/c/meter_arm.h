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
void path_layer_update_callback(Layer *layer, GContext *ctx);
int path_angle_add(int angle);
int get_angle_bounds(void);

/**
  * Animating the meter arm
  */
Animation *forward_animate, *backward_animate;
AnimationImplementation forward_impl, backward_impl;

void reset_animation(void);
void forward_animate_update(Animation *animation, const AnimationProgress progress);
void backward_animate_update(Animation *animation, const AnimationProgress progress);
void animate_meter_arm(int *toggle, int duration);

void toggle_meter_arm();

#endif