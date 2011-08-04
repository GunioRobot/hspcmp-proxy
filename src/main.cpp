#include <windows.h>
#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <boost/regex.hpp>

#ifdef __cplusplus
#define EXPORT extern "C" __declspec (dllexport)
#else
#define EXPORT __declspec (dllexport)
#endif

#define FUNC_LIST(V)           \
	V(aht_propupdate)          \
	V(aht_delmod)              \
	V(aht_findend)             \
	V(aht_findparts)           \
	V(aht_findstart)           \
	V(aht_getexid)             \
	V(aht_getmodaxis)          \
	V(aht_getmodcnt)           \
	V(aht_getopt)              \
	V(aht_getpage)             \
	V(aht_getparts)            \
	V(aht_getprjmax)           \
	V(aht_getprjsrc)           \
	V(aht_getprop)             \
	V(aht_getpropcnt)          \
	V(aht_getpropid)           \
	V(aht_getpropmode)         \
	V(aht_getproptype)         \
	V(aht_ini)                 \
	V(aht_linkmod)             \
	V(aht_listparts)           \
	V(aht_make)                \
	V(aht_makeend)             \
	V(aht_makeinit)            \
	V(aht_makeput)             \
	V(aht_parts)               \
	V(aht_prjload2)            \
	V(aht_prjload)             \
	V(aht_prjloade)            \
	V(aht_prjsave)             \
	V(aht_sendstr)             \
	V(aht_setmodaxis)          \
	V(aht_setpage)             \
	V(aht_setprop)             \
	V(aht_source)              \
	V(aht_stdbuf)              \
	V(aht_stdsize)             \
	V(aht_unlinkmod)           \
	V(hsc3_getruntime)         \
	V(hsc3_getsym)             \
	V(hsc3_make)               \
	V(hsc3_messize)            \
	V(hsc3_run)                \
	V(hsc_bye)                 \
	V(hsc_clrmes)              \
	V(hsc_comp)                \
	V(hsc_compath)             \
	V(hsc_getmes)              \
/*	V(hsc_ini) */              \
	V(hsc_objname)             \
	V(hsc_refname)             \
	V(hsc_ver)                 \
	V(pack_exe)                \
	V(pack_get)                \
	V(pack_ini)                \
	V(pack_make)               \
	V(pack_opt)                \
	V(pack_rt)                 \
	V(pack_view)

typedef BOOL (CALLBACK *HSPCMPFUNC)(int,int,int,int);

static HMODULE lib;
static HINSTANCE hInst;
typedef std::map<std::string, HSPCMPFUNC> function_map_t;
static function_map_t functions;
static const char * const default_dllname = "hspcmp-default.dll";

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD fdwReason, PVOID pvReserved) {
	if (fdwReason == DLL_PROCESS_ATTACH) {
		hInst = hInstance;
	}
	if (fdwReason == DLL_PROCESS_DETACH) {
		FreeLibrary(lib);
		lib = NULL;
	}
	return TRUE;
}

static HSPCMPFUNC get_func_ptr(const char *name) {
	function_map_t::iterator it = functions.find(name);
	if (it != functions.end()) {
		return it->second;
	}
	std::string modified_name = std::string("_") + name + "@16";
	HSPCMPFUNC fn = (HSPCMPFUNC)GetProcAddress(lib, modified_name.c_str());
	functions[name] = fn;
	return fn;
}

static void load_dll(const char *name) {
	char modulename[1024];
	GetModuleFileName(hInst, modulename, sizeof modulename);
	std::string libname = std::string(modulename) + "\\..\\" + name;
	FreeLibrary(lib);
	lib = LoadLibrary(libname.c_str());
	functions.clear();
	if (!lib) {
		static int failed_in_loading_default_dll = 0;
		if (!strcmp(name, default_dllname)) {
			if (failed_in_loading_default_dll) return;
			failed_in_loading_default_dll = 1;
		}
		std::string msg = std::string("can't load `") + name + "'.";
		MessageBox(NULL, msg.c_str(), "hspcmp proxy dll", MB_ICONINFORMATION | MB_OK);
	}
}

static int call_func(const char *name, int p1, int p2, int p3, int p4) {
	if (!lib) {
		load_dll(default_dllname);
		if (!lib) return 0;
	}
	HSPCMPFUNC fn = get_func_ptr(name);
	return fn(p1, p2, p3, p4);
}

static std::string search_compiler_name(const char *name) {
	std::ifstream ifs(name);
	std::string line;
	boost::regex re("^\\s*#\\s*compiler\\s*\"([^\"]+)\"");
	boost::smatch m;
	while (ifs && std::getline(ifs, line)) {
		if (boost::regex_search(line, m, re)) {
			return m.str(1);
		}
	}
	return default_dllname;
}

EXPORT BOOL WINAPI hsc_ini(int p1, int p2, int p3, int p4) {
	char *name = (char *)p2;
	std::string dllname = search_compiler_name(name);
	load_dll(dllname.c_str());
	return call_func("hsc_ini", p1, p2, p3, p4);
}

#define DEFINE_FUNC(name) \
	EXPORT BOOL WINAPI name(int p1, int p2, int p3, int p4) { \
		return call_func(#name, p1, p2, p3, p4);                \
	}

FUNC_LIST(DEFINE_FUNC)
