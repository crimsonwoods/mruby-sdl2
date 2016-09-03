#include "sdl2_mutex.h"
#include "mruby/value.h"
#include "mruby/data.h"
#include "mruby/class.h"

static struct RClass *class_Mutex     = NULL;
static struct RClass *class_Semaphore = NULL;
static struct RClass *class_Cond      = NULL;

typedef struct mrb_sdl2_mutex_data_t {
  SDL_mutex *mutex;
} mrb_sdl2_mutex_data_t;

typedef struct mrb_sdl2_semaphore_data_t {
  SDL_sem *semaphore;
} mrb_sdl2_semaphore_data_t;

typedef struct mrb_sdl2_cond_data_t {
  SDL_cond *cond;
} mrb_sdl2_cond_data_t;

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

static void
mrb_sdl2_semaphore_data_free(mrb_state *mrb, void *p)
{
  mrb_sdl2_semaphore_data_t *data =
    (mrb_sdl2_semaphore_data_t*)p;
  if (NULL != data) {
    if (NULL != data->semaphore) {
      SDL_DestroySemaphore(data->semaphore);
    }
    mrb_free(mrb, data);
  }
}

static void
mrb_sdl2_cond_data_free(mrb_state *mrb, void *p)
{
  mrb_sdl2_cond_data_t *data =
    (mrb_sdl2_cond_data_t*)p;
  if (NULL != data) {
    if (NULL != data->cond) {
      SDL_DestroyCond(data->cond);
    }
    mrb_free(mrb, data);
  }
}

static struct mrb_data_type mrb_sdl2_mutex_data_type = {
  "Mutex", &mrb_sdl2_mutex_data_free
};

static struct mrb_data_type mrb_sdl2_semaphore_data_type = {
  "Semaphore", &mrb_sdl2_semaphore_data_free
};

static struct mrb_data_type mrb_sdl2_cond_data_type = {
  "Cond", &mrb_sdl2_cond_data_free
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

/***************************************************************************
*
* class SDL2::Mutex
*
***************************************************************************/

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

/***************************************************************************
*
* class SDL2::Semaphore
*
***************************************************************************/

static mrb_value
mrb_sdl2_semaphore_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_semaphore_data_t *data =
    (mrb_sdl2_semaphore_data_t*)DATA_PTR(self);
  mrb_int initial_value = 0;
  mrb_get_args(mrb, "i", &initial_value);
  if (NULL != data) {
    data = (mrb_sdl2_semaphore_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_semaphore_data_t));
    if (NULL == data) {
      mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
    }
    data->semaphore = NULL;
  }
  data->semaphore = SDL_CreateSemaphore((Uint32)initial_value);
  if (NULL == data->semaphore) {
    mrb_free(mrb, data);
    mruby_sdl2_raise_error(mrb);
  }
  DATA_PTR(self)  = data;
  DATA_TYPE(self) = &mrb_sdl2_semaphore_data_type;
  return self;
}

static mrb_value
mrb_sdl2_semaphore_destroy(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_semaphore_data_t *data =
    (mrb_sdl2_semaphore_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_semaphore_data_type);
  if (NULL != data->semaphore) {
    SDL_DestroySemaphore(data->semaphore);
    data->semaphore = NULL;
  }
  return self;
}

static mrb_value
mrb_sdl2_semaphore_post(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_semaphore_data_t *data =
    (mrb_sdl2_semaphore_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_semaphore_data_type);
  if (NULL == data->semaphore) {
    return mrb_nil_value();
  }
  if (0 > SDL_SemPost(data->semaphore)) {
    mruby_sdl2_raise_error(mrb);
  }
  return self;
}

static mrb_value
mrb_sdl2_semaphore_wait(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_semaphore_data_t *data =
    (mrb_sdl2_semaphore_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_semaphore_data_type);
  if (NULL == data->semaphore) {
    return mrb_nil_value();
  }
  if (0 > SDL_SemWait(data->semaphore)) {
    mruby_sdl2_raise_error(mrb);
  }
  return self;
}

static mrb_value
mrb_sdl2_semaphore_try_wait(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_semaphore_data_t *data =
    (mrb_sdl2_semaphore_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_semaphore_data_type);
  if (NULL == data->semaphore) {
    return mrb_nil_value();
  }
  int const status = SDL_SemTryWait(data->semaphore);
  if (0 > status) {
    mruby_sdl2_raise_error(mrb);
  }
  if (0 == status) {
    return mrb_true_value();
  }
  return mrb_false_value();
}

static mrb_value
mrb_sdl2_semaphore_wait_timeout(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_semaphore_data_t *data =
    (mrb_sdl2_semaphore_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_semaphore_data_type);
  mrb_int ms;
  mrb_get_args(mrb, "i", &ms);
  if (NULL == data->semaphore) {
    return mrb_nil_value();
  }
  int const status = SDL_SemWaitTimeout(data->semaphore, (Uint32)ms);
  if (0 > status) {
    mruby_sdl2_raise_error(mrb);
  }
  if (0 == status) {
    return mrb_true_value();
  }
  return mrb_false_value();
}

static mrb_value
mrb_sdl2_semaphore_get_value(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_semaphore_data_t *data =
    (mrb_sdl2_semaphore_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_semaphore_data_type);
  if (NULL == data->semaphore) {
    return mrb_nil_value();
  }
  Uint32 const value = SDL_SemValue(data->semaphore);
  return mrb_fixnum_value((mrb_int)value);
}

