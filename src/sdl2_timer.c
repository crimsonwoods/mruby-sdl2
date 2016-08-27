#include "sdl2_timer.h"
#include "mruby/data.h"
#include "mruby/value.h"
#include "mruby/class.h"
#include "mruby/proc.h"
#include "threading.h"

static struct RClass *mod_Timer = NULL;

static mrb_value
mrb_sdl2_timer_get_ticks(mrb_state *mrb, mrb_value mod)
{
  Uint32 const ticks = SDL_GetTicks();
  if (ticks > MRB_INT_MAX) {
    return mrb_float_value(mrb, (mrb_float)ticks);
  }
  return mrb_fixnum_value((mrb_int)ticks);
}

static mrb_value
mrb_sdl2_timer_delay(mrb_state *mrb, mrb_value mod)
{
  mrb_value ms;
  mrb_get_args(mrb, "o", &ms);
  if (mrb_type(ms) == MRB_TT_FIXNUM) {
    SDL_Delay((Uint32)mrb_fixnum(ms));
  } else if (mrb_type(ms) == MRB_TT_FLOAT) {
    SDL_Delay((Uint32)mrb_float(ms));
  } else {
    mrb_raise(mrb, E_TYPE_ERROR, "expected Fixnum/Float");
  }
  return mod;
}

static mrb_value
mrb_sdl2_timer_get_performance_frequency(mrb_state *mrb, mrb_value mod)
{
  Uint64 const freq = SDL_GetPerformanceFrequency();
  if (freq > MRB_INT_MAX) {
    return mrb_float_value(mrb, (mrb_float)freq);
  }
  return mrb_fixnum_value((mrb_int)freq);
}

static mrb_value
mrb_sdl2_timer_get_performance_counter(mrb_state *mrb, mrb_value mod)
{
  Uint64 const counter = SDL_GetPerformanceCounter();
  if (counter > MRB_INT_MAX) {
    return mrb_float_value(mrb, (mrb_float)counter);
  }
  return mrb_fixnum_value((mrb_int)counter);
}

typedef struct mrb_sdl2_timer_param_t {
  mrb_state *mrb;
  mrb_value  callback;
} mrb_sdl2_timer_param_t;

static Uint32 mrb_sdl2_timer_callback(Uint32 interval, void *param)
{
  mrb_sdl2_timer_param_t *p = (mrb_sdl2_timer_param_t*)param;
  mrb_state *mrb = p->mrb;
  mrb_value  callback = p->callback;

  mrb_state *new_mrb = mrb_open_for_thread(mrb);
  if (NULL == new_mrb) {
    return 0;
  }

  mrb_value arg;
  mrb_value ret;
  if (interval > MRB_INT_MAX) {
    arg = mrb_float_value(new_mrb, interval);
  } else {
    arg = mrb_fixnum_value(interval);
  }
  ret = mrb_yield(new_mrb, callback, arg);

  mrb_close_for_thread(new_mrb);

  Uint32 next_interval;
  switch (mrb_type(ret)) {
  case MRB_TT_FIXNUM:
    next_interval = (Uint32)mrb_fixnum(ret);
    break;
  case MRB_TT_FLOAT:
    next_interval = (Uint32)mrb_float(ret);
    break;
  default:
    next_interval = 0; /* cancel next timer */
    break;
  }
  if (0 == next_interval) {
    mrb_free(mrb, p);
  }
  return next_interval;
}

static mrb_value
mrb_sdl2_timer_add(mrb_state *mrb, mrb_value mod)
{
  mrb_value interval, callback;
  mrb_get_args(mrb, "o&", &interval, &callback);
  if (mrb_nil_p(callback)) {
    mrb_raise(mrb, E_TYPE_ERROR, "non block argument is given.");
  }
  mrb_sdl2_timer_param_t *param = (mrb_sdl2_timer_param_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_timer_param_t));
  if (NULL == param) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
  }
  param->mrb = mrb;
  param->callback = callback;

  SDL_TimerID id = 0;
  switch (mrb_type(interval)) {
  case MRB_TT_FIXNUM:
    id = SDL_AddTimer((Uint32)mrb_fixnum(interval), mrb_sdl2_timer_callback, param);
    break;
  case MRB_TT_FLOAT:
    id = SDL_AddTimer((Uint32)mrb_float(interval), mrb_sdl2_timer_callback, param);
    break;
  default:
    mrb_free(mrb, param);
    mrb_raise(mrb, E_TYPE_ERROR, "expected Fixnum/Float");
    break;
  }

  if (0 == id) {
    mrb_free(mrb, param);
    mruby_sdl2_raise_error(mrb);
  }
  if (id > MRB_INT_MAX) {
    return mrb_float_value(mrb, id);
  }
  return mrb_fixnum_value(id);
}

static mrb_value
mrb_sdl2_timer_remove(mrb_state *mrb, mrb_value mod)
{
  mrb_value id;
  mrb_get_args(mrb, "o", &id);
  SDL_bool ret = SDL_FALSE;
  switch (mrb_type(id)) {
  case MRB_TT_FIXNUM:
    ret = SDL_RemoveTimer((SDL_TimerID)mrb_fixnum(id));
    break;
  case MRB_TT_FLOAT:
    ret = SDL_RemoveTimer((SDL_TimerID)mrb_float(id));
    break;
  default:
    mrb_raise(mrb, E_TYPE_ERROR, "expected Fixnum/Float");
    break;
  }
  return (SDL_FALSE == ret) ? mrb_false_value() : mrb_true_value();
}

void
mruby_sdl2_timer_init(mrb_state *mrb)
{
  mod_Timer = mrb_define_module_under(mrb, mod_SDL2, "Timer");

  mrb_define_module_function(mrb, mod_Timer, "ticks",        mrb_sdl2_timer_get_ticks,                 MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mod_Timer, "delay",        mrb_sdl2_timer_delay,                     MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mod_Timer, "perf_freq",    mrb_sdl2_timer_get_performance_frequency, MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mod_Timer, "perf_counter", mrb_sdl2_timer_get_performance_counter,   MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mod_Timer, "add",          mrb_sdl2_timer_add,                       MRB_ARGS_REQ(1) | MRB_ARGS_BLOCK());
  mrb_define_module_function(mrb, mod_Timer, "remove",       mrb_sdl2_timer_remove,                    MRB_ARGS_REQ(1));
}

void
mruby_sdl2_timer_final(mrb_state *mrb)
{
}

