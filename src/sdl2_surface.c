#include "sdl2_surface.h"
#include "sdl2_rect.h"
#include "mruby/data.h"
#include "mruby/class.h"
#include "mruby/string.h"
#include "mruby/array.h"
#include "mruby/variable.h"

static struct RClass *class_Surface;

typedef struct mrb_sdl2_video_surface_data_t {
  bool         is_associated;
  SDL_Surface *surface;
} mrb_sdl2_video_surface_data_t;

static void
mrb_sdl2_video_surface_data_free(mrb_state *mrb, void *p)
{
  mrb_sdl2_video_surface_data_t* data =
    (mrb_sdl2_video_surface_data_t*)p;
  if (NULL != data) {
    if ((NULL != data->surface) && (false == data->is_associated)) {
      SDL_FreeSurface(data->surface);
    }
    mrb_free(mrb, data);
  }
}

static struct mrb_data_type const mrb_sdl2_video_surface_data_type = {
  "Surface", mrb_sdl2_video_surface_data_free
};

mrb_value
mrb_sdl2_video_surface(mrb_state *mrb, SDL_Surface *surface, bool is_associated)
{
  mrb_sdl2_video_surface_data_t* data =
    (mrb_sdl2_video_surface_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_video_surface_data_t));
  if (NULL == data) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
  }
  data->is_associated = is_associated;
  data->surface = surface;
  return mrb_obj_value(Data_Wrap_Struct(mrb, class_Surface, &mrb_sdl2_video_surface_data_type, data));
}

SDL_Surface *
mrb_sdl2_video_surface_get_ptr(mrb_state *mrb, mrb_value surface)
{
  if (mrb_nil_p(surface)) {
    return NULL;
  }
  mrb_sdl2_video_surface_data_t *data =
    (mrb_sdl2_video_surface_data_t*)mrb_data_get_ptr(mrb, surface, &mrb_sdl2_video_surface_data_type);
  return data->surface;
}


static mrb_value
mrb_sdl2_video_surface_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_video_surface_data_t *data =
    (mrb_sdl2_video_surface_data_t*)DATA_PTR(self);
  uint32_t flags, rmask, gmask, bmask, amask;
  mrb_int width, height, depth;
  mrb_get_args(mrb, "iiiiiiii", &flags, &width, &height, &depth, &rmask, &gmask, &bmask, &amask);
  if (NULL == data) {
    data = (mrb_sdl2_video_surface_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_video_surface_data_t));
    if (NULL == data) {
      mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
    }
    data->surface = NULL;
  } else {
    if (NULL != data->surface) {
      SDL_FreeSurface(data->surface);
    }
  }
  data->surface = SDL_CreateRGBSurface(flags, width, height, depth, rmask, gmask, bmask, amask);
  if (NULL == data->surface) {
    mrb_free(mrb, data);
    mruby_sdl2_raise_error(mrb);
  }
  DATA_PTR(self) = data;
  DATA_TYPE(self) = &mrb_sdl2_video_surface_data_type;
  return self;
}

static mrb_value
mrb_sdl2_video_surface_free(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_video_surface_data_t *data =
    (mrb_sdl2_video_surface_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_video_surface_data_type);
  if ((NULL != data->surface) && (false == data->is_associated)) {
    SDL_FreeSurface(data->surface);
    data->surface = NULL;
  }
  return self;
}

static mrb_value
mrb_sdl2_video_surface_blit_scaled(mrb_state *mrb, mrb_value self)
{
  mrb_value src_rect, dst, dst_rect;
  mrb_get_args(mrb, "ooo", &src_rect, &dst, &dst_rect);
  SDL_Surface * const    ss = mrb_sdl2_video_surface_get_ptr(mrb, self);
  SDL_Rect const * const sr = mrb_sdl2_rect_get_ptr(mrb, src_rect);
  SDL_Surface * const    ds = mrb_sdl2_video_surface_get_ptr(mrb, dst);
  SDL_Rect * const       dr = mrb_sdl2_rect_get_ptr(mrb, dst_rect);
  int ret;
  if (NULL != dr) {
    SDL_Rect tmp = *dr;
    ret = SDL_BlitScaled(ss, sr, ds, &tmp);
  } else {
    ret = SDL_BlitScaled(ss, sr, ds, dr);
  }
  if (0 != ret) {
    mruby_sdl2_raise_error(mrb);
  }
  return self;
}

