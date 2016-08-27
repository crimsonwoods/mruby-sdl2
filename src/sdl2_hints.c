#include "sdl2_hints.h"
#include "mruby/string.h"

static mrb_value
mrb_sdl2_hints_clear(mrb_state *mrb, mrb_value mod)
{
  SDL_ClearHints();
  return mod;
}

static mrb_value
mrb_sdl2_hints_get(mrb_state *mrb, mrb_value mod)
{
  mrb_value name;
  mrb_get_args(mrb, "S", &name);
  if (mrb_nil_p(name)) {
    return mrb_nil_value();
  }
  char const *value = SDL_GetHint(RSTRING_PTR(name));
  if (NULL == value) {
    return mrb_nil_value();
  }
  return mrb_str_new_cstr(mrb, value);
}

static mrb_value
mrb_sdl2_hints_set(mrb_state *mrb, mrb_value mod)
{
  mrb_value name, value;
  mrb_get_args(mrb, "So", &name, &value);
  if (mrb_nil_p(name) || mrb_nil_p(value)) {
    return mrb_false_value();
  }
  if (mrb_type(value) != MRB_TT_STRING) {
    if (mrb_respond_to(mrb, value, mrb_intern(mrb, "to_s", 4))) {
      value = mrb_funcall(mrb, value, "to_s", 0);
      if (mrb_nil_p(value)) {
        return mrb_false_value();
      }
    } else {
      mrb_raise(mrb, E_TYPE_ERROR, "expect String");
    }
  }
  SDL_bool const ret = SDL_SetHint(RSTRING_PTR(name), RSTRING_PTR(value));
  return (SDL_FALSE == ret) ? mrb_false_value() : mrb_true_value();
}

static mrb_value
mrb_sdl2_hints_set_with_priority(mrb_state *mrb, mrb_value mod)
{
  mrb_value name, value;
  mrb_int priority;
  mrb_get_args(mrb, "Soi", &name, &value, &priority);
  if (mrb_nil_p(name) || mrb_nil_p(value)) {
    return mrb_false_value();
  }
  if (mrb_type(value) != MRB_TT_STRING) {
    if (mrb_respond_to(mrb, value, mrb_intern(mrb, "to_s", 4))) {
      value = mrb_funcall(mrb, value, "to_s", 0);
      if (mrb_nil_p(value)) {
        return mrb_false_value();
      }
    } else {
      mrb_raise(mrb, E_TYPE_ERROR, "expect String");
    }
  }
  SDL_bool const ret = SDL_SetHintWithPriority(RSTRING_PTR(name), RSTRING_PTR(value), (SDL_HintPriority)priority);
  return (SDL_FALSE == ret) ? mrb_false_value() : mrb_true_value();
}

