#include <pebble.h>

static Window *s_window;
static TextLayer *s_time_layer;
static TextLayer *second_ticker_layer;
static TextLayer *footer_layer;
static TextLayer *date_layer;

static TextLayer *background_top;
static TextLayer *background_bottom;

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  // Write the current hours and minutes into a buffer
  static char s_buffer[8];
  static char date_buffer[34];
  if (tick_time->tm_sec % 2 == 0) {
    strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ? "%H %M" : "%I %M", tick_time);
    text_layer_set_text(second_ticker_layer, " ");
  } else {
    strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ? "%H %M" : "%I %M", tick_time);
    text_layer_set_text(second_ticker_layer, ":");
  }

  strftime(date_buffer, sizeof(date_buffer), "%d/%m-%Y w.%W\n%A", tick_time);

  text_layer_set_text(s_time_layer, s_buffer);
  text_layer_set_text(date_layer, date_buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void prv_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  
  // GColorFromRGB(0, 0, 0)
  // GColorFromHEX(0x000000)

  // GColor backgroundColorTop = GColorCyan;
  GColor backgroundColorTop = GColorElectricBlue;
  GColor backgroundColorBottom = GColorRajah;

  GColor backgroundColor = GColorBlack;
  GColor foregroundColor = GColorWhite;
  
  foregroundColor = GColorBlack;
  foregroundColor = GColorRed;
  foregroundColor = GColorOrange;

  backgroundColor = GColorWhite;
  backgroundColor = GColorCeleste;
  backgroundColor = GColorElectricBlue;
  backgroundColor = GColorBulgarianRose;

  backgroundColor = GColorBlack;
  foregroundColor = GColorBlack;

  // backgroundColor = GColorWhite;
  // foregroundColor = GColorBlack;

  window_set_background_color(s_window, backgroundColor);

  background_top = text_layer_create(GRect(0, 0, bounds.size.w, bounds.size.h / 2));
  background_bottom = text_layer_create(GRect(0, bounds.size.h / 2, bounds.size.w, bounds.size.h));
  text_layer_set_background_color(background_top, backgroundColorTop);
  text_layer_set_background_color(background_bottom, backgroundColorBottom);

  // create and set second ticker layer
  second_ticker_layer = text_layer_create(GRect(0, PBL_IF_ROUND_ELSE(58, 52), bounds.size.w, 50));
  text_layer_set_background_color(second_ticker_layer, GColorClear);
  text_layer_set_text_color(second_ticker_layer, foregroundColor);
  text_layer_set_font(second_ticker_layer, fonts_get_system_font(FONT_KEY_LECO_42_NUMBERS));
  text_layer_set_text_alignment(second_ticker_layer, GTextAlignmentCenter);  

  s_time_layer = text_layer_create(GRect(0, PBL_IF_ROUND_ELSE(58, 52), bounds.size.w, 50));  
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, foregroundColor);
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_LECO_42_NUMBERS));
  // text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_ROBOTO_BOLD_SUBSET_49));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  footer_layer = text_layer_create(GRect(0, bounds.size.h / 4 * 3, bounds.size.w, 50));
  text_layer_set_background_color(footer_layer, GColorClear);
  text_layer_set_text_color(footer_layer, foregroundColor);
  text_layer_set_font(footer_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(footer_layer, GTextAlignmentCenter);
  text_layer_set_text(footer_layer, "v.1.0.5\n2024 (c) Roman Lazunin");

  date_layer = text_layer_create(GRect(0, 0, bounds.size.w, bounds.size.h / 3));
  text_layer_set_background_color(date_layer, GColorClear);
  text_layer_set_text_color(date_layer, foregroundColor);
  text_layer_set_font(date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24));
  text_layer_set_text_alignment(date_layer, GTextAlignmentCenter);
  
  layer_add_child(window_layer, text_layer_get_layer(background_top));
  layer_add_child(window_layer, text_layer_get_layer(background_bottom));

  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
  layer_add_child(window_layer, text_layer_get_layer(second_ticker_layer));
  layer_add_child(window_layer, text_layer_get_layer(footer_layer));
  layer_add_child(window_layer, text_layer_get_layer(date_layer));
}

static void prv_window_unload(Window *window) {
  text_layer_destroy(s_time_layer);
  text_layer_destroy(second_ticker_layer);
  text_layer_destroy(footer_layer);
  text_layer_destroy(date_layer);
  text_layer_destroy(background_top);
  text_layer_destroy(background_bottom);
}

static void prv_init(void) {
  
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);

  s_window = window_create();

    window_set_window_handlers(s_window, (WindowHandlers) {
    .load = prv_window_load,
    .unload = prv_window_unload,
  });

  window_stack_push(s_window, true);
  update_time();
}

static void prv_deinit(void) {
  window_destroy(s_window);
}

int main(void) {
  prv_init();
  app_event_loop();
  prv_deinit();
}