static mrb_value
mrb_sdl2_video_surface_blit_surface(mrb_state *mrb, mrb_value self)
{
  mrb_value src_rect, dst, dst_rect;
  mrb_get_args(mrb, "ooo", &src_rect, &dst, &dst_rect);
  SDL_Surface * const    ss = mrb_sdl2_video_surface_get_ptr(mrb, self);
  SDL_Rect const * const sr = mrb_sdl2_rect_get_ptr(mrb, src_rect);
  SDL_Surface * const    ds = mrb_sdl2_video_surface_get_ptr(mrb, dst);
  SDL_Rect * const       dr = mrb_sdl2_rect_get_ptr(mrb, dst_rect);
  if (NULL == dr) {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "cannot set 3rd argument nil.");
  }
  SDL_Rect tmp = *dr;
  if (0 != SDL_BlitSurface(ss, sr, ds, &tmp)) {
    mruby_sdl2_raise_error(mrb);
  }
  return self;
}

static mrb_value
mrb_sdl2_video_surface_convert_format(mrb_state *mrb, mrb_value self)
{
  mrb_raise(mrb, E_NOTIMP_ERROR, "not implemented.");
  return self;
}

static mrb_value
mrb_sdl2_video_surface_fill_rect(mrb_state *mrb, mrb_value self)
{
  uint32_t color;
  mrb_value rect;
  mrb_get_args(mrb, "i|o", &color, &rect);
  SDL_Surface *s = mrb_sdl2_video_surface_get_ptr(mrb, self);
  SDL_Rect const * const r = mrb_sdl2_rect_get_ptr(mrb, rect);
  if (0 != SDL_FillRect(s, r, color)) {
    mruby_sdl2_raise_error(mrb);
  }
  return self;
}

static mrb_value
mrb_sdl2_video_surface_fill_rects(mrb_state *mrb, mrb_value self)
{
  uint32_t color;
  mrb_value rects;
  mrb_get_args(mrb, "io", &color, &rects);
  if (!mrb_array_p(rects)) {
    mrb_raise(mrb, E_TYPE_ERROR, "given 2nd argument is unexpected type (expected Array).");
  }
  mrb_int const n = mrb_ary_len(mrb, rects);
  SDL_Surface *s = mrb_sdl2_video_surface_get_ptr(mrb, self);
  SDL_Rect r[n];
  mrb_int i;
  for (i = 0; i < n; ++i) {
    SDL_Rect const * const ptr = mrb_sdl2_rect_get_ptr(mrb, mrb_ary_ref(mrb, rects, i));
    if (NULL != ptr) {
      r[i] = *ptr;
    } else {
      r[i] = (SDL_Rect){ 0, 0, 0, 0 };
    }
  }
  if (0 != SDL_FillRects(s, r, n, color)) {
    mruby_sdl2_raise_error(mrb);
  }
  return self;
}

static mrb_value
mrb_sdl2_video_surface_get_clip_rect(mrb_state *mrb, mrb_value self)
{
  SDL_Surface *s = mrb_sdl2_video_surface_get_ptr(mrb, self);
  SDL_Rect rect;
  SDL_GetClipRect(s, &rect);
  return mrb_sdl2_rect_direct(mrb, &rect);
}

static mrb_value
mrb_sdl2_video_surface_set_clip_rect(mrb_state *mrb, mrb_value self)
{
  SDL_Surface *s = mrb_sdl2_video_surface_get_ptr(mrb, self);
  mrb_value arg;
  mrb_get_args(mrb, "o", &arg);
  SDL_Rect const * const rect = mrb_sdl2_rect_get_ptr(mrb, arg);
  if (SDL_FALSE == SDL_SetClipRect(s, rect)) {
    mruby_sdl2_raise_error(mrb);
  }
  return self;
}

static mrb_value
mrb_sdl2_video_surface_get_color_key(mrb_state *mrb, mrb_value self)
{
  uint32_t key;
  SDL_Surface *s = mrb_sdl2_video_surface_get_ptr(mrb, self);
  if (0 != SDL_GetColorKey(s, &key)) {
    mruby_sdl2_raise_error(mrb);
  }
  return mrb_fixnum_value(key);
}

static mrb_value
mrb_sdl2_video_surface_get_solor_key(mrb_state *mrb, mrb_value self)
{
  uint32_t key;
  int flag;
  SDL_Surface *s = mrb_sdl2_video_surface_get_ptr(mrb, self);
  mrb_get_args(mrb, "ii", &flag, &key); 
  if (0 != SDL_SetColorKey(s, flag, key)) {
    mruby_sdl2_raise_error(mrb);
  }
  return self;
}

