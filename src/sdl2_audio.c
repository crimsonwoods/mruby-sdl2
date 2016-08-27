#include "sdl2_audio.h"
#include "mruby/data.h"
#include "mruby/value.h"
#include "mruby/class.h"
#include "mruby/string.h"
#include "mruby/array.h"
#include "mruby/variable.h"

struct RClass *mod_Audio = NULL;
static struct RClass *class_AudioCVT    = NULL;
static struct RClass *class_AudioSpec   = NULL;
static struct RClass *class_AudioDevice = NULL;
static struct RClass *class_AudioData   = NULL;

typedef struct mrb_sdl2_audio_userdata_t {
  mrb_state *mrb;
  mrb_value  obj;
} mrb_sdl2_audio_userdata_t;

typedef struct mrb_sdl2_audio_audiocvt_data_t {
  SDL_AudioCVT cvt;
} mrb_sdl2_audio_audiocvt_data_t;

typedef struct mrb_sdl2_audio_audiospec_data_t {
  SDL_AudioSpec             spec;
  mrb_sdl2_audio_userdata_t udata;
} mrb_sdl2_audio_audiospec_data_t;

typedef struct mrb_sdl2_audio_audiodevice_data_t {
  SDL_AudioDeviceID id;
  SDL_AudioSpec     spec;
} mrb_sdl2_audio_audiodevice_data_t;

typedef struct mrb_sdl2_audio_audiodata_data_t {
  Uint8*                          buf;
  Uint32                          len;
} mrb_sdl2_audio_audiodata_data_t;

static void
mrb_sdl2_audio_audiocvt_data_free(mrb_state *mrb, void *p)
{
  mrb_sdl2_audio_audiocvt_data_t *data =
    (mrb_sdl2_audio_audiocvt_data_t*)p;
  if (NULL != data) {
    if (NULL != data->cvt.buf) {
      mrb_free(mrb, data->cvt.buf);
    }
    mrb_free(mrb, p);
  }
}

static void
mrb_sdl2_audio_audiospec_data_free(mrb_state *mrb, void *p)
{
  mrb_sdl2_audio_audiospec_data_t *data =
    (mrb_sdl2_audio_audiospec_data_t*)p;
  if (NULL != data) {
    mrb_free(mrb, data);
  }
}

static void
mrb_sdl2_audio_audiodevice_data_free(mrb_state *mrb, void *p)
{
  mrb_sdl2_audio_audiodevice_data_t *data =
    (mrb_sdl2_audio_audiodevice_data_t*)p;
  if (NULL != data) {
    if (0 < data->id) {
      SDL_CloseAudioDevice(data->id);
    }
    mrb_free(mrb, p);
  }
}

static void
mrb_sdl2_audio_audiodata_data_free(mrb_state *mrb, void *p)
{
  mrb_sdl2_audio_audiodata_data_t *data =
    (mrb_sdl2_audio_audiodata_data_t*)p;
  if (NULL != data) {
    if (NULL != data->buf) {
      SDL_FreeWAV(data->buf);
    }
    mrb_free(mrb, data);
  }
}

struct mrb_data_type const mrb_sdl2_audio_audiocvt_data_type = {
  "AudioCVT", mrb_sdl2_audio_audiocvt_data_free
};

struct mrb_data_type const mrb_sdl2_audio_audiospec_data_type = {
  "AudioSpec", mrb_sdl2_audio_audiospec_data_free
};

struct mrb_data_type const mrb_sdl2_audio_audiodevice_data_type = {
  "AudioDevice", mrb_sdl2_audio_audiodevice_data_free
};

struct mrb_data_type const mrb_sdl2_audio_audiodata_data_type = {
  "AudioData", mrb_sdl2_audio_audiodata_data_free
};

SDL_AudioSpec *
mrb_sdl2_audiospec_get_ptr(mrb_state *mrb, mrb_value value)
{
  if (mrb_nil_p(value)) {
    return NULL;
  }
  return &((mrb_sdl2_audio_audiospec_data_t*)mrb_data_get_ptr(mrb, value, &mrb_sdl2_audio_audiospec_data_type))->spec;
}

SDL_AudioCVT *
mrb_sdl2_audiocvt_get_ptr(mrb_state *mrb, mrb_value value)
{
  if (mrb_nil_p(value)) {
    return NULL;
  }
  return &((mrb_sdl2_audio_audiocvt_data_t*)mrb_data_get_ptr(mrb, value, &mrb_sdl2_audio_audiocvt_data_type))->cvt;
}

SDL_AudioDeviceID *
mrb_sdl2_audiodevice_get_ptr(mrb_state *mrb, mrb_value value)
{
  if (mrb_nil_p(value)) {
    return NULL;
  }
  return &((mrb_sdl2_audio_audiodevice_data_t*)mrb_data_get_ptr(mrb, value, &mrb_sdl2_audio_audiocvt_data_type))->id;
}

