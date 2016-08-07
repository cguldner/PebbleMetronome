/**
  * Menu icon credit: Metronome by Pavel Diatchenko from the Noun Project
  * Banner credit: http://static.wixstatic.com/media/871218_6a00fd1af0fa4420b8f9c6eb695af50b.jpg
  * Icon credit: http://3.bp.blogspot.com/-RVClYFeAFDM/UHGqotZtxqI/AAAAAAAAAlE/1rXjpQXtGs0/s1600/Metronome.gif
  */

#include "auxilary.h"
#include "metronome.h"

// For use with persisting data
static const int BPM_KEY = 0;
static const int FG_KEY = 1;
static const int BG_KEY = 2;
static const int VIBE_KEY = 3;
static const int FLASH_KEY = 4;

static AppTimer *metro_timer = NULL;
// Insert length of vibration
static uint32_t vibe_pat[1];
static int toggle_num = 0;



void update_bpm(int amount) {
    int new_bpm = bpm + amount;
    if(new_bpm < 1 || new_bpm > 300)
        return;
    bpm = new_bpm;
    text_layer_set_text(bpm_text_layer, int_to_str(bpm));
    text_layer_set_text(tempo_text_layer, get_tempo_marking(bpm));
}

void metro_loop_handler(void *data) {
    // Check if flash is enabled in the settings
    if(flashing) {
        toggle_num = !toggle_num;
        toggle_colors(&toggle_num);
    }
    
    VibePattern pat = {
        .durations = vibe_pat,
        .num_segments = ARRAY_LENGTH(vibe_pat),
    };
    vibes_enqueue_custom_pattern(pat);
    
    metro_timer = app_timer_register(convert_bpm(bpm), metro_loop_handler, &data);
}

void click_config_provider(void *context) {
    window_single_repeating_click_subscribe(BUTTON_ID_UP, 75, up_bpm_click_handler);
    window_single_repeating_click_subscribe(BUTTON_ID_DOWN, 75, down_bpm_click_handler);
    
    window_single_click_subscribe(BUTTON_ID_SELECT, select_play_click_handler);
    window_long_click_subscribe(BUTTON_ID_SELECT, 500, select_long_click_handler, NULL);
}

void aux_click_config_provider(void *context) {
    window_single_repeating_click_subscribe(BUTTON_ID_UP, 75, up_tempo_click_handler);
    window_single_repeating_click_subscribe(BUTTON_ID_DOWN, 75, down_tempo_click_handler);
    
    window_single_click_subscribe(BUTTON_ID_SELECT, select_settings_click_handler);
    window_long_click_subscribe(BUTTON_ID_SELECT, 500, select_long_click_handler, NULL);
}

void up_bpm_click_handler(ClickRecognizerRef recognizer, void *context) {
    update_bpm(1);
}

void up_tempo_click_handler(ClickRecognizerRef recognizer, void *context) {
    bpm = change_tempo_marking(1);
    text_layer_set_text(bpm_text_layer, int_to_str(bpm));
    text_layer_set_text(tempo_text_layer, get_tempo_marking(bpm));
    
    // Remove the aux action bar
    action_bar_layer_set_click_config_provider(prim_action_bar, click_config_provider);
    action_bar_layer_destroy(aux_action_bar);
    aux_action_bar = NULL;    
}

void select_play_click_handler(ClickRecognizerRef recognizer, void *context) {
    if(metro_timer == NULL) {
        action_bar_layer_set_icon_animated(prim_action_bar, BUTTON_ID_SELECT, gbitmap_create_with_resource(RESOURCE_ID_PAUSE_METRO), true);
        metro_timer = app_timer_register(convert_bpm(bpm), (AppTimerCallback) metro_loop_handler, (void *)(&toggle_num));
    }
    else {
        action_bar_layer_set_icon_animated(prim_action_bar, BUTTON_ID_SELECT, gbitmap_create_with_resource(RESOURCE_ID_START_METRO), true);
        app_timer_cancel(metro_timer);
        vibes_cancel();
        metro_timer = NULL;
        
        toggle_num = 0;
        toggle_colors(&toggle_num);
    }
}

