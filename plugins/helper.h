DEF_HELPER_2(before_insn, void, tl, ptr)
#if TARGET_LONG_BITS == 64
    DEF_HELPER_4(mem_callback, void, ptr, i64, i32, i32)
#else
    DEF_HELPER_4(mem_callback, void, ptr, i32, i32, i32)
#endif
