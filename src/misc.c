#include "misc.h"
#include "mruby/class.h"
#include "mruby/value.h"
#include "mruby/data.h"
#include "mruby/array.h"

static struct RClass *class_Buffer = NULL;
static struct RClass *class_FloatBuffer = NULL;

typedef struct mrb_sdl2_misc_buffer_data_t {
  void  *buffer;
  size_t size;
} mrb_sdl2_misc_buffer_data_t;

typedef struct mrb_sdl2_misc_floatbuffer_data_t {
  float *buffer;
  size_t size;
} mrb_sdl2_misc_floatbuffer_data_t;

static void
mrb_sdl2_misc_buffer_data_free(mrb_state *mrb, void *p)
{
  mrb_sdl2_misc_buffer_data_t *data =
    (mrb_sdl2_misc_buffer_data_t*)p;
  if (NULL != data) {
    mrb_free(mrb, data->buffer);
    mrb_free(mrb, p);
  }
}

static void
mrb_sdl2_misc_floatbuffer_data_free(mrb_state *mrb, void *p)
{
  mrb_sdl2_misc_floatbuffer_data_t *data =
    (mrb_sdl2_misc_floatbuffer_data_t*)p;
  if (NULL != data) {
    mrb_free(mrb, data->buffer);
    mrb_free(mrb, p);
  }
}

static struct mrb_data_type const mrb_sdl2_misc_buffer_data_type = {
  "Buffer", &mrb_sdl2_misc_buffer_data_free
};

static struct mrb_data_type const mrb_sdl2_misc_floatbuffer_data_type = {
  "FloatBuffer", &mrb_sdl2_misc_floatbuffer_data_free
};

