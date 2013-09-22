#include "sdl2_rect.h"
#include "mruby/data.h"
#include "mruby/class.h"
#include "mruby/variable.h"

static struct RClass *class_Rect = NULL;
static struct RClass *class_Point = NULL;
static struct RClass *class_Size = NULL;

typedef struct mrb_sdl2_rect_rect_data_t {
  SDL_Rect rect;
} mrb_sdl2_rect_rect_data_t;

typedef struct mrb_sdl2_rect_point_data_t {
  SDL_Point point;
} mrb_sdl2_rect_point_data_t;

typedef struct mrb_sdl2_rect_size_data_t {
  size_data_t size;
} mrb_sdl2_rect_size_data_t;

static void
mrb_sdl2_rect_rect_data_free(mrb_state *mrb, void *p)
{
  mrb_sdl2_rect_rect_data_t *data =
    (mrb_sdl2_rect_rect_data_t*)p;
  if (NULL != data) {
    mrb_free(mrb, data);
  }
}

static void
mrb_sdl2_rect_point_data_free(mrb_state *mrb, void *p)
{
  mrb_sdl2_rect_point_data_t *data =
    (mrb_sdl2_rect_point_data_t*)p;
  if (NULL != data) {
    mrb_free(mrb, data);
  }
}

static void
mrb_sdl2_rect_size_data_free(mrb_state *mrb, void *p)
{
  mrb_sdl2_rect_size_data_t *data =
    (mrb_sdl2_rect_size_data_t*)p;
  if (NULL != data) {
    mrb_free(mrb, data);
  }
}

static struct mrb_data_type const mrb_sdl2_rect_rect_data_type = {
  "Rect", mrb_sdl2_rect_rect_data_free
};

static struct mrb_data_type const mrb_sdl2_rect_point_data_type = {
  "Point", mrb_sdl2_rect_point_data_free
};

static struct mrb_data_type const mrb_sdl2_rect_size_data_type = {
  "Size", mrb_sdl2_rect_size_data_free
};

mrb_value
mrb_sdl2_rect(mrb_state *mrb, int x, int y, int w, int h)
{
  mrb_sdl2_rect_rect_data_t *data =
    (mrb_sdl2_rect_rect_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_rect_rect_data_t));
  if (NULL == data) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
  }
  data->rect.x = x;
  data->rect.y = y;
  data->rect.w = w;
  data->rect.h = h;
  return mrb_obj_value(Data_Wrap_Struct(mrb, class_Rect, &mrb_sdl2_rect_rect_data_type, data));
}

mrb_value
mrb_sdl2_point(mrb_state *mrb, int x, int y)
{
  mrb_sdl2_rect_point_data_t *data =
    (mrb_sdl2_rect_point_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_rect_point_data_t));
  if (NULL == data) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
  }
  data->point.x = x;
  data->point.y = y;
  return mrb_obj_value(Data_Wrap_Struct(mrb, class_Point, &mrb_sdl2_rect_point_data_type, data));
}

mrb_value
mrb_sdl2_size(mrb_state *mrb, int w, int h)
{
  mrb_sdl2_rect_size_data_t *data =
    (mrb_sdl2_rect_size_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_rect_size_data_t));
  if (NULL == data) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
  }
  data->size.w = w;
  data->size.h = h;
  return mrb_obj_value(Data_Wrap_Struct(mrb, class_Size, &mrb_sdl2_rect_size_data_type, data));
}

SDL_Rect *
mrb_sdl2_rect_get_ptr(mrb_state *mrb, mrb_value rect)
{
  if (mrb_nil_p(rect)) {
    return NULL;
  }
  mrb_sdl2_rect_rect_data_t *data =
    (mrb_sdl2_rect_rect_data_t*)mrb_data_get_ptr(mrb, rect, &mrb_sdl2_rect_rect_data_type);
  return &data->rect;
}

SDL_Point *
mrb_sdl2_point_get_ptr(mrb_state *mrb, mrb_value point)
{
  if (mrb_nil_p(point)) {
    return NULL;
  }
  mrb_sdl2_rect_point_data_t *data =
    (mrb_sdl2_rect_point_data_t*)mrb_data_get_ptr(mrb, point, &mrb_sdl2_rect_point_data_type);
  return &data->point;
}

size_data_t *
mrb_sdl2_size_get_ptr(mrb_state *mrb, mrb_value size)
{
  if (mrb_nil_p(size)) {
    return NULL;
  }
  mrb_sdl2_rect_size_data_t *data =
    (mrb_sdl2_rect_size_data_t*)mrb_data_get_ptr(mrb, size, &mrb_sdl2_rect_size_data_type);
  return &data->size;
}