mrb_value
mrb_sdl2_audiospec(mrb_state *mrb, SDL_AudioSpec const *value)
{
  mrb_sdl2_audio_audiospec_data_t *data =
    (mrb_sdl2_audio_audiospec_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_audio_audiospec_data_t));
  if (NULL == data) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
  }
  if (NULL == value) {
    data->spec = (SDL_AudioSpec){ 0, };
  } else {
    data->spec = *value;
  }
  return mrb_obj_value(Data_Wrap_Struct(mrb, class_AudioSpec, &mrb_sdl2_audio_audiospec_data_type, data));
}

mrb_value
mrb_sdl2_audiocvt(mrb_state *mrb, SDL_AudioCVT const *value)
{
  mrb_sdl2_audio_audiocvt_data_t *data =
    (mrb_sdl2_audio_audiocvt_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_audio_audiocvt_data_t));
  if (NULL == data) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
  }
  if (NULL == value) {
    data->cvt = (SDL_AudioCVT){ 0, };
  } else {
    data->cvt = *value;
  }
  return mrb_obj_value(Data_Wrap_Struct(mrb, class_AudioCVT, &mrb_sdl2_audio_audiocvt_data_type, data));
}

mrb_value
mrb_sdl2_audiodevice(mrb_state *mrb, SDL_AudioDeviceID id)
{
  mrb_sdl2_audio_audiodevice_data_t *data =
    (mrb_sdl2_audio_audiodevice_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_audio_audiodevice_data_t));
  if (NULL == data) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
  }
  data->id = id;
  return mrb_obj_value(Data_Wrap_Struct(mrb, class_AudioDevice, &mrb_sdl2_audio_audiodevice_data_type, data));
}

/***************************************************************************
*
* module SDL2::Audio
*
***************************************************************************/

static mrb_value
mrb_sdl2_audio_init(mrb_state *mrb, mrb_value mod)
{
  mrb_value name;
  mrb_get_args(mrb, "S", &name);
  if (0 != SDL_AudioInit(RSTRING_PTR(name))) {
    mruby_sdl2_raise_error(mrb);
  }
  return mod;
}

static mrb_value
mrb_sdl2_audio_quit(mrb_state *mrb, mrb_value mod)
{
  SDL_AudioQuit();
  return mod;
}

static mrb_value
mrb_sdl2_audio_open(mrb_state *mrb, mrb_value mod)
{
  mrb_value arg;
  mrb_get_args(mrb, "o", &arg);
  int ret;
  SDL_AudioSpec *desired = (SDL_AudioSpec*)mrb_sdl2_audiospec_get_ptr(mrb, arg);
  SDL_AudioSpec obtained;
  if (NULL == desired) {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "1st argument cannot be set to null.");
  }
  ret = SDL_OpenAudio(
          desired,
          &obtained);
  if (0 != ret) {
    mruby_sdl2_raise_error(mrb);
  }
  return mrb_sdl2_audiospec(mrb, &obtained);
}

static mrb_value
mrb_sdl2_audio_close(mrb_state *mrb, mrb_value mod)
{
  SDL_CloseAudio();
  return mod;
}

static mrb_value
mrb_sdl2_audio_open_device(mrb_state *mrb, mrb_value mod)
{
  mrb_value device, desired, obtained;
  mrb_bool iscapture;
  mrb_int allowed_changes;
  int const argc = mrb_get_args(mrb, "Sbo|oi", &device, &iscapture, &desired, &obtained, &allowed_changes);

  mrb_sdl2_audio_audiodevice_data_t *data =
    (mrb_sdl2_audio_audiodevice_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_audio_audiodevice_data_t));
  if (NULL == data) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
  }

  SDL_AudioDeviceID id = 0;
  switch (argc) {
  case 3:
    id = SDL_OpenAudioDevice(
          RSTRING_PTR(device),
          iscapture ? 1 : 0,
          mrb_sdl2_audiospec_get_ptr(mrb, desired),
          NULL,
          0);
    break;
  case 4:
    id = SDL_OpenAudioDevice(
          RSTRING_PTR(device),
          iscapture ? 1 : 0,
          mrb_sdl2_audiospec_get_ptr(mrb, desired),
          mrb_sdl2_audiospec_get_ptr(mrb, obtained),
          0);
    break;
  case 5:
    id = SDL_OpenAudioDevice(
          RSTRING_PTR(device),
          iscapture ? 1 : 0,
          mrb_sdl2_audiospec_get_ptr(mrb, desired),
          mrb_sdl2_audiospec_get_ptr(mrb, obtained),
          allowed_changes);
    break;
  default:
    mrb_free(mrb, data);
    mrb_raise(mrb, E_ARGUMENT_ERROR, "wrong number of arguments.");
    break;
  }
  if (0 == id) {
    mrb_free(mrb, data);
    mruby_sdl2_raise_error(mrb);
  }
  return mrb_obj_value(Data_Wrap_Struct(mrb, class_AudioDevice, &mrb_sdl2_audio_audiodevice_data_type, data));
}

