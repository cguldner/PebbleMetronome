#include "main.h"

static const int REPEAT_INTERVAL = 75;
static const int LONG_DELAY = 400;
// For use with persisting data with Clay
static const int SETTINGS_KEY = 7;

static AppTimer *metro_timer = NULL;

void metro_loop_handler(void *data) {
    // Invert the data based on whether the pointer was NULL or not
    int color_stat = data ? 0 : 1;
    
    if(settings.flashing) {
        toggle_colors(color_stat);
    }
    
    if(settings.vibrate) {
        VibePattern pat = {
            .durations = settings.vibe_pat,
            .num_segments = ARRAY_LENGTH(settings.vibe_pat),
        };
        vibes_enqueue_custom_pattern(pat);
    }
    animate_meter_arm(color_stat, convert_bpm_to_time());
    
    // Schedule the next timer
    metro_timer = app_timer_register(convert_bpm_to_time(), metro_loop_handler, color_stat ? &color_stat : NULL);
}

void click_config_provider(void *context) {
    window_single_repeating_click_subscribe(BUTTON_ID_UP, REPEAT_INTERVAL, up_bpm_click_handler);
    window_single_repeating_click_subscribe(BUTTON_ID_DOWN, REPEAT_INTERVAL, down_bpm_click_handler);
    
    window_single_click_subscribe(BUTTON_ID_SELECT, select_play_click_handler);
    window_long_click_subscribe(BUTTON_ID_SELECT, LONG_DELAY, select_long_click_handler, NULL);
}

void aux_click_config_provider(void *context) {
    window_single_repeating_click_subscribe(BUTTON_ID_UP, REPEAT_INTERVAL, up_tempo_click_handler);
    window_single_repeating_click_subscribe(BUTTON_ID_DOWN, REPEAT_INTERVAL, down_tempo_click_handler);
    
    window_single_click_subscribe(BUTTON_ID_SELECT, select_meter_click_handler);
    window_long_click_subscribe(BUTTON_ID_SELECT, LONG_DELAY, select_long_click_handler, NULL);
}

void up_bpm_click_handler(ClickRecognizerRef recognizer, void *context) {
    update_bpm(1);
}

void up_tempo_click_handler(ClickRecognizerRef recognizer, void *context) {
    // Remove the aux action bar
    action_bar_layer_set_click_config_provider(prim_action_bar, click_config_provider);
    action_bar_layer_destroy(aux_action_bar);
    aux_action_bar = NULL;
    
    settings.bpm = increment_tempo_marking();
    text_layer_set_text(bpm_text_layer, int_to_str(settings.bpm));
    text_layer_set_text(tempo_text_layer, get_tempo_marking());
}

void select_play_click_handler(ClickRecognizerRef recognizer, void *context) {
    if(metro_timer == NULL) {
        action_bar_layer_set_icon_animated(prim_action_bar, BUTTON_ID_SELECT, gbitmap_create_with_resource(RESOURCE_ID_PAUSE_METRO), true);
        metro_timer = app_timer_register(convert_bpm_to_time(), (AppTimerCallback)metro_loop_handler, NULL);
    }
    else {
        action_bar_layer_set_icon_animated(prim_action_bar, BUTTON_ID_SELECT, gbitmap_create_with_resource(RESOURCE_ID_START_METRO), true);
        app_timer_cancel(metro_timer);
        vibes_cancel();
        metro_timer = NULL;
        
        toggle_colors(false);
        reset_animation();
    }
}

void select_meter_click_handler(ClickRecognizerRef recognizer, void *context) {
    // Remove the aux action bar
    action_bar_layer_set_click_config_provider(prim_action_bar, click_config_provider);
    action_bar_layer_destroy(aux_action_bar);
    aux_action_bar = NULL;
}

void select_long_click_handler(ClickRecognizerRef recognizer, void *context) {
    // Don't vibrate if metronome is going
    if(metro_timer == NULL) {
        VibePattern pat = {
            .durations = settings.vibe_pat,
            .num_segments = ARRAY_LENGTH(settings.vibe_pat),
        };
        vibes_enqueue_custom_pattern(pat);
    }
    
    if(aux_action_bar == NULL) {
        // Add the aux action bar
        aux_action_bar = action_bar_layer_create();
        // Associate the action bar with the window:
        action_bar_layer_add_to_window(aux_action_bar, window);
        // Set the click config provider:
        action_bar_layer_set_click_config_provider(aux_action_bar, aux_click_config_provider);
    
        action_bar_layer_set_icon_animated(aux_action_bar, BUTTON_ID_UP, gbitmap_create_with_resource(RESOURCE_ID_UP_ARROW), true);
        //action_bar_layer_set_icon_animated(aux_action_bar, BUTTON_ID_SELECT, gbitmap_create_with_resource(RESOURCE_ID_METER_ARM), true);
        action_bar_layer_set_icon_animated(aux_action_bar, BUTTON_ID_DOWN, gbitmap_create_with_resource(RESOURCE_ID_DOWN_ARROW), true);
        
    }
    else {
        // Remove the aux action bar
        action_bar_layer_set_click_config_provider(prim_action_bar, click_config_provider);
        action_bar_layer_destroy(aux_action_bar);
        aux_action_bar = NULL;
    }
}

void down_bpm_click_handler(ClickRecognizerRef recognizer, void *context) {
    update_bpm(-1);
}

