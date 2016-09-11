#include "sdl2_events.h"
#include "sdl2_keyboard.h"
#include "mruby/value.h"
#include "mruby/data.h"
#include "mruby/class.h"
#include "mruby/string.h"
#include <SDL2/SDL_keyboard.h>

static struct RClass *mod_Input = NULL;
static struct RClass *class_ControllerAxisEvent   = NULL;
static struct RClass *class_ControllerButtonEvent = NULL;
static struct RClass *class_ControllerDeviceEvent = NULL;
static struct RClass *class_DollarGestureEvent    = NULL;
static struct RClass *class_DropEvent             = NULL;
static struct RClass *class_Event                 = NULL;
static struct RClass *class_JoyAxisEvent          = NULL;
static struct RClass *class_JoyBallEvent          = NULL;
static struct RClass *class_JoyButtonEvent        = NULL;
static struct RClass *class_JoyHatEvent           = NULL;
static struct RClass *class_JoyDeviceEvent        = NULL;
static struct RClass *class_KeyboardEvent         = NULL;
static struct RClass *class_MouseButtonEvent      = NULL;
static struct RClass *class_MouseMotionEvent      = NULL;
static struct RClass *class_MouseWheelEvent       = NULL;
static struct RClass *class_MultiGestureEvent     = NULL;
static struct RClass *class_OsEvent               = NULL;
static struct RClass *class_QuitEvent             = NULL;
static struct RClass *class_SysWMEvent            = NULL;
static struct RClass *class_TextEditingEvent      = NULL;
static struct RClass *class_TextInputEvent        = NULL;
static struct RClass *class_TouchFingerEvent      = NULL;
static struct RClass *class_UserEvent             = NULL;
static struct RClass *class_WindowEvent           = NULL;

typedef struct mrb_sdl2_input_event_data_t {
  SDL_Event event;
} mrb_sdl2_input_event_data_t;

static void
mrb_sdl2_input_event_data_free(mrb_state *mrb, void *p)
{
  mrb_free(mrb, p);
}

static struct mrb_data_type const mrb_sdl2_input_event_data_type = {
  "Event", &mrb_sdl2_input_event_data_free
};

SDL_Event *
mrb_sdl2_input_event_get_ptr(mrb_state *mrb, mrb_value value)
{
  if (mrb_nil_p(value)) {
    return NULL;
  }
  mrb_sdl2_input_event_data_t *data =
    (mrb_sdl2_input_event_data_t*)mrb_data_get_ptr(mrb, value, &mrb_sdl2_input_event_data_type);
  return &data->event;
}

mrb_value
mrb_sdl2_input_event(mrb_state *mrb, SDL_Event const *event)
{
  if (NULL == event) {
    return mrb_nil_value();
  }

  mrb_sdl2_input_event_data_t *data =
    (mrb_sdl2_input_event_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_input_event_data_t));
  if (NULL == data) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
  }
  data->event = *event;

  switch (event->type) {
  case SDL_QUIT:
    return mrb_obj_value(Data_Wrap_Struct(mrb, class_QuitEvent,   &mrb_sdl2_input_event_data_type, data));
  case SDL_APP_TERMINATING:
  case SDL_APP_LOWMEMORY:
  case SDL_APP_WILLENTERBACKGROUND:
  case SDL_APP_DIDENTERBACKGROUND:
  case SDL_APP_WILLENTERFOREGROUND:
  case SDL_APP_DIDENTERFOREGROUND:
    return mrb_obj_value(Data_Wrap_Struct(mrb, class_OsEvent, &mrb_sdl2_input_event_data_type, data));
  case SDL_WINDOWEVENT:
    return mrb_obj_value(Data_Wrap_Struct(mrb, class_WindowEvent, &mrb_sdl2_input_event_data_type, data));
  case SDL_SYSWMEVENT:
    return mrb_obj_value(Data_Wrap_Struct(mrb, class_SysWMEvent, &mrb_sdl2_input_event_data_type, data));
  case SDL_KEYDOWN:
  case SDL_KEYUP:
    return mrb_obj_value(Data_Wrap_Struct(mrb, class_KeyboardEvent, &mrb_sdl2_input_event_data_type, data));
  case SDL_TEXTEDITING:
    return mrb_obj_value(Data_Wrap_Struct(mrb, class_TextEditingEvent, &mrb_sdl2_input_event_data_type, data));
  case SDL_TEXTINPUT:
    return mrb_obj_value(Data_Wrap_Struct(mrb, class_TextInputEvent, &mrb_sdl2_input_event_data_type, data));
  case SDL_MOUSEMOTION:
    return mrb_obj_value(Data_Wrap_Struct(mrb, class_MouseMotionEvent, &mrb_sdl2_input_event_data_type, data));
  case SDL_MOUSEBUTTONDOWN:
  case SDL_MOUSEBUTTONUP:
    return mrb_obj_value(Data_Wrap_Struct(mrb, class_MouseButtonEvent, &mrb_sdl2_input_event_data_type, data));
  case SDL_MOUSEWHEEL:
    return mrb_obj_value(Data_Wrap_Struct(mrb, class_MouseWheelEvent, &mrb_sdl2_input_event_data_type, data));
  case SDL_JOYAXISMOTION:
    return mrb_obj_value(Data_Wrap_Struct(mrb, class_JoyAxisEvent, &mrb_sdl2_input_event_data_type, data));
  case SDL_JOYBALLMOTION:
    return mrb_obj_value(Data_Wrap_Struct(mrb, class_JoyBallEvent, &mrb_sdl2_input_event_data_type, data));
  case SDL_JOYHATMOTION:
    return mrb_obj_value(Data_Wrap_Struct(mrb, class_JoyHatEvent, &mrb_sdl2_input_event_data_type, data));
  case SDL_JOYBUTTONDOWN:
  case SDL_JOYBUTTONUP:
    return mrb_obj_value(Data_Wrap_Struct(mrb, class_JoyButtonEvent, &mrb_sdl2_input_event_data_type, data));
  case SDL_JOYDEVICEADDED:
  case SDL_JOYDEVICEREMOVED:
    return mrb_obj_value(Data_Wrap_Struct(mrb, class_JoyDeviceEvent, &mrb_sdl2_input_event_data_type, data));
  case SDL_CONTROLLERAXISMOTION:
    return mrb_obj_value(Data_Wrap_Struct(mrb, class_ControllerAxisEvent, &mrb_sdl2_input_event_data_type, data));
  case SDL_CONTROLLERBUTTONDOWN:
  case SDL_CONTROLLERBUTTONUP:
    return mrb_obj_value(Data_Wrap_Struct(mrb, class_ControllerButtonEvent, &mrb_sdl2_input_event_data_type, data));
  case SDL_CONTROLLERDEVICEADDED:
  case SDL_CONTROLLERDEVICEREMOVED:
  case SDL_CONTROLLERDEVICEREMAPPED:
    return mrb_obj_value(Data_Wrap_Struct(mrb, class_ControllerDeviceEvent, &mrb_sdl2_input_event_data_type, data));
  case SDL_FINGERDOWN:
  case SDL_FINGERUP:
  case SDL_FINGERMOTION:
    return mrb_obj_value(Data_Wrap_Struct(mrb, class_TouchFingerEvent, &mrb_sdl2_input_event_data_type, data));
  case SDL_DOLLARGESTURE:
  case SDL_DOLLARRECORD:
    return mrb_obj_value(Data_Wrap_Struct(mrb, class_DollarGestureEvent, &mrb_sdl2_input_event_data_type, data));
  case SDL_MULTIGESTURE:
    return mrb_obj_value(Data_Wrap_Struct(mrb, class_MultiGestureEvent, &mrb_sdl2_input_event_data_type, data));
  case SDL_CLIPBOARDUPDATE:
    break; /* missing event */
  case SDL_DROPFILE:
    return mrb_obj_value(Data_Wrap_Struct(mrb, class_DropEvent, &mrb_sdl2_input_event_data_type, data));
  case SDL_USEREVENT:
    return mrb_obj_value(Data_Wrap_Struct(mrb, class_UserEvent, &mrb_sdl2_input_event_data_type, data));
  default:
    if (event->type > SDL_USEREVENT) {
      return mrb_obj_value(Data_Wrap_Struct(mrb, class_UserEvent, &mrb_sdl2_input_event_data_type, data));
    } else {
      mrb_free(mrb, data);
      mrb_raise(mrb, E_RUNTIME_ERROR, "undefined event type.");
    }
    break;
  }

  mrb_free(mrb, data);
  return mrb_nil_value();
}

