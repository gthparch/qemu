#include "qemu/osdep.h"
#include "qemu-common.h"
#include "cpu.h"
#include "qemu/error-report.h"
#include "qemu/plugins.h"
#include "qemu/instrument.h"
#include "tcg/tcg.h"
#include "tcg/tcg-op.h"
#include "qemu/queue.h"
#include "qemu/option.h"
#include <gmodule.h>

typedef bool (*PluginInitFunc)(const char *);
typedef bool (*PluginNeedsBeforeInsnFunc)(uint64_t, void *);
typedef void (*PluginBeforeInsnFunc)(uint64_t, void *);
typedef void (*PluginAfterMemFunc)(void *, uint64_t, int, int);
typedef void (*PluginBeeforeInterupt)(void *, uint64_t);

typedef struct QemuPluginInfo {
    const char *filename;
    const char *args;
    GModule *g_module;

    PluginInitFunc init;
    PluginNeedsBeforeInsnFunc needs_before_insn;
    PluginBeforeInsnFunc before_insn;
    PluginAfterMemFunc after_mem;

    QLIST_ENTRY(QemuPluginInfo) next;
} QemuPluginInfo;

static QLIST_HEAD(, QemuPluginInfo) qemu_plugins
                                = QLIST_HEAD_INITIALIZER(qemu_plugins);

static QemuOptsList qemu_plugin_opts = {
    .name = "plugin",
    .head = QTAILQ_HEAD_INITIALIZER(qemu_plugin_opts.head),
    .desc = {
        {
            .name = "file",
            .type = QEMU_OPT_STRING,
        },{
            .name = "args",
            .type = QEMU_OPT_STRING,
        },
        { /* end of list */ }
    },
};

void qemu_plugin_parse_cmd_args(const char *optarg)
{
    QemuOpts *opts = qemu_opts_parse_noisily(&qemu_plugin_opts, optarg, false);
    qemu_plugin_load(qemu_opt_get(opts, "file"),
        qemu_opt_get(opts, "args"));
}

void qemu_plugin_load(const char *filename, const char *args)
{
#ifdef __APPLE__
    if (!filename) {
        error_report("plugin name was not specified");
        return;
    }
    QemuPluginInfo *info = NULL;
    void* g_module = dlopen(filename, RTLD_LAZY);
    if (!g_module) {
        error_report("can't load plugin '%s'", filename);
        return;
    }
    
    info = g_new0(QemuPluginInfo, 1);
    info->filename = g_strdup(filename);
    info->g_module = g_module;
    if (args) {
        info->args = g_strdup(args);
    }
    ((gpointer*)&info->init) = dlsym(g_module,"plugin_init");
    /* Get the instrumentation callbacks */
    ((gpointer*)&info->needs_before_insn) = dlsym(g_module, "plugin_needs_before_insn");
    ((gpointer*)&info->before_insn) = dlsym(g_module, "plugin_before_insn");
    ((gpointer*)&info->after_mem) = dlsym(g_module, "plugin_after_mem");

#else
    GModule *g_module;
    QemuPluginInfo *info = NULL;
    if (!filename) {
        error_report("plugin name was not specified");
        return;
    }
    g_module = g_module_open(filename,
        G_MODULE_BIND_LAZY | G_MODULE_BIND_LOCAL);
    if (!g_module) {
        error_report("can't load plugin '%s'", filename);
        fprintf(stderr,"error: %s",g_module_error ());
        return;
    }
    fprintf(stderr, "plugin '%s' Loaded!", filename);
    info = g_new0(QemuPluginInfo, 1);
    info->filename = g_strdup(filename);
    info->g_module = g_module;
    if (args) {
        info->args = g_strdup(args);
    }

    if (!g_module_symbol(g_module, "plugin_init", (gpointer*)&info->init) ) {
        fprintf(stderr, "plugin_init failed to load is: 0x%p !", info->init);
        fprintf(stderr, "plugin_init error: %s",g_module_error ());
        g_module_close (g_module);
        return;

    }
    /* Get the instrumentation callbacks */
    if (! g_module_symbol(g_module, "plugin_needs_before_insn",
        (gpointer*)&info->needs_before_insn) ) {
            fprintf(stderr, "needs_before_insn is: 0x%p !", info->needs_before_insn);
            fprintf(stderr, "needs_before_insn error: %s",g_module_error ());
            g_module_close (g_module);
            return;
    }
    if (! g_module_symbol(g_module, "plugin_before_insn",
        (gpointer*)&info->before_insn) ) {
            fprintf(stderr, "before_insn is: 0x%p !", info->before_insn);
            fprintf(stderr, "before_insn error: %s",g_module_error ());
            g_module_close (g_module);
            return;
        }

    if (! g_module_symbol(g_module, "plugin_after_mem",
        (gpointer*)&info->after_mem) ) {

            fprintf(stderr, "after_mem is: 0x%p !", info->after_mem);
            fprintf(stderr, "after_mem error: %s",g_module_error ());
            g_module_close (g_module);
            return;
        }

#endif
    QLIST_INSERT_HEAD(&qemu_plugins, info, next);

    return;
}

bool plugins_need_before_insn(target_ulong pc, CPUState *cpu)
{
    QemuPluginInfo *info;
    QLIST_FOREACH(info, &qemu_plugins, next) {
        if (info->needs_before_insn && info->needs_before_insn(pc, cpu)) {
            return true;
        }
    }

    return false;
}

void plugins_instrument_before_insn(target_ulong pc, CPUState *cpu)
{
    TCGv t_pc = tcg_const_tl(pc);
    TCGv_ptr t_cpu = tcg_const_ptr(cpu);
    /* We will dispatch plugins' callbacks in our own helper below */
    gen_helper_before_insn(t_pc, t_cpu);
    tcg_temp_free(t_pc);
    tcg_temp_free_ptr(t_cpu);
}

void helper_before_insn(target_ulong pc, void *cpu)
{
    QemuPluginInfo *info;
    QLIST_FOREACH(info, &qemu_plugins, next) {
        if (info->needs_before_insn && info->needs_before_insn(pc, cpu)) {
            if (info->before_insn) {
                info->before_insn(pc, cpu);
            }
        }
    }
}

void helper_mem_callback(void *cpu, 
#if TARGET_LONG_BITS == 64
                        uint64_t addr,
#else
                        uint32_t addr,
#endif

                         uint32_t size, uint32_t type)
{
#if TARGET_LONG_BITS == 64
    //fprintf(stderr, "addr: %lx, size: %d, type: %d\n", addr, size, type);
#else
    //fprintf(stderr, "addr: %d, size: %d, type: %d\n", addr, size, type);
#endif

    
    QemuPluginInfo *info;
    QLIST_FOREACH(info, &qemu_plugins, next) {
        if( info->after_mem) {
            info->after_mem(cpu, addr, size, type);
        }
    }
    return;
}

void qemu_plugins_init(void)
{
    QemuPluginInfo *info;
    QLIST_FOREACH(info, &qemu_plugins, next) {
        if (info->init) {
            info->init(info->args);
        } else {
            fprintf(stderr, "plugin failed to load");
        }
    }

#include "exec/helper-register.h"
}
