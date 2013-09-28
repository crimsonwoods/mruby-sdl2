#ifndef MRUBY_SDL2_AUDIO_H
#define MRUBY_SDL2_AUDIO_H

#include "sdl2.h"
#include <SDL2/SDL_audio.h>

#ifdef __cplusplus
extern "C" {
#endif

extern struct RClass *mod_Audio;

extern void mruby_sdl2_audio_init(mrb_state *mrb);
extern void mruby_sdl2_audio_final(mrb_state *mrb);

extern SDL_AudioSpec *mrb_sdl2_audiospec_get_ptr(mrb_state *mrb, mrb_value value);
extern SDL_AudioCVT  *mrb_sdl2_audiocvt_get_ptr(mrb_state *mrb, mrb_value value);
extern mrb_value      mrb_sdl2_audiospec(mrb_state *mrb, SDL_AudioSpec const *value);
extern mrb_value      mrb_sdl2_audiocvt(mrb_state *mrb, SDL_AudioCVT const *value);

#ifdef __cplusplus
}
#endif

#endif /* end of MRUBY_SDL2_AUDIO_H */