/***************************************************************************
*
* module SDL2::Input
*
***************************************************************************/

static mrb_value
mrb_sdl2_input_poll(mrb_state *mrb, mrb_value mod)
{
  SDL_Event event;
  if (0 == SDL_PollEvent(&event)) {
    return mrb_nil_value();
  }
  return mrb_sdl2_input_event(mrb, &event);
}

static mrb_value
mrb_sdl2_input_wait(mrb_state *mrb, mrb_value mod)
{
  SDL_Event event;
  if (0 == SDL_WaitEvent(&event)) {
    mruby_sdl2_raise_error(mrb);
  }
  return mrb_sdl2_input_event(mrb, &event);
}

static mrb_value
mrb_sdl2_input_wait_timeout(mrb_state *mrb, mrb_value mod)
{
  mrb_int timeout;
  mrb_get_args(mrb, "i", &timeout);
  SDL_Event event;
  if (0 == SDL_WaitEventTimeout(&event, timeout)) {
    return mrb_nil_value();
  }
  return mrb_sdl2_input_event(mrb, &event);
}


static mrb_value
mrb_sdl2_input_event_get_type(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_input_event_data_t *data =
    (mrb_sdl2_input_event_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_input_event_data_type);
  return mrb_fixnum_value(data->event.type);
}

static mrb_value
mrb_sdl2_input_event_state(mrb_state *mrb, mrb_value self)
{
  mrb_int type, state;
  mrb_get_args(mrb, "ii", &type, &state);
  return mrb_fixnum_value(SDL_EventState(type, state));
}

static mrb_value
mrb_sdl2_input_flush_event(mrb_state *mrb, mrb_value self)
{
  mrb_int min, max;
  int const argc = mrb_get_args(mrb, "i|i", &min, &max);
  if (1 == argc) {
    SDL_FlushEvent(min);
  } else {
    SDL_FlushEvents(min, max);
  }
  return self;
}

static mrb_value
mrb_sdl2_input_has_events(mrb_state *mrb, mrb_value self)
{
  mrb_int min, max;
  int const argc = mrb_get_args(mrb, "i|i", &min, &max);
  SDL_bool has_event;
  if (1 == argc) {
    has_event = SDL_HasEvent(min);
  } else {
    has_event = SDL_HasEvents(min, max);
  }
  return (SDL_FALSE == has_event) ? mrb_false_value() : mrb_true_value();
}

static mrb_value
mrb_sdl2_input_is_quit_requested(mrb_state *mrb, mrb_value self)
{
  return (SDL_FALSE == SDL_QuitRequested()) ? mrb_false_value() : mrb_true_value();
}

static mrb_value
mrb_sdl2_input_register(mrb_state *mrb, mrb_value self)
{
  mrb_int num;
  mrb_get_args(mrb, "i", &num);
  return mrb_fixnum_value(SDL_RegisterEvents(num));
}

static mrb_value
mrb_sdl2_input_push(mrb_state *mrb, mrb_value self)
{
  mrb_value event;
  mrb_get_args(mrb, "o", &event);
  mrb_sdl2_input_event_data_t *data =
    (mrb_sdl2_input_event_data_t*)mrb_data_get_ptr(mrb, event, &mrb_sdl2_input_event_data_type);
  int const ret = SDL_PushEvent(&data->event);
  if (0 > ret) {
    mruby_sdl2_raise_error(mrb);
  }
  return mrb_fixnum_value(ret);
}

/***************************************************************************
*
* class SDL2::Input::KeyboardEvent
*
***************************************************************************/

static mrb_value
mrb_sdl2_input_keyboardevent_get_timestamp(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_input_event_data_t *data =
    (mrb_sdl2_input_event_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_input_event_data_type);
  return mrb_fixnum_value(data->event.key.timestamp);
}

