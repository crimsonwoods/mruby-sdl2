#ifndef MRUBY_SDL2_THREAD_H
#define MRUBY_SDL2_THREAD_H

#include "sdl2.h"
#include <SDL2/SDL_thread.h>

#ifdef __cplusplus
extern "C" {
#endif

extern SDL_Thread * mrb_sdl2_thread_get_ptr(mrb_state *mrb, mrb_value value);
extern mrb_value mrb_sdl2_thread(mrb_state *mrb, SDL_Thread *thread);

extern void mruby_sdl2_thread_init(mrb_state *mrb);
extern void mruby_sdl2_thread_final(mrb_state *mrb);

#ifdef __cplusplus
}
#endif

#endif /* end of MRUBY_SDL2_THREAD_H */

