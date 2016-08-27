#include "sdl2_video.h"
#include "sdl2_rect.h"
#include "sdl2_render.h"
#include "sdl2_surface.h"
#include "mruby/class.h"
#include "mruby/data.h"
#include "mruby/string.h"
#include "mruby/array.h"
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>

struct RClass *mod_Video         = NULL;

static struct RClass *mod_GL            = NULL;
static struct RClass *class_DisplayMode = NULL;
static struct RClass *class_Window      = NULL;
static struct RClass *class_GLContext   = NULL;

typedef struct mrb_sdl2_video_displaymode_data_t {
  SDL_DisplayMode mode;
} mrb_sdl2_video_displaymode_data_t;

typedef struct mrb_sdl2_video_window_data_t {
  bool        is_associated;
  SDL_Window *window;
} mrb_sdl2_video_window_data_t;

typedef struct mrb_sdl2_video_glcontext_data_t {
  SDL_GLContext context;
} mrb_sdl2_video_glcontext_data_t;

static void
mrb_sdl2_video_displaymode_data_free(mrb_state *mrb, void *p)
{
  mrb_sdl2_video_displaymode_data_t *data =
    (mrb_sdl2_video_displaymode_data_t*)p;
  if (NULL != data) {
    mrb_free(mrb, data);
  }
}

static void
mrb_sdl2_video_window_data_free(mrb_state *mrb, void *p)
{
  mrb_sdl2_video_window_data_t *data =
    (mrb_sdl2_video_window_data_t*)p;
  if (NULL != data) {
    if ((!data->is_associated) && (NULL != data->window)) {
      SDL_DestroyWindow(data->window);
    }
    mrb_free(mrb, data);
  }
}

static void
mrb_sdl2_video_glcontext_data_free(mrb_state *mrb, void *p)
{
  mrb_sdl2_video_glcontext_data_t *data =
    (mrb_sdl2_video_glcontext_data_t*)p;
  if (NULL != data) {
    if (NULL != data->context) {
      SDL_GL_DeleteContext(data->context);
    }
    mrb_free(mrb, data);
  }
}

static struct mrb_data_type const mrb_sdl2_video_displaymode_data_type = {
  "DisplayMode", mrb_sdl2_video_displaymode_data_free
};

static struct mrb_data_type const mrb_sdl2_video_window_data_type = {
  "Window", mrb_sdl2_video_window_data_free
};

static struct mrb_data_type const mrb_sdl2_video_glcontext_data_type = {
  "GLContext", mrb_sdl2_video_glcontext_data_free
};

SDL_Window *
mrb_sdl2_video_window_get_ptr(mrb_state *mrb, mrb_value window)
{
  if (mrb_nil_p(window)) {
    return NULL;
  }
  mrb_sdl2_video_window_data_t *data =
    (mrb_sdl2_video_window_data_t*)mrb_data_get_ptr(mrb, window, &mrb_sdl2_video_window_data_type);
  return data->window;
}

mrb_value
mrb_sdl2_video_window(mrb_state *mrb, SDL_Window *window)
{
  mrb_sdl2_video_window_data_t *data =
    (mrb_sdl2_video_window_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_video_window_data_t));
  if (NULL != data) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
  }
  data->is_associated = false;
  data->window = window;
  return mrb_obj_value(Data_Wrap_Struct(mrb, class_Window, &mrb_sdl2_video_window_data_type, data));
}

mrb_value
mrb_sdl2_video_associated_window(mrb_state *mrb, SDL_Window *window)
{
  mrb_sdl2_video_window_data_t *data =
    (mrb_sdl2_video_window_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_video_window_data_t));
  if (NULL != data) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
  }
  data->is_associated = true;
  data->window = window;
  return mrb_obj_value(Data_Wrap_Struct(mrb, class_Window, &mrb_sdl2_video_window_data_type, data));
}

/***************************************************************************
*
* module SDL2::Video
*
***************************************************************************/

/*
 * SDL2::Video::init
 */
static mrb_value
mrb_sdl2_video_init(mrb_state *mrb, mrb_value self)
{
  mrb_value name;
  int const argc = mrb_get_args(mrb, "|o", &name);
  int ret = 0;
  if (0 == argc) {
    ret = SDL_VideoInit(NULL);
  } else if (mrb_string_p(name)) {
    ret = SDL_VideoInit(RSTRING_PTR(name));
  } else if (mrb_respond_to(mrb, name, mrb_intern(mrb, "to_s", 4))) {
    name = mrb_funcall(mrb, name, "to_s", 0);
    ret = SDL_VideoInit(RSTRING_PTR(name));
  } else {
    mrb_raise(mrb, E_TYPE_ERROR, "supplied argument cannot be converted to string.");
  }
  if (0 != ret) {
    mruby_sdl2_raise_error(mrb);
  }
  return mrb_nil_value();
}

/*
 * SDL2::Video::quit
 */
static mrb_value
mrb_sdl2_video_quit(mrb_state *mrb, mrb_value self)
{
  SDL_VideoQuit();
  return mrb_nil_value();
}

/*
 * SDL2::Video::screen_saver?
 */
static mrb_value
mrb_sdl2_video_get_screen_saver_enabled(mrb_state *mrb, mrb_value self)
{
  return SDL_IsScreenSaverEnabled() == SDL_FALSE ? mrb_false_value() : mrb_true_value();
}

