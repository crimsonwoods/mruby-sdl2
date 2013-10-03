#include "sdl2_keyboard.h"
#include "mruby/value.h"
#include "mruby/data.h"
#include "mruby/class.h"
#include <SDL2/SDL_keycode.h>

static struct RClass *class_Keysym = NULL;

typedef struct mrb_sdl2_keyboard_keysym_data_t {
  SDL_Keysym keysym;
} mrb_sdl2_keyboard_keysym_data_t;

static void
mrb_sdl2_keyboard_keysym_data_free(mrb_state *mrb, void *p)
{
  mrb_free(mrb, p);
}

static struct mrb_data_type const mrb_sdl2_keyboard_keysym_data_type = {
  "Keysym", mrb_sdl2_keyboard_keysym_data_free
};

SDL_Keysym *
mrb_sdl2_keyboard_keysym_get_ptr(mrb_state *mrb, mrb_value value)
{
  if (mrb_nil_p(value)) {
    return NULL;
  }
  return &((mrb_sdl2_keyboard_keysym_data_t*)mrb_data_get_ptr(mrb, value, &mrb_sdl2_keyboard_keysym_data_type))->keysym;
}

mrb_value
mrb_sdl2_keyboard_keysym(mrb_state *mrb, SDL_Keysym const *keysym)
{
  mrb_sdl2_keyboard_keysym_data_t *data =
    (mrb_sdl2_keyboard_keysym_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_keyboard_keysym_data_t));
  if (NULL == data) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
  }
  data->keysym = *keysym;
  return mrb_obj_value(Data_Wrap_Struct(mrb, class_Keysym, &mrb_sdl2_keyboard_keysym_data_type, data));
}


static mrb_value
mrb_sdl2_keyboard_keysym_get_scancode(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_keyboard_keysym_data_t *data =
    (mrb_sdl2_keyboard_keysym_data_t*)mrb_sdl2_keyboard_keysym_get_ptr(mrb, self);
  return mrb_fixnum_value(data->keysym.scancode);
}

static mrb_value
mrb_sdl2_keyboard_keysym_get_symbol(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_keyboard_keysym_data_t *data =
    (mrb_sdl2_keyboard_keysym_data_t*)mrb_sdl2_keyboard_keysym_get_ptr(mrb, self);
  return mrb_fixnum_value(data->keysym.sym);
}

static mrb_value
mrb_sdl2_keyboard_keysym_get_modifier(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_keyboard_keysym_data_t *data =
    (mrb_sdl2_keyboard_keysym_data_t*)mrb_sdl2_keyboard_keysym_get_ptr(mrb, self);
  return mrb_fixnum_value(data->keysym.mod);
}