static mrb_value
mrb_sdl2_video_surface_get_alpha_mod(mrb_state *mrb, mrb_value self)
{
  uint8_t alpha;
  SDL_Surface *s = mrb_sdl2_video_surface_get_ptr(mrb, self);
  if (0 != SDL_GetSurfaceAlphaMod(s, &alpha)) {
    mruby_sdl2_raise_error(mrb);
  }
  return mrb_fixnum_value(alpha);
}

static mrb_value
mrb_sdl2_video_surface_set_alpha_mod(mrb_state *mrb, mrb_value self)
{
  mrb_int alpha;
  mrb_get_args(mrb, "i", &alpha);
  SDL_Surface *s = mrb_sdl2_video_surface_get_ptr(mrb, self);
  if (0 != SDL_SetSurfaceAlphaMod(s, (uint8_t)(alpha & 0xff))) {
    mruby_sdl2_raise_error(mrb);
  }
  return self;
}

static mrb_value
mrb_sdl2_video_surface_get_blend_mode(mrb_state *mrb, mrb_value self)
{
  SDL_BlendMode mode;
  SDL_Surface *s = mrb_sdl2_video_surface_get_ptr(mrb, self);
  if (0 != SDL_GetSurfaceBlendMode(s, &mode)) {
    mruby_sdl2_raise_error(mrb);
  }
  return mrb_fixnum_value(mode);
}

static mrb_value
mrb_sdl2_video_surface_set_blend_mode(mrb_state *mrb, mrb_value self)
{
  mrb_int mode;
  mrb_get_args(mrb, "i", &mode);
  SDL_Surface *s = mrb_sdl2_video_surface_get_ptr(mrb, self);
  if (0 != SDL_SetSurfaceBlendMode(s, (SDL_BlendMode)mode)) {
    mruby_sdl2_raise_error(mrb);
  }
  return self;
}

static mrb_value
mrb_sdl2_video_surface_get_color_mod(mrb_state *mrb, mrb_value self)
{
  uint8_t r, g, b;
  SDL_Surface *s = mrb_sdl2_video_surface_get_ptr(mrb, self);
  if (0 != SDL_GetSurfaceColorMod(s, &r, &g, &b)) {
    mruby_sdl2_raise_error(mrb);
  }
  mrb_value rgb[3] = {
    mrb_fixnum_value(r),
    mrb_fixnum_value(g),
    mrb_fixnum_value(b)
  };
  return mrb_obj_new(mrb, mrb_class_get_under(mrb, mod_SDL2, "RGB"), 3, rgb);
}

static mrb_value
mrb_sdl2_video_surface_set_color_mod(mrb_state *mrb, mrb_value self)
{
  mrb_value color;
  SDL_Surface *s = mrb_sdl2_video_surface_get_ptr(mrb, self);
  mrb_get_args(mrb, "o", &color);
  if (!mrb_obj_is_kind_of(mrb, color, mrb_class_get_under(mrb, mod_SDL2, "RGB"))) {
    mrb_raise(mrb, E_TYPE_ERROR, "given 1st argument is unexpected type (expected RGB).");
  }
  uint8_t const r = mrb_fixnum(mrb_iv_get(mrb, color, mrb_intern(mrb, "@r", 2)));
  uint8_t const g = mrb_fixnum(mrb_iv_get(mrb, color, mrb_intern(mrb, "@g", 2)));
  uint8_t const b = mrb_fixnum(mrb_iv_get(mrb, color, mrb_intern(mrb, "@b", 2)));
  if (0 != SDL_SetSurfaceColorMod(s, r, g, b)) {
    mruby_sdl2_raise_error(mrb);
  }
  return self;
}

static mrb_value
mrb_sdl2_video_surface_set_palette(mrb_state *mrb, mrb_value self)
{
  mrb_raise(mrb, E_NOTIMP_ERROR, "not implemented.");
  return self;
}

static mrb_value
mrb_sdl2_video_surface_set_rle(mrb_state *mrb, mrb_value self)
{
  SDL_Surface *s = mrb_sdl2_video_surface_get_ptr(mrb, self);
  mrb_bool is_rle_enabled;
  mrb_get_args(mrb, "b", &is_rle_enabled);
  if (0 != SDL_SetSurfaceRLE(s, is_rle_enabled ? 1 : 0)) {
    mruby_sdl2_raise_error(mrb);
  }
  return self;
}

static mrb_value
mrb_sdl2_video_surface_lock(mrb_state *mrb, mrb_value self)
{
  SDL_Surface *s = mrb_sdl2_video_surface_get_ptr(mrb, self);
  if (0 != SDL_LockSurface(s)) {
    mruby_sdl2_raise_error(mrb);
  }
  return self;
}