/*
 * SDL2::Video::screen_saver=
 */
static mrb_value
mrb_sdl2_video_set_screen_saver_enabled(mrb_state *mrb, mrb_value self)
{
  mrb_bool enabled;
  mrb_get_args(mrb, "b", &enabled);
  if (enabled) {
    SDL_EnableScreenSaver();
  } else {
    SDL_DisableScreenSaver();
  }
  return self;
}

/*
 * SDL2::Video::video_drivers
 */
static mrb_value
mrb_sdl2_video_get_video_drivers(mrb_state *mrb, mrb_value self)
{
  int const n = SDL_GetNumVideoDrivers();
  if (n < 0) {
    mruby_sdl2_raise_error(mrb);
  }
  int i;
  mrb_value array = mrb_ary_new_capa(mrb, n);
  for (i = 0; i < n; ++i) {
    mrb_value const str = mrb_str_new_cstr(mrb, SDL_GetVideoDriver(i));
    mrb_ary_push(mrb, array, str);
  }
  return array;
}

/*
 * SDL2::Video::display_modes
 */
static mrb_value
mrb_sdl2_video_get_display_modes(mrb_state *mrb, mrb_value self)
{
  mrb_int index;
  mrb_get_args(mrb, "i", &index);
  int const n = SDL_GetNumDisplayModes(index);
  if (0 > n) {
    mruby_sdl2_raise_error(mrb);
  }
  mrb_value array = mrb_ary_new_capa(mrb, n);
  int i;
  for (i = 0; i < n; ++i) {
    mrb_sdl2_video_displaymode_data_t *data =
      (mrb_sdl2_video_displaymode_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_video_displaymode_data_t));
    if (NULL == data) {
      mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
    }
    if (0 == SDL_GetDisplayMode(index, i, &data->mode)) {
      mrb_value const item = mrb_obj_value(Data_Wrap_Struct(mrb, class_DisplayMode, &mrb_sdl2_video_displaymode_data_type, data));
      mrb_ary_push(mrb, array, item);
    } else {
      mrb_free(mrb, data);
    }
  }
  return array;
}

/*
 * SDL2::Video::displays
 */
static mrb_value
mrb_sdl2_video_get_displays(mrb_state *mrb, mrb_value self)
{
  int const n = SDL_GetNumVideoDisplays();
  if (0 > n) {
    mruby_sdl2_raise_error(mrb);
  }
  return mrb_fixnum_value(n);
}

/***************************************************************************
*
* module SDL2::Video::GL
*
***************************************************************************/

/*
 * SDL2::Video::GL::attribute_get
 */
static mrb_value
mrb_sdl2_video_gl_get_attribute(mrb_state *mrb, mrb_value self)
{
  mrb_raise(mrb, E_NOTIMP_ERROR, "not implemented.");
  return mrb_nil_value();
}

/*
 * SDL2::Video::GL::attribute_set
 */
static mrb_value
mrb_sdl2_video_gl_set_attribute(mrb_state *mrb, mrb_value self)
{
  mrb_raise(mrb, E_NOTIMP_ERROR, "not implemented.");
  return mrb_nil_value();
}

/*
 * SDL2::Video::GL::exntension_supported?
 */
static mrb_value
mrb_sdl2_video_gl_is_extension_supported(mrb_state *mrb, mrb_value self)
{
  mrb_raise(mrb, E_NOTIMP_ERROR, "not implemented.");
  return mrb_nil_value();
}

/*
 * SDL2::Video::GL::swap_interval
 */
static mrb_value
mrb_sdl2_video_gl_get_swap_interval(mrb_state *mrb, mrb_value self)
{
  mrb_raise(mrb, E_NOTIMP_ERROR, "not implemented.");
  return mrb_nil_value();
}

/*
 * SDL2::Video::GL::swap_interval=
 */
static mrb_value
mrb_sdl2_video_gl_set_swap_interval(mrb_state *mrb, mrb_value self)
{
  mrb_raise(mrb, E_NOTIMP_ERROR, "not implemented.");
  return mrb_nil_value();
}

/*
 * SDL2::Video::GL::swap_window
 */
static mrb_value
mrb_sdl2_video_gl_swap_window(mrb_state *mrb, mrb_value self)
{
  mrb_raise(mrb, E_NOTIMP_ERROR, "not implemented.");
  return mrb_nil_value();
}

/***************************************************************************
*
* class SDL2::Video::Window
*
***************************************************************************/

/*
 * SDL2::Video::Window.initialize
 */
static mrb_value
mrb_sdl2_video_window_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_video_window_data_t *data =
    (mrb_sdl2_video_window_data_t*)DATA_PTR(self);
  mrb_value title;
  mrb_int x, y, w, h;
  uint32_t flags;
  int const argc = mrb_get_args(mrb, "|Siiiii", &title, &x, &y, &w, &h, &flags);
  if ((0 != argc) && (6 != argc)) {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "wrong number of arguments.");
  }
  if (NULL == data) {
    data = (mrb_sdl2_video_window_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_video_window_data_t));
    if (NULL == data) {
      mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
    }
  }
  if (6 == argc) {
    data->is_associated = false;
    data->window = SDL_CreateWindow(RSTRING_PTR(title), x, y, w, h, flags);
    if (NULL == data->window) {
      mruby_sdl2_raise_error(mrb);
    }
  } else {
    data->is_associated = false;
    data->window = NULL;
  }
  DATA_PTR(self) = data;
  DATA_TYPE(self) = &mrb_sdl2_video_window_data_type;
  return self;
}

