#ifndef MRUBY_SDL2_TIMER_H
#define MRUBY_SDL2_TIMER_H

#include "sdl2.h"
#include <SDL2/SDL_timer.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void mruby_sdl2_timer_init(mrb_state *mrb);
extern void mruby_sdl2_timer_final(mrb_state *mrb);

#ifdef __cplusplus
}
#endif

#endif /* end of MRUBY_SDL2_TIMER_H */

