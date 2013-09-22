#ifndef MRUBY_SDL2_RENDER_H
#define MRUBY_SDL2_RENDER_H

#include "sdl2.h"
#include <SDL2/SDL_render.h>

extern SDL_Renderer *mrb_sdl2_video_renderer_get_ptr(mrb_state *mrb, mrb_value renderer);

extern mrb_value mrb_sdl2_video_renderer(mrb_state *mrb, SDL_Renderer *renderer);

extern void mruby_sdl2_video_renderer_init(mrb_state *mrb, struct RClass *mod_Video);
extern void mruby_sdl2_video_renderer_final(mrb_state *mrb, struct RClass *mod_Video);


#endif /* end of MRUBY_SDL2_RENDER_H */