void select_long_click_handler(ClickRecognizerRef recognizer, void *context) {
    // Don't vibrate if metronome is going
    if(metro_timer == NULL) {
        VibePattern pat = {
            .durations = vibe_pat,
            .num_segments = ARRAY_LENGTH(vibe_pat),
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
        action_bar_layer_set_icon_animated(aux_action_bar, BUTTON_ID_SELECT, gbitmap_create_with_resource(RESOURCE_ID_SETTINGS), true);
        action_bar_layer_set_icon_animated(aux_action_bar, BUTTON_ID_DOWN, gbitmap_create_with_resource(RESOURCE_ID_DOWN_ARROW), true);
    }
    else {
        // Remove the aux action bar
        action_bar_layer_set_click_config_provider(prim_action_bar, click_config_provider);
        action_bar_layer_destroy(aux_action_bar);
        aux_action_bar = NULL;
    }
}

void select_settings_click_handler(ClickRecognizerRef recognizer, void *context) {
    /*open_settings();*/
}

void down_bpm_click_handler(ClickRecognizerRef recognizer, void *context) {
    update_bpm(-1);
}

void down_tempo_click_handler(ClickRecognizerRef recognizer, void *context) {
    bpm = change_tempo_marking(-1);
    text_layer_set_text(bpm_text_layer, int_to_str(bpm));
    text_layer_set_text(tempo_text_layer, get_tempo_marking(bpm));
    
    // Remove the aux action bar
    action_bar_layer_set_click_config_provider(prim_action_bar, click_config_provider);
    action_bar_layer_destroy(aux_action_bar);
    aux_action_bar = NULL;    
}

void window_load(Window *window) {
    // Initialize the action bar:
    prim_action_bar = action_bar_layer_create();
    // Associate the action bar with the window:
    action_bar_layer_add_to_window(prim_action_bar, window);
    // Set the click config provider:
    action_bar_layer_set_click_config_provider(prim_action_bar, click_config_provider);

    action_bar_layer_set_icon_animated(prim_action_bar, BUTTON_ID_UP, gbitmap_create_with_resource(RESOURCE_ID_ADD_BEATS), true);
    action_bar_layer_set_icon_animated(prim_action_bar, BUTTON_ID_SELECT, gbitmap_create_with_resource(RESOURCE_ID_START_METRO), true);
    action_bar_layer_set_icon_animated(prim_action_bar, BUTTON_ID_DOWN, gbitmap_create_with_resource(RESOURCE_ID_SUBTRACT_BEATS), true);
    
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);

    bpm_text_layer = text_layer_create(GRect(0, bounds.size.h/2 - 21, bounds.size.w - ACTION_BAR_WIDTH, 45));
    // Use a system font in a TextLayer
    text_layer_set_font(bpm_text_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_MEDIUM_NUMBERS));
    text_layer_set_text(bpm_text_layer, int_to_str(bpm));
    text_layer_set_text_alignment(bpm_text_layer, GTextAlignmentCenter);

    tempo_text_layer = text_layer_create(GRect(0, bounds.size.h/2 + 41, bounds.size.w - ACTION_BAR_WIDTH, 45));
    // Use a system font in a TextLayer
    text_layer_set_font(tempo_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
    text_layer_set_text(tempo_text_layer, get_tempo_marking(bpm));
    text_layer_set_text_alignment(tempo_text_layer, GTextAlignmentCenter);
    
    layer_add_child(window_layer, text_layer_get_layer(bpm_text_layer));
    layer_add_child(window_layer, text_layer_get_layer(tempo_text_layer));
}

void window_unload(Window *window) {
    text_layer_destroy(bpm_text_layer);
}

// App message handler to receive data from Clay preferences
void prv_inbox_received_handler(DictionaryIterator *iter, void *context) {
    // Read color preferences
    Tuple *fg_color_t = dict_find(iter, MESSAGE_KEY_ForegroundColor);
    if(fg_color_t) {
        fg_color = fg_color_t->value->int32;
    }

    Tuple *bg_color_t = dict_find(iter, MESSAGE_KEY_BackgroundColor);
    if(bg_color_t) {
        bg_color = bg_color_t->value->int32;
    }

    // Read boolean preferences
    Tuple *flashing_t = dict_find(iter, MESSAGE_KEY_Flashing);
    if(flashing_t) {
        flashing = flashing_t->value->int32 == 1;
    }

    Tuple *vibes_t = dict_find(iter, MESSAGE_KEY_VibeLength);
    if(vibes_t) {
        vibe_pat[0] = vibes_t->value->int32;
    }

}