static mrb_value
mrb_sdl2_misc_buffer_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_value arg;
  mrb_get_args(mrb, "o", &arg);

  mrb_sdl2_misc_buffer_data_t *data =
    (mrb_sdl2_misc_buffer_data_t*)DATA_PTR(self);

  if (NULL == data) {
    data = (mrb_sdl2_misc_buffer_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_misc_buffer_data_t));
    if (NULL == data) {
      mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
    }
    data->buffer = NULL;
    data->size   = 0;
  }

  enum mrb_vtype const arg_type = mrb_type(arg);
  switch (arg_type) {
  case MRB_TT_FIXNUM:
    data->size = (size_t)mrb_fixnum(arg);
    break;
  case MRB_TT_FLOAT:
    data->size = (size_t)mrb_float(arg);
    break;
  case MRB_TT_STRING:
    data->size = (size_t)mrb_float(mrb_funcall(mrb, arg, "to_f", 0));
    break;
  case MRB_TT_ARRAY:
    {
      mrb_int const n = mrb_ary_len(mrb, arg);
      if (0 == n) {
        mrb_raise(mrb, E_ARGUMENT_ERROR, "cannot accept empty array.");
      }
      mrb_value const item = mrb_ary_ref(mrb, arg, 0);
      enum mrb_vtype const item_vtype = mrb_type(item);
      switch (item_vtype) {
      case MRB_TT_FIXNUM:
        data->size = n * sizeof(mrb_int);
        break;
      case MRB_TT_FLOAT:
        data->size = n * sizeof(mrb_float);
        break;
      case MRB_TT_STRING:
        data->size = n * sizeof(mrb_float);
        break;
      default:
        if (mrb_respond_to(mrb, item, mrb_intern2(mrb, "to_f", 4))) {
          data->size = n * sizeof(mrb_float);
        } else if (mrb_respond_to(mrb, item, mrb_intern2(mrb, "to_i", 4))) {
          data->size = n * sizeof(mrb_int);
        } else {
          mrb_raise(mrb, E_TYPE_ERROR, "expected Fixnum/Float/String or convertible type");
        }
        break;
      }
    }
    break;
  default:
    if (mrb_respond_to(mrb, arg, mrb_intern2(mrb, "to_f", 4))) {
      data->size = (size_t)mrb_float(mrb_funcall(mrb, arg, "to_f", 0));
    } else if (mrb_respond_to(mrb, arg, mrb_intern2(mrb, "to_i", 4))) {
      data->size = (size_t)mrb_fixnum(mrb_funcall(mrb, arg, "to_i", 0));
    } else {
      mrb_raise(mrb, E_TYPE_ERROR, "expected Fixnum/Float/String/Array or comvertible type");
    }
    break;
  }

  data->buffer = mrb_malloc(mrb, data->size);
  if (NULL == data->buffer) {
    mrb_free(mrb, data);
    mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
  }

  if (arg_type == MRB_TT_ARRAY) {
    mrb_int const n = mrb_ary_len(mrb, arg);
    mrb_int i;
    enum mrb_vtype item_type = MRB_TT_MAXDEFINE;
    for (i = 0; i < n; ++i) {
      mrb_value const item = mrb_ary_ref(mrb, arg, i);
      switch (mrb_type(item)) {
      case MRB_TT_FIXNUM:
        if (item_type == MRB_TT_FIXNUM) {
          ((mrb_int*)data->buffer)[i] = mrb_fixnum(item);
        } else if (item_type == MRB_TT_FLOAT) {
          ((mrb_float*)data->buffer)[i] = (mrb_float)mrb_fixnum(item);
        } else {
          ((mrb_int*)data->buffer)[i] = mrb_fixnum(item);
          item_type = MRB_TT_FIXNUM;
        }
        break;
      case MRB_TT_FLOAT:
        if (item_type == MRB_TT_FLOAT) {
          ((mrb_float*)data->buffer)[i] = mrb_float(item);
        } else if (item_type == MRB_TT_FLOAT) {
          ((mrb_int*)data->buffer)[i] = (mrb_int)mrb_float(item);
        } else {
          ((mrb_float*)data->buffer)[i] = mrb_float(item);
          item_type = MRB_TT_FLOAT;
        }
        break;
      case MRB_TT_STRING:
        if (item_type == MRB_TT_FLOAT) {
          ((mrb_float*)data->buffer)[i] = mrb_float(mrb_funcall(mrb, arg, "to_f", 0));
        } else if (item_type == MRB_TT_FLOAT) {
          ((mrb_int*)data->buffer)[i] = mrb_fixnum(mrb_funcall(mrb, arg, "to_i", 0));
        } else {
          ((mrb_float*)data->buffer)[i] = mrb_float(mrb_funcall(mrb, arg, "to_f", 0));
          item_type = MRB_TT_FLOAT;
        }
        break;
      default:
        if (item_type == MRB_TT_FLOAT) {
          ((mrb_float*)data->buffer)[i] = mrb_float(mrb_funcall(mrb, arg, "to_f", 0));
        } else if (item_type == MRB_TT_FLOAT) {
          ((mrb_int*)data->buffer)[i] = mrb_fixnum(mrb_funcall(mrb, arg, "to_i", 0));
        } else {
          if (mrb_respond_to(mrb, arg, mrb_intern2(mrb, "to_f", 4))) {
            ((mrb_float*)data->buffer)[i] = mrb_float(mrb_funcall(mrb, arg, "to_f", 0));
            item_type = MRB_TT_FLOAT;
          } else if (mrb_respond_to(mrb, arg, mrb_intern2(mrb, "to_i", 4))) {
            ((mrb_int*)data->buffer)[i] = mrb_fixnum(mrb_funcall(mrb, arg, "to_i", 0));
            item_type = MRB_TT_FIXNUM;
          } else {
            mrb_free(mrb, data->buffer);
            mrb_free(mrb, data);
            mrb_raise(mrb, E_TYPE_ERROR, "expected Fixnum/Float/String or convertible type");
          }
        }
        break;
      }
    }
  }

  DATA_PTR(self) = data;
  DATA_TYPE(self) = &mrb_sdl2_misc_buffer_data_type;

  return self;
}

static mrb_value
mrb_sdl2_misc_buffer_get_address(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_misc_buffer_data_t *data =
    (mrb_sdl2_misc_buffer_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_misc_buffer_data_type);
  return mrb_float_value(mrb, (mrb_float)(intptr_t)data->buffer);
}

