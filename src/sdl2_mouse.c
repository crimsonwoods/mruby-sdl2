#include "sdl2_mouse.h"
#include "sdl2_video.h"
#include "sdl2_surface.h"
#include "sdl2_rect.h"
#include "mruby/value.h"
#include "mruby/data.h"
#include "mruby/class.h"

static struct RClass *mod_Mouse = NULL;
static struct RClass *class_Cursor = NULL;

typedef struct mrb_sdl2_input_mouse_cursor_data_t {
  bool        is_associated;
  SDL_Cursor *cursor;
} mrb_sdl2_input_mouse_cursor_data_t;

static void
mrb_sdl2_input_mouse_cursor_data_free(mrb_state *mrb, void *p)
{
  mrb_sdl2_input_mouse_cursor_data_t *data =
    (mrb_sdl2_input_mouse_cursor_data_t*)p;
  if (NULL != data) {
    if ((!data->is_associated) && (NULL != data->cursor)) {
      SDL_FreeCursor(data->cursor);
    }
    mrb_free(mrb, data);
  }
}

static struct mrb_data_type const mrb_sdl2_input_mouse_cursor_data_type = {
  "Cursor", &mrb_sdl2_input_mouse_cursor_data_free
};

SDL_Cursor *
mrb_sdl2_input_cursor_get_ptr(mrb_state *mrb, mrb_value cursor)
{
  if (mrb_nil_p(cursor)) {
    return NULL;
  }

  mrb_sdl2_input_mouse_cursor_data_t *data =
    (mrb_sdl2_input_mouse_cursor_data_t*)mrb_data_get_ptr(mrb, cursor, &mrb_sdl2_input_mouse_cursor_data_type);
  return data->cursor;
}

mrb_value
mrb_sdl2_input_cursor(mrb_state *mrb, SDL_Cursor *cursor)
{
  if (NULL == cursor) {
    return mrb_nil_value();
  }

  mrb_sdl2_input_mouse_cursor_data_t *data =
    (mrb_sdl2_input_mouse_cursor_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_input_mouse_cursor_data_t));
  if (NULL == data) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
  }
  data->is_associated = false;
  data->cursor = cursor;
  return mrb_obj_value(Data_Wrap_Struct(mrb, class_Cursor, &mrb_sdl2_input_mouse_cursor_data_type, data));
}

mrb_value
mrb_sdl2_input_associated_cursor(mrb_state *mrb, SDL_Cursor *cursor)
{
  if (NULL == cursor) {
    return mrb_nil_value();
  }

  mrb_sdl2_input_mouse_cursor_data_t *data =
    (mrb_sdl2_input_mouse_cursor_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_input_mouse_cursor_data_t));
  if (NULL == data) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
  }
  data->is_associated = true;
  data->cursor = cursor;
  return mrb_obj_value(Data_Wrap_Struct(mrb, class_Cursor, &mrb_sdl2_input_mouse_cursor_data_type, data));
}

/***************************************************************************
*
* module SDL2::Input::Mouse
*
***************************************************************************/

static mrb_value
mrb_sdl2_input_mouse_mouse_focus(mrb_state *mrb, mrb_value self)
{
  return mrb_sdl2_video_associated_window(mrb, SDL_GetMouseFocus());
}

static mrb_value
mrb_sdl2_input_mouse_mouse_state(mrb_state *mrb, mrb_value self)
{
  Uint32 state;
  if (SDL_FALSE == SDL_GetRelativeMouseMode()) {
    state = SDL_GetMouseState(NULL, NULL);
  } else {
    state = SDL_GetRelativeMouseState(NULL, NULL);
  }
  return mrb_fixnum_value(state);
}

static mrb_value
mrb_sdl2_input_mouse_get_location(mrb_state *mrb, mrb_value self)
{
  int x, y;
  if (SDL_FALSE == SDL_GetRelativeMouseMode()) {
    (void)SDL_GetMouseState(&x, &y);
  } else {
    (void)SDL_GetRelativeMouseState(&x, &y);
  }
  return mrb_sdl2_point(mrb, x, y);
}