void init(void) {
    window = window_create();
    window_set_click_config_provider(window, click_config_provider);
    window_set_window_handlers(window, (WindowHandlers) {
        .load = window_load,
        .unload = window_unload,
    });
    const bool animated = true;
    
    if(persist_exists(BPM_KEY)) bpm = (int)persist_read_int(BPM_KEY);
    else bpm = 120;
    if(persist_exists(FG_KEY)) fg_color = (int)persist_read_int(FG_KEY);
    else fg_color = 0xffffff;
    if(persist_exists(BG_KEY)) bg_color = (int)persist_read_int(BG_KEY);
    else bg_color = 0x000000;
    if(persist_exists(VIBE_KEY)) vibe_pat[0] = (int)persist_read_int(VIBE_KEY);
    else vibe_pat[0] = 50;
    if(persist_exists(FLASH_KEY)) flashing = (bool)persist_read_bool(FLASH_KEY);
    else flashing = true;
    
    // Open AppMessage connection
    app_message_register_inbox_received(prv_inbox_received_handler);
    app_message_open(128, 128);
    
    window_stack_push(window, animated);
}

void deinit(void) {
    app_timer_cancel(metro_timer);
    // Store the bpm
    persist_write_int(BPM_KEY, bpm);
    persist_write_int(FG_KEY, fg_color);
    persist_write_int(BG_KEY, bg_color);
    persist_write_int(VIBE_KEY, vibe_pat[0]);
    persist_write_bool(FLASH_KEY, flashing);
    // Destroy main Window
    window_destroy(window);
}

int main(void) {
    init();
    toggle_num = 0;
    toggle_colors(&toggle_num);
    app_event_loop();
    deinit();
}


/*static void main_window_load(Window *window) {
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);

    // Create UI
    s_output_layer = text_layer_create(bounds);
    text_layer_set_text(s_output_layer, "Use SELECT to start/stop the background worker.");
    text_layer_set_text_alignment(s_output_layer, GTextAlignmentCenter);
    layer_add_child(window_layer, text_layer_get_layer(s_output_layer));
    #ifdef PBL_ROUND
    text_layer_enable_screen_text_flow_and_paging(s_output_layer, 8);
    #endif

    s_ticks_layer = text_layer_create(GRect(PBL_IF_RECT_ELSE(5, 0), 135, bounds.size.w, 30));
    text_layer_set_text(s_ticks_layer, "No data yet.");
    text_layer_set_text_alignment(s_ticks_layer, PBL_IF_RECT_ELSE(GTextAlignmentLeft, 
                                                                  GTextAlignmentCenter));
    layer_add_child(window_layer, text_layer_get_layer(s_ticks_layer));
    #ifdef PBL_ROUND
    text_layer_enable_screen_text_flow_and_paging(s_ticks_layer, 8);
    #endif
}

static void main_window_unload(Window *window) {
    // Destroy UI
    text_layer_destroy(s_output_layer);
    text_layer_destroy(s_ticks_layer);
}*/

/*
#include <pebble.h>

static Window *window;
static TextLayer *text_layer;

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "Select");
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "Up");
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "Down");
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  text_layer = text_layer_create(GRect(0, 72, bounds.size.w, 20));
  text_layer_set_text(text_layer, "Press a button");
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
}

static void window_unload(Window *window) {
  text_layer_destroy(text_layer);
}

static void init(void) {
  window = window_create();
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(window, animated);
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);
  while(true) {
      window_set_background_color(window, GColorBlack);
      window_stack_push(window, true);
      
      psleep(1000);
      
      window_set_background_color(window, GColorWhite);
      window_stack_push(window, true);
  }
    
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Pushed new window");
  app_event_loop();
  deinit();
}
*/