static mrb_value
mrb_sdl2_misc_buffer_get_size(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_misc_buffer_data_t *data =
    (mrb_sdl2_misc_buffer_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_misc_buffer_data_type);
  return mrb_float_value(mrb, (mrb_float)data->size);
}

static mrb_value
mrb_sdl2_misc_buffer_get_cptr(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_misc_buffer_data_t *data =
    (mrb_sdl2_misc_buffer_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_misc_buffer_data_type);
  return mrb_cptr_value(mrb, data->buffer);
}


static mrb_value
mrb_sdl2_misc_floatbuffer_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_value arg;
  mrb_get_args(mrb, "o", &arg);

  mrb_sdl2_misc_floatbuffer_data_t *data =
    (mrb_sdl2_misc_floatbuffer_data_t*)DATA_PTR(self);

  if (NULL == data) {
    data = (mrb_sdl2_misc_floatbuffer_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_misc_floatbuffer_data_t));
    if (NULL == data) {
      mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
    }
    data->buffer = NULL;
    data->size   = 0;
  }

  enum mrb_vtype const arg_type = mrb_type(arg);
  switch (arg_type) {
  case MRB_TT_FIXNUM:
    data->size = (size_t)mrb_fixnum(arg);
    break;
  case MRB_TT_FLOAT:
    data->size = (size_t)mrb_float(arg);
    break;
  case MRB_TT_STRING:
    data->size = (size_t)mrb_float(mrb_funcall(mrb, arg, "to_f", 0));
    break;
  case MRB_TT_ARRAY:
    {
      mrb_int const n = mrb_ary_len(mrb, arg);
      if (0 == n) {
        mrb_raise(mrb, E_ARGUMENT_ERROR, "cannot accept empty array.");
      }
      data->size = n;
    }
    break;
  default:
    if (mrb_respond_to(mrb, arg, mrb_intern2(mrb, "to_f", 4))) {
      data->size = (size_t)mrb_float(mrb_funcall(mrb, arg, "to_f", 0));
    } else if (mrb_respond_to(mrb, arg, mrb_intern2(mrb, "to_i", 4))) {
      data->size = (size_t)mrb_fixnum(mrb_funcall(mrb, arg, "to_i", 0));
    } else {
      mrb_raise(mrb, E_TYPE_ERROR, "expected Fixnum/Float/String/Array or comvertible type");
    }
    break;
  }

  data->buffer = mrb_malloc(mrb, sizeof(float) * data->size);
  if (NULL == data->buffer) {
    mrb_free(mrb, data);
    mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
  }
  size_t i;
  for (i = 0; i < data->size; ++i) {
    data->buffer[i] = 0.0f;
  }

  if (arg_type == MRB_TT_ARRAY) {
    mrb_int const n = mrb_ary_len(mrb, arg);
    for (i = 0; i < n; ++i) {
      mrb_value const item = mrb_ary_ref(mrb, arg, i);
      switch (mrb_type(item)) {
      case MRB_TT_FIXNUM:
        data->buffer[i] = mrb_fixnum(item);
        break;
      case MRB_TT_FLOAT:
        data->buffer[i] = mrb_float(item);
        break;
      case MRB_TT_STRING:
        data->buffer[i] = mrb_float(mrb_funcall(mrb, arg, "to_f", 0));
        break;
      default:
        if (mrb_respond_to(mrb, arg, mrb_intern2(mrb, "to_f", 4))) {
          data->buffer[i] = mrb_float(mrb_funcall(mrb, arg, "to_f", 0));
        } else if (mrb_respond_to(mrb, arg, mrb_intern2(mrb, "to_i", 4))) {
          data->buffer[i] = mrb_fixnum(mrb_funcall(mrb, arg, "to_i", 0));
        } else {
          mrb_free(mrb, data->buffer);
          mrb_free(mrb, data);
          mrb_raise(mrb, E_TYPE_ERROR, "expected Fixnum/Float/String or convertible type");
        }
        break;
      }
    }
  }

  DATA_PTR(self) = data;
  DATA_TYPE(self) = &mrb_sdl2_misc_floatbuffer_data_type;

  return self;
}

