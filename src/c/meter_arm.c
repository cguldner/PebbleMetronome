#include "meter_arm.h"

#define METER_ARM_W 3
#define METER_ARM_H 80
#define METER_BOX_W 4
#define METER_BOX_H 10

static int angle_bounds;

// The metronome arm drawn as a polygon
const GPathInfo METER_ARM_POINTS = {
    // Number of points
    8,
    // Position of each point
    (GPoint []) {{-METER_ARM_W - METER_BOX_W, -METER_ARM_H}, {-METER_ARM_W - METER_BOX_W, -METER_ARM_H - METER_BOX_H}, {METER_ARM_W + METER_BOX_W, -METER_ARM_H - METER_BOX_H},
                 {METER_ARM_W + METER_BOX_W, -METER_ARM_H}, {METER_ARM_W, -METER_ARM_H}, {METER_ARM_W, 0}, {-METER_ARM_W, 0}, {-METER_ARM_W, -METER_ARM_H}}
};

void create_meter_arm(void) {
    GRect bounds = layer_get_bounds(window_layer);
    angle_bounds = get_angle_bounds();
    
    s_path_layer = layer_create(bounds);
    s_path_angle = -angle_bounds;
    layer_set_update_proc(s_path_layer, path_layer_update_callback);
    layer_add_child(window_layer, s_path_layer);

    // Move all paths to the center of the screen
    gpath_move_to(s_meter_path, GPoint((bounds.size.w - (ACTION_BAR_WIDTH==30 ? ACTION_BAR_WIDTH : 0))/2, bounds.size.h));
}

void path_layer_update_callback(Layer *layer, GContext *ctx) {
    // You can rotate the path before rendering
    gpath_rotate_to(s_meter_path, (TRIG_MAX_ANGLE / 360) * s_path_angle);

    graphics_context_set_fill_color(ctx, GColorFromHEX(meter_color));
    gpath_draw_filled(ctx, s_meter_path);
}

/**
 * Updates the angle by the given amount, circling around
 * if the angle would end up being greater than 360
 * @param angle - How much to increase the angle by
 */
static int path_angle_add(int angle) {
    return s_path_angle = (s_path_angle + angle) % 360;
}

/**
 * Put the meter arm in the original position
 */
static void reset_path_angle() {
    path_angle_add(-angle_bounds - s_path_angle);
}

int get_angle_bounds(void) {
    int angle = 0;
    float ab = ACTION_BAR_WIDTH;
    GRect wb = layer_get_bounds(window_layer);
    
    /* Gets the ratio of the triangle sides between the horizontal and the meter arm */
    float hypotenuse = METER_ARM_H + sm_sqrt(sm_powint(METER_BOX_W + METER_ARM_W, 2) + sm_powint(METER_BOX_H, 2));
    // Meter arm is centered with respect to action bar, so just half that distance
    #ifdef PBL_RECT
        float horizontal_leg = (wb.size.w - ab) / 2;
    #else
    // Meter arm is in center of screen, so half the distance of the screen minus the action bar width
        float horizontal_leg = .5 * wb.size.w - ab - 5;
    #endif
    
    angle = sm_acosd(horizontal_leg / hypotenuse);
    
    return 90 - angle;
}

void reset_meter_arm_animation(void) {
    if(forward_animate) animation_destroy(forward_animate);
    if(backward_animate) animation_destroy(backward_animate);
    reset_path_angle();
    layer_mark_dirty(s_path_layer);
}

static void forward_animate_update(Animation *animation, const AnimationProgress progress) {
    int progress_percent = ((int)progress * 100) / ANIMATION_NORMALIZED_MAX;
    int delta = angle_bounds*2*progress_percent/100 - (s_path_angle+angle_bounds);
    
    path_angle_add(delta);
    layer_mark_dirty(s_path_layer);
}

static void backward_animate_update(Animation *animation, const AnimationProgress progress) {
    int progress_percent = ((int)progress * 100) / ANIMATION_NORMALIZED_MAX;
    int delta = -angle_bounds*2*progress_percent/100 - (s_path_angle-angle_bounds);
    
    path_angle_add(delta);
    layer_mark_dirty(s_path_layer);
}

void animate_meter_arm(bool direction, int duration) {
    if(direction) {
        // Create a new Animation
        forward_animate = animation_create();
        animation_set_curve(forward_animate, AnimationCurveLinear);
        animation_set_duration(forward_animate, duration);
        // Create the AnimationImplementation
        forward_impl = (AnimationImplementation){
            .update = forward_animate_update
        };
        animation_set_implementation(forward_animate, &forward_impl);
        animation_schedule(forward_animate);
    }
    else {
        backward_animate = animation_create();
        animation_set_curve(backward_animate, AnimationCurveLinear);
        animation_set_duration(backward_animate, duration);
        backward_impl = (AnimationImplementation){
            .update = backward_animate_update
        };
        animation_set_implementation(backward_animate, &backward_impl);
        animation_schedule(backward_animate);
    }
}

void toggle_meter_arm_visibility() {
    GRect bounds = layer_get_bounds(window_layer);
    // Centers the text on the round watches, looks weird if off center
    int action_bar_w = ACTION_BAR_WIDTH==30 ? ACTION_BAR_WIDTH : 0,
        bpm_y_pos, tempo_y_pos;
    bool hide_layer;
    if(settings.meter_arm) {
        hide_layer = false;
        // Move to make room for the meter arm
        bpm_y_pos = STATUS_BAR_LAYER_HEIGHT;
        tempo_y_pos = STATUS_BAR_LAYER_HEIGHT + 40;
        text_layer_set_font(bpm_text_layer, fonts_get_system_font(FONT_KEY_BITHAM_34_MEDIUM_NUMBERS));
        text_layer_set_font(tempo_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
    }
    else {
        hide_layer = true;
        // Center the frames
        bpm_y_pos = bounds.size.h/2 - 25;
        tempo_y_pos = bounds.size.h/2 + 35;
        text_layer_set_font(bpm_text_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_MEDIUM_NUMBERS));
        text_layer_set_font(tempo_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
    }
    layer_set_hidden(s_path_layer, hide_layer);
    layer_set_frame((Layer *)bpm_text_layer, GRect(0, bpm_y_pos, bounds.size.w - action_bar_w, 45));
    layer_set_frame((Layer *)tempo_text_layer, GRect(0, tempo_y_pos, bounds.size.w - action_bar_w, 40));
}