void down_tempo_click_handler(ClickRecognizerRef recognizer, void *context) {
    // Remove the aux action bar
    action_bar_layer_set_click_config_provider(prim_action_bar, click_config_provider);
    action_bar_layer_destroy(aux_action_bar);
    aux_action_bar = NULL;
    
    settings.bpm = decrement_tempo_marking();
    text_layer_set_text(bpm_text_layer, int_to_str(settings.bpm));
    text_layer_set_text(tempo_text_layer, get_tempo_marking());
}

void window_load(Window *window) {
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);
    int status_bar_h = PBL_IF_RECT_ELSE(16, 24);

    // Just initialize the layer
    bpm_text_layer = text_layer_create(GRect(0,0,0,0));
    // Set the text to the BPM number as a string
    text_layer_set_text(bpm_text_layer, int_to_str(settings.bpm));

    tempo_text_layer = text_layer_create(GRect(0,0,0,0));
    // Set the text to the tempo marking the BPM currently falls under
    text_layer_set_text(tempo_text_layer, get_tempo_marking());
    
    text_layer_set_text_alignment(bpm_text_layer, GTextAlignmentCenter);
    text_layer_set_text_alignment(tempo_text_layer, GTextAlignmentCenter);
    layer_add_child(window_layer, text_layer_get_layer(bpm_text_layer));
    layer_add_child(window_layer, text_layer_get_layer(tempo_text_layer));
    
    // Initialize the action bar
    prim_action_bar = action_bar_layer_create();
    action_bar_layer_add_to_window(prim_action_bar, window);
    action_bar_layer_set_click_config_provider(prim_action_bar, click_config_provider);
    
    // Initialize the status bar
    StatusBarLayer *status_bar = status_bar_layer_create();
    GRect frame = GRect(0, 0, bounds.size.w, STATUS_BAR_LAYER_HEIGHT);
    layer_set_frame(status_bar_layer_get_layer(status_bar), frame);
    layer_add_child(window_layer, status_bar_layer_get_layer(status_bar));

    action_bar_layer_set_icon_animated(prim_action_bar, BUTTON_ID_UP, gbitmap_create_with_resource(RESOURCE_ID_ADD_BEATS), true);
    action_bar_layer_set_icon_animated(prim_action_bar, BUTTON_ID_SELECT, gbitmap_create_with_resource(RESOURCE_ID_START_METRO), true);
    action_bar_layer_set_icon_animated(prim_action_bar, BUTTON_ID_DOWN, gbitmap_create_with_resource(RESOURCE_ID_SUBTRACT_BEATS), true);
    
    meter_color = settings.fg_color;
    
    create_meter_arm();
    
    toggle_meter_arm_visibility();
}

void window_unload(Window *window) {
    text_layer_destroy(bpm_text_layer);
    text_layer_destroy(tempo_text_layer);
}

void prv_inbox_received_handler(DictionaryIterator *iter, void *context) {
    // Read color preferences
    Tuple *fg_color_t = dict_find(iter, MESSAGE_KEY_ForegroundColor);
    if(fg_color_t) settings.fg_color = fg_color_t->value->int32;

    Tuple *bg_color_t = dict_find(iter, MESSAGE_KEY_BackgroundColor);
    if(bg_color_t) settings.bg_color = bg_color_t->value->int32;
    
    Tuple *flashing_t = dict_find(iter, MESSAGE_KEY_Flashing);
    if(flashing_t) settings.flashing = flashing_t->value->int32 == 1;
    
    Tuple *meter_arm_t = dict_find(iter, MESSAGE_KEY_MeterArm);
    if(meter_arm_t) settings.meter_arm = meter_arm_t->value->int32 == 1;

    Tuple *vibe_pat_t = dict_find(iter, MESSAGE_KEY_VibeLength);
    if(vibe_pat_t) settings.vibe_pat[0] = vibe_pat_t->value->int32;
    
    Tuple *vibe_t = dict_find(iter, MESSAGE_KEY_Vibrate); 
    if(vibe_t) settings.vibrate = vibe_t->value->int32;
    
    toggle_colors(false);
    
    toggle_meter_arm_visibility();
}

void prv_save_settings() {
    // Store the bpm and preferences
    persist_write_data(SETTINGS_KEY, &settings, sizeof(settings));
}

void prv_load_settings() {
    // Load the default settings
    settings.bpm = 120;
    settings.fg_color = PBL_IF_BW_ELSE(0x000000, 0x00FF55);
    settings.bg_color = PBL_IF_BW_ELSE(0xffffff, 0x000055);
    settings.flashing = false;
    settings.meter_arm = true;
    settings.vibrate = true;
    settings.vibe_pat[0] = 50;
    // Read settings from persistent storage, if they exist
    persist_read_data(SETTINGS_KEY, &settings, sizeof(settings));
}

void init(void) {
    // Pass the corresponding GPathInfo to initialize a GPath
    s_meter_path = gpath_create(&METER_ARM_POINTS);
    
    window = window_create();
    window_set_click_config_provider(window, click_config_provider);
    window_set_window_handlers(window, (WindowHandlers) {
        .load = window_load,
        .unload = window_unload, 
    });
    
    prv_load_settings();
    
    // Open AppMessage connection
    app_message_register_inbox_received(prv_inbox_received_handler);
    app_message_open(128, 128);
    
    window_stack_push(window, true);
}

void deinit(void) {
    app_timer_cancel(metro_timer);
    // Destroy main Window
    prv_save_settings();
    window_destroy(window);
    
    gpath_destroy(s_meter_path);
}

int main(void) {
    init();
    
    // Set the colors to the defaults read from storage
    toggle_colors(false);
    
    app_event_loop();
    deinit();
}