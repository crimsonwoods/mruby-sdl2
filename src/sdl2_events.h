#ifndef MRUBY_SDL2_EVENTS_H
#define MRUBY_SDL2_EVENTS_H

#include "sdl2.h"
#include <SDL2/SDL_events.h>

#ifdef __cplusplus
extern "C" {
#endif

extern SDL_Event *mrb_sdl2_input_event_get_ptr(mrb_state *mrb, mrb_value value);
extern mrb_value mrb_sdl2_input_event(mrb_state *mrb, SDL_Event const *event);

extern void mruby_sdl2_events_init(mrb_state *mrb);
extern void mruby_sdl2_events_final(mrb_state *mrb);

#ifdef __cplusplus
}
#endif

#endif /* end of MRUBY_SDL2_EVENTS_H */

