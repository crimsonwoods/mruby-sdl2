#ifndef MRUBY_SDL2_RECT_H
#define MRUBY_SDL2_RECT_H

#include "sdl2.h"
#include <SDL2/SDL_rect.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct size_data_t {
  int w;
  int h;
} size_data_t;

extern void mruby_sdl2_rect_init(mrb_state *mrb);
extern void mruby_sdl2_rect_final(mrb_state *mrb);

extern mrb_value mrb_sdl2_rect(mrb_state *mrb, int x, int y, int w, int h);
extern mrb_value mrb_sdl2_point(mrb_state *mrb, int x, int y);
extern mrb_value mrb_sdl2_size(mrb_state *mrb, int w, int h);

extern SDL_Rect    *mrb_sdl2_rect_get_ptr(mrb_state *mrb, mrb_value rect);
extern SDL_Point   *mrb_sdl2_point_get_ptr(mrb_state *mrb, mrb_value point);
extern size_data_t *mrb_sdl2_size_get_ptr(mrb_state *mrb, mrb_value size);

#ifdef __cplusplus
}
#endif

#endif /* end of MRUBY_SDL2_RECT_H */