/*
 * SDL2::Video::Window.create
 */
static mrb_value
mrb_sdl2_video_window_create(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_video_window_data_t *data =
    (mrb_sdl2_video_window_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_video_window_data_type);
  if (NULL != data->window) {
    mrb_raise(mrb, class_SDL2Error, "window has already been created.");
  }
  mrb_value title;
  mrb_int x, y, w, h;
  uint32_t flags;
  mrb_get_args(mrb, "Siiiii", &title, &x, &y, &w, &h, &flags);
  data->window = SDL_CreateWindow(RSTRING_PTR(title), x, y, w, h, flags);
  if (NULL == data->window) {
    mruby_sdl2_raise_error(mrb);
  }
  return self;
}

static mrb_value
mrb_sdl2_video_window_create_with_renderer(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_video_window_data_t *data =
    (mrb_sdl2_video_window_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_video_window_data_type);
  if (NULL != data->window) {
    mrb_raise(mrb, class_SDL2Error, "window has already been created.");
  }
  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;
  mrb_int w, h;
  uint32_t flags;
  mrb_get_args(mrb, "iii", &w, &h, &flags);
  int const ret = SDL_CreateWindowAndRenderer(w, h, flags, &window, &renderer);
  if (0 != ret) {
    mruby_sdl2_raise_error(mrb);
  }
  data->window = window;
  if (NULL == renderer) {
    return mrb_nil_value();
  }
  return mrb_sdl2_video_renderer(mrb, renderer);
}

/*
 * SDL2::Window.destroy
 */
static mrb_value
mrb_sdl2_video_window_destroy(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_video_window_data_t *data =
    (mrb_sdl2_video_window_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_video_window_data_type);
  if (NULL != data->window) {
    SDL_DestroyWindow(data->window);
    data->window = NULL;
  }
  return mrb_nil_value();
}

/*
 * SDL2::Window.size
 */
static mrb_value
mrb_sdl2_video_window_get_size(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_video_window_data_t *data =
    (mrb_sdl2_video_window_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_video_window_data_type);
  if (NULL == data->window) {
    return mrb_nil_value();
  }
  int w, h;
  SDL_GetWindowSize(data->window, &w, &h);
  return mrb_sdl2_size(mrb, w, h);
}

static mrb_value
mrb_sdl2_video_window_set_size(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_video_window_data_t *data =
    (mrb_sdl2_video_window_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_video_window_data_type);
  mrb_value arg;
  mrb_get_args(mrb, "o", &arg);
  size_data_t const * const s = mrb_sdl2_size_get_ptr(mrb, arg);
  SDL_SetWindowSize(data->window, s->w, s->h);
  return self;

}

static mrb_value
mrb_sdl2_video_window_get_position(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_video_window_data_t *data =
    (mrb_sdl2_video_window_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_video_window_data_type);
  if (NULL == data->window) {
    return mrb_nil_value();
  }
  int x, y;
  SDL_GetWindowPosition(data->window, &x, &y);
  return mrb_sdl2_point(mrb, x, y);
}

static mrb_value
mrb_sdl2_video_window_set_position(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_video_window_data_t *data =
    (mrb_sdl2_video_window_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_video_window_data_type);
  mrb_value arg;
  mrb_get_args(mrb, "o", &arg);
  SDL_Point const * const p = mrb_sdl2_point_get_ptr(mrb, arg);
  SDL_SetWindowPosition(data->window, p->x, p->y);
  return self;
}

static mrb_value
mrb_sdl2_video_window_get_title(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_video_window_data_t *data =
    (mrb_sdl2_video_window_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_video_window_data_type);
  if (NULL == data->window) {
    return mrb_nil_value();
  }
  char const *title = SDL_GetWindowTitle(data->window);
  if (NULL == title) {
    return mrb_nil_value();
  }
  return mrb_str_new_cstr(mrb, title);
}

static mrb_value
mrb_sdl2_video_window_set_title(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_video_window_data_t *data =
    (mrb_sdl2_video_window_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_video_window_data_type);
  if (NULL == data->window) {
    return mrb_nil_value();
  }
  mrb_value title;
  mrb_get_args(mrb, "S", &title);
  SDL_SetWindowTitle(data->window, RSTRING_PTR(title));
  return self;
}

static mrb_value
mrb_sdl2_video_window_set_fullscreen(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_video_window_data_t *data =
    (mrb_sdl2_video_window_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_video_window_data_type);
  if (NULL == data->window) {
    return mrb_nil_value();
  }
  uint32_t flags;
  mrb_get_args(mrb, "i", &flags);
  if (0 != SDL_SetWindowFullscreen(data->window, flags)) {
    mruby_sdl2_raise_error(mrb);
  }
  return self;
}

static mrb_value
mrb_sdl2_video_window_maximize(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_video_window_data_t *data =
    (mrb_sdl2_video_window_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_video_window_data_type);
  if (NULL == data->window) {
    return mrb_nil_value();
  }
  SDL_MaximizeWindow(data->window);
  return self;
}