void
mruby_sdl2_hints_init(mrb_state *mrb)
{
  struct RClass *mod_Hints = mrb_define_module_under(mrb, mod_SDL2, "Hints");

  mrb_define_module_function(mrb, mod_Hints, "clear",             mrb_sdl2_hints_clear,             MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mod_Hints, "get",               mrb_sdl2_hints_get,               MRB_ARGS_REQ(1));
  mrb_define_module_function(mrb, mod_Hints, "set",               mrb_sdl2_hints_set,               MRB_ARGS_REQ(2));
  mrb_define_module_function(mrb, mod_Hints, "set_with_priority", mrb_sdl2_hints_set_with_priority, MRB_ARGS_REQ(3));

  /* SDL_HintPriority */
  int arena_size = mrb_gc_arena_save(mrb);
  mrb_define_const(mrb, mod_Hints, "SDL_HINT_DEFAULT",  mrb_fixnum_value(SDL_HINT_DEFAULT));
  mrb_define_const(mrb, mod_Hints, "SDL_HINT_NORMAL",   mrb_fixnum_value(SDL_HINT_NORMAL));
  mrb_define_const(mrb, mod_Hints, "SDL_HINT_OVERRIDE", mrb_fixnum_value(SDL_HINT_OVERRIDE));
  mrb_gc_arena_restore(mrb, arena_size);

  arena_size = mrb_gc_arena_save(mrb);
  mrb_define_const(mrb, mod_Hints, "SDL_HINT_FRAMEBUFFER_ACCELERATION",         mrb_str_new_cstr(mrb, SDL_HINT_FRAMEBUFFER_ACCELERATION));
  mrb_define_const(mrb, mod_Hints, "SDL_HINT_IDLE_TIMER_DISABLED",              mrb_str_new_cstr(mrb, SDL_HINT_IDLE_TIMER_DISABLED));
  mrb_define_const(mrb, mod_Hints, "SDL_HINT_ORIENTATIONS",                     mrb_str_new_cstr(mrb, SDL_HINT_ORIENTATIONS));
  mrb_define_const(mrb, mod_Hints, "SDL_HINT_RENDER_DRIVER",                    mrb_str_new_cstr(mrb, SDL_HINT_RENDER_DRIVER));
  mrb_define_const(mrb, mod_Hints, "SDL_HINT_RENDER_OPENGL_SHADERS",            mrb_str_new_cstr(mrb, SDL_HINT_RENDER_OPENGL_SHADERS));
  mrb_define_const(mrb, mod_Hints, "SDL_HINT_RENDER_SCALE_QUALITY",             mrb_str_new_cstr(mrb, SDL_HINT_RENDER_SCALE_QUALITY));
  mrb_define_const(mrb, mod_Hints, "SDL_HINT_RENDER_VSYNC",                     mrb_str_new_cstr(mrb, SDL_HINT_RENDER_VSYNC));
  mrb_define_const(mrb, mod_Hints, "SDL_HINT_VIDEO_X11_XVIDMODE",               mrb_str_new_cstr(mrb, SDL_HINT_VIDEO_X11_XVIDMODE));
  mrb_define_const(mrb, mod_Hints, "SDL_HINT_VIDEO_X11_XINERAMA",               mrb_str_new_cstr(mrb, SDL_HINT_VIDEO_X11_XINERAMA));
  mrb_define_const(mrb, mod_Hints, "SDL_HINT_VIDEO_X11_XRANDR",                 mrb_str_new_cstr(mrb, SDL_HINT_VIDEO_X11_XRANDR));
  mrb_define_const(mrb, mod_Hints, "SDL_HINT_GRAB_KEYBOARD",                    mrb_str_new_cstr(mrb, SDL_HINT_GRAB_KEYBOARD));
  mrb_define_const(mrb, mod_Hints, "SDL_HINT_VIDEO_MINIMIZE_ON_FOCUS_LOSS",     mrb_str_new_cstr(mrb, SDL_HINT_VIDEO_MINIMIZE_ON_FOCUS_LOSS));
  mrb_define_const(mrb, mod_Hints, "SDL_HINT_IDLE_TIMER_DISABLED",              mrb_str_new_cstr(mrb, SDL_HINT_IDLE_TIMER_DISABLED));
  mrb_define_const(mrb, mod_Hints, "SDL_HINT_ORIENTATIONS",                     mrb_str_new_cstr(mrb, SDL_HINT_ORIENTATIONS));
  mrb_define_const(mrb, mod_Hints, "SDL_HINT_XINPUT_ENABLED",                   mrb_str_new_cstr(mrb, SDL_HINT_XINPUT_ENABLED));
  mrb_define_const(mrb, mod_Hints, "SDL_HINT_GAMECONTROLLERCONFIG",             mrb_str_new_cstr(mrb, SDL_HINT_GAMECONTROLLERCONFIG));
  mrb_define_const(mrb, mod_Hints, "SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS", mrb_str_new_cstr(mrb, SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS));
  mrb_define_const(mrb, mod_Hints, "SDL_HINT_ALLOW_TOPMOST",                    mrb_str_new_cstr(mrb, SDL_HINT_ALLOW_TOPMOST));
  mrb_define_const(mrb, mod_Hints, "SDL_HINT_TIMER_RESOLUTION",                 mrb_str_new_cstr(mrb, SDL_HINT_TIMER_RESOLUTION));
  mrb_gc_arena_restore(mrb, arena_size);
}

void
mruby_sdl2_hints_final(mrb_state *mrb)
{
}

