#ifndef MRUBY_SDL2_SURFACE_H
#define MRUBY_SDL2_SURFACE_H

#include "sdl2.h"
#include <SDL2/SDL_surface.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void mruby_sdl2_video_surface_init(mrb_state *mrb, struct RClass *mod_Video);
extern void mruby_sdl2_video_surface_final(mrb_state *mrb, struct RClass *mod_Video);

extern mrb_value mrb_sdl2_video_surface(mrb_state *mrb, SDL_Surface *surface, bool is_associated);

extern SDL_Surface *mrb_sdl2_video_surface_get_ptr(mrb_state *mrb, mrb_value surface);

#ifdef __cplusplus
}
#endif

#endif /* end of MRUBY_SDL2_SURFACE_H */

