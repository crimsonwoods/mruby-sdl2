#include "threading.h"
#include "mruby/gc.h"

extern void mrb_init_symtbl(mrb_state*);
extern void mrb_init_class(mrb_state*);
extern void mrb_init_object(mrb_state*);
extern void mrb_init_kernel(mrb_state*);
extern void mrb_init_comparable(mrb_state*);
extern void mrb_init_enumerable(mrb_state*);
extern void mrb_init_symbol(mrb_state*);
extern void mrb_init_exception(mrb_state*);
extern void mrb_init_proc(mrb_state*);
extern void mrb_init_string(mrb_state*);
extern void mrb_init_array(mrb_state*);
extern void mrb_init_hash(mrb_state*);
extern void mrb_init_numeric(mrb_state*);
extern void mrb_init_range(mrb_state*);
extern void mrb_init_gc(mrb_state*);
extern void mrb_init_math(mrb_state*);
extern void mrb_init_mrblib(mrb_state*);
extern void mrb_init_mrbgems(mrb_state*);
extern void mrb_final_mrbgems(mrb_state*);

#ifndef STACK_INIT_SIZE
#define STACK_INIT_SIZE 128
#endif
#ifndef CALLINFO_INIT_SIZE
#define CALLINFO_INIT_SIZE 32
#endif

static void
stack_init_for_thread(mrb_state *mrb) {
  struct mrb_context *c = mrb->c;

  /* mrb_assert(mrb->stack == NULL); */
  c->stbase = (mrb_value *)mrb_calloc(mrb, STACK_INIT_SIZE, sizeof(mrb_value));
  c->stend = c->stbase + STACK_INIT_SIZE;
  c->stack = c->stbase;

  /* mrb_assert(ci == NULL); */
  c->cibase = (mrb_callinfo *)mrb_calloc(mrb, CALLINFO_INIT_SIZE, sizeof(mrb_callinfo));
  c->ciend = c->cibase + CALLINFO_INIT_SIZE;
  c->ci = c->cibase;
  c->ci->target_class = mrb->object_class;
}

mrb_state *
mrb_open_for_thread(mrb_state *vm)
{
  static const struct mrb_context mrb_context_zero = { 0 };
  mrb_state *mrb;

#ifdef MRB_NAN_BOXING
  mrb_assert(sizeof(void*) == 4);
#endif

  mrb = (mrb_state *)(vm->allocf)(NULL, NULL, sizeof(mrb_state), vm->ud);
  if (mrb == NULL) return NULL;

  *mrb = *vm;
  mrb->jmp = NULL;
  mrb->c = (struct mrb_context*)mrb_malloc(mrb, sizeof(struct mrb_context));
  *mrb->c = mrb_context_zero;
  mrb->root_c = mrb->c;
  stack_init_for_thread(mrb);
  mrb_gc_arena_restore(mrb, 0);

  return mrb;
}

void
mrb_close_for_thread(mrb_state *mrb)
{
  mrb_free_context(mrb, mrb->root_c);
  mrb_free(mrb, mrb);
}

