#include "sdl2_render.h"
#include "sdl2_video.h"
#include "sdl2_rect.h"
#include "sdl2_surface.h"
#include "mruby/data.h"
#include "mruby/class.h"
#include "mruby/array.h"

static struct RClass *class_Renderer     = NULL;
static struct RClass *class_Texture      = NULL;
static struct RClass *class_PixelBuffer  = NULL;
static struct RClass *class_RendererInfo = NULL;

typedef struct mrb_sdl2_video_renderer_data_t {
  SDL_Renderer *renderer;
} mrb_sdl2_video_renderer_data_t;

typedef struct mrb_sdl2_video_texture_data_t {
  SDL_Texture *texture;
} mrb_sdl2_video_texture_data_t;

typedef struct mrb_sdl2_video_pixelbuf_data_t {
  pixelbuf_data_t data;
} mrb_sdl2_video_pixelbuf_data_t;

typedef struct mrb_sdl2_video_rendererinfo_data_t {
  SDL_RendererInfo info;
} mrb_sdl2_video_rendererinfo_data_t;

static void
mrb_sdl2_video_renderer_data_free(mrb_state *mrb, void *p)
{
  mrb_sdl2_video_renderer_data_t *data =
    (mrb_sdl2_video_renderer_data_t*)p;
  if (NULL != data) {
    if (NULL != data->renderer) {
      SDL_DestroyRenderer(data->renderer);
    }
    mrb_free(mrb, data);
  }
}

static void
mrb_sdl2_video_texture_data_free(mrb_state *mrb, void *p)
{
  mrb_sdl2_video_texture_data_t *data =
    (mrb_sdl2_video_texture_data_t*)p;
  if (NULL != p) {
    if (NULL != data->texture) {
      SDL_DestroyTexture(data->texture);
    }
    mrb_free(mrb, data);
  }
}

static void
mrb_sdl2_video_pixelbuf_data_free(mrb_state *mrb, void *p)
{
  mrb_sdl2_video_pixelbuf_data_t *data =
    (mrb_sdl2_video_pixelbuf_data_t*)p;
  if (NULL != data) {
    mrb_free(mrb, data);
  }
}

static void
mrb_sdl2_video_rendererinfo_data_free(mrb_state *mrb, void *p)
{
  mrb_sdl2_video_rendererinfo_data_t *data =
    (mrb_sdl2_video_rendererinfo_data_t*)p;
  if (NULL != data) {
    mrb_free(mrb, data);
  }
}

static struct mrb_data_type const mrb_sdl2_video_renderer_data_type = {
  "Renderer", mrb_sdl2_video_renderer_data_free
};

static struct mrb_data_type const mrb_sdl2_video_texture_data_type = {
  "Texture", mrb_sdl2_video_texture_data_free
};

static struct mrb_data_type const mrb_sdl2_video_pixelbuf_data_type = {
  "PixelBuffer", mrb_sdl2_video_pixelbuf_data_free
};

static struct mrb_data_type const mrb_sdl2_video_rendererinfo_data_type = {
  "RendererInfo", mrb_sdl2_video_rendererinfo_data_free
};

SDL_Renderer *
mrb_sdl2_video_renderer_get_ptr(mrb_state *mrb, mrb_value renderer)
{
  if (mrb_nil_p(renderer)) {
    return NULL;
  }
  mrb_sdl2_video_renderer_data_t *data =
    (mrb_sdl2_video_renderer_data_t*)mrb_data_get_ptr(mrb, renderer, &mrb_sdl2_video_renderer_data_type);
  return data->renderer;
}

SDL_Texture *
mrb_sdl2_video_texture_get_ptr(mrb_state *mrb, mrb_value texture)
{
  if (mrb_nil_p(texture)) {
    return NULL;
  }
  mrb_sdl2_video_texture_data_t *data =
    (mrb_sdl2_video_texture_data_t*)mrb_data_get_ptr(mrb, texture, &mrb_sdl2_video_texture_data_type);
  return data->texture;
}

SDL_RendererInfo *
mrb_sdl2_video_rendererinfo_get_ptr(mrb_state *mrb, mrb_value info)
{
  if (mrb_nil_p(info)) {
    return NULL;
  }
  mrb_sdl2_video_rendererinfo_data_t *data =
    (mrb_sdl2_video_rendererinfo_data_t*)mrb_data_get_ptr(mrb, info, &mrb_sdl2_video_rendererinfo_data_type);
  return &data->info;
}

pixelbuf_data_t *
mrb_sdl2_video_pixelbuf_get_ptr(mrb_state *mrb, mrb_value pbuf)
{
  if (mrb_nil_p(pbuf)) {
    return NULL;
  }
  mrb_sdl2_video_pixelbuf_data_t *data =
    (mrb_sdl2_video_pixelbuf_data_t*)mrb_data_get_ptr(mrb, pbuf, &mrb_sdl2_video_pixelbuf_data_type);
  return &data->data;
}

mrb_value
mrb_sdl2_video_renderer(mrb_state *mrb, SDL_Renderer *renderer)
{
  mrb_sdl2_video_renderer_data_t *data =
    (mrb_sdl2_video_renderer_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_video_renderer_data_t));
  if (NULL == data) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
  }
  data->renderer = renderer;
  return mrb_obj_value(Data_Wrap_Struct(mrb, class_Renderer, &mrb_sdl2_video_renderer_data_type, data));
}