static mrb_value
mrb_sdl2_rect_rect_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_int x, y, w, h;
  int const argc = mrb_get_args(mrb, "|iiii", &x, &y, &w, &h);
  mrb_sdl2_rect_rect_data_t *data =
    (mrb_sdl2_rect_rect_data_t*)DATA_PTR(self);

  if (data == NULL) {
    data = (mrb_sdl2_rect_rect_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_rect_rect_data_t));
    if (NULL == data) {
      mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
    }
  }

  switch (argc) {
  case 0:
    data->rect.x = 0;
    data->rect.y = 0;
    data->rect.w = 0;
    data->rect.h = 0;
    break;
  case 1:
    data->rect.x = x;
    data->rect.y = 0;
    data->rect.w = 0;
    data->rect.h = 0;
    break;
  case 2:
    data->rect.x = x;
    data->rect.y = y;
    data->rect.w = 0;
    data->rect.h = 0;
    break;
  case 3:
    data->rect.x = x;
    data->rect.y = y;
    data->rect.w = w;
    data->rect.h = 0;
    break;
  case 4:
    data->rect.x = x;
    data->rect.y = y;
    data->rect.w = w;
    data->rect.h = h;
    break;
  }

  DATA_PTR(self) = data;
  DATA_TYPE(self) = &mrb_sdl2_rect_rect_data_type;

  return self;
}

static mrb_value
mrb_sdl2_rect_rect_get_x(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(mrb_sdl2_rect_get_ptr(mrb, self)->x);
}

static mrb_value
mrb_sdl2_rect_rect_set_x(mrb_state *mrb, mrb_value self)
{
  mrb_int x;
  mrb_get_args(mrb, "i", &x);
  mrb_sdl2_rect_get_ptr(mrb, self)->x = x;
  return self;
}

static mrb_value
mrb_sdl2_rect_rect_get_y(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(mrb_sdl2_rect_get_ptr(mrb, self)->y);
}

static mrb_value
mrb_sdl2_rect_rect_set_y(mrb_state *mrb, mrb_value self)
{
  mrb_int y;
  mrb_get_args(mrb, "i", &y);
  mrb_sdl2_rect_get_ptr(mrb, self)->y = y;
  return self;
}

static mrb_value
mrb_sdl2_rect_rect_get_w(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(mrb_sdl2_rect_get_ptr(mrb, self)->w);
}

static mrb_value
mrb_sdl2_rect_rect_set_w(mrb_state *mrb, mrb_value self)
{
  mrb_int w;
  mrb_get_args(mrb, "i", &w);
  mrb_sdl2_rect_get_ptr(mrb, self)->w = w;
  return self;
}

static mrb_value
mrb_sdl2_rect_rect_get_h(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(mrb_sdl2_rect_get_ptr(mrb, self)->h);
}

static mrb_value
mrb_sdl2_rect_rect_set_h(mrb_state *mrb, mrb_value self)
{
  mrb_int h;
  mrb_get_args(mrb, "i", &h);
  mrb_sdl2_rect_get_ptr(mrb, self)->h = h;
  return self;
}

static mrb_value
mrb_sdl2_rect_rect_equal(mrb_state *mrb, mrb_value self)
{
  mrb_value arg;
  mrb_get_args(mrb, "o", &arg);
  SDL_Rect const * const lhs = mrb_sdl2_rect_get_ptr(mrb, self);
  SDL_Rect const * const rhs = mrb_sdl2_rect_get_ptr(mrb, arg);
  if ((lhs->x != rhs->x) || (lhs->y != rhs->y) ||
      (lhs->w != rhs->w) || (lhs->h != rhs->h)) {
    return mrb_false_value();
  }
  return mrb_true_value();
}

static mrb_value
mrb_sdl2_rect_rect_get_position(mrb_state *mrb, mrb_value self)
{
  SDL_Rect const * const rect = mrb_sdl2_rect_get_ptr(mrb, self);
  mrb_sdl2_rect_point_data_t *data =
    (mrb_sdl2_rect_point_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_rect_point_data_t));
  if (NULL == data) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
  }
  data->point.x = rect->x;
  data->point.y = rect->y;
  return mrb_obj_value(Data_Wrap_Struct(mrb, class_Point, &mrb_sdl2_rect_point_data_type, data));
}