static mrb_value
mrb_sdl2_audio_pause(mrb_state *mrb, mrb_value mod)
{
  mrb_bool pause;
  mrb_get_args(mrb, "b", &pause);
  SDL_PauseAudio(pause ? 1 : 0);
  return pause ? mrb_true_value() : mrb_false_value();
}

static mrb_value
mrb_sdl2_audio_get_drivers(mrb_state *mrb, mrb_value mod)
{
  int const n = SDL_GetNumAudioDrivers();
  int i;
  mrb_value array = mrb_ary_new_capa(mrb, n);
  for (i = 0; i < n; ++i) {
    char const * const name = SDL_GetAudioDriver(i);
    if (NULL == name) {
      continue;
    }
    mrb_ary_push(mrb, array, mrb_str_new_cstr(mrb, name));
  }
  return array;
}

static mrb_value
mrb_sdl2_audio_get_devices(mrb_state *mrb, mrb_value mod)
{
  mrb_bool iscapture = false;
  mrb_get_args(mrb, "|b", &iscapture);
  int const n = SDL_GetNumAudioDevices(iscapture ? 1 : 0);
  int i;
  mrb_value array = mrb_ary_new_capa(mrb, n);
  for (i = 0; i < n; ++i) {
    char const * const name = SDL_GetAudioDeviceName(i, iscapture ? 1 : 0);
    if (NULL == name) {
      continue;
    }
    mrb_ary_push(mrb, array, mrb_str_new_cstr(mrb, name));
  }
  return array;
}

static mrb_value
mrb_sdl2_audio_get_current_driver(mrb_state *mrb, mrb_value mod)
{
  char const * const name = SDL_GetCurrentAudioDriver();
  if (NULL == name) {
    return mrb_nil_value();
  }
  return mrb_str_new_cstr(mrb, name);
}

static mrb_value
mrb_sdl2_audio_lock(mrb_state *mrb, mrb_value mod)
{
  SDL_LockAudio();
  return mod;
}

static mrb_value
mrb_sdl2_audio_unlock(mrb_state *mrb, mrb_value mod)
{
  SDL_UnlockAudio();
  return mod;
}

static mrb_value
mrb_sdl2_audio_mix_audio(mrb_state *mrb, mrb_value mod)
{
  mrb_value dst, src;
  mrb_int dpos, spos, len, volume;
  mrb_get_args(mrb, "oioiii", &dst, &dpos, &src, &spos, &len, &volume);
  Uint8 *dst_ptr = NULL;
  Uint8 const *src_ptr = NULL;
  if (mrb_type(dst) == MRB_TT_CPTR) {
    dst_ptr = (Uint8*)mrb_cptr(dst);
  }
  if (mrb_type(src) == MRB_TT_CPTR) {
    src_ptr = (Uint8 const *)mrb_cptr(src);
  } else if (mrb_type(src) == MRB_TT_DATA) {
    if (DATA_TYPE(src) == &mrb_sdl2_audio_audiodata_data_type) {
      mrb_sdl2_audio_audiodata_data_t const *data =
        (mrb_sdl2_audio_audiodata_data_t*)mrb_data_get_ptr(mrb, src, &mrb_sdl2_audio_audiodata_data_type);
      if (NULL != data->buf) {
        src_ptr = (Uint8 const *)data->buf;
        if (spos > data->len) {
          len = 0;
        } else if (len > (data->len - spos)) {
          len = data->len - spos;
        }
      } else {
        mrb_raise(mrb, E_ARGUMENT_ERROR, "given argument has no audio buffer.");
      }
    }
    if (DATA_TYPE(src) == &mrb_sdl2_audio_audiocvt_data_type) {
      mrb_sdl2_audio_audiocvt_data_t const *data =
        (mrb_sdl2_audio_audiocvt_data_t*)mrb_data_get_ptr(mrb, src, &mrb_sdl2_audio_audiocvt_data_type);
      if (NULL != data->cvt.buf) {
        src_ptr = (Uint8 const *)data->cvt.buf;
        size_t const sz = data->cvt.len * data->cvt.len_mult;
        if (spos > sz) {
          len = 0;
        } else if (len > (sz - spos)) {
          len = sz - spos;
        }
      } else {
        mrb_raise(mrb, E_ARGUMENT_ERROR, "given argument has no audio buffer.");
      }
    }
  }
  SDL_MixAudio(&dst_ptr[dpos], &src_ptr[spos], (Uint32)len, (int)volume);
  return mod;
}

static mrb_value
mrb_sdl2_auiod_get_status(mrb_state *mrb, mrb_value mod)
{
  return mrb_fixnum_value(SDL_GetAudioStatus());
}

/***************************************************************************
*
* class SDL2::Audio::AudioSpec
*
***************************************************************************/

