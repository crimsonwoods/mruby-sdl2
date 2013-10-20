#ifndef MRUBY_SDL2_THREADING_H
#define MRUBY_SDL2_THREADING_H

#include "mruby.h"

#ifdef __cplusplus
extern "C" {
#endif

extern mrb_state *mrb_open_for_thread(mrb_state *vm);
extern void       mrb_close_for_thread(mrb_state *mrb);

#ifdef __cplusplus
}
#endif

#endif /* end of MRUBY_SDL2_THREADING_H */