static mrb_value
mrb_sdl2_rect_rect_set_position(mrb_state *mrb, mrb_value self)
{
  mrb_value arg;
  mrb_get_args(mrb, "o", &arg);
  SDL_Rect * const lhs = mrb_sdl2_rect_get_ptr(mrb, self);
  if (mrb_type(arg) != MRB_TT_DATA) {
    mrb_raise(mrb, E_TYPE_ERROR, "unexpected type of argument.");
  }
  if (DATA_TYPE(arg) == &mrb_sdl2_rect_point_data_type) {
    SDL_Point const * const rhs = mrb_sdl2_point_get_ptr(mrb, arg);
    lhs->x = rhs->x;
    lhs->y = rhs->y;
  } else if (DATA_TYPE(arg) == &mrb_sdl2_rect_point_data_type) {
    SDL_Rect const * const rhs = mrb_sdl2_rect_get_ptr(mrb, arg);
    lhs->x = rhs->x;
    lhs->y = rhs->y;
  } else {
    mrb_raise(mrb, E_TYPE_ERROR, "unexpected type of argument.");
  }
  return self;
}

static mrb_value
mrb_sdl2_rect_rect_get_size(mrb_state *mrb, mrb_value self)
{
  SDL_Rect const * const rect = mrb_sdl2_rect_get_ptr(mrb, self);
  mrb_sdl2_rect_size_data_t *data =
    (mrb_sdl2_rect_size_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_rect_size_data_t));
  if (NULL == data) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
  }
  data->size.w = rect->w;
  data->size.h = rect->h;
  return mrb_obj_value(Data_Wrap_Struct(mrb, class_Size, &mrb_sdl2_rect_size_data_type, data));
}

static mrb_value
mrb_sdl2_rect_rect_set_size(mrb_state *mrb, mrb_value self)
{
  mrb_value arg;
  mrb_get_args(mrb, "o", &arg);
  SDL_Rect * const lhs = mrb_sdl2_rect_get_ptr(mrb, self);
  if (mrb_type(arg) != MRB_TT_DATA) {
    mrb_raise(mrb, E_TYPE_ERROR, "unexpected type of argument.");
  }
  if (DATA_TYPE(arg) == &mrb_sdl2_rect_size_data_type) {
    size_data_t const * const rhs = mrb_sdl2_size_get_ptr(mrb, arg);
    lhs->w = rhs->w;
    lhs->h = rhs->h;
  } else if (DATA_TYPE(arg) == &mrb_sdl2_rect_point_data_type) {
    SDL_Rect const * const rhs = mrb_sdl2_rect_get_ptr(mrb, arg);
    lhs->w = rhs->w;
    lhs->h = rhs->h;
  } else {
    mrb_raise(mrb, E_TYPE_ERROR, "unexpected type of argument.");
  }
  return self;
}


static mrb_value
mrb_sdl2_rect_point_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_int x;
  mrb_int y;
  int const argc = mrb_get_args(mrb, "|ii", &x, &y);
  mrb_sdl2_rect_point_data_t *data =
    (mrb_sdl2_rect_point_data_t*)DATA_PTR(self);

  if (data == NULL) {
    data = (mrb_sdl2_rect_point_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_rect_point_data_t));
    if (NULL == data) {
      mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
    }
  }

  switch (argc) {
  case 0:
    data->point.x = 0;
    data->point.y = 0;
    break;
  case 1:
    data->point.x = x;
    data->point.y = 0;
    break;
  case 2:
    data->point.x = x;
    data->point.y = y;
    break;
  }

  DATA_PTR(self) = data;
  DATA_TYPE(self) = &mrb_sdl2_rect_point_data_type;
  return self;
}

static mrb_value
mrb_sdl2_rect_point_get_x(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(mrb_sdl2_point_get_ptr(mrb, self)->x);
}

static mrb_value
mrb_sdl2_rect_point_set_x(mrb_state *mrb, mrb_value self)
{
  mrb_int x;
  mrb_get_args(mrb, "i", &x);
  mrb_sdl2_rect_get_ptr(mrb, self)->x = x;
  return self;
}

static mrb_value
mrb_sdl2_rect_point_get_y(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(mrb_sdl2_point_get_ptr(mrb, self)->y);
}

static mrb_value
mrb_sdl2_rect_point_set_y(mrb_state *mrb, mrb_value self)
{
  mrb_int y;
  mrb_get_args(mrb, "i", &y);
  mrb_sdl2_rect_get_ptr(mrb, self)->y = y;
  return self;
}


static mrb_value
mrb_sdl2_rect_size_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_int w, h;
  int const argc = mrb_get_args(mrb, "|ii", &w, &h);
  mrb_sdl2_rect_size_data_t *data =
    (mrb_sdl2_rect_size_data_t*)DATA_PTR(self);

  if (data == NULL) {
    data = (mrb_sdl2_rect_size_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_rect_size_data_t));
    if (NULL == data) {
      mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
    }
  }

  switch (argc) {
  case 0:
    data->size.w = 0;
    data->size.h = 0;
    break;
  case 1:
    data->size.w = w;
    data->size.h = 0;
    break;
  case 2:
    data->size.w = w;
    data->size.h = h;
    break;
  }

  DATA_PTR(self) = data;
  DATA_TYPE(self) = &mrb_sdl2_rect_size_data_type;
  return self;
}