mrb_value
mrb_sdl2_video_texture(mrb_state *mrb, SDL_Texture *texture)
{
  mrb_sdl2_video_texture_data_t *data =
    (mrb_sdl2_video_texture_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_video_texture_data_t));
  if (NULL == data) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
  }
  data->texture = texture;
  return mrb_obj_value(Data_Wrap_Struct(mrb, class_Texture, &mrb_sdl2_video_texture_data_type, data));
}

mrb_value
mrb_sdl2_video_rendererinfo(mrb_state *mrb, SDL_RendererInfo *info)
{
  mrb_sdl2_video_rendererinfo_data_t *data =
    (mrb_sdl2_video_rendererinfo_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_video_rendererinfo_data_t));
  if (NULL == data) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
  }
  if (NULL != info) {
    data->info = *info;
  }
  return mrb_obj_value(Data_Wrap_Struct(mrb, class_RendererInfo, &mrb_sdl2_video_rendererinfo_data_type, data));
}

/***************************************************************************
*
* class SDL2::Video::Renderer
*
***************************************************************************/

/*
 * SDL2::Video::Renderer.initialize
 */
static mrb_value
mrb_sdl2_video_renderer_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_video_renderer_data_t *data =
    (mrb_sdl2_video_renderer_data_t*)DATA_PTR(self);
  mrb_value obj;
  mrb_int index = -1, flags = 0;
  int const argc = mrb_get_args(mrb, "o|ii", &obj, &index, &flags);
  if ((1 != argc) && (3 != argc)) {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "wrong number of arguments.");
  }
  if (NULL == data) {
    data = (mrb_sdl2_video_renderer_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_video_renderer_data_t));
    if (NULL == data) {
      mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
    }
    data->renderer = NULL;
  }
  SDL_Renderer *renderer = NULL;
  if (mrb_obj_is_instance_of(mrb, obj, mrb_class_get_under(mrb, mod_Video, "Window"))) {
    SDL_Window *window = mrb_sdl2_video_window_get_ptr(mrb, obj);
    renderer = SDL_CreateRenderer(window, index, flags);
  } else if (mrb_obj_is_instance_of(mrb, obj, mrb_class_get_under(mrb, mod_Video, "Surface"))) {
    SDL_Surface *surface = mrb_sdl2_video_surface_get_ptr(mrb, obj);
    renderer = SDL_CreateSoftwareRenderer(surface);
  } else {
    mrb_free(mrb, data);
    mrb_raise(mrb, E_TYPE_ERROR, "given 1st argument is unexpected type (expected Surface or Window).");
  }
  if (NULL == renderer) {
    mrb_free(mrb, data);
    mruby_sdl2_raise_error(mrb);
  }
  data->renderer = renderer;
  DATA_PTR(self) = data;
  DATA_TYPE(self) = &mrb_sdl2_video_renderer_data_type;
  return self;
}

/*
 * SDL2::Video::Renderer.destroy
 */
static mrb_value
mrb_sdl2_video_renderer_destroy(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_video_renderer_data_t *data =
    (mrb_sdl2_video_renderer_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_video_renderer_data_type);
  if (NULL != data->renderer) {
    SDL_DestroyRenderer(data->renderer);
    data->renderer = NULL;
  }
  return self;
}

static mrb_value
mrb_sdl2_video_renderer_get_draw_blend_mode(mrb_state *mrb, mrb_value self)
{
  SDL_Renderer *renderer = mrb_sdl2_video_renderer_get_ptr(mrb, self);
  SDL_BlendMode mode;
  if (0 != SDL_GetRenderDrawBlendMode(renderer, &mode)) {
    mruby_sdl2_raise_error(mrb);
  }
  return mrb_fixnum_value(mode);
}

static mrb_value
mrb_sdl2_video_renderer_set_draw_blend_mode(mrb_state *mrb, mrb_value self)
{
  SDL_Renderer *renderer = mrb_sdl2_video_renderer_get_ptr(mrb, self);
  mrb_int mode;
  mrb_get_args(mrb, "i", &mode);
  if (0 != SDL_SetRenderDrawBlendMode(renderer, (SDL_BlendMode)mode)) {
    mruby_sdl2_raise_error(mrb);
  }
  return self;
}

static mrb_value
mrb_sdl2_video_renderer_get_draw_color(mrb_state *mrb, mrb_value self)
{
  SDL_Renderer *renderer = mrb_sdl2_video_renderer_get_ptr(mrb, self);
  uint8_t r, g, b, a;
  if (0 != SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a)) {
    mruby_sdl2_raise_error(mrb);
  }
  mrb_value rgba[] = {
    mrb_fixnum_value(r),
    mrb_fixnum_value(g),
    mrb_fixnum_value(b),
    mrb_fixnum_value(a),
  };
  return mrb_obj_new(mrb, mrb_class_get_under(mrb, mod_SDL2, "RGBA"), 4, rgba);
}

static mrb_value
mrb_sdl2_video_renderer_set_draw_color(mrb_state *mrb, mrb_value self)
{
  SDL_Renderer *renderer = mrb_sdl2_video_renderer_get_ptr(mrb, self);
  uint8_t r, g, b, a;
  mrb_value rgba;
  mrb_get_args(mrb, "o", &rgba);
  if (!mrb_obj_is_kind_of(mrb, rgba, mrb_class_get_under(mrb, mod_SDL2, "RGB"))) {
    mrb_raise(mrb, E_TYPE_ERROR, "given argument is unexpected type (expected RGB).");
  }
  r = mrb_fixnum(mrb_funcall(mrb, rgba, "r", 0));
  g = mrb_fixnum(mrb_funcall(mrb, rgba, "g", 0));
  b = mrb_fixnum(mrb_funcall(mrb, rgba, "b", 0));
  if (mrb_respond_to(mrb, rgba, mrb_intern(mrb, "a", 1))) {
    a = mrb_fixnum(mrb_funcall(mrb, rgba, "a", 0));
  } else {
    a = SDL_ALPHA_OPAQUE;
  }
  if (0 != SDL_SetRenderDrawColor(renderer, r, g, b, a)) {
    mruby_sdl2_raise_error(mrb);
  }
  return self;
}