static mrb_value
mrb_sdl2_video_window_minimize(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_video_window_data_t *data =
    (mrb_sdl2_video_window_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_video_window_data_type);
  if (NULL == data->window) {
    return mrb_nil_value();
  }
  SDL_MinimizeWindow(data->window);
  return self;
}

static mrb_value
mrb_sdl2_video_window_show(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_video_window_data_t *data =
    (mrb_sdl2_video_window_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_video_window_data_type);
  if (NULL == data->window) {
    return mrb_nil_value();
  }
  SDL_ShowWindow(data->window);
  return self;
}

static mrb_value
mrb_sdl2_video_window_hide(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_video_window_data_t *data =
    (mrb_sdl2_video_window_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_video_window_data_type);
  if (NULL == data->window) {
    return mrb_nil_value();
  }
  SDL_HideWindow(data->window);
  return self;
}

static mrb_value
mrb_sdl2_video_window_restore(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_video_window_data_t *data =
    (mrb_sdl2_video_window_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_video_window_data_type);
  if (NULL == data->window) {
    return mrb_nil_value();
  }
  SDL_RestoreWindow(data->window);
  return self;
}

static mrb_value
mrb_sdl2_video_window_raise(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_video_window_data_t *data =
    (mrb_sdl2_video_window_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_video_window_data_type);
  if (NULL == data->window) {
    return mrb_nil_value();
  }
  SDL_RaiseWindow(data->window);
  return self;
}

static mrb_value
mrb_sdl2_video_window_make_current(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_video_window_data_t *data =
    (mrb_sdl2_video_window_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_video_window_data_type);
  if (NULL == data->window) {
    return mrb_nil_value();
  }
  mrb_value arg;
  mrb_get_args(mrb, "o", &arg);
  SDL_GLContext *context = NULL;
  if (!mrb_nil_p(arg)) {
    context =
      ((mrb_sdl2_video_glcontext_data_t*)mrb_data_get_ptr(mrb, arg, &mrb_sdl2_video_glcontext_data_type))->context;
  }
  if (0 != SDL_GL_MakeCurrent(data->window, context)) {
    mruby_sdl2_raise_error(mrb);
  }
  return self;
}

static mrb_value
mrb_sdl2_video_window_set_icon(mrb_state *mrb, mrb_value self)
{
  mrb_raise(mrb, E_NOTIMP_ERROR, "not implemented.");
  return self;
}

static mrb_value
mrb_sdl2_video_window_get_brightness(mrb_state *mrb, mrb_value self)
{
  mrb_raise(mrb, E_NOTIMP_ERROR, "not implemented.");
  return self;
}

static mrb_value
mrb_sdl2_video_window_set_brightness(mrb_state *mrb, mrb_value self)
{
  mrb_raise(mrb, E_NOTIMP_ERROR, "not implemented.");
  return self;
}

static mrb_value
mrb_sdl2_video_window_get_display_index(mrb_state *mrb, mrb_value self)
{
  mrb_raise(mrb, E_NOTIMP_ERROR, "not implemented.");
  return self;
}

static mrb_value
mrb_sdl2_video_window_get_display_mode(mrb_state *mrb, mrb_value self)
{
  mrb_raise(mrb, E_NOTIMP_ERROR, "not implemented.");
  return self;
}

static mrb_value
mrb_sdl2_video_window_set_display_mode(mrb_state *mrb, mrb_value self)
{
  mrb_raise(mrb, E_NOTIMP_ERROR, "not implemented.");
  return self;
}

static mrb_value
mrb_sdl2_video_window_get_flags(mrb_state *mrb, mrb_value self)
{
  mrb_raise(mrb, E_NOTIMP_ERROR, "not implemented.");
  return self;
}

static mrb_value
mrb_sdl2_video_window_get_gamma_ramp(mrb_state *mrb, mrb_value self)
{
  mrb_raise(mrb, E_NOTIMP_ERROR, "not implemented.");
  return self;
}

static mrb_value
mrb_sdl2_video_window_set_gamma_ramp(mrb_state *mrb, mrb_value self)
{
  mrb_raise(mrb, E_NOTIMP_ERROR, "not implemented.");
  return self;
}

static mrb_value
mrb_sdl2_video_window_get_grab(mrb_state *mrb, mrb_value self)
{
  mrb_raise(mrb, E_NOTIMP_ERROR, "not implemented.");
  return self;
}

static mrb_value
mrb_sdl2_video_window_set_grab(mrb_state *mrb, mrb_value self)
{
  mrb_raise(mrb, E_NOTIMP_ERROR, "not implemented.");
  return self;
}

static mrb_value
mrb_sdl2_video_window_get_id(mrb_state *mrb, mrb_value self)
{
  mrb_raise(mrb, E_NOTIMP_ERROR, "not implemented.");
  return self;
}

static mrb_value
mrb_sdl2_video_window_get_maximum_size(mrb_state *mrb, mrb_value self)
{
  mrb_raise(mrb, E_NOTIMP_ERROR, "not implemented.");
  return self;
}

static mrb_value
mrb_sdl2_video_window_set_maximum_size(mrb_state *mrb, mrb_value self)
{
  mrb_raise(mrb, E_NOTIMP_ERROR, "not implemented.");
  return self;
}