static void
mrb_sdl2_audio_audiospec_callback(void *userdata, Uint8 *stream, int len)
{
  mrb_sdl2_audio_userdata_t *data = (mrb_sdl2_audio_userdata_t*)userdata;
  mrb_state *mrb = data->mrb;
  mrb_value obj = data->obj;

  SDL_AudioSpec const * const spec = mrb_sdl2_audiospec_get_ptr(mrb, obj);
  mrb_value udata = mrb_iv_get(mrb, obj, mrb_intern(mrb, "userdata", 8));
  mrb_value block = mrb_iv_get(mrb, obj, mrb_intern(mrb, "callback", 8));

  SDL_memset(stream, spec->silence, len);

  if (mrb_nil_p(block)) {
    return;
  }

  // TODO migrate thread context if necessary.

  mrb_value args[3] = {
    udata,
    mrb_cptr_value(mrb, stream),
    mrb_fixnum_value(len)
  };
  mrb_yield_argv(mrb, block, 3, args);
}

static mrb_value
mrb_sdl2_audio_audiospec_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_int freq, format, channels, samples;
  int const argc = mrb_get_args(mrb, "|iiii", &freq, &format, &channels, &samples);

  mrb_sdl2_audio_audiospec_data_t *data =
    (mrb_sdl2_audio_audiospec_data_t*)DATA_PTR(self);

  if (NULL == data) {
    data = (mrb_sdl2_audio_audiospec_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_audio_audiospec_data_t));
    if (NULL == data) {
      mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
    }
    data->spec = (SDL_AudioSpec){ 0, };
    data->spec.callback = &mrb_sdl2_audio_audiospec_callback;
    data->spec.userdata = (void*)&data->udata;
  }

  data->udata.mrb = mrb;
  data->udata.obj = self;
  if (0 < argc) {
    data->spec.freq = (int)freq;
  } else {
    data->spec.freq = 22050;
  }
  if (1 < argc) {
    data->spec.format = (SDL_AudioFormat)format;
  } else {
    data->spec.format = AUDIO_F32SYS;
  }
  if (2 < argc) {
    data->spec.channels = (Uint8)channels;
  } else {
    data->spec.channels = 2;
  }
  if (3 < argc) {
    data->spec.samples = (Uint16)samples;
  } else {
    data->spec.samples = 4096;
  }

  DATA_PTR(self) = data;
  DATA_TYPE(self) = &mrb_sdl2_audio_audiospec_data_type;
  return self;
}

static mrb_value
mrb_sdl2_audio_audiospec_get_freq(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(mrb_sdl2_audiospec_get_ptr(mrb, self)->freq);
}

static mrb_value
mrb_sdl2_audio_audiospec_set_freq(mrb_state *mrb, mrb_value self)
{
  mrb_int freq;
  mrb_get_args(mrb, "i", &freq);
  mrb_sdl2_audiospec_get_ptr(mrb, self)->freq = (int)freq;
  return self;
}

static mrb_value
mrb_sdl2_audio_audiospec_get_format(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(mrb_sdl2_audiospec_get_ptr(mrb, self)->format);
}

static mrb_value
mrb_sdl2_audio_audiospec_set_format(mrb_state *mrb, mrb_value self)
{
  mrb_int format;
  mrb_get_args(mrb, "i", &format);
  mrb_sdl2_audiospec_get_ptr(mrb, self)->format = (SDL_AudioFormat)format;
  return self;
}

static mrb_value
mrb_sdl2_audio_audiospec_get_channels(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(mrb_sdl2_audiospec_get_ptr(mrb, self)->channels);
}

static mrb_value
mrb_sdl2_audio_audiospec_set_channels(mrb_state *mrb, mrb_value self)
{
  mrb_int channels;
  mrb_get_args(mrb, "i", &channels);
  mrb_sdl2_audiospec_get_ptr(mrb, self)->channels = (Uint8)channels;
  return self;
}

static mrb_value
mrb_sdl2_audio_audiospec_get_silence(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(mrb_sdl2_audiospec_get_ptr(mrb, self)->silence);
}

static mrb_value
mrb_sdl2_audio_audiospec_get_samples(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(mrb_sdl2_audiospec_get_ptr(mrb, self)->samples);
}

static mrb_value
mrb_sdl2_audio_audiospec_set_samples(mrb_state *mrb, mrb_value self)
{
  mrb_int samples;
  mrb_get_args(mrb, "i", &samples);
  mrb_sdl2_audiospec_get_ptr(mrb, self)->samples = (Uint16)samples;
  return self;
}

static mrb_value
mrb_sdl2_audio_audiospec_get_size(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(mrb_sdl2_audiospec_get_ptr(mrb, self)->size);
}

static mrb_value
mrb_sdl2_audio_audiospec_get_callback(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, mrb_intern(mrb, "callback", 8));
}

static mrb_value
mrb_sdl2_audio_audiospec_set_callback(mrb_state *mrb, mrb_value self)
{
  mrb_value proc;
  mrb_get_args(mrb, "o", &proc);
  if (mrb_type(proc) != MRB_TT_PROC) {
    mrb_raise(mrb, E_TYPE_ERROR, "given value is not proc object.");
  }
  mrb_iv_set(mrb, self, mrb_intern(mrb, "callback", 8), proc);
  return self;
}