static mrb_value
mrb_sdl2_video_renderer_set_target(mrb_state *mrb, mrb_value self)
{
  SDL_Renderer *renderer = mrb_sdl2_video_renderer_get_ptr(mrb, self);
  mrb_value arg;
  mrb_get_args(mrb, "o", &arg);
  SDL_Texture *texture = mrb_sdl2_video_texture_get_ptr(mrb, arg);
  if (0 != SDL_SetRenderTarget(renderer, texture)) {
    mruby_sdl2_raise_error(mrb);
  }
  return self;
}

static mrb_value
mrb_sdl2_video_renderer_get_info(mrb_state *mrb, mrb_value self)
{
  SDL_Renderer *renderer = mrb_sdl2_video_renderer_get_ptr(mrb, self);
  SDL_RendererInfo info;
  if (0 != SDL_GetRendererInfo(renderer, &info)) {
    mruby_sdl2_raise_error(mrb);
  }
  return mrb_sdl2_video_rendererinfo(mrb, &info);
}

static mrb_value
mrb_sdl2_video_renderer_clear(mrb_state *mrb, mrb_value self)
{
  SDL_Renderer *renderer = mrb_sdl2_video_renderer_get_ptr(mrb, self);
  if (0 != SDL_RenderClear(renderer)) {
    mruby_sdl2_raise_error(mrb);
  }
  return self;
}

static mrb_value
mrb_sdl2_video_renderer_copy(mrb_state *mrb, mrb_value self)
{
  SDL_Renderer *renderer = mrb_sdl2_video_renderer_get_ptr(mrb, self);
  mrb_value texture, src_rect, dst_rect;
  int const argc = mrb_get_args(mrb, "o|oo", &texture, &src_rect, &dst_rect);
  SDL_Texture *t = mrb_sdl2_video_texture_get_ptr(mrb, texture);
  SDL_Rect const *sr = NULL;
  SDL_Rect const *dr = NULL;
  if (argc > 1) {
    sr = mrb_sdl2_rect_get_ptr(mrb, src_rect);
  }
  if (argc > 2) {
    dr = mrb_sdl2_rect_get_ptr(mrb, dst_rect);
  }
  if (0 != SDL_RenderCopy(renderer, t, sr, dr)) {
    mruby_sdl2_raise_error(mrb);
  }
  return self;
}

static mrb_value
mrb_sdl2_video_renderer_copy_ex(mrb_state *mrb, mrb_value self)
{
  SDL_Renderer *renderer = mrb_sdl2_video_renderer_get_ptr(mrb, self);
  mrb_value texture, src_rect, dst_rect, center;
  mrb_float angle;
  mrb_int flip;
  int const argc = mrb_get_args(mrb, "o|oofoi", &texture, &src_rect, &dst_rect, &angle, &center, &flip);
  SDL_Texture *t = mrb_sdl2_video_texture_get_ptr(mrb, texture);
  SDL_Rect const *sr = NULL;
  SDL_Rect const *dr = NULL;
  double a = 0;
  SDL_Point *c = NULL;
  SDL_RendererFlip f = SDL_FLIP_NONE;
  if (argc > 1) {
    sr = mrb_sdl2_rect_get_ptr(mrb, src_rect);
  }
  if (argc > 2) {
    dr = mrb_sdl2_rect_get_ptr(mrb, dst_rect);
  }
  if (argc > 3) {
    a = angle;
  }
  if (argc > 4) {
    c = mrb_sdl2_point_get_ptr(mrb, center);
  }
  if (argc > 5) {
    f = (SDL_RendererFlip)flip;
  }
  if (0 != SDL_RenderCopyEx(renderer, t, sr, dr, a, c, f)) {
    mruby_sdl2_raise_error(mrb);
  }
  return self;
}

static mrb_value
mrb_sdl2_video_renderer_draw_line(mrb_state *mrb, mrb_value self)
{
  SDL_Renderer *renderer = mrb_sdl2_video_renderer_get_ptr(mrb, self);
  mrb_value p1, p2;
  mrb_get_args(mrb, "oo", &p1, &p2);
  SDL_Point const * const point1 = mrb_sdl2_point_get_ptr(mrb, p1);
  SDL_Point const * const point2 = mrb_sdl2_point_get_ptr(mrb, p2);
  if (0 != SDL_RenderDrawLine(renderer, point1->x, point1->y, point2->x, point2->y)) {
    mruby_sdl2_raise_error(mrb);
  }
  return self;
}

static mrb_value
mrb_sdl2_video_renderer_draw_lines(mrb_state *mrb, mrb_value self)
{
  SDL_Renderer *renderer = mrb_sdl2_video_renderer_get_ptr(mrb, self);
  mrb_value *argv;
  mrb_int argc;
  mrb_get_args(mrb, "*", &argv, &argc);
  SDL_Point points[argc];
  mrb_int i;
  for (i = 0; i < argc; ++i) {
    SDL_Point const * const p = mrb_sdl2_point_get_ptr(mrb, argv[i]);
    if (NULL != p) {
      points[i] = *p;
    } else {
      points[i] = (SDL_Point){ 0, 0 };
    }
  }
  if (0 != SDL_RenderDrawLines(renderer, points, argc)) {
    mruby_sdl2_raise_error(mrb);
  }
  return self;
}