/***************************************************************************
*
* class SDL2::Cond
*
***************************************************************************/

static mrb_value
mrb_sdl2_cond_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_cond_data_t *data =
    (mrb_sdl2_cond_data_t*)DATA_PTR(self);
  if (NULL == data) {
    data = (mrb_sdl2_cond_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_cond_data_t));
    if (NULL == data) {
      mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
    }
  }
  data->cond = SDL_CreateCond();
  if (NULL == data->cond) {
    mrb_free(mrb, data);
    mruby_sdl2_raise_error(mrb);
  }
  DATA_PTR(self) = data;
  DATA_TYPE(self) = &mrb_sdl2_cond_data_type;
  return self;
}

static mrb_value
mrb_sdl2_cond_destroy(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_cond_data_t *data =
    (mrb_sdl2_cond_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_cond_data_type);
  if (NULL != data->cond) {
    SDL_DestroyCond(data->cond);
    data->cond = NULL;
  }
  return self;
}

static mrb_value
mrb_sdl2_cond_broadcast(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_cond_data_t *data =
    (mrb_sdl2_cond_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_cond_data_type);
  if (NULL == data->cond) {
    return mrb_nil_value();
  }
  if (0 > SDL_CondBroadcast(data->cond)) {
    mruby_sdl2_raise_error(mrb);
  }
  return self;
}

static mrb_value
mrb_sdl2_cond_signal(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_cond_data_t *data =
    (mrb_sdl2_cond_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_cond_data_type);
  if (NULL == data->cond) {
    return mrb_nil_value();
  }
  if (0 > SDL_CondSignal(data->cond)) {
    mruby_sdl2_raise_error(mrb);
  }
  return self;
}

static mrb_value
mrb_sdl2_cond_wait(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_cond_data_t *data =
    (mrb_sdl2_cond_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_cond_data_type);
  if (NULL == data->cond) {
    return mrb_nil_value();
  }
  mrb_value mutex;
  mrb_get_args(mrb, "o", &mutex);
  mrb_sdl2_mutex_data_t *mutex_data =
    (mrb_sdl2_mutex_data_t*)mrb_data_get_ptr(mrb, mutex, &mrb_sdl2_mutex_data_type);
  if (0 > SDL_CondWait(data->cond, mutex_data->mutex)) {
    mruby_sdl2_raise_error(mrb);
  }
  return self;
}

static mrb_value
mrb_sdl2_cond_wait_timeout(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_cond_data_t *data =
    (mrb_sdl2_cond_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_cond_data_type);
  if (NULL == data->cond) {
    return mrb_nil_value();
  }
  mrb_value mutex;
  mrb_int ms;
  mrb_get_args(mrb, "oi", &mutex, &ms);
  mrb_sdl2_mutex_data_t *mutex_data =
    (mrb_sdl2_mutex_data_t*)mrb_data_get_ptr(mrb, mutex, &mrb_sdl2_mutex_data_type);
  if (0 > SDL_CondWaitTimeout(data->cond, mutex_data->mutex, (Uint32)ms)) {
    mruby_sdl2_raise_error(mrb);
  }
  return self;
}

void
mruby_sdl2_mutex_init(mrb_state *mrb)
{
  class_Mutex     = mrb_define_class_under(mrb, mod_SDL2, "Mutex",     mrb->object_class);
  class_Semaphore = mrb_define_class_under(mrb, mod_SDL2, "Semaphore", mrb->object_class);
  class_Cond      = mrb_define_class_under(mrb, mod_SDL2, "Cond",      mrb->object_class);

  MRB_SET_INSTANCE_TT(class_Mutex,     MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_Semaphore, MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_Cond,      MRB_TT_DATA);

  mrb_define_method(mrb, class_Mutex, "initialize", mrb_sdl2_mutex_initialize, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Mutex, "destroy",    mrb_sdl2_mutex_destroy,    MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Mutex, "lock",       mrb_sdl2_mutex_lock,       MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Mutex, "try_lock",   mrb_sdl2_mutex_try_lock,   MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Mutex, "unlock",     mrb_sdl2_mutex_unlock,     MRB_ARGS_NONE());

  mrb_define_method(mrb, class_Semaphore, "initialize",   mrb_sdl2_semaphore_initialize,   MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Semaphore, "destroy",      mrb_sdl2_semaphore_destroy,      MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Semaphore, "post",         mrb_sdl2_semaphore_post,         MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Semaphore, "wait",         mrb_sdl2_semaphore_wait,         MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Semaphore, "try_wait",     mrb_sdl2_semaphore_try_wait,     MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Semaphore, "wait_timeout", mrb_sdl2_semaphore_wait_timeout, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Semaphore, "value",        mrb_sdl2_semaphore_get_value,    MRB_ARGS_NONE());

  mrb_define_method(mrb, class_Cond, "initialize",   mrb_sdl2_cond_initialize,   MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Cond, "destroy",      mrb_sdl2_cond_destroy,      MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Cond, "broadcast",    mrb_sdl2_cond_broadcast,    MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Cond, "signal",       mrb_sdl2_cond_signal,       MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Cond, "wait",         mrb_sdl2_cond_wait,         MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Cond, "wait_timeout", mrb_sdl2_cond_wait_timeout, MRB_ARGS_REQ(2));
}

void
mruby_sdl2_mutex_final(mrb_state *mrb)
{
}