static mrb_value
mrb_sdl2_input_keyboardevent_get_window_id(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_input_event_data_t *data =
    (mrb_sdl2_input_event_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_input_event_data_type);
  return mrb_fixnum_value(data->event.key.windowID);
}

static mrb_value
mrb_sdl2_input_keyboardevent_get_state(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_input_event_data_t *data =
    (mrb_sdl2_input_event_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_input_event_data_type);
  return mrb_fixnum_value(data->event.key.state);
}

static mrb_value
mrb_sdl2_input_keyboardevent_get_repeat(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_input_event_data_t *data =
    (mrb_sdl2_input_event_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_input_event_data_type);
  return mrb_fixnum_value(data->event.key.repeat);
}

static mrb_value
mrb_sdl2_input_keyboardevent_get_keysym(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_input_event_data_t *data =
    (mrb_sdl2_input_event_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_input_event_data_type);
  return mrb_sdl2_keyboard_keysym(mrb, &data->event.key.keysym);
}

/***************************************************************************
*
* class SDL2::Input::MouseButtonEvent
*
***************************************************************************/

static mrb_value
mrb_sdl2_input_mousebuttonevent_get_timestamp(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_input_event_data_t *data =
    (mrb_sdl2_input_event_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_input_event_data_type);
  return mrb_fixnum_value(data->event.button.timestamp);
}

static mrb_value
mrb_sdl2_input_mousebuttonevent_get_window_id(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_input_event_data_t *data =
    (mrb_sdl2_input_event_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_input_event_data_type);
  return mrb_fixnum_value(data->event.button.timestamp);
}

static mrb_value
mrb_sdl2_input_mousebuttonevent_get_which(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_input_event_data_t *data =
    (mrb_sdl2_input_event_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_input_event_data_type);
  return mrb_fixnum_value(data->event.button.which);
}

static mrb_value
mrb_sdl2_input_mousebuttonevent_get_button(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_input_event_data_t *data =
    (mrb_sdl2_input_event_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_input_event_data_type);
  return mrb_fixnum_value(data->event.button.button);
}

static mrb_value
mrb_sdl2_input_mousebuttonevent_get_state(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_input_event_data_t *data =
    (mrb_sdl2_input_event_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_input_event_data_type);
  return mrb_fixnum_value(data->event.button.state);
}

static mrb_value
mrb_sdl2_input_mousebuttonevent_get_x(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_input_event_data_t *data =
    (mrb_sdl2_input_event_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_input_event_data_type);
  return mrb_fixnum_value(data->event.button.x);
}

static mrb_value
mrb_sdl2_input_mousebuttonevent_get_y(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_input_event_data_t *data =
    (mrb_sdl2_input_event_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_input_event_data_type);
  return mrb_fixnum_value(data->event.button.y);
}


/***************************************************************************
*
* class SDL2::Input::MouseMotionEvent
*
***************************************************************************/

static mrb_value
mrb_sdl2_input_mousemotionevent_get_timestamp(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_input_event_data_t *data =
    (mrb_sdl2_input_event_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_input_event_data_type);
  return mrb_fixnum_value(data->event.motion.timestamp);
}

static mrb_value
mrb_sdl2_input_mousemotionevent_get_window_id(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_input_event_data_t *data =
    (mrb_sdl2_input_event_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_input_event_data_type);
  return mrb_fixnum_value(data->event.motion.windowID);
}

static mrb_value
mrb_sdl2_input_mousemotionevent_get_which(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_input_event_data_t *data =
    (mrb_sdl2_input_event_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_input_event_data_type);
  return mrb_fixnum_value(data->event.motion.which);
}

static mrb_value
mrb_sdl2_input_mousemotionevent_get_state(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_input_event_data_t *data =
    (mrb_sdl2_input_event_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_input_event_data_type);
  return mrb_fixnum_value(data->event.motion.state);
}

static mrb_value
mrb_sdl2_input_mousemotionevent_get_x(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_input_event_data_t *data =
    (mrb_sdl2_input_event_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_input_event_data_type);
  return mrb_fixnum_value(data->event.motion.x);
}

static mrb_value
mrb_sdl2_input_mousemotionevent_get_y(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_input_event_data_t *data =
    (mrb_sdl2_input_event_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_input_event_data_type);
  return mrb_fixnum_value(data->event.motion.y);
}

static mrb_value
mrb_sdl2_input_mousemotionevent_get_xrel(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_input_event_data_t *data =
    (mrb_sdl2_input_event_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_input_event_data_type);
  return mrb_fixnum_value(data->event.motion.xrel);
}

static mrb_value
mrb_sdl2_input_mousemotionevent_get_yrel(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_input_event_data_t *data =
    (mrb_sdl2_input_event_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_input_event_data_type);
  return mrb_fixnum_value(data->event.motion.yrel);
}


/***************************************************************************
*
* class SDL2::Input::MouseWheelEvent
*
***************************************************************************/

static mrb_value
mrb_sdl2_input_mousewheelevent_get_timestamp(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_input_event_data_t *data =
    (mrb_sdl2_input_event_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_input_event_data_type);
  return mrb_fixnum_value(data->event.wheel.timestamp);
}

static mrb_value
mrb_sdl2_input_mousewheelevent_get_window_id(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_input_event_data_t *data =
    (mrb_sdl2_input_event_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_input_event_data_type);
  return mrb_fixnum_value(data->event.wheel.windowID);
}

static mrb_value
mrb_sdl2_input_mousewheelevent_get_which(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_input_event_data_t *data =
    (mrb_sdl2_input_event_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_input_event_data_type);
  return mrb_fixnum_value(data->event.wheel.which);
}

static mrb_value
mrb_sdl2_input_mousewheelevent_get_x(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_input_event_data_t *data =
    (mrb_sdl2_input_event_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_input_event_data_type);
  return mrb_fixnum_value(data->event.wheel.x);
}

static mrb_value
mrb_sdl2_input_mousewheelevent_get_y(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_input_event_data_t *data =
    (mrb_sdl2_input_event_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_input_event_data_type);
  return mrb_fixnum_value(data->event.wheel.y);
}

