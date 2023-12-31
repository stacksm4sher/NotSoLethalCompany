#ifndef NOTSOLETHALCOMPANY_UTIL_H
#define NOTSOLETHALCOMPANY_UTIL_H

#include <windows.h>
#include <stdio.h>
#include <MinHook.h>

#include "esp.h"

typedef void *(*t_mono_thread_attach)(void *monoDomain);

typedef void *(*t_mono_get_root_domain)();

typedef void *(*t_mono_domain_assembly_open)(void *domain, const char *name);

typedef void *(*t_mono_assembly_get_image)(void *assembly);

typedef void *(*t_mono_class_from_name)(void *image, const char *namespace, const char *name);

typedef void *(*t_mono_class_get_method_from_name)(void *clazz, const char *name, int param_count);

typedef void *(*t_mono_compile_method)(const void *method);

typedef void *(*t_mono_class_get_field_from_name)(const void *clazz, const char *name);

typedef void *(*t_mono_field_get_value)(const void *domain, const void *field, const void *object);

typedef void *(*t_mono_array_length)(void *array);

typedef void *(*t_mono_class_vtable)(void *domain, void *clazz);

typedef void (*t_mono_field_static_get_value) (void *vt, void *field, void *pValue);

typedef void (*t_mono_runtime_class_init) (void *vt);

typedef void *(*t_mono_class_get_property_from_name)(void *clazz, const char *name);

typedef struct MonoProperty
{
    void *parentClazz;
    const char *name;
    void *getMethod;
    void *setMethod;
    unsigned int attrs;
} monoProperty;

const static char *GAME_ASSEMBLY = "ENTER YOUR PATH TO Assembly-CSharp.dll";
const static char *UNITY_ENGINE = "UnityEngine.dll";
const static char *MSCORLIB = "mscorlib.dll";

extern HINSTANCE pHinstance;

void *
GetMethod(const char *clazzName, const char *namespace, const char *methodName, const char *assembly, int argsCnt);

void *
GetFieldValue(const char *clazzName, const char *fieldName, const char *namespace, const char *assembly, const void *object, unsigned int fieldSize);

void *
GetStaticFieldValue(const char *clazzName,
                    const char *fieldName,
                    const char *namespace,
                    const char *assembly,
                    unsigned int fieldSize);

void *
GetClazz(const char *clazzName, const char *namespace, const char *assembly);

void *
GetProperty(void *clazz, const char *name);

DWORD WINAPI
Hook(LPVOID lpParam);

#endif //NOTSOLETHALCOMPANY_UTIL_H
