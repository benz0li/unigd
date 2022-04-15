// Generated by cpp11: do not edit by hand
// clang-format off

#ifndef UNIGD_EXTERNAL_API_H
#define UNIGD_EXTERNAL_API_H

#define R_NO_REMAP
#include <Rinternals.h>
#include <R_ext/Rdynload.h>

#include <unigd_types.h>

static inline int ugd_c_test_fun() {
    static int(*p_ugd_c_test_fun)() = NULL;
    if (p_ugd_c_test_fun == NULL) {
        p_ugd_c_test_fun = (int(*)()) R_GetCCallable("unigd", "_unigd_ex_ugd_c_test_fun");
    }
    return p_ugd_c_test_fun();
}

// Attach client to unigd device.
static inline bool ugd_attach_client(int devnum, const std::shared_ptr<unigd::graphics_client>& t_client) {
    static bool(*p_ugd_c_attach_client)(int, const std::shared_ptr<unigd::graphics_client>&) = NULL;
    if (p_ugd_c_attach_client == NULL) {
        p_ugd_c_attach_client = (bool(*)(int, const std::shared_ptr<unigd::graphics_client>&)) R_GetCCallable("unigd", "_unigd_ex_ugd_c_attach_client");
    }
    return p_ugd_c_attach_client(devnum, t_client);
}

static inline bool ugd_renderers_find(const std::__cxx11::string& id, const unigd::renderers::renderer_gen** renderer) {
    static bool(*p_ugd_c_renderers_find)(const std::__cxx11::string&, const unigd::renderers::renderer_gen**) = NULL;
    if (p_ugd_c_renderers_find == NULL) {
        p_ugd_c_renderers_find = (bool(*)(const std::__cxx11::string&, const unigd::renderers::renderer_gen**)) R_GetCCallable("unigd", "_unigd_ex_ugd_c_renderers_find");
    }
    return p_ugd_c_renderers_find(id, renderer);
}

static inline bool ugd_renderers_find_info(const std::__cxx11::string& id, const unigd::renderers::renderer_info** renderer) {
    static bool(*p_ugd_c_renderers_find_info)(const std::__cxx11::string&, const unigd::renderers::renderer_info**) = NULL;
    if (p_ugd_c_renderers_find_info == NULL) {
        p_ugd_c_renderers_find_info = (bool(*)(const std::__cxx11::string&, const unigd::renderers::renderer_info**)) R_GetCCallable("unigd", "_unigd_ex_ugd_c_renderers_find_info");
    }
    return p_ugd_c_renderers_find_info(id, renderer);
}

#endif // UNIGD_EXTERNAL_API_H