static mrb_value
mrb_sdl2_audio_audiospec_get_userdata(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, mrb_intern(mrb, "userdata", 8));
}

static mrb_value
mrb_sdl2_audio_audiospec_set_userdata(mrb_state *mrb, mrb_value self)
{
  mrb_value obj;
  mrb_get_args(mrb, "o", &obj);
  mrb_iv_set(mrb, self, mrb_intern(mrb, "userdata", 8), obj);
  return self;
}

/***************************************************************************
*
* class SDL2::Audio::AudioCVT
*
***************************************************************************/

static mrb_value
mrb_sdl2_audio_audiocvt_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_value src_spec, src_data;
  mrb_int format, channels, freq;
  mrb_get_args(mrb, "ooiii", &src_spec, &src_data, &format, &channels, &freq);

  mrb_sdl2_audio_audiospec_data_t *sspec =
    (mrb_sdl2_audio_audiospec_data_t*)mrb_data_get_ptr(mrb, src_spec, &mrb_sdl2_audio_audiospec_data_type);

  mrb_sdl2_audio_audiodata_data_t *sdata = 
    (mrb_sdl2_audio_audiodata_data_t*)mrb_data_get_ptr(mrb, src_data, &mrb_sdl2_audio_audiodata_data_type);

  mrb_sdl2_audio_audiocvt_data_t *data =
    (mrb_sdl2_audio_audiocvt_data_t*)DATA_PTR(self);

  if (NULL == data) {
    data = (mrb_sdl2_audio_audiocvt_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_audio_audiocvt_data_t));
    if (NULL == data) {
      mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
    }
    data->cvt = (SDL_AudioCVT){ 0, };
  }

  int const ret = SDL_BuildAudioCVT(
                    &data->cvt,
                    sspec->spec.format,
                    sspec->spec.channels,
                    sspec->spec.freq,
                    (SDL_AudioFormat)format,
                    (Uint8)channels,
                    (int)freq);
  if (ret < 0) {
    mruby_sdl2_raise_error(mrb);
  }
  if (0 != sdata->len) {
    data->cvt.len = sdata->len;
  } else {
    data->cvt.len = sspec->spec.size;
  }
  data->cvt.buf = mrb_malloc(mrb, data->cvt.len_mult * data->cvt.len);
  if (NULL == data->cvt.buf) {
    mrb_free(mrb, data);
    mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
  }
  if (NULL != sdata->buf) {
    SDL_memcpy(data->cvt.buf, sdata->buf, sdata->len);
  }

  DATA_PTR(self) = data;
  DATA_TYPE(self) = &mrb_sdl2_audio_audiocvt_data_type;

  return self;
}

static mrb_value
mrb_sdl2_audio_audiocvt_convert(mrb_state *mrb, mrb_value self)
{
  if (0 != SDL_ConvertAudio(mrb_sdl2_audiocvt_get_ptr(mrb, self))) {
    mruby_sdl2_raise_error(mrb);
  }
  return self;
}

/***************************************************************************
*
* class SDL2::Audio::AudioDevice
*
***************************************************************************/

static mrb_value
mrb_sdl2_audio_audiodevice_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_audio_audiodevice_data_t *data =
    (mrb_sdl2_audio_audiodevice_data_t*)DATA_PTR(self);
  mrb_value devname, spec;
  mrb_bool iscapture;
  mrb_int allowed_changes;
  mrb_get_args(mrb, "oboi", &devname, &iscapture, &spec, &allowed_changes);
  char const *device = NULL;
  SDL_AudioSpec *desired = mrb_sdl2_audiospec_get_ptr(mrb, spec);
  SDL_AudioSpec obtained;

  if (mrb_nil_p(devname)) {
    device = NULL;
  } else if (mrb_type(devname) == MRB_TT_STRING) {
    device = RSTRING_PTR(devname);
  } else {
    mrb_raise(mrb, E_TYPE_ERROR, "expected String");
  }

  if (NULL == data) {
    data = (mrb_sdl2_audio_audiodevice_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_audio_audiodevice_data_t));
    if (NULL == data) {
      mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
    }
    data->id = 0;
    data->spec = (SDL_AudioSpec){ 0, };
  }

  SDL_AudioDeviceID id = SDL_OpenAudioDevice(
    device, iscapture ? 1 : 0, desired, &obtained, allowed_changes);
  if (0 == id) {
    mruby_sdl2_raise_error(mrb);
  }

  data->id = id;
  data->spec = obtained;

  DATA_PTR(self) = data;
  DATA_TYPE(self) = &mrb_sdl2_audio_audiodevice_data_type;

  return self;
}

static mrb_value
mrb_sdl2_audio_audiodevice_close(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_audio_audiodevice_data_t *data =
    (mrb_sdl2_audio_audiodevice_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_audio_audiodevice_data_type);
  if (0 < data->id) {
    SDL_CloseAudioDevice(data->id);
    data->id = 0;
  }
  return self;
}