/***************************************************************************
*
* class SDL2::Input::QuitEvent
*
***************************************************************************/

static mrb_value
mrb_sdl2_input_quitevent_timestamp(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_input_event_data_t *data =
    (mrb_sdl2_input_event_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_input_event_data_type);
  return mrb_fixnum_value(data->event.quit.timestamp);
}

/***************************************************************************
*
* class SDL2::Input::QuitEvent
*
***************************************************************************/

typedef struct mrb_sdl2_input_user_data_t {
  enum mrb_vtype type;
  union udata_t {
    mrb_int   fixnum_value;
    mrb_float float_value;
    void *    cptr_value;
    char *    string_value;
  } data;
} mrb_sdl2_input_user_data_t;

static void *
mrb_sdl2_input_to_voidp(mrb_state *mrb, mrb_value data)
{
  mrb_sdl2_input_user_data_t *udata =
    (mrb_sdl2_input_user_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_input_user_data_t));
  if (NULL == udata) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
  }
  switch (mrb_type(data)) {
  case MRB_TT_FIXNUM:
    udata->data.fixnum_value = mrb_fixnum(data);
    break;
  case MRB_TT_FLOAT:
    udata->data.float_value = mrb_float(data);
    break;
  case MRB_TT_STRING: {
    size_t const len = RSTRING_LEN(data);
    char *str = (char*)mrb_malloc(mrb, sizeof(char) * (len + 1));
    if (NULL == str) {
      mrb_free(mrb, udata);
      mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
    }
    str[len] = '\0';
    udata->data.string_value = str;
    break;
  }
  case MRB_TT_CPTR:
    udata->data.cptr_value = mrb_cptr(data);
    break;
  default:
    mrb_free(mrb, udata);
    mrb_raise(mrb, E_TYPE_ERROR, "expected Fixnum/Float/String/Cptr");
    break;
  }
  udata->type = mrb_type(data);
  return (void*)udata;
}

static mrb_value
mrb_sdl2_input_to_value(mrb_state *mrb, void *data)
{
  mrb_sdl2_input_user_data_t *udata =
    (mrb_sdl2_input_user_data_t*)data;
  if (NULL == udata) {
    return mrb_nil_value();
  }
  mrb_value value;
  switch (udata->type) {
  case MRB_TT_FIXNUM:
    value = mrb_fixnum_value(udata->data.fixnum_value);
    break;
  case MRB_TT_FLOAT:
    value = mrb_float_value(mrb, udata->data.float_value);
    break;
  case MRB_TT_STRING:
    value = mrb_str_new_cstr(mrb, udata->data.string_value);
    mrb_free(mrb, udata->data.string_value);
    break;
  case MRB_TT_CPTR:
    value = mrb_cptr_value(mrb, udata->data.cptr_value);
    break;
  default:
    value = mrb_nil_value();
    break;
  }
  mrb_free(mrb, data);
  return value;
}

static mrb_value
mrb_sdl2_input_userevent_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_input_event_data_t *data =
    (mrb_sdl2_input_event_data_t*)DATA_PTR(self);

  if (NULL == data) {
    data = (mrb_sdl2_input_event_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_input_event_data_t));
    if (NULL == data) {
      mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
    }
    data->event = (SDL_Event){ 0 };
  }

  mrb_int type, code;
  mrb_value data1, data2;
  int const argc = mrb_get_args(mrb, "ii|oo", &type, &code, &data1, &data2);

  switch (argc) {
  case 4:
    data->event.user.data2 = mrb_sdl2_input_to_voidp(mrb, data2);
  case 3:
    data->event.user.data1 = mrb_sdl2_input_to_voidp(mrb, data1);
  case 2:
    data->event.user.type = type;
    data->event.user.code = code;
    break;
  }

  DATA_PTR(self) = data;
  DATA_TYPE(self) = &mrb_sdl2_input_event_data_type;

  return self;
}

static mrb_value
mrb_sdl2_input_userevent_set_type(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_input_event_data_t *data =
    (mrb_sdl2_input_event_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_input_event_data_type);
  mrb_int type;
  mrb_get_args(mrb, "i", &type);
  data->event.user.type = type;
  return self;
}

static mrb_value
mrb_sdl2_input_userevent_get_timestamp(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_input_event_data_t *data =
    (mrb_sdl2_input_event_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_input_event_data_type);
  return mrb_fixnum_value(data->event.user.timestamp);
}

static mrb_value
mrb_sdl2_input_userevent_set_timestamp(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_input_event_data_t *data =
    (mrb_sdl2_input_event_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_input_event_data_type);
  mrb_int timestamp;
  mrb_get_args(mrb, "i", &timestamp);
  data->event.user.timestamp = timestamp;
  return self;
}

static mrb_value
mrb_sdl2_input_userevent_get_window_id(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_input_event_data_t *data =
    (mrb_sdl2_input_event_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_input_event_data_type);
  return mrb_fixnum_value(data->event.user.windowID);
}

static mrb_value
mrb_sdl2_input_userevent_set_window_id(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_input_event_data_t *data =
    (mrb_sdl2_input_event_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_input_event_data_type);
  mrb_int windowID;
  mrb_get_args(mrb, "i", &windowID);
  data->event.user.windowID = windowID;
  return self;
}

static mrb_value
mrb_sdl2_input_userevent_get_code(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_input_event_data_t *data =
    (mrb_sdl2_input_event_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_input_event_data_type);
  return mrb_fixnum_value(data->event.user.code);
}

static mrb_value
mrb_sdl2_input_userevent_set_code(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_input_event_data_t *data =
    (mrb_sdl2_input_event_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_input_event_data_type);
  mrb_int code;
  mrb_get_args(mrb, "i", &code);
  data->event.user.code = code;
  return self;
}

static mrb_value
mrb_sdl2_input_userevent_get_data1(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_input_event_data_t *data =
    (mrb_sdl2_input_event_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_input_event_data_type);
  return mrb_sdl2_input_to_value(mrb, data->event.user.data1);
}

