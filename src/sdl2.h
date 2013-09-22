#ifndef MRUBY_SDL2_H
#define MRUBY_SDL2_H

#include "mruby.h"
#include <stdint.h>
#include <stdbool.h>

extern struct RClass *mod_SDL2;
extern struct RClass *class_SDL2Error;

extern void mruby_sdl2_module_init(mrb_state *mrb);
extern void mruby_sdl2_module_final(mrb_state *mrb);

extern void mruby_sdl2_raise_error(mrb_state *mrb);

#endif /* end of MRUBY_SDL2_H */

