#ifndef MRUBY_SDL2_KEYBOARD_H
#define MRUBY_SDL2_KEYBOARD_H

#include "sdl2.h"
#include <SDL2/SDL_keyboard.h>

#ifdef __cplusplus
extern "C" {
#endif

extern SDL_Keysym *mrb_sdl2_keyboard_keysym_get_ptr(mrb_state *mrb, mrb_value value);
extern mrb_value mrb_sdl2_keyboard_keysym(mrb_state *mrb, SDL_Keysym const *keysym);

extern void mruby_sdl2_keyboard_init(mrb_state *mrb);
extern void mruby_sdl2_keyboard_final(mrb_state *mrb);

#ifdef __cplusplus
}
#endif

#endif /* end of MRUBY_SDL2_KEYBOARD_H */

