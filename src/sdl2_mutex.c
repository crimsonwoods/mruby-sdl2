#include "sdl2_mutex.h"
#include "mruby/value.h"
#include "mruby/data.h"
#include "mruby/class.h"

static struct RClass *class_Mutex;

typedef struct mrb_sdl2_mutex_data_t {
  SDL_mutex *mutex;
} mrb_sdl2_mutex_data_t;

static void
mrb_sdl2_mutex_data_free(mrb_state *mrb, void *p)
{
  mrb_sdl2_mutex_data_t *data =
    (mrb_sdl2_mutex_data_t*)p;
  if (NULL != data) {
    if (NULL != data->mutex) {
      SDL_DestroyMutex(data->mutex);
    }
    mrb_free(mrb, data);
  }
}

static struct mrb_data_type mrb_sdl2_mutex_data_type = {
  "Mutex", &mrb_sdl2_mutex_data_free
};

SDL_mutex *
mrb_sdl2_mutex_get_ptr(mrb_state *mrb, mrb_value mutex)
{
  if (mrb_nil_p(mutex)) {
    return NULL;
  }
  mrb_sdl2_mutex_data_t *data =
    (mrb_sdl2_mutex_data_t*)mrb_data_get_ptr(mrb, mutex, &mrb_sdl2_mutex_data_type);
  return data->mutex;
}

mrb_value
mrb_sdl2_mutex(mrb_state *mrb, SDL_mutex *mutex)
{
  if (NULL == mutex) {
    return mrb_nil_value();
  }
  mrb_sdl2_mutex_data_t *data =
    (mrb_sdl2_mutex_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_mutex_data_t));
  if (NULL == data) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
  }
  data->mutex = mutex;
  return mrb_obj_value(Data_Wrap_Struct(mrb, class_Mutex, &mrb_sdl2_mutex_data_type, data));
}


static mrb_value
mrb_sdl2_mutex_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_mutex_data_t *data =
    (mrb_sdl2_mutex_data_t*)DATA_PTR(self);
  if (NULL == data) {
    data = (mrb_sdl2_mutex_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_mutex_data_t));
    if (NULL == data) {
      mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
    }
    data->mutex = NULL;
  }
  data->mutex = SDL_CreateMutex();
  if (NULL == data->mutex) {
    mrb_free(mrb, data);
    mruby_sdl2_raise_error(mrb);
  }
  DATA_PTR(self) = data;
  DATA_TYPE(self) = &mrb_sdl2_mutex_data_type;
  return self;
}

static mrb_value
mrb_sdl2_mutex_destroy(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_mutex_data_t *data =
    (mrb_sdl2_mutex_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_mutex_data_type);
  if (NULL != data->mutex) {
    SDL_DestroyMutex(data->mutex);
    data->mutex = NULL;
  }
  return self;
}

static mrb_value
mrb_sdl2_mutex_lock(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_mutex_data_t *data =
    (mrb_sdl2_mutex_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_mutex_data_type);
  if (NULL != data->mutex) {
    if (0 > SDL_LockMutex(data->mutex)) {
      mruby_sdl2_raise_error(mrb);
    }
  }
  return self;
}

static mrb_value
mrb_sdl2_mutex_try_lock(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_mutex_data_t *data =
    (mrb_sdl2_mutex_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_mutex_data_type);
  if (NULL != data->mutex) {
    int const status = SDL_TryLockMutex(data->mutex);
    if (0 == status) {
      return mrb_true_value();
    }
    if (SDL_MUTEX_TIMEDOUT == status) {
      return mrb_false_value();
    }
    mruby_sdl2_raise_error(mrb);
  }
  return self;
}

static mrb_value
mrb_sdl2_mutex_unlock(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_mutex_data_t *data =
    (mrb_sdl2_mutex_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_mutex_data_type);
  if (NULL != data->mutex) {
    if (0 > SDL_UnlockMutex(data->mutex)) {
      mruby_sdl2_raise_error(mrb);
    }
  }
  return self;
}


void
mruby_sdl2_mutex_init(mrb_state *mrb)
{
  class_Mutex = mrb_define_class_under(mrb, mod_SDL2, "Mutex", mrb->object_class);

  MRB_SET_INSTANCE_TT(class_Mutex, MRB_TT_DATA);

  mrb_define_method(mrb, class_Mutex, "initialize", mrb_sdl2_mutex_initialize, ARGS_NONE());
  mrb_define_method(mrb, class_Mutex, "destroy",    mrb_sdl2_mutex_destroy,    ARGS_NONE());
  mrb_define_method(mrb, class_Mutex, "lock",       mrb_sdl2_mutex_lock,       ARGS_NONE());
  mrb_define_method(mrb, class_Mutex, "try_lock",   mrb_sdl2_mutex_try_lock,   ARGS_NONE());
  mrb_define_method(mrb, class_Mutex, "unlock",     mrb_sdl2_mutex_unlock,     ARGS_NONE());
}

void
mruby_sdl2_mutex_final(mrb_state *mrb)
{
}