static mrb_value
mrb_sdl2_video_renderer_draw_point(mrb_state *mrb, mrb_value self)
{
  SDL_Renderer *renderer = mrb_sdl2_video_renderer_get_ptr(mrb, self);
  mrb_value p;
  mrb_get_args(mrb, "o", &p);
  SDL_Point const * const point = mrb_sdl2_point_get_ptr(mrb, p);
  if (0 != SDL_RenderDrawPoint(renderer, point->x, point->y)) {
    mruby_sdl2_raise_error(mrb);
  }
  return self;
}

static mrb_value
mrb_sdl2_video_renderer_draw_points(mrb_state *mrb, mrb_value self)
{
  SDL_Renderer *renderer = mrb_sdl2_video_renderer_get_ptr(mrb, self);
  mrb_value *argv;
  mrb_int argc;
  mrb_get_args(mrb, "*", &argv, &argc);
  SDL_Point points[argc];
  mrb_int i;
  for (i = 0; i < argc; ++i) {
    SDL_Point const * const p = mrb_sdl2_point_get_ptr(mrb, argv[i]);
    if (NULL != p) {
      points[i] = *p;
    } else {
      points[i] = (SDL_Point){ 0, 0 };
    }
  }
  if (0 != SDL_RenderDrawPoints(renderer, points, argc)) {
    mruby_sdl2_raise_error(mrb);
  }
  return self;
}

static mrb_value
mrb_sdl2_video_renderer_draw_rect(mrb_state *mrb, mrb_value self)
{
  SDL_Renderer *renderer = mrb_sdl2_video_renderer_get_ptr(mrb, self);
  mrb_value arg;
  mrb_get_args(mrb, "o", &arg);
  SDL_Rect const * const r = mrb_sdl2_rect_get_ptr(mrb, arg);
  if (0 != SDL_RenderDrawRect(renderer, r)) {
    mruby_sdl2_raise_error(mrb);
  }
  return self;
}

static mrb_value
mrb_sdl2_video_renderer_draw_rects(mrb_state *mrb, mrb_value self)
{
  SDL_Renderer *renderer = mrb_sdl2_video_renderer_get_ptr(mrb, self);
  mrb_value *argv;
  mrb_int argc;
  mrb_get_args(mrb, "*", &argv, &argc);
  SDL_Rect rects[argc];
  mrb_int i;
  for (i = 0; i < argc; ++i) {
    SDL_Rect const * const r = mrb_sdl2_rect_get_ptr(mrb, argv[i]);
    if (NULL != r) {
      rects[i] = *r;
    } else {
      rects[i] = (SDL_Rect){ 0, 0, 0, 0 };
    }
  }
  if (0 != SDL_RenderDrawRects(renderer, rects, argc)) {
    mruby_sdl2_raise_error(mrb);
  }
  return self;
}

static mrb_value
mrb_sdl2_video_renderer_fill_rect(mrb_state *mrb, mrb_value self)
{
  SDL_Renderer *renderer = mrb_sdl2_video_renderer_get_ptr(mrb, self);
  mrb_value arg;
  mrb_get_args(mrb, "o", &arg);
  SDL_Rect const * const r = mrb_sdl2_rect_get_ptr(mrb, arg);
  if (0 != SDL_RenderFillRect(renderer, r)) {
    mruby_sdl2_raise_error(mrb);
  }
  return self;
}

static mrb_value
mrb_sdl2_video_renderer_fill_rects(mrb_state *mrb, mrb_value self)
{
  SDL_Renderer *renderer = mrb_sdl2_video_renderer_get_ptr(mrb, self);
  mrb_value *argv;
  mrb_int argc;
  mrb_get_args(mrb, "*", &argv, &argc);
  SDL_Rect rects[argc];
  mrb_int i;
  for (i = 0; i < argc; ++i) {
    SDL_Rect const * const r = mrb_sdl2_rect_get_ptr(mrb, argv[i]);
    if (NULL != r) {
      rects[i] = *r;
    } else {
      rects[i] = (SDL_Rect){ 0, 0, 0, 0 };
    }
  }
  if (0 != SDL_RenderFillRects(renderer, rects, argc)) {
    mruby_sdl2_raise_error(mrb);
  }
  return self;
}

static mrb_value
mrb_sdl2_video_renderer_get_clip_rect(mrb_state *mrb, mrb_value self)
{
  SDL_Renderer *renderer = mrb_sdl2_video_renderer_get_ptr(mrb, self);
  SDL_Rect rect;
  SDL_RenderGetClipRect(renderer, &rect);
  return mrb_sdl2_rect_direct(mrb, &rect);
}

static mrb_value
mrb_sdl2_video_renderer_set_clip_rect(mrb_state *mrb, mrb_value self)
{
  SDL_Renderer *renderer = mrb_sdl2_video_renderer_get_ptr(mrb, self);
  mrb_value arg;
  mrb_get_args(mrb, "o", &arg);
  SDL_Rect const * const rect = mrb_sdl2_rect_get_ptr(mrb, arg);
  if (0 != SDL_RenderSetClipRect(renderer, rect)) {
    mruby_sdl2_raise_error(mrb);
  }
  return self;
}