void
mruby_sdl2_keyboard_init(mrb_state *mrb)
{
  struct RClass *mod_Input = mrb_class_get_under(mrb, mod_SDL2, "Input");

  class_Keysym = mrb_define_class_under(mrb, mod_Input, "Keysym", mrb->object_class);

  MRB_SET_INSTANCE_TT(class_Keysym, MRB_TT_DATA);

  mrb_define_method(mrb, class_Keysym, "scancode", mrb_sdl2_keyboard_keysym_get_scancode, ARGS_NONE());
  mrb_define_method(mrb, class_Keysym, "symbol",   mrb_sdl2_keyboard_keysym_get_symbol,   ARGS_NONE());
  mrb_define_method(mrb, class_Keysym, "modifier", mrb_sdl2_keyboard_keysym_get_modifier, ARGS_NONE());

  /* SDL_Scancode */
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_0", mrb_fixnum_value(SDL_SCANCODE_0));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_1", mrb_fixnum_value(SDL_SCANCODE_1));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_2", mrb_fixnum_value(SDL_SCANCODE_2));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_3", mrb_fixnum_value(SDL_SCANCODE_3));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_4", mrb_fixnum_value(SDL_SCANCODE_4));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_5", mrb_fixnum_value(SDL_SCANCODE_5));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_6", mrb_fixnum_value(SDL_SCANCODE_6));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_7", mrb_fixnum_value(SDL_SCANCODE_7));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_8", mrb_fixnum_value(SDL_SCANCODE_8));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_9", mrb_fixnum_value(SDL_SCANCODE_9));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_A", mrb_fixnum_value(SDL_SCANCODE_A));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_B", mrb_fixnum_value(SDL_SCANCODE_B));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_C", mrb_fixnum_value(SDL_SCANCODE_C));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_D", mrb_fixnum_value(SDL_SCANCODE_D));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_E", mrb_fixnum_value(SDL_SCANCODE_E));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_F", mrb_fixnum_value(SDL_SCANCODE_F));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_G", mrb_fixnum_value(SDL_SCANCODE_G));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_H", mrb_fixnum_value(SDL_SCANCODE_H));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_I", mrb_fixnum_value(SDL_SCANCODE_I));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_J", mrb_fixnum_value(SDL_SCANCODE_J));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_K", mrb_fixnum_value(SDL_SCANCODE_K));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_L", mrb_fixnum_value(SDL_SCANCODE_L));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_M", mrb_fixnum_value(SDL_SCANCODE_M));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_N", mrb_fixnum_value(SDL_SCANCODE_N));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_O", mrb_fixnum_value(SDL_SCANCODE_O));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_P", mrb_fixnum_value(SDL_SCANCODE_P));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_Q", mrb_fixnum_value(SDL_SCANCODE_Q));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_R", mrb_fixnum_value(SDL_SCANCODE_R));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_S", mrb_fixnum_value(SDL_SCANCODE_S));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_T", mrb_fixnum_value(SDL_SCANCODE_T));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_U", mrb_fixnum_value(SDL_SCANCODE_U));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_V", mrb_fixnum_value(SDL_SCANCODE_V));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_W", mrb_fixnum_value(SDL_SCANCODE_W));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_X", mrb_fixnum_value(SDL_SCANCODE_X));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_Y", mrb_fixnum_value(SDL_SCANCODE_Y));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_Z", mrb_fixnum_value(SDL_SCANCODE_Z));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_F1", mrb_fixnum_value(SDL_SCANCODE_F1));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_F2", mrb_fixnum_value(SDL_SCANCODE_F2));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_F3", mrb_fixnum_value(SDL_SCANCODE_F3));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_F4", mrb_fixnum_value(SDL_SCANCODE_F4));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_F5", mrb_fixnum_value(SDL_SCANCODE_F5));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_F6", mrb_fixnum_value(SDL_SCANCODE_F6));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_F7", mrb_fixnum_value(SDL_SCANCODE_F7));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_F8", mrb_fixnum_value(SDL_SCANCODE_F8));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_F9", mrb_fixnum_value(SDL_SCANCODE_F9));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_F10", mrb_fixnum_value(SDL_SCANCODE_F10));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_F11", mrb_fixnum_value(SDL_SCANCODE_F11));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_F12", mrb_fixnum_value(SDL_SCANCODE_F12));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_F13", mrb_fixnum_value(SDL_SCANCODE_F13));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_F14", mrb_fixnum_value(SDL_SCANCODE_F14));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_F15", mrb_fixnum_value(SDL_SCANCODE_F15));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_F16", mrb_fixnum_value(SDL_SCANCODE_F16));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_F17", mrb_fixnum_value(SDL_SCANCODE_F17));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_F18", mrb_fixnum_value(SDL_SCANCODE_F18));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_F19", mrb_fixnum_value(SDL_SCANCODE_F19));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_F20", mrb_fixnum_value(SDL_SCANCODE_F20));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_F21", mrb_fixnum_value(SDL_SCANCODE_F21));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_F22", mrb_fixnum_value(SDL_SCANCODE_F22));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_F23", mrb_fixnum_value(SDL_SCANCODE_F23));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_F24", mrb_fixnum_value(SDL_SCANCODE_F24));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_BACKSPACE", mrb_fixnum_value(SDL_SCANCODE_BACKSPACE));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_CAPSLOCK", mrb_fixnum_value(SDL_SCANCODE_CAPSLOCK));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_DELETE", mrb_fixnum_value(SDL_SCANCODE_DELETE));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_ESCAPE", mrb_fixnum_value(SDL_SCANCODE_ESCAPE));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_END", mrb_fixnum_value(SDL_SCANCODE_END));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_HOME", mrb_fixnum_value(SDL_SCANCODE_HOME));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_INSERT", mrb_fixnum_value(SDL_SCANCODE_INSERT));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_LALT", mrb_fixnum_value(SDL_SCANCODE_LALT));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_LCTRL", mrb_fixnum_value(SDL_SCANCODE_LCTRL));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_LEFT", mrb_fixnum_value(SDL_SCANCODE_LEFT));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_LSHIFT", mrb_fixnum_value(SDL_SCANCODE_LSHIFT));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_PAGEDOWN", mrb_fixnum_value(SDL_SCANCODE_PAGEDOWN));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_PAGEUP", mrb_fixnum_value(SDL_SCANCODE_PAGEUP));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_PRINTSCREEN", mrb_fixnum_value(SDL_SCANCODE_PRINTSCREEN));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_RALT", mrb_fixnum_value(SDL_SCANCODE_RALT));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_RCTRL", mrb_fixnum_value(SDL_SCANCODE_RCTRL));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_RETURN", mrb_fixnum_value(SDL_SCANCODE_RETURN));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_RIGHT", mrb_fixnum_value(SDL_SCANCODE_RIGHT));
  mrb_define_const(mrb, mod_Input, "SDL_SCANCODE_RSHIFT", mrb_fixnum_value(SDL_SCANCODE_RSHIFT));
 
  /* SDL_Keycode */
  mrb_define_const(mrb, mod_Input, "SDLK_0", mrb_fixnum_value(SDLK_0));
  mrb_define_const(mrb, mod_Input, "SDLK_1", mrb_fixnum_value(SDLK_1));
  mrb_define_const(mrb, mod_Input, "SDLK_2", mrb_fixnum_value(SDLK_2));
  mrb_define_const(mrb, mod_Input, "SDLK_3", mrb_fixnum_value(SDLK_3));
  mrb_define_const(mrb, mod_Input, "SDLK_4", mrb_fixnum_value(SDLK_4));
  mrb_define_const(mrb, mod_Input, "SDLK_5", mrb_fixnum_value(SDLK_5));
  mrb_define_const(mrb, mod_Input, "SDLK_6", mrb_fixnum_value(SDLK_6));
  mrb_define_const(mrb, mod_Input, "SDLK_7", mrb_fixnum_value(SDLK_7));
  mrb_define_const(mrb, mod_Input, "SDLK_8", mrb_fixnum_value(SDLK_8));
  mrb_define_const(mrb, mod_Input, "SDLK_9", mrb_fixnum_value(SDLK_9));
  mrb_define_const(mrb, mod_Input, "SDLK_A", mrb_fixnum_value(SDLK_a));
  mrb_define_const(mrb, mod_Input, "SDLK_B", mrb_fixnum_value(SDLK_b));
  mrb_define_const(mrb, mod_Input, "SDLK_C", mrb_fixnum_value(SDLK_c));
  mrb_define_const(mrb, mod_Input, "SDLK_D", mrb_fixnum_value(SDLK_d));
  mrb_define_const(mrb, mod_Input, "SDLK_E", mrb_fixnum_value(SDLK_e));
  mrb_define_const(mrb, mod_Input, "SDLK_F", mrb_fixnum_value(SDLK_f));
  mrb_define_const(mrb, mod_Input, "SDLK_G", mrb_fixnum_value(SDLK_g));
  mrb_define_const(mrb, mod_Input, "SDLK_H", mrb_fixnum_value(SDLK_h));
  mrb_define_const(mrb, mod_Input, "SDLK_I", mrb_fixnum_value(SDLK_i));
  mrb_define_const(mrb, mod_Input, "SDLK_J", mrb_fixnum_value(SDLK_j));
  mrb_define_const(mrb, mod_Input, "SDLK_K", mrb_fixnum_value(SDLK_k));
  mrb_define_const(mrb, mod_Input, "SDLK_L", mrb_fixnum_value(SDLK_l));
  mrb_define_const(mrb, mod_Input, "SDLK_M", mrb_fixnum_value(SDLK_m));
  mrb_define_const(mrb, mod_Input, "SDLK_N", mrb_fixnum_value(SDLK_n));
  mrb_define_const(mrb, mod_Input, "SDLK_O", mrb_fixnum_value(SDLK_o));
  mrb_define_const(mrb, mod_Input, "SDLK_P", mrb_fixnum_value(SDLK_p));
  mrb_define_const(mrb, mod_Input, "SDLK_Q", mrb_fixnum_value(SDLK_q));
  mrb_define_const(mrb, mod_Input, "SDLK_R", mrb_fixnum_value(SDLK_r));
  mrb_define_const(mrb, mod_Input, "SDLK_S", mrb_fixnum_value(SDLK_s));
  mrb_define_const(mrb, mod_Input, "SDLK_T", mrb_fixnum_value(SDLK_t));
  mrb_define_const(mrb, mod_Input, "SDLK_U", mrb_fixnum_value(SDLK_u));
  mrb_define_const(mrb, mod_Input, "SDLK_V", mrb_fixnum_value(SDLK_v));
  mrb_define_const(mrb, mod_Input, "SDLK_W", mrb_fixnum_value(SDLK_w));
  mrb_define_const(mrb, mod_Input, "SDLK_X", mrb_fixnum_value(SDLK_x));
  mrb_define_const(mrb, mod_Input, "SDLK_Y", mrb_fixnum_value(SDLK_y));
  mrb_define_const(mrb, mod_Input, "SDLK_Z", mrb_fixnum_value(SDLK_z));
  mrb_define_const(mrb, mod_Input, "SDLK_F1", mrb_fixnum_value(SDLK_F1));
  mrb_define_const(mrb, mod_Input, "SDLK_F2", mrb_fixnum_value(SDLK_F2));
  mrb_define_const(mrb, mod_Input, "SDLK_F3", mrb_fixnum_value(SDLK_F3));
  mrb_define_const(mrb, mod_Input, "SDLK_F4", mrb_fixnum_value(SDLK_F4));
  mrb_define_const(mrb, mod_Input, "SDLK_F5", mrb_fixnum_value(SDLK_F5));
  mrb_define_const(mrb, mod_Input, "SDLK_F6", mrb_fixnum_value(SDLK_F6));
  mrb_define_const(mrb, mod_Input, "SDLK_F7", mrb_fixnum_value(SDLK_F7));
  mrb_define_const(mrb, mod_Input, "SDLK_F8", mrb_fixnum_value(SDLK_F8));
  mrb_define_const(mrb, mod_Input, "SDLK_F9", mrb_fixnum_value(SDLK_F9));
  mrb_define_const(mrb, mod_Input, "SDLK_F10", mrb_fixnum_value(SDLK_F10));
  mrb_define_const(mrb, mod_Input, "SDLK_F11", mrb_fixnum_value(SDLK_F11));
  mrb_define_const(mrb, mod_Input, "SDLK_F12", mrb_fixnum_value(SDLK_F12));
  mrb_define_const(mrb, mod_Input, "SDLK_F13", mrb_fixnum_value(SDLK_F13));
  mrb_define_const(mrb, mod_Input, "SDLK_F14", mrb_fixnum_value(SDLK_F14));
  mrb_define_const(mrb, mod_Input, "SDLK_F15", mrb_fixnum_value(SDLK_F15));
  mrb_define_const(mrb, mod_Input, "SDLK_F16", mrb_fixnum_value(SDLK_F16));
  mrb_define_const(mrb, mod_Input, "SDLK_F17", mrb_fixnum_value(SDLK_F17));
  mrb_define_const(mrb, mod_Input, "SDLK_F18", mrb_fixnum_value(SDLK_F18));
  mrb_define_const(mrb, mod_Input, "SDLK_F19", mrb_fixnum_value(SDLK_F19));
  mrb_define_const(mrb, mod_Input, "SDLK_F20", mrb_fixnum_value(SDLK_F20));
  mrb_define_const(mrb, mod_Input, "SDLK_F21", mrb_fixnum_value(SDLK_F21));
  mrb_define_const(mrb, mod_Input, "SDLK_F22", mrb_fixnum_value(SDLK_F22));
  mrb_define_const(mrb, mod_Input, "SDLK_F23", mrb_fixnum_value(SDLK_F23));
  mrb_define_const(mrb, mod_Input, "SDLK_F24", mrb_fixnum_value(SDLK_F24));
  mrb_define_const(mrb, mod_Input, "SDLK_BACKSPACE",   mrb_fixnum_value(SDLK_BACKSPACE));
  mrb_define_const(mrb, mod_Input, "SDLK_CAPSLOCK",    mrb_fixnum_value(SDLK_CAPSLOCK));
  mrb_define_const(mrb, mod_Input, "SDLK_DELETE",      mrb_fixnum_value(SDLK_DELETE));
  mrb_define_const(mrb, mod_Input, "SDLK_ESCAPE",      mrb_fixnum_value(SDLK_ESCAPE));
  mrb_define_const(mrb, mod_Input, "SDLK_END",         mrb_fixnum_value(SDLK_END));
  mrb_define_const(mrb, mod_Input, "SDLK_HOME",        mrb_fixnum_value(SDLK_HOME));
  mrb_define_const(mrb, mod_Input, "SDLK_INSERT",      mrb_fixnum_value(SDLK_INSERT));
  mrb_define_const(mrb, mod_Input, "SDLK_LALT",        mrb_fixnum_value(SDLK_LALT));
  mrb_define_const(mrb, mod_Input, "SDLK_LCTRL",       mrb_fixnum_value(SDLK_LCTRL));
  mrb_define_const(mrb, mod_Input, "SDLK_LEFT",        mrb_fixnum_value(SDLK_LEFT));
  mrb_define_const(mrb, mod_Input, "SDLK_LSHIFT",      mrb_fixnum_value(SDLK_LSHIFT));
  mrb_define_const(mrb, mod_Input, "SDLK_PAGEDOWN",    mrb_fixnum_value(SDLK_PAGEDOWN));
  mrb_define_const(mrb, mod_Input, "SDLK_PAGEUP",      mrb_fixnum_value(SDLK_PAGEUP));
  mrb_define_const(mrb, mod_Input, "SDLK_PRINTSCREEN", mrb_fixnum_value(SDLK_PRINTSCREEN));
  mrb_define_const(mrb, mod_Input, "SDLK_RALT",        mrb_fixnum_value(SDLK_RALT));
  mrb_define_const(mrb, mod_Input, "SDLK_RCTRL",       mrb_fixnum_value(SDLK_RCTRL));
  mrb_define_const(mrb, mod_Input, "SDLK_RETURN",      mrb_fixnum_value(SDLK_RETURN));
  mrb_define_const(mrb, mod_Input, "SDLK_RIGHT",       mrb_fixnum_value(SDLK_RIGHT));
  mrb_define_const(mrb, mod_Input, "SDLK_RSHIFT",      mrb_fixnum_value(SDLK_RSHIFT));
  mrb_define_const(mrb, mod_Input, "SDLK_AMPERSAND",   mrb_fixnum_value(SDLK_AMPERSAND));
  mrb_define_const(mrb, mod_Input, "SDLK_ASTERISK",    mrb_fixnum_value(SDLK_ASTERISK));
  mrb_define_const(mrb, mod_Input, "SDLK_AT",          mrb_fixnum_value(SDLK_AT));
  mrb_define_const(mrb, mod_Input, "SDLK_CARET",       mrb_fixnum_value(SDLK_CARET));
  mrb_define_const(mrb, mod_Input, "SDLK_COLON",       mrb_fixnum_value(SDLK_COLON));
  mrb_define_const(mrb, mod_Input, "SDLK_DOLLAR",      mrb_fixnum_value(SDLK_DOLLAR));
  mrb_define_const(mrb, mod_Input, "SDLK_EXCLAIM",     mrb_fixnum_value(SDLK_EXCLAIM));
  mrb_define_const(mrb, mod_Input, "SDLK_GREATER",     mrb_fixnum_value(SDLK_GREATER));
  mrb_define_const(mrb, mod_Input, "SDLK_HASH",        mrb_fixnum_value(SDLK_HASH));
  mrb_define_const(mrb, mod_Input, "SDLK_LEFTPAREN",   mrb_fixnum_value(SDLK_LEFTPAREN));
  mrb_define_const(mrb, mod_Input, "SDLK_LESS",        mrb_fixnum_value(SDLK_LESS));
  mrb_define_const(mrb, mod_Input, "SDLK_PERCENT",     mrb_fixnum_value(SDLK_PERCENT));
  mrb_define_const(mrb, mod_Input, "SDLK_PLUS",        mrb_fixnum_value(SDLK_PLUS));
  mrb_define_const(mrb, mod_Input, "SDLK_QUESTION",    mrb_fixnum_value(SDLK_QUESTION));
  mrb_define_const(mrb, mod_Input, "SDLK_QUOTEDBL",    mrb_fixnum_value(SDLK_QUOTEDBL));
  mrb_define_const(mrb, mod_Input, "SDLK_RIGHTPAREN",  mrb_fixnum_value(SDLK_RIGHTPAREN));
  mrb_define_const(mrb, mod_Input, "SDLK_UNDERSCORE",  mrb_fixnum_value(SDLK_UNDERSCORE));
}

void
mruby_sdl2_keyboard_final(mrb_state *mrb)
{
}

