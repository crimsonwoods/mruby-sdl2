#include "sdl2_thread.h"
#include "mruby/value.h"
#include "mruby/data.h"
#include "mruby/proc.h"
#include "mruby/class.h"

static struct RClass *class_Thread;

typedef struct mrb_sdl2_thread_data_t {
  SDL_Thread *thread;
} mrb_sdl2_thread_data_t;

static void
mrb_sdl2_thread_data_free(mrb_state *mrb, void *p)
{
  mrb_sdl2_thread_data_t *data =
    (mrb_sdl2_thread_data_t*)p;
  if (NULL != data) {
    if (NULL != data->thread) {
      SDL_WaitThread(data->thread, NULL);
    }
    mrb_free(mrb, data);
  }
}

static struct mrb_data_type const mrb_sdl2_thread_data_type = {
  "Thread", mrb_sdl2_thread_data_free
};

SDL_Thread *
mrb_sdl2_thread_get_ptr(mrb_state *mrb, mrb_value value)
{
  if (mrb_nil_p(value)) {
    return NULL;
  }
  mrb_sdl2_thread_data_t *data = 
    (mrb_sdl2_thread_data_t*)mrb_data_get_ptr(mrb, value, &mrb_sdl2_thread_data_type);
  return data->thread;
}

mrb_value
mrb_sdl2_thread(mrb_state *mrb, SDL_Thread *thread)
{
  if (NULL == thread) {
    return mrb_nil_value();
  }

  mrb_sdl2_thread_data_t *data =
    (mrb_sdl2_thread_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_thread_data_t));
  if (NULL == data) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
  }
  data->thread = thread;
  return mrb_obj_value(Data_Wrap_Struct(mrb, class_Thread, &mrb_sdl2_thread_data_type, data));
}

typedef struct mrb_sdl2_thread_param_t {
  mrb_state *mrb;
  mrb_value  proc;
} mrb_sdl2_thread_param_t;

static int
mrb_sdl2_thread_function(void *data)
{
  mrb_sdl2_thread_param_t *param =
    (mrb_sdl2_thread_param_t*)data;
  mrb_state *mrb = param->mrb;
  mrb_value  proc = param->proc;

  mrb_free(mrb, param);

  /* open new RiteVM instance */
  mrb_state *thread_mrb = mrb_open_allocf(mrb->allocf, mrb->ud);

  /* back up each field to bring back after. */
  mrb_irep **irep         = thread_mrb->irep;
  size_t irep_len         = thread_mrb->irep_len;
  size_t irep_capa        = thread_mrb->irep_capa;
  struct kh_n2s *name2sym = thread_mrb->name2sym;

  /* set shared fields. */
  thread_mrb->irep      = mrb->irep;
  thread_mrb->irep_len  = mrb->irep_len;
  thread_mrb->irep_capa = mrb->irep_capa;
  thread_mrb->name2sym  = mrb->name2sym;

  mrb_value const ret = mrb_yield(thread_mrb, proc, mrb_nil_value());

  /* bring back each fields. */
  thread_mrb->irep      = irep;
  thread_mrb->irep_len  = irep_len;
  thread_mrb->irep_capa = irep_capa;
  thread_mrb->name2sym  = name2sym;

  /* shut down the VM */
  mrb_close(thread_mrb);

  if (mrb_type(ret) == MRB_TT_FIXNUM) {
    return mrb_fixnum(ret);
  }
  return 0;
}

static mrb_value
mrb_sdl2_thread_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_thread_data_t *data =
    (mrb_sdl2_thread_data_t*)DATA_PTR(self);

  if (NULL == data) {
    data = (mrb_sdl2_thread_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_thread_data_t));
    if (NULL == data) {
      mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
    }
    data->thread = NULL;
  }

  mrb_value proc;
  mrb_get_args(mrb, "&", &proc);

  mrb_sdl2_thread_param_t *param =
    (mrb_sdl2_thread_param_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_thread_param_t));
  if (NULL == param) {
    mrb_free(mrb, data);
    mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
  }
  param->mrb = mrb;
  param->proc = proc;

  data->thread = SDL_CreateThread(mrb_sdl2_thread_function, "SDL_Thread", param);
  if (NULL == data->thread) {
    mrb_free(mrb, param);
    mrb_free(mrb, data);
    mruby_sdl2_raise_error(mrb);
  }

  DATA_PTR(self) = data;
  DATA_TYPE(self) = &mrb_sdl2_thread_data_type;
  return self;
}

static mrb_value
mrb_sdl2_thread_wait(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_thread_data_t *data =
    (mrb_sdl2_thread_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_thread_data_type);
  if (NULL == data->thread) {
    return self;
  }
  int status = 0;
  SDL_WaitThread(data->thread, &status);
  data->thread = NULL;
  return mrb_fixnum_value(status);
}

void
mruby_sdl2_thread_init(mrb_state *mrb)
{
  class_Thread = mrb_define_class_under(mrb, mod_SDL2, "Thread", mrb->object_class);

  MRB_SET_INSTANCE_TT(class_Thread, MRB_TT_DATA);

  mrb_define_method(mrb, class_Thread, "initialize", mrb_sdl2_thread_initialize, ARGS_BLOCK());
  mrb_define_method(mrb, class_Thread, "wait",       mrb_sdl2_thread_wait,       ARGS_NONE());
  mrb_define_method(mrb, class_Thread, "join",       mrb_sdl2_thread_wait,       ARGS_NONE());
}

void
mruby_sdl2_thread_final(mrb_state *mrb)
{
}