static mrb_value
mrb_sdl2_rect_size_get_w(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(mrb_sdl2_size_get_ptr(mrb, self)->w);
}

static mrb_value
mrb_sdl2_rect_size_set_w(mrb_state *mrb, mrb_value self)
{
  mrb_int w;
  mrb_get_args(mrb, "i", &w);
  mrb_sdl2_size_get_ptr(mrb, self)->w = w;
  return self;
}

static mrb_value
mrb_sdl2_rect_size_get_h(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(mrb_sdl2_size_get_ptr(mrb, self)->h);
}

static mrb_value
mrb_sdl2_rect_size_set_h(mrb_state *mrb, mrb_value self)
{
  mrb_int h;
  mrb_get_args(mrb, "i", &h);
  mrb_sdl2_size_get_ptr(mrb, self)->h = h;
  return self;
}


void
mruby_sdl2_rect_init(mrb_state *mrb)
{
  class_Rect  = mrb_define_class_under(mrb, mod_SDL2, "Rect", mrb->object_class);
  class_Point = mrb_define_class_under(mrb, mod_SDL2, "Point", mrb->object_class);
  class_Size  = mrb_define_class_under(mrb, mod_SDL2, "Size", mrb->object_class);

  MRB_SET_INSTANCE_TT(class_Rect,  MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_Point, MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_Size,  MRB_TT_DATA);

  mrb_define_method(mrb, class_Rect, "initialize", mrb_sdl2_rect_rect_initialize,   ARGS_OPT(4));
  mrb_define_method(mrb, class_Rect, "x",          mrb_sdl2_rect_rect_get_x,        ARGS_NONE());
  mrb_define_method(mrb, class_Rect, "x=",         mrb_sdl2_rect_rect_set_x,        ARGS_REQ(1));
  mrb_define_method(mrb, class_Rect, "y",          mrb_sdl2_rect_rect_get_y,        ARGS_NONE());
  mrb_define_method(mrb, class_Rect, "y=",         mrb_sdl2_rect_rect_set_y,        ARGS_REQ(1));
  mrb_define_method(mrb, class_Rect, "w",          mrb_sdl2_rect_rect_get_w,        ARGS_NONE());
  mrb_define_method(mrb, class_Rect, "w=",         mrb_sdl2_rect_rect_set_w,        ARGS_REQ(1));
  mrb_define_method(mrb, class_Rect, "h",          mrb_sdl2_rect_rect_get_h,        ARGS_NONE());
  mrb_define_method(mrb, class_Rect, "h=",         mrb_sdl2_rect_rect_set_h,        ARGS_REQ(1));
  mrb_define_method(mrb, class_Rect, "=",          mrb_sdl2_rect_rect_equal,        ARGS_REQ(1));
  mrb_define_method(mrb, class_Rect, "position",   mrb_sdl2_rect_rect_get_position, ARGS_NONE());
  mrb_define_method(mrb, class_Rect, "position=",  mrb_sdl2_rect_rect_set_position, ARGS_REQ(1));
  mrb_define_method(mrb, class_Rect, "size",       mrb_sdl2_rect_rect_get_size,     ARGS_NONE());
  mrb_define_method(mrb, class_Rect, "size=",      mrb_sdl2_rect_rect_set_size,     ARGS_REQ(1));

  mrb_define_method(mrb, class_Point, "initialize", mrb_sdl2_rect_point_initialize, ARGS_OPT(2));
  mrb_define_method(mrb, class_Point, "x",          mrb_sdl2_rect_point_get_x,      ARGS_NONE());
  mrb_define_method(mrb, class_Point, "x=",         mrb_sdl2_rect_point_set_x,      ARGS_REQ(1));
  mrb_define_method(mrb, class_Point, "y",          mrb_sdl2_rect_point_get_y,      ARGS_NONE());
  mrb_define_method(mrb, class_Point, "y=",         mrb_sdl2_rect_point_set_y,      ARGS_REQ(1));

  mrb_define_method(mrb, class_Size, "initialize", mrb_sdl2_rect_size_initialize, ARGS_OPT(2));
  mrb_define_method(mrb, class_Size, "w",          mrb_sdl2_rect_size_get_w,      ARGS_NONE());
  mrb_define_method(mrb, class_Size, "w=",         mrb_sdl2_rect_size_set_w,      ARGS_REQ(1));
  mrb_define_method(mrb, class_Size, "h",          mrb_sdl2_rect_size_get_h,      ARGS_NONE());
  mrb_define_method(mrb, class_Size, "h=",         mrb_sdl2_rect_size_set_h,      ARGS_REQ(1));
}

void
mruby_sdl2_rect_final(mrb_state *mrb)
{
}

