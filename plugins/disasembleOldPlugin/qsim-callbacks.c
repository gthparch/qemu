#include "qsim-callbacks.h"
#include <stdint-uintn.h>
//#include "qsim-plugin.h"

extern bool atomic_flag;
extern int qsim_id;
extern uint64_t qsim_icount;
extern atomic_cb_t  qsim_atomic_cb;

#ifdef TARGET_I386 || TARGET_X86_64

void helper_atomic_callback(void)
{
    atomic_flag = !atomic_flag;
    // pid based callbacks
    if (!qsim_sys_callbacks && curr_tpid[qsim_id] != qsim_tpid)
        return;

    /* if atomic callback returns non-zero, suspend execution */
    if (qsim_gen_callbacks && qsim_atomic_cb && qsim_atomic_cb(qsim_id))
        qsim_swap_ctx();

    return;
}


#elif TARGET_ARM || TARGET_AARCH64

void HELPER(atomic_callback)(void)
{
    atomic_flag = !atomic_flag;
    /* if atomic callback returns non-zero, suspend execution */
    if (qsim_atomic_cb && qsim_atomic_cb(qsim_id))
        qsim_swap_ctx();

    return;
}
#endif

void HELPER(qsim_callback)(void)
{
    qsim_icount--;
    if (qsim_icount == 0) {
        qsim_swap_ctx();
    }

    return;
}