static mrb_value
mrb_sdl2_input_userevent_set_data1(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_input_event_data_t *data =
    (mrb_sdl2_input_event_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_input_event_data_type);
  mrb_value data1;
  mrb_get_args(mrb, "o", &data1);
  data->event.user.data1 = mrb_sdl2_input_to_voidp(mrb, data1);
  return self;
}

static mrb_value
mrb_sdl2_input_userevent_get_data2(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_input_event_data_t *data =
    (mrb_sdl2_input_event_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_input_event_data_type);
  return mrb_sdl2_input_to_value(mrb, data->event.user.data2);
}

static mrb_value
mrb_sdl2_input_userevent_set_data2(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_input_event_data_t *data =
    (mrb_sdl2_input_event_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_input_event_data_type);
  mrb_value data2;
  mrb_get_args(mrb, "o", &data2);
  data->event.user.data2 = mrb_sdl2_input_to_voidp(mrb, data2);
  return self;
}


/***************************************************************************
*
* class SDL2::Input::WindowEvent
*
***************************************************************************/

static mrb_value
mrb_sdl2_input_windowevent_get_timestamp(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_input_event_data_t *data =
    (mrb_sdl2_input_event_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_input_event_data_type);
  return mrb_fixnum_value(data->event.window.timestamp);
}

static mrb_value
mrb_sdl2_input_windowevent_get_window_id(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_input_event_data_t *data =
    (mrb_sdl2_input_event_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_input_event_data_type);
  return mrb_fixnum_value(data->event.window.windowID);
}

static mrb_value
mrb_sdl2_input_windowevent_get_event(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_input_event_data_t *data =
    (mrb_sdl2_input_event_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_input_event_data_type);
  return mrb_fixnum_value(data->event.window.event);
}

static mrb_value
mrb_sdl2_input_windowevent_get_data1(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_input_event_data_t *data =
    (mrb_sdl2_input_event_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_input_event_data_type);
  return mrb_fixnum_value(data->event.window.data1);
}

static mrb_value
mrb_sdl2_input_windowevent_get_data2(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_input_event_data_t *data =
    (mrb_sdl2_input_event_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_input_event_data_type);
  return mrb_fixnum_value(data->event.window.data2);
}


