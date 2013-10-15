#ifndef MRUBY_SDL2_MUTEX_H
#define MRUBY_SDL2_MUTEX_H

#include "sdl2.h"
#include <SDL2/SDL_mutex.h>

#ifdef __cplusplus
extern "C" {
#endif

extern SDL_mutex *mrb_sdl2_mutex_get_ptr(mrb_state *mrb, mrb_value mutex);
extern mrb_value  mrb_sdl2_mutex(mrb_state *mrb, SDL_mutex *mutex);

extern void mruby_sdl2_mutex_init(mrb_state *mrb);
extern void mruby_sdl2_mutex_final(mrb_state *mrb);

#ifdef __cplusplus
}
#endif

#endif /* end of MRUBY_SDL2_MUTEX_H */