static mrb_value
mrb_sdl2_video_renderer_get_view_port(mrb_state *mrb, mrb_value self)
{
  SDL_Renderer *renderer = mrb_sdl2_video_renderer_get_ptr(mrb, self);
  SDL_Rect rect;
  SDL_RenderGetViewport(renderer, &rect);
  return mrb_sdl2_rect_direct(mrb, &rect);
}

static mrb_value
mrb_sdl2_video_renderer_set_view_port(mrb_state *mrb, mrb_value self)
{
  SDL_Renderer *renderer = mrb_sdl2_video_renderer_get_ptr(mrb, self);
  mrb_value arg;
  mrb_get_args(mrb, "o", &arg);
  SDL_Rect const * const rect = mrb_sdl2_rect_get_ptr(mrb, arg);
  if (0 != SDL_RenderSetViewport(renderer, rect)) {
    mruby_sdl2_raise_error(mrb);
  }
  return self;
}

static mrb_value
mrb_sdl2_video_renderer_present(mrb_state *mrb, mrb_value self)
{
  SDL_Renderer *renderer = mrb_sdl2_video_renderer_get_ptr(mrb, self);
  SDL_RenderPresent(renderer);
  return self;
}

static mrb_value
mrb_sdl2_video_renderer_read_pixels(mrb_state *mrb, mrb_value self)
{
  mrb_raise(mrb, E_NOTIMP_ERROR, "not implemented.");
  return mrb_nil_value();
}

static mrb_value
mrb_sdl2_video_renderer_set_logical_size(mrb_state *mrb, mrb_value self)
{
  SDL_Renderer *renderer = mrb_sdl2_video_renderer_get_ptr(mrb, self);
  mrb_int w, h;
  mrb_get_args(mrb, "ii", &w, &h);
  if (0 != SDL_RenderSetLogicalSize(renderer, w, h)) {
    mruby_sdl2_raise_error(mrb);
  }
  return self;
}

/***************************************************************************
*
* class SDL2::Video::Texture
*
***************************************************************************/

static mrb_value
mrb_sdl2_video_texture_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_video_texture_data_t *data =
    (mrb_sdl2_video_texture_data_t*)DATA_PTR(self);
  mrb_value *argv;
  mrb_int argc;
  mrb_get_args(mrb, "*", &argv, &argc);
  if ((2 != argc) && (5 != argc)) {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "wrong number of arguments.");
  }
  if (NULL == data) {
    data = (mrb_sdl2_video_texture_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_video_texture_data_t));
    if (NULL == data) {
      mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
    }
    data->texture = NULL;
  } else if (NULL != data->texture) {
    SDL_DestroyTexture(data->texture);
    data->texture = NULL;
  }
  SDL_Texture *texture = NULL;
  if (2 == argc) {
    SDL_Renderer *renderer = mrb_sdl2_video_renderer_get_ptr(mrb, argv[0]);
    SDL_Surface  *surface  = mrb_sdl2_video_surface_get_ptr(mrb, argv[1]);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
  }
  if (5 == argc) {
    SDL_Renderer *renderer = mrb_sdl2_video_renderer_get_ptr(mrb, argv[0]);
    if (!mrb_fixnum_p(argv[1]) ||
        !mrb_fixnum_p(argv[2]) ||
        !mrb_fixnum_p(argv[3]) ||
        !mrb_fixnum_p(argv[4])) {
      mrb_raise(mrb, E_TYPE_ERROR, "given argument is unexpected type (expected Fixnum).");
    }
    uint32_t const format  = mrb_fixnum(argv[1]);
    int const access       = mrb_fixnum(argv[2]);
    int const w            = mrb_fixnum(argv[3]);
    int const h            = mrb_fixnum(argv[4]);
    texture = SDL_CreateTexture(renderer, format, access, w, h);
  }
  if (NULL == texture) {
    mrb_free(mrb, data);
    mruby_sdl2_raise_error(mrb);
  }
  data->texture = texture;
  DATA_PTR(self) = data;
  DATA_TYPE(self) = &mrb_sdl2_video_texture_data_type;
  return self;
}

static mrb_value
mrb_sdl2_video_texture_destroy(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_video_texture_data_t *data =
    (mrb_sdl2_video_texture_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_video_texture_data_type);
  if (NULL != data->texture) {
    SDL_DestroyTexture(data->texture);
    data->texture = NULL;
  }
  return self;
}

static mrb_value
mrb_sdl2_video_texture_get_alpha_mod(mrb_state *mrb, mrb_value self)
{
  SDL_Texture *texture = mrb_sdl2_video_texture_get_ptr(mrb, self);
  uint8_t alpha;
  if (0 != SDL_GetTextureAlphaMod(texture, &alpha)) {
    mruby_sdl2_raise_error(mrb);
  }
  return mrb_fixnum_value(alpha);
}

static mrb_value
mrb_sdl2_video_texture_set_alpha_mod(mrb_state *mrb, mrb_value self)
{
  SDL_Texture *texture = mrb_sdl2_video_texture_get_ptr(mrb, self);
  mrb_int alpha;
  mrb_get_args(mrb, "i", &alpha);
  if (0 != SDL_SetTextureAlphaMod(texture, (uint8_t)(alpha & 0xff))) {
    mruby_sdl2_raise_error(mrb);
  }
  return self;
}

static mrb_value
mrb_sdl2_video_texture_get_blend_mode(mrb_state *mrb, mrb_value self)
{
  SDL_Texture *texture = mrb_sdl2_video_texture_get_ptr(mrb, self);
  SDL_BlendMode mode;
  if (0 != SDL_GetTextureBlendMode(texture, &mode)) {
    mruby_sdl2_raise_error(mrb);
  }
  return mrb_fixnum_value(mode);
}