static mrb_value
mrb_sdl2_input_mouse_get_x(mrb_state *mrb, mrb_value self)
{
  int x;
  if (SDL_FALSE == SDL_GetRelativeMouseMode()) {
    (void)SDL_GetMouseState(&x, NULL);
  } else {
    (void)SDL_GetRelativeMouseState(&x, NULL);
  }
  return mrb_fixnum_value(x);
}

static mrb_value
mrb_sdl2_input_mouse_get_y(mrb_state *mrb, mrb_value self)
{
  int y;
  if (SDL_FALSE == SDL_GetRelativeMouseMode()) {
    (void)SDL_GetMouseState(NULL, &y);
  } else {
    (void)SDL_GetRelativeMouseState(NULL, &y);
  }
  return mrb_fixnum_value(y);
}

static mrb_value
mrb_sdl2_input_mouse_is_relative(mrb_state *mrb, mrb_value self)
{
  return (SDL_FALSE == SDL_GetRelativeMouseMode()) ? mrb_false_value() : mrb_true_value();
}

static mrb_value
mrb_sdl2_input_mouse_set_relative(mrb_state *mrb, mrb_value self)
{
  mrb_bool is_relative;
  mrb_get_args(mrb, "b", &is_relative);
  if (0 > SDL_SetRelativeMouseMode(is_relative ? SDL_TRUE : SDL_FALSE)) {
    mruby_sdl2_raise_error(mrb);
  }
  return self;
}

static mrb_value
mrb_sdl2_input_mouse_get_cursor(mrb_state *mrb, mrb_value self)
{
  return mrb_sdl2_input_associated_cursor(mrb, SDL_GetCursor());
}

static mrb_value
mrb_sdl2_input_mouse_set_cursor(mrb_state *mrb, mrb_value self)
{
  mrb_value cursor;
  mrb_get_args(mrb, "o", &cursor);
  mrb_sdl2_input_mouse_cursor_data_t *data =
    (mrb_sdl2_input_mouse_cursor_data_t*)mrb_data_get_ptr(mrb, cursor, &mrb_sdl2_input_mouse_cursor_data_type);
  SDL_SetCursor(data->cursor);
  data->is_associated = true;
  return self;
}

static mrb_value
mrb_sdl2_input_mouse_show_cursor(mrb_state *mrb, mrb_value self)
{
  if (0 > SDL_ShowCursor(1)) {
    mruby_sdl2_raise_error(mrb);
  }
  return self;
}

static mrb_value
mrb_sdl2_input_mouse_hide_cursor(mrb_state *mrb, mrb_value self)
{
  if (0 > SDL_ShowCursor(0)) {
    mruby_sdl2_raise_error(mrb);
  }
  return self;
}

static mrb_value
mrb_sdl2_input_mouse_warp(mrb_state *mrb, mrb_value self)
{
  mrb_value window;
  mrb_int x, y;
  mrb_get_args(mrb, "oii", &window, &x, &y);
  SDL_Window *w = mrb_sdl2_video_window_get_ptr(mrb, window);
  SDL_WarpMouseInWindow(w, x, y);
  return self;
}

/***************************************************************************
*
* class SDL2::Input::Mouse::Cursor
*
***************************************************************************/