static mrb_value
mrb_sdl2_audio_audiodevice_get_spec(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_audio_audiodevice_data_t *data =
    (mrb_sdl2_audio_audiodevice_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_audio_audiodevice_data_type);
  return mrb_sdl2_audiospec(mrb, &data->spec);
}

static mrb_value
mrb_sdl2_audio_audiodevice_pause(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_audio_audiodevice_data_t *data =
    (mrb_sdl2_audio_audiodevice_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_audio_audiodevice_data_type);
  mrb_bool pause;
  mrb_get_args(mrb, "b", &pause);
  if (0 < data->id) {
    SDL_PauseAudioDevice(data->id, pause ? 1 : 0);
  }
  return self;
}

static mrb_value
mrb_sdl2_audio_audiodevice_lock(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_audio_audiodevice_data_t *data =
    (mrb_sdl2_audio_audiodevice_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_audio_audiodevice_data_type);
  if (0 < data->id) {
    SDL_LockAudioDevice(data->id);
  }
  return self;
}

static mrb_value
mrb_sdl2_audio_audiodevice_unlock(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_audio_audiodevice_data_t *data =
    (mrb_sdl2_audio_audiodevice_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_audio_audiodevice_data_type);
  if (0 < data->id) {
    SDL_UnlockAudioDevice(data->id);
  }
  return self;
}

static mrb_value
mrb_sdl2_audio_audiodevice_get_status(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_audio_audiodevice_data_t *data =
    (mrb_sdl2_audio_audiodevice_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_audio_audiodevice_data_type);
  if (0 < data->id) {
    return mrb_fixnum_value(SDL_GetAudioDeviceStatus(data->id));
  }
  return mrb_nil_value();
}

/***************************************************************************
*
* class SDL2::Audio::AudioDevice
*
***************************************************************************/

static mrb_value
mrb_sdl2_audio_audiodata_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_value file;
  mrb_sdl2_audio_audiodata_data_t *data =
    (mrb_sdl2_audio_audiodata_data_t*)DATA_PTR(self);

  mrb_sdl2_audio_audiospec_data_t *spec =
    (mrb_sdl2_audio_audiospec_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_audio_audiospec_data_t));
  if (NULL == spec) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
  }

  if (NULL == data) {
    data = (mrb_sdl2_audio_audiodata_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_audio_audiodata_data_t));
    if (NULL == data) {
      mrb_free(mrb, spec);
      mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
    }
    data->buf = NULL;
    data->len = 0;
  }

  mrb_get_args(mrb, "S", &file);
  if (NULL == SDL_LoadWAV(RSTRING_PTR(file), &spec->spec, &data->buf, &data->len)) {
    mrb_free(mrb, spec);
    mrb_free(mrb, data);
    mruby_sdl2_raise_error(mrb);
  }

  mrb_value s = mrb_obj_value(Data_Wrap_Struct(mrb, class_AudioSpec, &mrb_sdl2_audio_audiospec_data_type, spec));

  spec->spec.userdata = &spec->udata;
  spec->spec.callback = &mrb_sdl2_audio_audiospec_callback;
  spec->udata.mrb = mrb;
  spec->udata.obj = s;

  mrb_iv_set(mrb, self, mrb_intern(mrb, "spec", 4), s);

  DATA_PTR(self) = data;
  DATA_TYPE(self) = &mrb_sdl2_audio_audiodata_data_type;

  return self;
}

static mrb_value
mrb_sdl2_audio_audiodata_destroy(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_audio_audiodata_data_t *data =
    (mrb_sdl2_audio_audiodata_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_audio_audiodata_data_type);
  if (NULL != data->buf) {
    SDL_FreeWAV(data->buf);
    data->buf = NULL;
    data->len = 0;
  }
  return self;
}

static mrb_value
mrb_sdl2_audio_audiodata_get_spec(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, mrb_intern(mrb, "spec", 4));
}

static mrb_value
mrb_sdl2_audio_audiodata_get_buffer(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_audio_audiodata_data_t *data =
    (mrb_sdl2_audio_audiodata_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_audio_audiodata_data_type);
  return mrb_cptr_value(mrb, data->buf);
}

static mrb_value
mrb_sdl2_audio_audiodata_get_length(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_audio_audiodata_data_t *data =
    (mrb_sdl2_audio_audiodata_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_audio_audiodata_data_type);
  return mrb_fixnum_value(data->len);
}