static mrb_value
mrb_sdl2_video_texture_set_blend_mode(mrb_state *mrb, mrb_value self)
{
  SDL_Texture *texture = mrb_sdl2_video_texture_get_ptr(mrb, self);
  mrb_int mode;
  mrb_get_args(mrb, "i", &mode);
  if (0 != SDL_SetTextureBlendMode(texture, (SDL_BlendMode)mode)) {
    mruby_sdl2_raise_error(mrb);
  }
  return self;
}

static mrb_value
mrb_sdl2_video_texture_get_color_mod(mrb_state *mrb, mrb_value self)
{
  SDL_Texture *texture = mrb_sdl2_video_texture_get_ptr(mrb, self);
  uint8_t r, g, b;
  if (0 != SDL_GetTextureColorMod(texture, &r, &g, &b)) {
    mruby_sdl2_raise_error(mrb);
  }
  mrb_value rgb[] = {
    mrb_fixnum_value(r),
    mrb_fixnum_value(g),
    mrb_fixnum_value(b)
  };
  return mrb_obj_new(mrb, mrb_class_get_under(mrb, mod_SDL2, "RGB"), 3, rgb);
}

static mrb_value
mrb_sdl2_video_texture_set_color_mod(mrb_state *mrb, mrb_value self)
{
  SDL_Texture *texture = mrb_sdl2_video_texture_get_ptr(mrb, self);
  mrb_value rgb;
  mrb_get_args(mrb, "o", &rgb);
  if (!mrb_obj_is_kind_of(mrb, rgb, mrb_class_get_under(mrb, mod_SDL2, "RGB"))) {
    mrb_raise(mrb, E_TYPE_ERROR, "given argument is unexpected type (expected RGB).");
  }
  uint8_t const r = mrb_fixnum(mrb_funcall(mrb, rgb, "r", 0));
  uint8_t const g = mrb_fixnum(mrb_funcall(mrb, rgb, "g", 0));
  uint8_t const b = mrb_fixnum(mrb_funcall(mrb, rgb, "b", 0));
  if (0 != SDL_SetTextureColorMod(texture, r, g, b)) {
    mruby_sdl2_raise_error(mrb);
  }
  return self;
}

static mrb_value
mrb_sdl2_video_texture_lock(mrb_state *mrb, mrb_value self)
{
  mrb_raise(mrb, E_NOTIMP_ERROR, "not implemented.");
  return mrb_nil_value();
}

static mrb_value
mrb_sdl2_video_texture_unlock(mrb_state *mrb, mrb_value self)
{
  mrb_raise(mrb, E_NOTIMP_ERROR, "not implemented.");
  return mrb_nil_value();
}

static mrb_value
mrb_sdl2_video_texture_get_format(mrb_state *mrb, mrb_value self)
{
  SDL_Texture *texture = mrb_sdl2_video_texture_get_ptr(mrb, self);
  uint32_t format;
  if (0 != SDL_QueryTexture(texture, &format, NULL, NULL, NULL)) {
    mruby_sdl2_raise_error(mrb);
  }
  return mrb_fixnum_value(format);
}

static mrb_value
mrb_sdl2_video_texture_get_access(mrb_state *mrb, mrb_value self)
{
  SDL_Texture *texture = mrb_sdl2_video_texture_get_ptr(mrb, self);
  int access;
  if (0 != SDL_QueryTexture(texture, NULL, &access, NULL, NULL)) {
    mruby_sdl2_raise_error(mrb);
  }
  return mrb_fixnum_value(access);
}

static mrb_value
mrb_sdl2_video_texture_get_width(mrb_state *mrb, mrb_value self)
{
  SDL_Texture *texture = mrb_sdl2_video_texture_get_ptr(mrb, self);
  int w;
  if (0 != SDL_QueryTexture(texture, NULL, NULL, &w, NULL)) {
    mruby_sdl2_raise_error(mrb);
  }
  return mrb_fixnum_value(w);
}

static mrb_value
mrb_sdl2_video_texture_get_height(mrb_state *mrb, mrb_value self)
{
  SDL_Texture *texture = mrb_sdl2_video_texture_get_ptr(mrb, self);
  int h;
  if (0 != SDL_QueryTexture(texture, NULL, NULL, NULL, &h)) {
    mruby_sdl2_raise_error(mrb);
  }
  return mrb_fixnum_value(h);
}

static mrb_value
mrb_sdl2_video_texture_update(mrb_state *mrb, mrb_value self)
{
  mrb_raise(mrb, E_NOTIMP_ERROR, "not implemented.");
  return mrb_nil_value();
}

/***************************************************************************
*
* class SDL2::Video::PixelBuffer
*
***************************************************************************/

static mrb_value
mrb_sdl2_video_pixelbuf_get_pitch(mrb_state *mrb, mrb_value self)
{
  pixelbuf_data_t *data = mrb_sdl2_video_pixelbuf_get_ptr(mrb, self);
  return mrb_fixnum_value(data->pitch);
}

static mrb_value
mrb_sdl2_video_pixelbuf_get_rect(mrb_state *mrb, mrb_value self)
{
  pixelbuf_data_t *data = mrb_sdl2_video_pixelbuf_get_ptr(mrb, self);
  return mrb_sdl2_rect_direct(mrb, &data->rect);
}

/***************************************************************************
*
* class SDL2::Video::RendererInfo
*
***************************************************************************/