static mrb_value
mrb_sdl2_video_window_get_minimum_size(mrb_state *mrb, mrb_value self)
{
  mrb_raise(mrb, E_NOTIMP_ERROR, "not implemented.");
  return self;
}

static mrb_value
mrb_sdl2_video_window_set_minimum_size(mrb_state *mrb, mrb_value self)
{
  mrb_raise(mrb, E_NOTIMP_ERROR, "not implemented.");
  return self;
}

static mrb_value
mrb_sdl2_video_window_get_surface(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_video_window_data_t *data =
    (mrb_sdl2_video_window_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_video_window_data_type);
  if (NULL == data->window) {
    return mrb_nil_value();
  }
  SDL_Surface *s = SDL_GetWindowSurface(data->window);
  if (NULL == s) {
    mruby_sdl2_raise_error(mrb);
  }
  return mrb_sdl2_video_surface(mrb, s, true);
}

static mrb_value
mrb_sdl2_video_window_update_surface(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_video_window_data_t *data =
    (mrb_sdl2_video_window_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_video_window_data_type);
  if (NULL == data->window) {
    return mrb_nil_value();
  }
  if (0 != SDL_UpdateWindowSurface(data->window)) {
    mruby_sdl2_raise_error(mrb);
  }
  return self;
}

static mrb_value
mrb_sdl2_video_window_update_surface_rects(mrb_state *mrb, mrb_value self)
{
  mrb_raise(mrb, E_NOTIMP_ERROR, "not implemented.");
  return self;
}

static mrb_value
mrb_sdl2_video_window_get_renderer(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_video_window_data_t *data =
    (mrb_sdl2_video_window_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_video_window_data_type);
  if (NULL == data->window) {
    return mrb_nil_value();
  }
  SDL_Renderer *renderer = SDL_GetRenderer(data->window);
  if (NULL == renderer) {
    mruby_sdl2_raise_error(mrb);
  }
  return mrb_sdl2_video_renderer(mrb, renderer);
}

static mrb_value
mrb_sdl2_video_window_swap(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_video_window_data_t *data =
    (mrb_sdl2_video_window_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_video_window_data_type);
  if (NULL == data->window) {
    return mrb_nil_value();
  }
  SDL_GL_SwapWindow(data->window);
  return self;
}

/***************************************************************************
*
* class SDL2::Video::GLContext
*
***************************************************************************/

/*
 * SDL2::Video::GLContext.initialize
 */
static mrb_value
mrb_sdl2_video_glcontext_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_video_glcontext_data_t *data =
    (mrb_sdl2_video_glcontext_data_t*)DATA_PTR(self);
  mrb_value window;
  int const argc = mrb_get_args(mrb, "|o", &window);
  if (NULL == data) {
    data = (mrb_sdl2_video_glcontext_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_video_glcontext_data_t));
    if (NULL == data) {
      mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
    }
  }
  SDL_GLContext context = NULL;
  if (0 != argc) {
    mrb_sdl2_video_window_data_t *window_data =
      (mrb_sdl2_video_window_data_t*)mrb_data_get_ptr(mrb, window, &mrb_sdl2_video_window_data_type);
    context = SDL_GL_CreateContext(window_data->window);
    if (NULL == context) {
      mrb_free(mrb, data);
      mruby_sdl2_raise_error(mrb);
    }
  }
  data->context = context;
  DATA_PTR(self) = data;
  DATA_TYPE(self) = &mrb_sdl2_video_glcontext_data_type;
  return self;
}

/*
 * SDL2::Video::GLContext.create
 */
static mrb_value
mrb_sdl2_video_glcontext_create(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_video_glcontext_data_t *data =
    (mrb_sdl2_video_glcontext_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_video_glcontext_data_type);
  mrb_value window;
  mrb_get_args(mrb, "o", &window);
  SDL_GLContext context = NULL;
  if (mrb_nil_p(window)) {
    context = SDL_GL_CreateContext(NULL);
  } else {
    mrb_sdl2_video_window_data_t *window_data =
      (mrb_sdl2_video_window_data_t*)mrb_data_get_ptr(mrb, window, &mrb_sdl2_video_window_data_type);
    context = SDL_GL_CreateContext(window_data->window);
  }
  if (NULL == context) {
    mruby_sdl2_raise_error(mrb);
  }
  data->context = context;
  return self;
}

/*
 * SDL2::Video::GLContext.delete
 */
static mrb_value
mrb_sdl2_video_glcontext_delete(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_video_glcontext_data_t *data =
    (mrb_sdl2_video_glcontext_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_video_glcontext_data_type);
  SDL_GL_DeleteContext(data->context);
  data->context = NULL;
  return self;
}


