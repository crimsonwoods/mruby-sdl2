#ifndef MRUBY_SDL2_VERSION_H
#define MRUBY_SDL2_VERSION_H

#include "sdl2.h"
#include <SDL2/SDL_version.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct mrb_sdl2_version_data_t {
  SDL_version version;
  char const *revision;
  int         revnum;
} mrb_sdl2_version_data_t;

extern mrb_sdl2_version_data_t *mrb_sdl2_version_get_ptr(mrb_state *mrb, mrb_value value);
extern mrb_value mrb_sdl2_version(mrb_state *mrb, SDL_version *version, char const *rev, int revnum);

extern void mruby_sdl2_version_init(mrb_state *mrb);
extern void mruby_sdl2_version_final(mrb_state *mrb);

#ifdef __cplusplus
}
#endif

#endif /* end of MRUBY_SDL2_VERSION_H */