static mrb_value
mrb_sdl2_input_mouse_cursor_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_input_mouse_cursor_data_t *data =
    (mrb_sdl2_input_mouse_cursor_data_t*)DATA_PTR(self);

  if (NULL == data) {
    data = (mrb_sdl2_input_mouse_cursor_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_input_mouse_cursor_data_t));
    if (NULL == data) {
      mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
    }
    data->is_associated = false;
    data->cursor = NULL;
  }

  SDL_Cursor *cursor = NULL;
  if (3 == mrb->c->ci->argc) {
    mrb_value surface;
    mrb_int hot_x, hot_y;
    mrb_get_args(mrb, "oii", &surface, &hot_x, &hot_y);
    SDL_Surface *s = mrb_sdl2_video_surface_get_ptr(mrb, surface);
    cursor = SDL_CreateColorCursor(s, hot_x, hot_y);
  } else if (6 == mrb->c->ci->argc) {
    mrb_value data_ptr, mask_ptr;
    mrb_int w, h, hot_x, hot_y;
    mrb_get_args(mrb, "ooiiii", &data_ptr, &mask_ptr, &w, &h, &hot_x, &hot_y);
    cursor = SDL_CreateCursor(
      (Uint8 const *)mrb_cptr(data_ptr),
      (Uint8 const *)mrb_cptr(mask_ptr),
      w,
      h,
      hot_x,
      hot_y);
  } else {
    mrb_free(mrb, data);
    mrb_raise(mrb, E_ARGUMENT_ERROR, "wrong number of arguments.");
  }
  if (NULL == cursor) {
    mrb_free(mrb, data);
    mruby_sdl2_raise_error(mrb);
  }

  data->cursor = cursor;

  DATA_PTR(self) = data;
  DATA_TYPE(self) = &mrb_sdl2_input_mouse_cursor_data_type;
  return self;
}

static mrb_value
mrb_sdl2_input_mouse_cursor_free(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_input_mouse_cursor_data_t *data =
    (mrb_sdl2_input_mouse_cursor_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_input_mouse_cursor_data_type);
  if ((!data->is_associated) && (NULL != data->cursor)) {
    SDL_FreeCursor(data->cursor);
    data->cursor = NULL;
  }
  return self;
}


void
mruby_sdl2_mouse_init(mrb_state *mrb)
{
  struct RClass * mod_Input = mrb_module_get_under(mrb, mod_SDL2, "Input");

  mod_Mouse = mrb_define_module_under(mrb, mod_Input, "Mouse");

  class_Cursor = mrb_define_class_under(mrb, mod_Mouse, "Cursor", mrb->object_class);

  MRB_SET_INSTANCE_TT(class_Cursor, MRB_TT_DATA);

  mrb_define_module_function(mrb, mod_Mouse, "mouse_focus", mrb_sdl2_input_mouse_mouse_focus,  MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mod_Mouse, "mouse_state", mrb_sdl2_input_mouse_mouse_state,  MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mod_Mouse, "location",    mrb_sdl2_input_mouse_get_location, MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mod_Mouse, "x",           mrb_sdl2_input_mouse_get_x,        MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mod_Mouse, "y",           mrb_sdl2_input_mouse_get_y,        MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mod_Mouse, "relative?",   mrb_sdl2_input_mouse_is_relative,  MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mod_Mouse, "relative=",   mrb_sdl2_input_mouse_set_relative, MRB_ARGS_REQ(1));
  mrb_define_module_function(mrb, mod_Mouse, "cursor",      mrb_sdl2_input_mouse_get_cursor,   MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mod_Mouse, "cursor=",     mrb_sdl2_input_mouse_set_cursor,   MRB_ARGS_REQ(1));
  mrb_define_module_function(mrb, mod_Mouse, "cursor_show", mrb_sdl2_input_mouse_show_cursor,  MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mod_Mouse, "cursor_hide", mrb_sdl2_input_mouse_hide_cursor,  MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mod_Mouse, "warp",        mrb_sdl2_input_mouse_warp,         MRB_ARGS_REQ(2));

  mrb_define_method(mrb, class_Cursor, "initialize", mrb_sdl2_input_mouse_cursor_initialize, MRB_ARGS_REQ(3));
  mrb_define_method(mrb, class_Cursor, "free",       mrb_sdl2_input_mouse_cursor_free,       MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Cursor, "destroy",    mrb_sdl2_input_mouse_cursor_free,       MRB_ARGS_NONE());
}

void
mruby_sdl2_mouse_final(mrb_state *mrb)
{
}