static mrb_value
mrb_sdl2_video_rendererinfo_get_name(mrb_state *mrb, mrb_value self)
{
  SDL_RendererInfo *info = mrb_sdl2_video_rendererinfo_get_ptr(mrb, self);
  return mrb_str_new_cstr(mrb, info->name);
}

static mrb_value
mrb_sdl2_video_rendererinfo_get_flags(mrb_state *mrb, mrb_value self)
{
  SDL_RendererInfo *info = mrb_sdl2_video_rendererinfo_get_ptr(mrb, self);
  return mrb_fixnum_value(info->flags);
}

static mrb_value
mrb_sdl2_video_rendererinfo_get_texture_formats(mrb_state *mrb, mrb_value self)
{
  SDL_RendererInfo *info = mrb_sdl2_video_rendererinfo_get_ptr(mrb, self);
  uint32_t const n = info->num_texture_formats;
  mrb_value array = mrb_ary_new_capa(mrb, n);
  uint32_t i;
  for (i = 0; i < n; ++i) {
    mrb_ary_push(mrb, array, mrb_fixnum_value(info->texture_formats[i]));
  }
  return array;
}

static mrb_value
mrb_sdl2_video_rendererinfo_get_max_texture_width(mrb_state *mrb, mrb_value self)
{
  SDL_RendererInfo *info = mrb_sdl2_video_rendererinfo_get_ptr(mrb, self);
  return mrb_fixnum_value(info->max_texture_width);
}

static mrb_value
mrb_sdl2_video_rendererinfo_get_max_texture_height(mrb_state *mrb, mrb_value self)
{
  SDL_RendererInfo *info = mrb_sdl2_video_rendererinfo_get_ptr(mrb, self);
  return mrb_fixnum_value(info->max_texture_height);
}