void
mruby_sdl2_events_init(mrb_state *mrb)
{
  mod_Input = mrb_define_module_under(mrb, mod_SDL2, "Input");

  class_Event       = mrb_define_class_under(mrb, mod_Input, "Event",       mrb->object_class);

  class_ControllerAxisEvent   = mrb_define_class_under(mrb, mod_Input, "ControllerAxisEvent",   class_Event);
  class_ControllerButtonEvent = mrb_define_class_under(mrb, mod_Input, "ControllerButtonEvent", class_Event);
  class_ControllerDeviceEvent = mrb_define_class_under(mrb, mod_Input, "ControllerDeviceEvent", class_Event);
  class_DollarGestureEvent    = mrb_define_class_under(mrb, mod_Input, "DollarGestureEvent",    class_Event);
  class_DropEvent             = mrb_define_class_under(mrb, mod_Input, "DropEvent",             class_Event);
  class_JoyAxisEvent          = mrb_define_class_under(mrb, mod_Input, "JoyAxisEvent",          class_Event);
  class_JoyBallEvent          = mrb_define_class_under(mrb, mod_Input, "JoyBallEvent",          class_Event);
  class_JoyButtonEvent        = mrb_define_class_under(mrb, mod_Input, "JoyButtonEvent",        class_Event);
  class_JoyHatEvent           = mrb_define_class_under(mrb, mod_Input, "JoyHatEvent",           class_Event);
  class_JoyDeviceEvent        = mrb_define_class_under(mrb, mod_Input, "JoyDeviceEvent",        class_Event);
  class_KeyboardEvent         = mrb_define_class_under(mrb, mod_Input, "KeyboardEvent",         class_Event);
  class_MouseButtonEvent      = mrb_define_class_under(mrb, mod_Input, "MouseButtonEvent",      class_Event);
  class_MouseMotionEvent      = mrb_define_class_under(mrb, mod_Input, "MouseMotionEvent",      class_Event);
  class_MouseWheelEvent       = mrb_define_class_under(mrb, mod_Input, "MouseWheelEvent",       class_Event);
  class_MultiGestureEvent     = mrb_define_class_under(mrb, mod_Input, "MultiGestureEvent",     class_Event);
  class_OsEvent               = mrb_define_class_under(mrb, mod_Input, "OsEvent",               class_Event);
  class_QuitEvent             = mrb_define_class_under(mrb, mod_Input, "QuitEvent",             class_Event);
  class_SysWMEvent            = mrb_define_class_under(mrb, mod_Input, "SysWMEvent",            class_Event);
  class_TextEditingEvent      = mrb_define_class_under(mrb, mod_Input, "TextEditingEvent",      class_Event);
  class_TextInputEvent        = mrb_define_class_under(mrb, mod_Input, "TextInputEvent",        class_Event);
  class_TouchFingerEvent      = mrb_define_class_under(mrb, mod_Input, "TouchFingerEvent",      class_Event);
  class_UserEvent             = mrb_define_class_under(mrb, mod_Input, "UserEvent",             class_Event);
  class_WindowEvent           = mrb_define_class_under(mrb, mod_Input, "WindowEvent",           class_Event);

  MRB_SET_INSTANCE_TT(class_Event,                 MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_ControllerAxisEvent,   MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_ControllerButtonEvent, MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_ControllerDeviceEvent, MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_DollarGestureEvent,    MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_DropEvent,             MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_JoyAxisEvent,          MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_JoyBallEvent,          MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_JoyButtonEvent,        MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_JoyHatEvent,           MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_JoyDeviceEvent,        MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_KeyboardEvent,         MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_MouseButtonEvent,      MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_MouseMotionEvent,      MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_MouseWheelEvent,       MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_MultiGestureEvent,     MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_OsEvent,               MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_QuitEvent,             MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_SysWMEvent,            MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_TextEditingEvent,      MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_TextInputEvent,        MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_TouchFingerEvent,      MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_UserEvent,             MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_WindowEvent,           MRB_TT_DATA);

  mrb_define_module_function(mrb, mod_Input, "poll",            mrb_sdl2_input_poll,              MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mod_Input, "wait",            mrb_sdl2_input_wait,              MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mod_Input, "wait_timeout",    mrb_sdl2_input_wait_timeout,      MRB_ARGS_REQ(1));
  mrb_define_module_function(mrb, mod_Input, "event_state",     mrb_sdl2_input_event_state,       MRB_ARGS_REQ(2));
  mrb_define_module_function(mrb, mod_Input, "flush",           mrb_sdl2_input_flush_event,       MRB_ARGS_REQ(1) | MRB_ARGS_OPT(1));
  mrb_define_module_function(mrb, mod_Input, "has_events?",     mrb_sdl2_input_has_events,        MRB_ARGS_REQ(1) | MRB_ARGS_OPT(1));
  mrb_define_module_function(mrb, mod_Input, "quit_requested?", mrb_sdl2_input_is_quit_requested, MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mod_Input, "register",        mrb_sdl2_input_register,          MRB_ARGS_REQ(1));
  mrb_define_module_function(mrb, mod_Input, "push",            mrb_sdl2_input_push,              MRB_ARGS_REQ(1));

  mrb_define_method(mrb, class_Event, "type", mrb_sdl2_input_event_get_type, MRB_ARGS_NONE());

  mrb_define_method(mrb, class_KeyboardEvent, "timestamp", mrb_sdl2_input_keyboardevent_get_timestamp, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_KeyboardEvent, "window_id", mrb_sdl2_input_keyboardevent_get_window_id, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_KeyboardEvent, "state",     mrb_sdl2_input_keyboardevent_get_state,     MRB_ARGS_NONE());
  mrb_define_method(mrb, class_KeyboardEvent, "repeat",    mrb_sdl2_input_keyboardevent_get_repeat,    MRB_ARGS_NONE());
  mrb_define_method(mrb, class_KeyboardEvent, "keysym",    mrb_sdl2_input_keyboardevent_get_keysym,    MRB_ARGS_NONE());

  mrb_define_method(mrb, class_MouseButtonEvent, "timestamp", mrb_sdl2_input_mousebuttonevent_get_timestamp, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_MouseButtonEvent, "window_id", mrb_sdl2_input_mousebuttonevent_get_window_id, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_MouseButtonEvent, "which",     mrb_sdl2_input_mousebuttonevent_get_which,     MRB_ARGS_NONE());
  mrb_define_method(mrb, class_MouseButtonEvent, "button",    mrb_sdl2_input_mousebuttonevent_get_button,    MRB_ARGS_NONE());
  mrb_define_method(mrb, class_MouseButtonEvent, "state",     mrb_sdl2_input_mousebuttonevent_get_state,     MRB_ARGS_NONE());
  mrb_define_method(mrb, class_MouseButtonEvent, "x",         mrb_sdl2_input_mousebuttonevent_get_x,         MRB_ARGS_NONE());
  mrb_define_method(mrb, class_MouseButtonEvent, "y",         mrb_sdl2_input_mousebuttonevent_get_y,         MRB_ARGS_NONE());

  mrb_define_method(mrb, class_MouseMotionEvent, "timestamp", mrb_sdl2_input_mousemotionevent_get_timestamp, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_MouseMotionEvent, "window_id", mrb_sdl2_input_mousemotionevent_get_window_id, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_MouseMotionEvent, "which",     mrb_sdl2_input_mousemotionevent_get_which,     MRB_ARGS_NONE());
  mrb_define_method(mrb, class_MouseMotionEvent, "state",     mrb_sdl2_input_mousemotionevent_get_state,     MRB_ARGS_NONE());
  mrb_define_method(mrb, class_MouseMotionEvent, "x",         mrb_sdl2_input_mousemotionevent_get_x,         MRB_ARGS_NONE());
  mrb_define_method(mrb, class_MouseMotionEvent, "y",         mrb_sdl2_input_mousemotionevent_get_y,         MRB_ARGS_NONE());
  mrb_define_method(mrb, class_MouseMotionEvent, "xrel",      mrb_sdl2_input_mousemotionevent_get_xrel,      MRB_ARGS_NONE());
  mrb_define_method(mrb, class_MouseMotionEvent, "yrel",      mrb_sdl2_input_mousemotionevent_get_yrel,      MRB_ARGS_NONE());

  mrb_define_method(mrb, class_MouseWheelEvent, "timestamp", mrb_sdl2_input_mousewheelevent_get_timestamp, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_MouseWheelEvent, "window_id", mrb_sdl2_input_mousewheelevent_get_window_id, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_MouseWheelEvent, "which",     mrb_sdl2_input_mousewheelevent_get_which,     MRB_ARGS_NONE());
  mrb_define_method(mrb, class_MouseWheelEvent, "x",         mrb_sdl2_input_mousewheelevent_get_x,         MRB_ARGS_NONE());
  mrb_define_method(mrb, class_MouseWheelEvent, "y",         mrb_sdl2_input_mousewheelevent_get_y,         MRB_ARGS_NONE());

  mrb_define_method(mrb, class_QuitEvent, "timestamp", mrb_sdl2_input_quitevent_timestamp, MRB_ARGS_NONE());

  mrb_define_method(mrb, class_UserEvent, "initialize",  mrb_sdl2_input_userevent_initialize,    MRB_ARGS_REQ(2) | MRB_ARGS_OPT(2));
  mrb_define_method(mrb, class_UserEvent, "type=",       mrb_sdl2_input_userevent_set_type,      MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_UserEvent, "timestamp",   mrb_sdl2_input_userevent_get_timestamp, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_UserEvent, "timestamp=",  mrb_sdl2_input_userevent_set_timestamp, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_UserEvent, "window_id",   mrb_sdl2_input_userevent_get_window_id, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_UserEvent, "window_id=",  mrb_sdl2_input_userevent_set_window_id, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_UserEvent, "code",        mrb_sdl2_input_userevent_get_code,      MRB_ARGS_NONE());
  mrb_define_method(mrb, class_UserEvent, "code=",       mrb_sdl2_input_userevent_set_code,      MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_UserEvent, "data1",       mrb_sdl2_input_userevent_get_data1,     MRB_ARGS_NONE());
  mrb_define_method(mrb, class_UserEvent, "data1=",      mrb_sdl2_input_userevent_set_data1,     MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_UserEvent, "data2",       mrb_sdl2_input_userevent_get_data2,     MRB_ARGS_NONE());
  mrb_define_method(mrb, class_UserEvent, "data2=",      mrb_sdl2_input_userevent_set_data2,     MRB_ARGS_REQ(1));

  mrb_define_method(mrb, class_WindowEvent, "timestamp", mrb_sdl2_input_windowevent_get_timestamp, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_WindowEvent, "window_id", mrb_sdl2_input_windowevent_get_window_id, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_WindowEvent, "event",     mrb_sdl2_input_windowevent_get_event,     MRB_ARGS_NONE());
  mrb_define_method(mrb, class_WindowEvent, "data1",     mrb_sdl2_input_windowevent_get_data1,     MRB_ARGS_NONE());
  mrb_define_method(mrb, class_WindowEvent, "data2",     mrb_sdl2_input_windowevent_get_data2,     MRB_ARGS_NONE());

  int arena_size = mrb_gc_arena_save(mrb);

  /* SDL_EventType */
  mrb_define_const(mrb, mod_Input, "SDL_FIRSTEVENT",               mrb_fixnum_value(SDL_FIRSTEVENT));
  mrb_define_const(mrb, mod_Input, "SDL_QUIT",                     mrb_fixnum_value(SDL_QUIT));
  mrb_define_const(mrb, mod_Input, "SDL_APP_TERMINATING",          mrb_fixnum_value(SDL_APP_TERMINATING));
  mrb_define_const(mrb, mod_Input, "SDL_APP_LOWMEMORY",            mrb_fixnum_value(SDL_APP_LOWMEMORY));
  mrb_define_const(mrb, mod_Input, "SDL_APP_WILLENTERBACKGROUND",  mrb_fixnum_value(SDL_APP_WILLENTERBACKGROUND));
  mrb_define_const(mrb, mod_Input, "SDL_APP_DIDENTERBACKGROUND",   mrb_fixnum_value(SDL_APP_DIDENTERBACKGROUND));
  mrb_define_const(mrb, mod_Input, "SDL_APP_WILLENTERFOREGROUND",  mrb_fixnum_value(SDL_APP_WILLENTERFOREGROUND));
  mrb_define_const(mrb, mod_Input, "SDL_APP_DIDENTERFOREGROUND",   mrb_fixnum_value(SDL_APP_DIDENTERFOREGROUND));
  mrb_define_const(mrb, mod_Input, "SDL_WINDOWEVENT",              mrb_fixnum_value(SDL_WINDOWEVENT));
  mrb_define_const(mrb, mod_Input, "SDL_SYSWMEVENT",               mrb_fixnum_value(SDL_SYSWMEVENT));
  mrb_define_const(mrb, mod_Input, "SDL_KEYDOWN",                  mrb_fixnum_value(SDL_KEYDOWN));
  mrb_define_const(mrb, mod_Input, "SDL_KEYUP",                    mrb_fixnum_value(SDL_KEYUP));
  mrb_define_const(mrb, mod_Input, "SDL_TEXTEDITING",              mrb_fixnum_value(SDL_TEXTEDITING));
  mrb_define_const(mrb, mod_Input, "SDL_TEXTINPUT",                mrb_fixnum_value(SDL_TEXTINPUT));
  mrb_define_const(mrb, mod_Input, "SDL_MOUSEMOTION",              mrb_fixnum_value(SDL_MOUSEMOTION));
  mrb_define_const(mrb, mod_Input, "SDL_MOUSEBUTTONDOWN",          mrb_fixnum_value(SDL_MOUSEBUTTONDOWN));
  mrb_define_const(mrb, mod_Input, "SDL_MOUSEBUTTONUP",            mrb_fixnum_value(SDL_MOUSEBUTTONUP));
  mrb_define_const(mrb, mod_Input, "SDL_BUTTON_LEFT",              mrb_fixnum_value(SDL_BUTTON_LEFT));
  mrb_define_const(mrb, mod_Input, "SDL_BUTTON_MIDDLE",            mrb_fixnum_value(SDL_BUTTON_MIDDLE));
  mrb_define_const(mrb, mod_Input, "SDL_BUTTON_RIGHT",             mrb_fixnum_value(SDL_BUTTON_RIGHT));
  mrb_define_const(mrb, mod_Input, "SDL_BUTTON_X1",                mrb_fixnum_value(SDL_BUTTON_X1));
  mrb_define_const(mrb, mod_Input, "SDL_BUTTON_X2",                mrb_fixnum_value(SDL_BUTTON_X2));
  mrb_define_const(mrb, mod_Input, "SDL_MOUSEWHEEL",               mrb_fixnum_value(SDL_MOUSEWHEEL));
  mrb_define_const(mrb, mod_Input, "SDL_JOYAXISMOTION",            mrb_fixnum_value(SDL_JOYAXISMOTION));
  mrb_define_const(mrb, mod_Input, "SDL_JOYBALLMOTION",            mrb_fixnum_value(SDL_JOYBALLMOTION));
  mrb_define_const(mrb, mod_Input, "SDL_JOYHATMOTION",             mrb_fixnum_value(SDL_JOYHATMOTION));
  mrb_define_const(mrb, mod_Input, "SDL_JOYBUTTONDOWN",            mrb_fixnum_value(SDL_JOYBUTTONDOWN));
  mrb_define_const(mrb, mod_Input, "SDL_JOYBUTTONUP",              mrb_fixnum_value(SDL_JOYBUTTONUP));
  mrb_define_const(mrb, mod_Input, "SDL_JOYDEVICEADDED",           mrb_fixnum_value(SDL_JOYDEVICEADDED));
  mrb_define_const(mrb, mod_Input, "SDL_JOYDEVICEREMOVED",         mrb_fixnum_value(SDL_JOYDEVICEREMOVED));
  mrb_define_const(mrb, mod_Input, "SDL_CONTROLLERAXISMOTION",     mrb_fixnum_value(SDL_CONTROLLERAXISMOTION));
  mrb_define_const(mrb, mod_Input, "SDL_CONTROLLERBUTTONDOWN",     mrb_fixnum_value(SDL_CONTROLLERBUTTONDOWN));
  mrb_define_const(mrb, mod_Input, "SDL_CONTROLLERBUTTONUP",       mrb_fixnum_value(SDL_CONTROLLERBUTTONUP));
  mrb_define_const(mrb, mod_Input, "SDL_CONTROLLERDEVICEADDED",    mrb_fixnum_value(SDL_CONTROLLERDEVICEADDED));
  mrb_define_const(mrb, mod_Input, "SDL_CONTROLLERDEVICEREMOVED",  mrb_fixnum_value(SDL_CONTROLLERDEVICEREMOVED));
  mrb_define_const(mrb, mod_Input, "SDL_CONTROLLERDEVICEREMAPPED", mrb_fixnum_value(SDL_CONTROLLERDEVICEREMAPPED));
  mrb_define_const(mrb, mod_Input, "SDL_FINGERDOWN",               mrb_fixnum_value(SDL_FINGERDOWN));
  mrb_define_const(mrb, mod_Input, "SDL_FINGERUP",                 mrb_fixnum_value(SDL_FINGERUP));
  mrb_define_const(mrb, mod_Input, "SDL_FINGERMOTION",             mrb_fixnum_value(SDL_FINGERMOTION));
  mrb_define_const(mrb, mod_Input, "SDL_DOLLARGESTURE",            mrb_fixnum_value(SDL_DOLLARGESTURE));
  mrb_define_const(mrb, mod_Input, "SDL_DOLLARRECORD",             mrb_fixnum_value(SDL_DOLLARRECORD));
  mrb_define_const(mrb, mod_Input, "SDL_MULTIGESTURE",             mrb_fixnum_value(SDL_MULTIGESTURE));
  mrb_define_const(mrb, mod_Input, "SDL_CLIPBOARDUPDATE",          mrb_fixnum_value(SDL_CLIPBOARDUPDATE));
  mrb_define_const(mrb, mod_Input, "SDL_DROPFILE",                 mrb_fixnum_value(SDL_DROPFILE));
  mrb_define_const(mrb, mod_Input, "SDL_USEREVENT",                mrb_fixnum_value(SDL_USEREVENT));
  mrb_define_const(mrb, mod_Input, "SDL_LASTEVENT",                mrb_fixnum_value(SDL_LASTEVENT));

  mrb_gc_arena_restore(mrb, arena_size);
  arena_size = mrb_gc_arena_save(mrb);

  /* SDL_WindowEventID */
  mrb_define_const(mrb, mod_Input, "SDL_WINDOWEVENT_NONE",         mrb_fixnum_value(SDL_WINDOWEVENT_NONE));
  mrb_define_const(mrb, mod_Input, "SDL_WINDOWEVENT_SHOWN",        mrb_fixnum_value(SDL_WINDOWEVENT_SHOWN));
  mrb_define_const(mrb, mod_Input, "SDL_WINDOWEVENT_HIDDEN",       mrb_fixnum_value(SDL_WINDOWEVENT_HIDDEN));
  mrb_define_const(mrb, mod_Input, "SDL_WINDOWEVENT_EXPOSED",      mrb_fixnum_value(SDL_WINDOWEVENT_EXPOSED));
  mrb_define_const(mrb, mod_Input, "SDL_WINDOWEVENT_MOVED",        mrb_fixnum_value(SDL_WINDOWEVENT_MOVED));
  mrb_define_const(mrb, mod_Input, "SDL_WINDOWEVENT_RESIZED",      mrb_fixnum_value(SDL_WINDOWEVENT_RESIZED));
  mrb_define_const(mrb, mod_Input, "SDL_WINDOWEVENT_SIZE_CHANGED", mrb_fixnum_value(SDL_WINDOWEVENT_SIZE_CHANGED));
  mrb_define_const(mrb, mod_Input, "SDL_WINDOWEVENT_MINIMIZED",    mrb_fixnum_value(SDL_WINDOWEVENT_MINIMIZED));
  mrb_define_const(mrb, mod_Input, "SDL_WINDOWEVENT_MAXIMIZED",    mrb_fixnum_value(SDL_WINDOWEVENT_MAXIMIZED));
  mrb_define_const(mrb, mod_Input, "SDL_WINDOWEVENT_RESTORED",     mrb_fixnum_value(SDL_WINDOWEVENT_RESTORED));
  mrb_define_const(mrb, mod_Input, "SDL_WINDOWEVENT_ENTER",        mrb_fixnum_value(SDL_WINDOWEVENT_ENTER));
  mrb_define_const(mrb, mod_Input, "SDL_WINDOWEVENT_LEAVE",        mrb_fixnum_value(SDL_WINDOWEVENT_LEAVE));
  mrb_define_const(mrb, mod_Input, "SDL_WINDOWEVENT_FOCUS_GAINED", mrb_fixnum_value(SDL_WINDOWEVENT_FOCUS_GAINED));
  mrb_define_const(mrb, mod_Input, "SDL_WINDOWEVENT_FOCUS_LOST",   mrb_fixnum_value(SDL_WINDOWEVENT_FOCUS_LOST));
  mrb_define_const(mrb, mod_Input, "SDL_WINDOWEVENT_CLOSE",        mrb_fixnum_value(SDL_WINDOWEVENT_CLOSE));

  /* Event state */
  mrb_define_const(mrb, mod_Input, "SDL_QUERY",   mrb_fixnum_value(SDL_QUERY));
  mrb_define_const(mrb, mod_Input, "SDL_IGNORE",  mrb_fixnum_value(SDL_IGNORE));
  mrb_define_const(mrb, mod_Input, "SDL_DISABLE", mrb_fixnum_value(SDL_DISABLE));
  mrb_define_const(mrb, mod_Input, "SDL_ENABLE",  mrb_fixnum_value(SDL_ENABLE));

  mrb_gc_arena_restore(mrb, arena_size);
}

void
mruby_sdl2_events_final(mrb_state *mrb)
{
}
