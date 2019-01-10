#ifndef __QSIM__CALLBACK_H__
#define __QSIM__CALLBACK_H__

#include "helper-head.h"

//DEF_HELPER_4(inst_callback, void, env, i64, i32, i32)
//DEF_HELPER_3(inst_br_callback, void, env, i64, i32)
//DEF_HELPER_3(reg_read_callback, void, env, i32, i32)
//DEF_HELPER_3(reg_write_callback, void, env, i32, i32)
//DEF_HELPER_4(load_callback_pre, void, env, i64, i32, i32)
//DEF_HELPER_4(load_callback_post, void, env, i64, i32, i32)
//DEF_HELPER_4(store_callback_pre, void, env, i64, i32, tl)
//DEF_HELPER_4(store_callback_post, void, env, i64, i32, tl)
DEF_HELPER_0(atomic_callback, void)
DEF_HELPER_0(qsim_callback, void)

#endif //__QSIM__CALLBACK_H__