void
mruby_sdl2_video_renderer_init(mrb_state *mrb, struct RClass *mod_Video)
{
  class_Renderer     = mrb_define_class_under(mrb, mod_Video, "Renderer",     mrb->object_class);
  class_Texture      = mrb_define_class_under(mrb, mod_Video, "Texture",      mrb->object_class);
  class_PixelBuffer  = mrb_define_class_under(mrb, mod_Video, "PixelBuffer",  mrb->object_class);
  class_RendererInfo = mrb_define_class_under(mrb, mod_Video, "RendererInfo", mrb->object_class);

  MRB_SET_INSTANCE_TT(class_Renderer,     MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_Texture,      MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_PixelBuffer,  MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_RendererInfo, MRB_TT_DATA);

  mrb_define_method(mrb, class_Renderer, "initialize",       mrb_sdl2_video_renderer_initialize,          MRB_ARGS_REQ(1) | MRB_ARGS_OPT(2));
  mrb_define_method(mrb, class_Renderer, "destroy",          mrb_sdl2_video_renderer_destroy,             MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Renderer, "draw_blend_mode",  mrb_sdl2_video_renderer_get_draw_blend_mode, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Renderer, "draw_blend_mode=", mrb_sdl2_video_renderer_set_draw_blend_mode, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Renderer, "draw_color",       mrb_sdl2_video_renderer_get_draw_color,      MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Renderer, "draw_color=",      mrb_sdl2_video_renderer_set_draw_color,      MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Renderer, "target=",          mrb_sdl2_video_renderer_set_target,          MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Renderer, "info",             mrb_sdl2_video_renderer_get_info,            MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Renderer, "clear",            mrb_sdl2_video_renderer_clear,               MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Renderer, "copy",             mrb_sdl2_video_renderer_copy,                MRB_ARGS_REQ(1) | MRB_ARGS_OPT(2));
  mrb_define_method(mrb, class_Renderer, "copy_ex",          mrb_sdl2_video_renderer_copy_ex,             MRB_ARGS_REQ(1) | MRB_ARGS_OPT(5));
  mrb_define_method(mrb, class_Renderer, "draw_line",        mrb_sdl2_video_renderer_draw_line,           MRB_ARGS_REQ(2));
  mrb_define_method(mrb, class_Renderer, "draw_lines",       mrb_sdl2_video_renderer_draw_lines,          MRB_ARGS_ANY());
  mrb_define_method(mrb, class_Renderer, "draw_point",       mrb_sdl2_video_renderer_draw_point,          MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Renderer, "draw_points",      mrb_sdl2_video_renderer_draw_points,         MRB_ARGS_ANY());
  mrb_define_method(mrb, class_Renderer, "draw_rect",        mrb_sdl2_video_renderer_draw_rect,           MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Renderer, "draw_rects",       mrb_sdl2_video_renderer_draw_rects,          MRB_ARGS_ANY());
  mrb_define_method(mrb, class_Renderer, "fill_rect",        mrb_sdl2_video_renderer_fill_rect,           MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Renderer, "fill_rects",       mrb_sdl2_video_renderer_fill_rects,          MRB_ARGS_ANY());
  mrb_define_method(mrb, class_Renderer, "clip_rect",        mrb_sdl2_video_renderer_get_clip_rect,       MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Renderer, "clip_rect=",       mrb_sdl2_video_renderer_set_clip_rect,       MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Renderer, "view_port",        mrb_sdl2_video_renderer_get_view_port,       MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Renderer, "view_port=",       mrb_sdl2_video_renderer_set_view_port,       MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Renderer, "present",          mrb_sdl2_video_renderer_present,             MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Renderer, "read_pixels",      mrb_sdl2_video_renderer_read_pixels,         MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Renderer, "set_logical_size", mrb_sdl2_video_renderer_set_logical_size,    MRB_ARGS_REQ(2));

  int arena_size = mrb_gc_arena_save(mrb);

  /* SDL_RendererFlags */
  mrb_define_const(mrb, class_Renderer, "SDL_RENDERER_SOFTWARE",      mrb_fixnum_value(SDL_RENDERER_SOFTWARE));
  mrb_define_const(mrb, class_Renderer, "SDL_RENDERER_ACCELERATED",   mrb_fixnum_value(SDL_RENDERER_ACCELERATED));
  mrb_define_const(mrb, class_Renderer, "SDL_RENDERER_PRESENTVSYNC",  mrb_fixnum_value(SDL_RENDERER_PRESENTVSYNC));
  mrb_define_const(mrb, class_Renderer, "SDL_RENDERER_TARGETTEXTURE", mrb_fixnum_value(SDL_RENDERER_TARGETTEXTURE));

  mrb_gc_arena_restore(mrb, arena_size);
  arena_size = mrb_gc_arena_save(mrb);

  /* SDL_RendererFlip */
  mrb_define_const(mrb, class_Renderer, "SDL_FLIP_NONE",       mrb_fixnum_value(SDL_FLIP_NONE));
  mrb_define_const(mrb, class_Renderer, "SDL_FLIP_HORIZONTAL", mrb_fixnum_value(SDL_FLIP_HORIZONTAL));
  mrb_define_const(mrb, class_Renderer, "SDL_FLIP_VERTICAL",   mrb_fixnum_value(SDL_FLIP_VERTICAL));

  mrb_gc_arena_restore(mrb, arena_size);
  arena_size = mrb_gc_arena_save(mrb);

  mrb_define_method(mrb, class_Texture, "initialize",  mrb_sdl2_video_texture_initialize,     MRB_ARGS_REQ(2));
  mrb_define_method(mrb, class_Texture, "destroy",     mrb_sdl2_video_texture_destroy,        MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Texture, "alpha_mod",   mrb_sdl2_video_texture_get_alpha_mod,  MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Texture, "alpha_mod=",  mrb_sdl2_video_texture_set_alpha_mod,  MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Texture, "blend_mode",  mrb_sdl2_video_texture_get_blend_mode, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Texture, "blend_mode=", mrb_sdl2_video_texture_set_blend_mode, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Texture, "color_mod",   mrb_sdl2_video_texture_get_color_mod,  MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Texture, "color_mod=",  mrb_sdl2_video_texture_set_color_mod,  MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Texture, "lock",        mrb_sdl2_video_texture_lock,           MRB_ARGS_OPT(1));
  mrb_define_method(mrb, class_Texture, "unlock",      mrb_sdl2_video_texture_unlock,         MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Texture, "format",      mrb_sdl2_video_texture_get_format,     MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Texture, "access",      mrb_sdl2_video_texture_get_access,     MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Texture, "width",       mrb_sdl2_video_texture_get_width,      MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Texture, "height",      mrb_sdl2_video_texture_get_height,     MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Texture, "update",      mrb_sdl2_video_texture_update,         MRB_ARGS_REQ(1));

  mrb_gc_arena_restore(mrb, arena_size);
  arena_size = mrb_gc_arena_save(mrb);

  /* SDL_TextureAccess */
  mrb_define_const(mrb, class_Texture, "SDL_TEXTUREACCESS_STATIC",    mrb_fixnum_value(SDL_TEXTUREACCESS_STATIC));
  mrb_define_const(mrb, class_Texture, "SDL_TEXTUREACCESS_STREAMING", mrb_fixnum_value(SDL_TEXTUREACCESS_STREAMING));

  /* SDL_TextureModulate */
  mrb_define_const(mrb, class_Texture, "SDL_TEXTUREMODULATE_NONE",  mrb_fixnum_value(SDL_TEXTUREMODULATE_NONE));
  mrb_define_const(mrb, class_Texture, "SDL_TEXTUREMODULATE_COLOR", mrb_fixnum_value(SDL_TEXTUREMODULATE_COLOR));
  mrb_define_const(mrb, class_Texture, "SDL_TEXTUREMODULATE_ALPHA", mrb_fixnum_value(SDL_TEXTUREMODULATE_ALPHA));

  mrb_gc_arena_restore(mrb, arena_size);
  arena_size = mrb_gc_arena_save(mrb);

  mrb_define_method(mrb, class_PixelBuffer, "pitch", mrb_sdl2_video_pixelbuf_get_pitch, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_PixelBuffer, "rect",  mrb_sdl2_video_pixelbuf_get_rect,  MRB_ARGS_NONE());

  mrb_define_method(mrb, class_RendererInfo, "name",               mrb_sdl2_video_rendererinfo_get_name,               MRB_ARGS_NONE());
  mrb_define_method(mrb, class_RendererInfo, "flags",              mrb_sdl2_video_rendererinfo_get_flags,              MRB_ARGS_NONE());
  mrb_define_method(mrb, class_RendererInfo, "texture_formats",    mrb_sdl2_video_rendererinfo_get_texture_formats,    MRB_ARGS_NONE());
  mrb_define_method(mrb, class_RendererInfo, "max_texture_width",  mrb_sdl2_video_rendererinfo_get_max_texture_width,  MRB_ARGS_NONE());
  mrb_define_method(mrb, class_RendererInfo, "max_texture_height", mrb_sdl2_video_rendererinfo_get_max_texture_height, MRB_ARGS_NONE());

  mrb_gc_arena_restore(mrb, arena_size);
}

void
mruby_sdl2_video_renderer_final(mrb_state *mrb, struct RClass *mod_Video)
{
}