void
mruby_sdl2_video_init(mrb_state *mrb)
{
  mod_Video = mrb_define_module_under(mrb, mod_SDL2,  "Video");
  mod_GL    = mrb_define_module_under(mrb, mod_Video, "GL");
  class_DisplayMode = mrb_define_class_under(mrb, mod_Video, "DisplayMode", mrb->object_class);
  class_Window      = mrb_define_class_under(mrb, mod_Video, "Window",      mrb->object_class);
  class_GLContext   = mrb_define_class_under(mrb, mod_Video, "GLContext",   mrb->object_class);

  MRB_SET_INSTANCE_TT(class_DisplayMode, MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_Window,      MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_GLContext,   MRB_TT_DATA);

  mrb_define_module_function(mrb, mod_Video, "init", mrb_sdl2_video_init, MRB_ARGS_REQ(1));
  mrb_define_module_function(mrb, mod_Video, "quit", mrb_sdl2_video_quit, MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mod_Video, "screen_saver?", mrb_sdl2_video_get_screen_saver_enabled, MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mod_Video, "screen_saver=", mrb_sdl2_video_set_screen_saver_enabled, MRB_ARGS_REQ(1));
  mrb_define_module_function(mrb, mod_Video, "video_drivers", mrb_sdl2_video_get_video_drivers,        MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mod_Video, "display_modes", mrb_sdl2_video_get_display_modes,        MRB_ARGS_REQ(1));
  mrb_define_module_function(mrb, mod_Video, "displays",      mrb_sdl2_video_get_displays,             MRB_ARGS_NONE());

  mrb_define_module_function(mrb, mod_GL, "attribute_get",        mrb_sdl2_video_gl_get_attribute,          MRB_ARGS_REQ(1));
  mrb_define_module_function(mrb, mod_GL, "attribute_set",        mrb_sdl2_video_gl_set_attribute,          MRB_ARGS_REQ(2));
  mrb_define_module_function(mrb, mod_GL, "extension_supported?", mrb_sdl2_video_gl_is_extension_supported, MRB_ARGS_REQ(1));
  mrb_define_module_function(mrb, mod_GL, "swap_interval",        mrb_sdl2_video_gl_get_swap_interval,      MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mod_GL, "swap_interval=",       mrb_sdl2_video_gl_set_swap_interval,      MRB_ARGS_REQ(1));
  mrb_define_module_function(mrb, mod_GL, "swap_window",          mrb_sdl2_video_gl_swap_window,            MRB_ARGS_REQ(1));

  mrb_define_method(mrb, class_Window, "initialize",           mrb_sdl2_video_window_initialize,           MRB_ARGS_OPT(6));
  mrb_define_method(mrb, class_Window, "create",               mrb_sdl2_video_window_create,               MRB_ARGS_REQ(6));
  mrb_define_method(mrb, class_Window, "create_with_renderer", mrb_sdl2_video_window_create_with_renderer, MRB_ARGS_REQ(3));
  mrb_define_method(mrb, class_Window, "destroy",              mrb_sdl2_video_window_destroy,              MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Window, "size",                 mrb_sdl2_video_window_get_size,             MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Window, "size=",                mrb_sdl2_video_window_set_size,             MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Window, "position",             mrb_sdl2_video_window_get_position,         MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Window, "position=",            mrb_sdl2_video_window_set_position,         MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Window, "title",                mrb_sdl2_video_window_get_title,            MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Window, "title=",               mrb_sdl2_video_window_set_title,            MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Window, "fullscreen=",          mrb_sdl2_video_window_set_fullscreen,       MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Window, "maximize",             mrb_sdl2_video_window_maximize,             MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Window, "minimize",             mrb_sdl2_video_window_minimize,             MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Window, "show",                 mrb_sdl2_video_window_show,                 MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Window, "hide",                 mrb_sdl2_video_window_hide,                 MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Window, "restore",              mrb_sdl2_video_window_restore,              MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Window, "raise",                mrb_sdl2_video_window_raise,                MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Window, "make_current",         mrb_sdl2_video_window_make_current,         MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Window, "icon=",                mrb_sdl2_video_window_set_icon,             MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Window, "brightness",           mrb_sdl2_video_window_get_brightness,       MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Window, "brightness=",          mrb_sdl2_video_window_set_brightness,       MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Window, "diplay_index",         mrb_sdl2_video_window_get_display_index,    MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Window, "display_mode",         mrb_sdl2_video_window_get_display_mode,     MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Window, "display_mode=",        mrb_sdl2_video_window_set_display_mode,     MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Window, "flags",                mrb_sdl2_video_window_get_flags,            MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Window, "gamma_ramp",           mrb_sdl2_video_window_get_gamma_ramp,       MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Window, "gamma_ramp=",          mrb_sdl2_video_window_set_gamma_ramp,       MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Window, "grab",                 mrb_sdl2_video_window_get_grab,             MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Window, "grab=",                mrb_sdl2_video_window_set_grab,             MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Window, "id",                   mrb_sdl2_video_window_get_id,               MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Window, "maximum_size",         mrb_sdl2_video_window_get_maximum_size,     MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Window, "maximum_size=",        mrb_sdl2_video_window_set_maximum_size,     MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Window, "minimum_size",         mrb_sdl2_video_window_get_minimum_size,     MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Window, "minimum_size=",        mrb_sdl2_video_window_set_minimum_size,     MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Window, "surface",              mrb_sdl2_video_window_get_surface,          MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Window, "update_surface",       mrb_sdl2_video_window_update_surface,       MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Window, "update_surface_rects", mrb_sdl2_video_window_update_surface_rects, MRB_ARGS_ANY());
  mrb_define_method(mrb, class_Window, "renderer",             mrb_sdl2_video_window_get_renderer,         MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Window, "swap",                 mrb_sdl2_video_window_swap,                 MRB_ARGS_NONE());

  mrb_define_method(mrb, class_GLContext, "initialize", mrb_sdl2_video_glcontext_initialize, MRB_ARGS_OPT(1));
  mrb_define_method(mrb, class_GLContext, "create",     mrb_sdl2_video_glcontext_create,     MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_GLContext, "delete",     mrb_sdl2_video_glcontext_delete,     MRB_ARGS_NONE());
  mrb_define_method(mrb, class_GLContext, "destroy",    mrb_sdl2_video_glcontext_delete,     MRB_ARGS_NONE());

  int arena_size = mrb_gc_arena_save(mrb);

  /* SDL_BlendMode */
  mrb_define_const(mrb, mod_Video, "SDL_BLENDMODE_NONE",  mrb_fixnum_value(SDL_BLENDMODE_NONE));
  mrb_define_const(mrb, mod_Video, "SDL_BLENDMODE_BLEND", mrb_fixnum_value(SDL_BLENDMODE_BLEND));
  mrb_define_const(mrb, mod_Video, "SDL_BLENDMODE_ADD",   mrb_fixnum_value(SDL_BLENDMODE_ADD));
  mrb_define_const(mrb, mod_Video, "SDL_BLENDMODE_MOD",   mrb_fixnum_value(SDL_BLENDMODE_MOD));

  mrb_gc_arena_restore(mrb, arena_size);
  arena_size = mrb_gc_arena_save(mrb);

  /* SDL_WindowFlags */
  mrb_define_const(mrb, class_Window, "SDL_WINDOW_FULLSCREEN",         mrb_fixnum_value(SDL_WINDOW_FULLSCREEN));
  mrb_define_const(mrb, class_Window, "SDL_WINDOW_FULLSCREEN_DESKTOP", mrb_fixnum_value(SDL_WINDOW_FULLSCREEN_DESKTOP));
  mrb_define_const(mrb, class_Window, "SDL_WINDOW_OPENGL",             mrb_fixnum_value(SDL_WINDOW_OPENGL));
  mrb_define_const(mrb, class_Window, "SDL_WINDOW_SHOWN",              mrb_fixnum_value(SDL_WINDOW_SHOWN));
  mrb_define_const(mrb, class_Window, "SDL_WINDOW_HIDDEN",             mrb_fixnum_value(SDL_WINDOW_HIDDEN));
  mrb_define_const(mrb, class_Window, "SDL_WINDOW_BORDERLESS",         mrb_fixnum_value(SDL_WINDOW_BORDERLESS));
  mrb_define_const(mrb, class_Window, "SDL_WINDOW_RESIZABLE",          mrb_fixnum_value(SDL_WINDOW_RESIZABLE));
  mrb_define_const(mrb, class_Window, "SDL_WINDOW_MINIMIZED",          mrb_fixnum_value(SDL_WINDOW_MINIMIZED));
  mrb_define_const(mrb, class_Window, "SDL_WINDOW_MAXIMIZED",          mrb_fixnum_value(SDL_WINDOW_MAXIMIZED));
  mrb_define_const(mrb, class_Window, "SDL_WINDOW_INPUT_GRABBED",      mrb_fixnum_value(SDL_WINDOW_INPUT_GRABBED));
  mrb_define_const(mrb, class_Window, "SDL_WINDOW_INPUT_FOCUS",        mrb_fixnum_value(SDL_WINDOW_INPUT_FOCUS));
  mrb_define_const(mrb, class_Window, "SDL_WINDOW_MOUSE_FOCUS",        mrb_fixnum_value(SDL_WINDOW_MOUSE_FOCUS));
  mrb_define_const(mrb, class_Window, "SDL_WINDOW_FOREIGN",            mrb_fixnum_value(SDL_WINDOW_FOREIGN));

  mrb_define_const(mrb, class_Window, "SDL_WINDOWPOS_CENTERED",  mrb_fixnum_value(SDL_WINDOWPOS_CENTERED));
  mrb_define_const(mrb, class_Window, "SDL_WINDOWPOS_UNDEFINED", mrb_fixnum_value(SDL_WINDOWPOS_UNDEFINED));

  mrb_gc_arena_restore(mrb, arena_size);
  arena_size = mrb_gc_arena_save(mrb);

  /* SDL_GLattr */
  mrb_define_const(mrb, mod_GL, "SDL_GL_RED_SIZE",                   mrb_fixnum_value(SDL_GL_RED_SIZE));
  mrb_define_const(mrb, mod_GL, "SDL_GL_GREEN_SIZE",                 mrb_fixnum_value(SDL_GL_GREEN_SIZE));
  mrb_define_const(mrb, mod_GL, "SDL_GL_BLUE_SIZE",                  mrb_fixnum_value(SDL_GL_BLUE_SIZE));
  mrb_define_const(mrb, mod_GL, "SDL_GL_ALPHA_SIZE",                 mrb_fixnum_value(SDL_GL_ALPHA_SIZE));
  mrb_define_const(mrb, mod_GL, "SDL_GL_BUFFER_SIZE",                mrb_fixnum_value(SDL_GL_BUFFER_SIZE));
  mrb_define_const(mrb, mod_GL, "SDL_GL_DOUBLEBUFFER",               mrb_fixnum_value(SDL_GL_DOUBLEBUFFER));
  mrb_define_const(mrb, mod_GL, "SDL_GL_DEPTH_SIZE",                 mrb_fixnum_value(SDL_GL_DEPTH_SIZE));
  mrb_define_const(mrb, mod_GL, "SDL_GL_STENCIL_SIZE",               mrb_fixnum_value(SDL_GL_STENCIL_SIZE));
  mrb_define_const(mrb, mod_GL, "SDL_GL_ACCUM_RED_SIZE",             mrb_fixnum_value(SDL_GL_ACCUM_RED_SIZE));
  mrb_define_const(mrb, mod_GL, "SDL_GL_ACCUM_GREEN_SIZE",           mrb_fixnum_value(SDL_GL_ACCUM_GREEN_SIZE));
  mrb_define_const(mrb, mod_GL, "SDL_GL_ACCUM_BLUE_SIZE",            mrb_fixnum_value(SDL_GL_ACCUM_BLUE_SIZE));
  mrb_define_const(mrb, mod_GL, "SDL_GL_ACCUM_ALPHA_SIZE",           mrb_fixnum_value(SDL_GL_ACCUM_ALPHA_SIZE));
  mrb_define_const(mrb, mod_GL, "SDL_GL_STEREO",                     mrb_fixnum_value(SDL_GL_STEREO));
  mrb_define_const(mrb, mod_GL, "SDL_GL_MULTISAMPLEBUFFERS",         mrb_fixnum_value(SDL_GL_MULTISAMPLEBUFFERS));
  mrb_define_const(mrb, mod_GL, "SDL_GL_MULTISAMPLESAMPLES",         mrb_fixnum_value(SDL_GL_MULTISAMPLESAMPLES));
  mrb_define_const(mrb, mod_GL, "SDL_GL_ACCELERATED_VISUAL",         mrb_fixnum_value(SDL_GL_ACCELERATED_VISUAL));
  mrb_define_const(mrb, mod_GL, "SDL_GL_CONTEXT_MAJOR_VERSION",      mrb_fixnum_value(SDL_GL_CONTEXT_MAJOR_VERSION));
  mrb_define_const(mrb, mod_GL, "SDL_GL_CONTEXT_MINOR_VERSION",      mrb_fixnum_value(SDL_GL_CONTEXT_MINOR_VERSION));
  mrb_define_const(mrb, mod_GL, "SDL_GL_CONTEXT_EGL",                mrb_fixnum_value(SDL_GL_CONTEXT_EGL));
  mrb_define_const(mrb, mod_GL, "SDL_GL_CONTEXT_FLAGS",              mrb_fixnum_value(SDL_GL_CONTEXT_FLAGS));
  mrb_define_const(mrb, mod_GL, "SDL_GL_CONTEXT_PROFILE_MASK",       mrb_fixnum_value(SDL_GL_CONTEXT_PROFILE_MASK));
  mrb_define_const(mrb, mod_GL, "SDL_GL_SHARE_WITH_CURRENT_CONTEXT", mrb_fixnum_value(SDL_GL_SHARE_WITH_CURRENT_CONTEXT));

  mrb_gc_arena_restore(mrb, arena_size);
  arena_size = mrb_gc_arena_save(mrb);

  /* SDL_GLcontextFlag */
  mrb_define_const(mrb, mod_GL, "SDL_GL_CONTEXT_DEBUG_FLAG",              mrb_fixnum_value(SDL_GL_CONTEXT_DEBUG_FLAG));
  mrb_define_const(mrb, mod_GL, "SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG", mrb_fixnum_value(SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG));
  mrb_define_const(mrb, mod_GL, "SDL_GL_CONTEXT_ROBUST_ACCESS_FLAG",      mrb_fixnum_value(SDL_GL_CONTEXT_ROBUST_ACCESS_FLAG));
  mrb_define_const(mrb, mod_GL, "SDL_GL_CONTEXT_RESET_ISOLATION_FLAG",    mrb_fixnum_value(SDL_GL_CONTEXT_RESET_ISOLATION_FLAG));

  /* SDL_GLprofile */
  mrb_define_const(mrb, mod_GL, "SDL_GL_CONTEXT_PROFILE_CORE",          mrb_fixnum_value(SDL_GL_CONTEXT_PROFILE_CORE));
  mrb_define_const(mrb, mod_GL, "SDL_GL_CONTEXT_PROFILE_COMPATIBILITY", mrb_fixnum_value(SDL_GL_CONTEXT_PROFILE_COMPATIBILITY));
  mrb_define_const(mrb, mod_GL, "SDL_GL_CONTEXT_PROFILE_ES",            mrb_fixnum_value(SDL_GL_CONTEXT_PROFILE_ES));

  mrb_gc_arena_restore(mrb, arena_size);
  arena_size = mrb_gc_arena_save(mrb);

  mruby_sdl2_video_renderer_init(mrb, mod_Video);
  mruby_sdl2_video_surface_init(mrb, mod_Video);

  mrb_gc_arena_restore(mrb, arena_size);
}

void
mruby_sdl2_video_final(mrb_state *mrb)
{
  mruby_sdl2_video_surface_final(mrb, mod_Video);
  mruby_sdl2_video_renderer_final(mrb, mod_Video);
}