static mrb_value
mrb_sdl2_video_surface_unlock(mrb_state *mrb, mrb_value self)
{
  SDL_Surface *s = mrb_sdl2_video_surface_get_ptr(mrb, self);
  SDL_UnlockSurface(s);
  return self;
}


/*
 * SDL2::Video::Surface::load_bmp
 */
static mrb_value
mrb_sdl2_video_surface_load_bmp(mrb_state *mrb, mrb_value self)
{
  mrb_value file;
  mrb_get_args(mrb, "S", &file);
  SDL_Surface *surface = SDL_LoadBMP(RSTRING_PTR(file));
  if (NULL == surface) {
    mruby_sdl2_raise_error(mrb);
  }
  return mrb_sdl2_video_surface(mrb, surface, false);
}

/*
 * SDL2::Video::Surface::save_bmp
 */
static mrb_value
mrb_sdl2_video_surface_save_bmp(mrb_state *mrb, mrb_value self)
{
  mrb_value surface, file;
  mrb_get_args(mrb, "oo", &surface, &file);
  SDL_Surface * const s = mrb_sdl2_video_surface_get_ptr(mrb, surface);
  if (0 != SDL_SaveBMP(s, RSTRING_PTR(file))) {
    mruby_sdl2_raise_error(mrb);
  }
  return mrb_nil_value();
}


void
mruby_sdl2_video_surface_init(mrb_state *mrb, struct RClass *mod_Video)
{
  class_Surface = mrb_define_class_under(mrb, mod_Video, "Surface", mrb->object_class);

  MRB_SET_INSTANCE_TT(class_Surface, MRB_TT_DATA);

  mrb_define_method(mrb, class_Surface, "initialize",     mrb_sdl2_video_surface_initialize,     MRB_ARGS_REQ(8));
  mrb_define_method(mrb, class_Surface, "free",           mrb_sdl2_video_surface_free,           MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Surface, "destroy",        mrb_sdl2_video_surface_free,           MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Surface, "blit_scaled",    mrb_sdl2_video_surface_blit_scaled,    MRB_ARGS_REQ(2) | MRB_ARGS_OPT(1));
  mrb_define_method(mrb, class_Surface, "blit_surface",   mrb_sdl2_video_surface_blit_surface,   MRB_ARGS_REQ(3));
  mrb_define_method(mrb, class_Surface, "convert_format", mrb_sdl2_video_surface_convert_format, MRB_ARGS_REQ(2));
  mrb_define_method(mrb, class_Surface, "fill_rect",      mrb_sdl2_video_surface_fill_rect,      MRB_ARGS_REQ(1) | MRB_ARGS_OPT(1));
  mrb_define_method(mrb, class_Surface, "fill_rects",     mrb_sdl2_video_surface_fill_rects,     MRB_ARGS_REQ(2));
  mrb_define_method(mrb, class_Surface, "clip_rect",      mrb_sdl2_video_surface_get_clip_rect,  MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Surface, "clip_rect=",     mrb_sdl2_video_surface_set_clip_rect,  MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Surface, "color_key_get",  mrb_sdl2_video_surface_get_color_key,  MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Surface, "color_key_set",  mrb_sdl2_video_surface_get_solor_key,  MRB_ARGS_REQ(2));
  mrb_define_method(mrb, class_Surface, "alpha_mod",      mrb_sdl2_video_surface_get_alpha_mod,  MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Surface, "alpha_mod=",     mrb_sdl2_video_surface_set_alpha_mod,  MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Surface, "blend_mode",     mrb_sdl2_video_surface_get_blend_mode, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Surface, "blend_mode=",    mrb_sdl2_video_surface_set_blend_mode, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Surface, "color_mod",      mrb_sdl2_video_surface_get_color_mod,  MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Surface, "color_mod=",     mrb_sdl2_video_surface_set_color_mod,  MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Surface, "palette",        mrb_sdl2_video_surface_set_palette,    MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Surface, "rle",            mrb_sdl2_video_surface_set_rle,        MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Surface, "lock",           mrb_sdl2_video_surface_lock,           MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Surface, "unlock",         mrb_sdl2_video_surface_unlock,         MRB_ARGS_NONE());

  mrb_define_class_method(mrb, class_Surface, "load_bmp", mrb_sdl2_video_surface_load_bmp, MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb, class_Surface, "save_bmp", mrb_sdl2_video_surface_save_bmp, MRB_ARGS_REQ(2));
}

void
mruby_sdl2_video_surface_final(mrb_state *mrb, struct RClass *mod_Video)
{
}



