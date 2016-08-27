#include "sdl2_version.h"
#include "mruby/value.h"
#include "mruby/data.h"
#include "mruby/class.h"
#include "mruby/string.h"
#include <SDL2/SDL_revision.h>

static struct RClass *class_Version = NULL;

static void
mrb_sdl2_version_data_free(mrb_state *mrb, void *p)
{
  mrb_free(mrb, p);
}

static struct mrb_data_type const mrb_sdl2_version_data_type = {
  "Version", mrb_sdl2_version_data_free
};

mrb_sdl2_version_data_t *
mrb_sdl2_version_get_ptr(mrb_state *mrb, mrb_value value)
{
  if (mrb_nil_p(value)) {
    return NULL;
  }
  return mrb_data_get_ptr(mrb, value, &mrb_sdl2_version_data_type);
}

mrb_value
mrb_sdl2_version(mrb_state *mrb, SDL_version *version, char const *rev, int revnum)
{
  mrb_sdl2_version_data_t *data =
    (mrb_sdl2_version_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_version_data_t));
  if (NULL == data) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
  }
  data->version  = *version;
  data->revision = rev;
  data->revnum   = revnum;
  return mrb_obj_value(Data_Wrap_Struct(mrb, class_Version, &mrb_sdl2_version_data_type, data));
}

static mrb_value
mrb_sdl2_version_get_major(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_version_data_t *data =
    (mrb_sdl2_version_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_version_data_type);
  return mrb_fixnum_value(data->version.major);
}

static mrb_value
mrb_sdl2_version_get_minor(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_version_data_t *data =
    (mrb_sdl2_version_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_version_data_type);
  return mrb_fixnum_value(data->version.minor);
}

static mrb_value
mrb_sdl2_version_get_patch(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_version_data_t *data =
    (mrb_sdl2_version_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_version_data_type);
  return mrb_fixnum_value(data->version.patch);
}

static mrb_value
mrb_sdl2_version_get_revision(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_version_data_t *data =
    (mrb_sdl2_version_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_version_data_type);
  return mrb_fixnum_value(data->revnum);
}

static mrb_value
mrb_sdl2_version_get_revision_str(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_version_data_t *data =
    (mrb_sdl2_version_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_version_data_type);
  return mrb_str_new_cstr(mrb, data->revision);
}

static mrb_value
mrb_sdl2_version_to_s(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_version_data_t *data =
    (mrb_sdl2_version_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_version_data_type);
  mrb_value const major  = mrb_funcall(mrb, mrb_fixnum_value(data->version.major), "to_s", 0);
  mrb_value const minor  = mrb_funcall(mrb, mrb_fixnum_value(data->version.minor), "to_s", 0);
  mrb_value const patch  = mrb_funcall(mrb, mrb_fixnum_value(data->version.patch), "to_s", 0);
  mrb_value const revnum = mrb_funcall(mrb, mrb_fixnum_value(data->revnum), "to_s", 0);
  mrb_value const revision = mrb_str_new_cstr(mrb, data->revision);
  mrb_value str = mrb_str_new_cstr(mrb, "#<SDL2::Version: major=");
  str = mrb_str_append(mrb, str, major);
  str = mrb_str_cat2(mrb, str, ", minor=");
  str = mrb_str_append(mrb, str, minor);
  str = mrb_str_cat2(mrb, str, ", patch=");
  str = mrb_str_append(mrb, str, patch);
  str = mrb_str_cat2(mrb, str, ", revision=");
  str = mrb_str_append(mrb, str, revnum);
  str = mrb_str_cat2(mrb, str, ", ");
  str = mrb_str_append(mrb, str, revision);
  str = mrb_str_cat2(mrb, str, ">");
  return str;
}


static mrb_value
mrb_sdl2_version_get_compiled(mrb_state *mrb, mrb_value cls)
{
  SDL_version v;
  SDL_VERSION(&v);
  return mrb_sdl2_version(mrb, &v, SDL_REVISION, SDL_REVISION_NUMBER);
}

static mrb_value
mrb_sdl2_version_get_linked(mrb_state *mrb, mrb_value cls)
{
  SDL_version v;
  SDL_GetVersion(&v);
  return mrb_sdl2_version(mrb, &v, SDL_GetRevision(), SDL_GetRevisionNumber());
}

void
mruby_sdl2_version_init(mrb_state *mrb)
{
  class_Version = mrb_define_class_under(mrb, mod_SDL2, "Version", mrb->object_class);

  MRB_SET_INSTANCE_TT(class_Version, MRB_TT_DATA);

  mrb_define_method(mrb, class_Version, "major",        mrb_sdl2_version_get_major,        MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Version, "minor",        mrb_sdl2_version_get_minor,        MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Version, "patch",        mrb_sdl2_version_get_patch,        MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Version, "revision",     mrb_sdl2_version_get_revision,     MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Version, "revision_str", mrb_sdl2_version_get_revision_str, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Version, "to_s",         mrb_sdl2_version_to_s,             MRB_ARGS_NONE());

  mrb_define_class_method(mrb, class_Version, "compiled", mrb_sdl2_version_get_compiled, MRB_ARGS_NONE());
  mrb_define_class_method(mrb, class_Version, "linked",   mrb_sdl2_version_get_linked,   MRB_ARGS_NONE());
}

void
mruby_sdl2_version_final(mrb_state *mrb)
{
}