static mrb_value
mrb_sdl2_misc_floatbuffer_get_address(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_misc_floatbuffer_data_t *data =
    (mrb_sdl2_misc_floatbuffer_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_misc_floatbuffer_data_type);
  return mrb_float_value(mrb, (mrb_float)(intptr_t)data->buffer);
}

static mrb_value
mrb_sdl2_misc_floatbuffer_get_size(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_misc_floatbuffer_data_t *data =
    (mrb_sdl2_misc_floatbuffer_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_misc_floatbuffer_data_type);
  return mrb_float_value(mrb, (mrb_float)data->size);
}

static mrb_value
mrb_sdl2_misc_floatbuffer_get_cptr(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_misc_floatbuffer_data_t *data =
    (mrb_sdl2_misc_floatbuffer_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_misc_floatbuffer_data_type);
  return mrb_cptr_value(mrb, data->buffer);
}

static mrb_value
mrb_sdl2_misc_floatbuffer_get_at(mrb_state *mrb, mrb_value self)
{
  mrb_int index;
  mrb_get_args(mrb, "i", &index);
  mrb_sdl2_misc_floatbuffer_data_t *data =
    (mrb_sdl2_misc_floatbuffer_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_misc_floatbuffer_data_type);
  if ((index < 0) || (index >= data->size)) {
    mrb_raise(mrb, E_INDEX_ERROR, "index out of bounds.");
  }
  return mrb_float_value(mrb, data->buffer[index]);
}

static mrb_value
mrb_sdl2_misc_floatbuffer_set_at(mrb_state *mrb, mrb_value self)
{
  mrb_int index;
  mrb_float value;
  mrb_get_args(mrb, "if", &index, &value);
  mrb_sdl2_misc_floatbuffer_data_t *data =
    (mrb_sdl2_misc_floatbuffer_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_misc_floatbuffer_data_type);
  if ((index < 0) || (index >= data->size)) {
    mrb_raise(mrb, E_INDEX_ERROR, "index out of bounds.");
  }
  data->buffer[index] = (float)value;
  return self;
}

void
mruby_sdl2_misc_init(mrb_state *mrb)
{
  class_Buffer      = mrb_define_class_under(mrb, mod_SDL2, "Buffer",      mrb->object_class);
  class_FloatBuffer = mrb_define_class_under(mrb, mod_SDL2, "FloatBuffer", mrb->object_class);

  MRB_SET_INSTANCE_TT(class_Buffer,      MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_FloatBuffer, MRB_TT_DATA);

  mrb_define_method(mrb, class_Buffer, "initialize", mrb_sdl2_misc_buffer_initialize,  ARGS_REQ(1));
  mrb_define_method(mrb, class_Buffer, "address",    mrb_sdl2_misc_buffer_get_address, ARGS_NONE());
  mrb_define_method(mrb, class_Buffer, "size",       mrb_sdl2_misc_buffer_get_size,    ARGS_NONE());
  mrb_define_method(mrb, class_Buffer, "cptr",       mrb_sdl2_misc_buffer_get_cptr,    ARGS_NONE());

  mrb_define_method(mrb, class_FloatBuffer, "initialize", mrb_sdl2_misc_floatbuffer_initialize,  ARGS_REQ(1));
  mrb_define_method(mrb, class_FloatBuffer, "address",    mrb_sdl2_misc_floatbuffer_get_address, ARGS_NONE());
  mrb_define_method(mrb, class_FloatBuffer, "size",       mrb_sdl2_misc_floatbuffer_get_size,    ARGS_NONE());
  mrb_define_method(mrb, class_FloatBuffer, "cptr",       mrb_sdl2_misc_floatbuffer_get_cptr,    ARGS_NONE());
  mrb_define_method(mrb, class_FloatBuffer, "[]",         mrb_sdl2_misc_floatbuffer_get_at,      ARGS_REQ(1));
  mrb_define_method(mrb, class_FloatBuffer, "[]=",        mrb_sdl2_misc_floatbuffer_set_at,      ARGS_REQ(2));
}

void
mruby_sdl2_misc_final(mrb_state *mrb)
{
}