void
mruby_sdl2_audio_init(mrb_state *mrb)
{
  mod_Audio = mrb_define_module_under(mrb, mod_SDL2, "Audio");
  class_AudioCVT    = mrb_define_class_under(mrb, mod_Audio, "AudioCVT",    mrb->object_class);
  class_AudioSpec   = mrb_define_class_under(mrb, mod_Audio, "AudioSpec",   mrb->object_class);
  class_AudioDevice = mrb_define_class_under(mrb, mod_Audio, "AudioDevice", mrb->object_class);
  class_AudioData   = mrb_define_class_under(mrb, mod_Audio, "AudioData",   mrb->object_class);

  MRB_SET_INSTANCE_TT(class_AudioCVT,    MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_AudioSpec,   MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_AudioDevice, MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_AudioData,   MRB_TT_DATA);

  mrb_define_module_function(mrb, mod_Audio, "init",           mrb_sdl2_audio_init,               MRB_ARGS_REQ(1));
  mrb_define_module_function(mrb, mod_Audio, "quit",           mrb_sdl2_audio_quit,               MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mod_Audio, "open",           mrb_sdl2_audio_open,               MRB_ARGS_REQ(1));
  mrb_define_module_function(mrb, mod_Audio, "close",          mrb_sdl2_audio_close,              MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mod_Audio, "open_device",    mrb_sdl2_audio_open_device,        MRB_ARGS_REQ(3) | MRB_ARGS_OPT(2));
  mrb_define_module_function(mrb, mod_Audio, "pause",          mrb_sdl2_audio_pause,              MRB_ARGS_REQ(1));
  mrb_define_module_function(mrb, mod_Audio, "drivers",        mrb_sdl2_audio_get_drivers,        MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mod_Audio, "devices",        mrb_sdl2_audio_get_devices,        MRB_ARGS_OPT(1));
  mrb_define_module_function(mrb, mod_Audio, "current_driver", mrb_sdl2_audio_get_current_driver, MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mod_Audio, "lock",           mrb_sdl2_audio_lock,               MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mod_Audio, "unlock",         mrb_sdl2_audio_unlock,             MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mod_Audio, "mix_audio",      mrb_sdl2_audio_mix_audio,          MRB_ARGS_REQ(4));
  mrb_define_module_function(mrb, mod_Audio, "status",         mrb_sdl2_auiod_get_status,         MRB_ARGS_NONE());

  mrb_define_method(mrb, class_AudioSpec, "initialize", mrb_sdl2_audio_audiospec_initialize,   MRB_ARGS_OPT(4));
  mrb_define_method(mrb, class_AudioSpec, "freq",       mrb_sdl2_audio_audiospec_get_freq,     MRB_ARGS_NONE());
  mrb_define_method(mrb, class_AudioSpec, "freq=",      mrb_sdl2_audio_audiospec_set_freq,     MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_AudioSpec, "format",     mrb_sdl2_audio_audiospec_get_format,   MRB_ARGS_NONE());
  mrb_define_method(mrb, class_AudioSpec, "format=",    mrb_sdl2_audio_audiospec_set_format,   MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_AudioSpec, "channels",   mrb_sdl2_audio_audiospec_get_channels, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_AudioSpec, "channels=",  mrb_sdl2_audio_audiospec_set_channels, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_AudioSpec, "silence",    mrb_sdl2_audio_audiospec_get_silence,  MRB_ARGS_NONE());
  mrb_define_method(mrb, class_AudioSpec, "samples",    mrb_sdl2_audio_audiospec_get_samples,  MRB_ARGS_NONE());
  mrb_define_method(mrb, class_AudioSpec, "samples=",   mrb_sdl2_audio_audiospec_set_samples,  MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_AudioSpec, "size",       mrb_sdl2_audio_audiospec_get_size,     MRB_ARGS_NONE());
  mrb_define_method(mrb, class_AudioSpec, "callback",   mrb_sdl2_audio_audiospec_get_callback, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_AudioSpec, "callback=",  mrb_sdl2_audio_audiospec_set_callback, MRB_ARGS_BLOCK());
  mrb_define_method(mrb, class_AudioSpec, "userdata",   mrb_sdl2_audio_audiospec_get_userdata, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_AudioSpec, "userdata=",  mrb_sdl2_audio_audiospec_set_userdata, MRB_ARGS_REQ(1));

  mrb_define_method(mrb, class_AudioCVT, "initialize", mrb_sdl2_audio_audiocvt_initialize, MRB_ARGS_REQ(4));
  mrb_define_method(mrb, class_AudioCVT, "convert",    mrb_sdl2_audio_audiocvt_convert,    MRB_ARGS_NONE());

  mrb_define_method(mrb, class_AudioDevice, "initialize", mrb_sdl2_audio_audiodevice_initialize, MRB_ARGS_REQ(3));
  mrb_define_method(mrb, class_AudioDevice, "close",      mrb_sdl2_audio_audiodevice_close,      MRB_ARGS_NONE());
  mrb_define_method(mrb, class_AudioDevice, "spec",       mrb_sdl2_audio_audiodevice_get_spec,   MRB_ARGS_NONE());
  mrb_define_method(mrb, class_AudioDevice, "pause",      mrb_sdl2_audio_audiodevice_pause,      MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_AudioDevice, "lock",       mrb_sdl2_audio_audiodevice_lock,       MRB_ARGS_NONE());
  mrb_define_method(mrb, class_AudioDevice, "unlock",     mrb_sdl2_audio_audiodevice_unlock,     MRB_ARGS_NONE());
  mrb_define_method(mrb, class_AudioDevice, "status",     mrb_sdl2_audio_audiodevice_get_status, MRB_ARGS_NONE());

  mrb_define_method(mrb, class_AudioData, "initialize", mrb_sdl2_audio_audiodata_initialize, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_AudioData, "destroy",    mrb_sdl2_audio_audiodata_destroy,    MRB_ARGS_NONE());
  mrb_define_method(mrb, class_AudioData, "spec",       mrb_sdl2_audio_audiodata_get_spec,   MRB_ARGS_NONE());
  mrb_define_method(mrb, class_AudioData, "buffer",     mrb_sdl2_audio_audiodata_get_buffer, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_AudioData, "length",     mrb_sdl2_audio_audiodata_get_length, MRB_ARGS_NONE());

  int arena_size = mrb_gc_arena_save(mrb);

  /* SDL_AudioFormat */
  mrb_define_const(mrb, mod_Audio, "AUDIO_S8",     mrb_fixnum_value(AUDIO_S8));
  mrb_define_const(mrb, mod_Audio, "AUDIO_U8",     mrb_fixnum_value(AUDIO_U8));
  mrb_define_const(mrb, mod_Audio, "AUDIO_S16LSB", mrb_fixnum_value(AUDIO_S16LSB));
  mrb_define_const(mrb, mod_Audio, "AUDIO_S16MSB", mrb_fixnum_value(AUDIO_S16MSB));
  mrb_define_const(mrb, mod_Audio, "AUDIO_S16SYS", mrb_fixnum_value(AUDIO_S16SYS));
  mrb_define_const(mrb, mod_Audio, "AUDIO_S16",    mrb_fixnum_value(AUDIO_S16));
  mrb_define_const(mrb, mod_Audio, "AUDIO_U16LSB", mrb_fixnum_value(AUDIO_U16LSB));
  mrb_define_const(mrb, mod_Audio, "AUDIO_U16MSB", mrb_fixnum_value(AUDIO_U16MSB));
  mrb_define_const(mrb, mod_Audio, "AUDIO_U16SYS", mrb_fixnum_value(AUDIO_U16SYS));
  mrb_define_const(mrb, mod_Audio, "AUDIO_U16",    mrb_fixnum_value(AUDIO_U16));
  mrb_define_const(mrb, mod_Audio, "AUDIO_S32LSB", mrb_fixnum_value(AUDIO_S32LSB));
  mrb_define_const(mrb, mod_Audio, "AUDIO_S32MSB", mrb_fixnum_value(AUDIO_S32MSB));
  mrb_define_const(mrb, mod_Audio, "AUDIO_S32SYS", mrb_fixnum_value(AUDIO_S32SYS));
  mrb_define_const(mrb, mod_Audio, "AUDIO_S32",    mrb_fixnum_value(AUDIO_S32));
  mrb_define_const(mrb, mod_Audio, "AUDIO_F32LSB", mrb_fixnum_value(AUDIO_F32LSB));
  mrb_define_const(mrb, mod_Audio, "AUDIO_F32MSB", mrb_fixnum_value(AUDIO_F32MSB));
  mrb_define_const(mrb, mod_Audio, "AUDIO_F32SYS", mrb_fixnum_value(AUDIO_F32SYS));
  mrb_define_const(mrb, mod_Audio, "AUDIO_F32",    mrb_fixnum_value(AUDIO_F32));

  mrb_gc_arena_restore(mrb, arena_size);
  arena_size = mrb_gc_arena_save(mrb);

  /* SDL_AudioStatus */
  mrb_define_const(mrb, mod_Audio, "SDL_AUDIO_STOPPED", mrb_fixnum_value(SDL_AUDIO_STOPPED));
  mrb_define_const(mrb, mod_Audio, "SDL_AUDIO_PLAYING", mrb_fixnum_value(SDL_AUDIO_PLAYING));
  mrb_define_const(mrb, mod_Audio, "SDL_AUDIO_PAUSED",  mrb_fixnum_value(SDL_AUDIO_PAUSED));

  mrb_define_const(mrb, mod_Audio, "SDL_AUDIO_ALLOW_FREQUENCY_CHANGE", mrb_fixnum_value(SDL_AUDIO_ALLOW_FREQUENCY_CHANGE));
  mrb_define_const(mrb, mod_Audio, "SDL_AUDIO_ALLOW_FORMAT_CHANGE",    mrb_fixnum_value(SDL_AUDIO_ALLOW_FORMAT_CHANGE));
  mrb_define_const(mrb, mod_Audio, "SDL_AUDIO_ALLOW_CHANNELS_CHANGE",  mrb_fixnum_value(SDL_AUDIO_ALLOW_CHANNELS_CHANGE));
  mrb_define_const(mrb, mod_Audio, "SDL_AUDIO_ALLOW_ANY_CHANGE",       mrb_fixnum_value(SDL_AUDIO_ALLOW_ANY_CHANGE));

  mrb_gc_arena_restore(mrb, arena_size);
}

void
mruby_sdl2_audio_final(mrb_state *mrb)
{
}

