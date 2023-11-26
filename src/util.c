#include "util.h"

t_mono_thread_attach mono_thread_attach;
t_mono_get_root_domain mono_get_root_domain;
t_mono_domain_assembly_open mono_domain_assembly_open;
t_mono_assembly_get_image mono_assembly_get_image;
t_mono_class_from_name mono_class_from_name;
t_mono_class_get_method_from_name mono_class_get_method_from_name;
t_mono_compile_method mono_compile_method;
t_mono_class_get_field_from_name mono_class_get_field_from_name;
t_mono_field_get_value mono_field_get_value;
t_mono_array_length mono_array_length;

void
InitializeMonoFuncs()
{
    HMODULE hMono = GetModuleHandleA("mono-2.0-bdwgc.dll");
    if (hMono == NULL) return;

    mono_thread_attach = (t_mono_thread_attach) GetProcAddress(hMono, "mono_thread_attach");
    mono_get_root_domain = (t_mono_get_root_domain) GetProcAddress(hMono, "mono_get_root_domain");
    mono_domain_assembly_open = (t_mono_domain_assembly_open) GetProcAddress(hMono, "mono_domain_assembly_open");
    mono_assembly_get_image = (t_mono_assembly_get_image) GetProcAddress(hMono, "mono_assembly_get_image");
    mono_class_from_name = (t_mono_class_from_name) GetProcAddress(hMono, "mono_class_from_name");
    mono_class_get_method_from_name = (t_mono_class_get_method_from_name) GetProcAddress(hMono, "mono_class_get_method_from_name");
    mono_compile_method = (t_mono_compile_method) GetProcAddress(hMono, "mono_compile_method");
    mono_class_get_field_from_name = (t_mono_class_get_field_from_name) GetProcAddress(hMono, "mono_class_get_field_from_name");
    mono_field_get_value = (t_mono_field_get_value) GetProcAddress(hMono, "mono_field_get_value");
    mono_array_length = (t_mono_array_length) GetProcAddress(hMono, "mono_array_length");

    void *domain = mono_get_root_domain();
    mono_thread_attach(domain);
}

void *
GetMethod(const char *clazzName, const char *namespace, const char *methodName, const char *assembly, const int argsCnt)
{
    void *pDomain = mono_get_root_domain();

    if (pDomain == NULL) return NULL;

    void *pAssembly = mono_domain_assembly_open(pDomain, assembly);
    if (pAssembly == NULL) return NULL;

    void *pImage = mono_assembly_get_image(pAssembly);
    if (pImage == NULL) return NULL;

    void *pClazz = mono_class_from_name(pImage, namespace, clazzName);
    if (pClazz == NULL) return NULL;

    void *pMethod = mono_class_get_method_from_name(pClazz, methodName, argsCnt);
    if (pMethod == NULL) return NULL;

    void *pCompileMethod = mono_compile_method(pMethod);
    return pCompileMethod;
}

void *
GetFieldValue(const char *clazzName, const char *fieldName, const char *namespace, const char *assembly, const void *object)
{
    void *pDomain = mono_get_root_domain();
    if (pDomain == NULL) return NULL;

    void *pAssembly = mono_domain_assembly_open(pDomain, assembly);
    if (pAssembly == NULL) return NULL;

    void *pImage = mono_assembly_get_image(pAssembly);
    if (pImage == NULL) return NULL;

    void *pClazz = mono_class_from_name(pImage, namespace, clazzName);
    if (pClazz == NULL) return NULL;

    void *pField = mono_class_get_field_from_name(pClazz, fieldName);
    if (pField == NULL) return NULL;

    void *fieldValue = malloc(8);
    mono_field_get_value(object, pField, fieldValue);

    return fieldValue;
}

void *
GetClazz(const char *clazzName, const char *namespace, const char *assembly)
{
    void *pDomain = mono_get_root_domain();
    if (pDomain == NULL)
    {
        return NULL;
    }

    void *pAssembly = mono_domain_assembly_open(pDomain, assembly);
    if (pAssembly == NULL)
    {
        return NULL;
    }

    void *pImage = mono_assembly_get_image(pAssembly);
    if (pImage == NULL)
    {
        return NULL;
    }

    void *pClazz = mono_class_from_name(pImage, namespace, clazzName);
    return pClazz;
}

DWORD __stdcall
EjectThread(LPVOID lpParameter)
{
    Sleep(100);
    FreeLibraryAndExitThread(pHinstance, 0);
}

void
shutdown0(FILE *fp, const char *reason, const char *status)
{
    printf("Reason: %s;\nStatus: %s\n", reason, status);
    Sleep(5000);
    if (fp != NULL)
    {
        fclose(fp);
    }
    FreeConsole();
    CreateThread(0, 0, EjectThread, 0, 0, 0);
}

DWORD WINAPI
Hook(LPVOID lpParam)
{
    FreeConsole();
    AllocConsole();

    FILE *fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);

    InitializeMonoFuncs();

    MH_STATUS status = MH_Initialize();
    const char *statusAsString;

    if (status != MH_OK)
    {
        statusAsString = MH_StatusToString(status);
        shutdown0(fp, "MinHook init failed!", statusAsString);
        return 0;
    }


    pUpdateA = GetMethod("EnemyAI", "", "Update", GAME_ASSEMBLY, -1);
    status = MH_CreateHook(pUpdateA, detourUpdate, (LPVOID *) &pUpdateATarget);

    if (status != MH_OK)
    {
        statusAsString = MH_StatusToString(status);
        shutdown0(fp, "MinHook create failed!", statusAsString);
        return 1;
    }

    while (!GetAsyncKeyState(VK_END))
    {

        if (GetAsyncKeyState(VK_INSERT))
        {
            printf("Enabled!\n");
            status = MH_EnableHook(MH_ALL_HOOKS);

            if (status != MH_OK)
            {
                statusAsString = MH_StatusToString(status);
                shutdown0(fp, "MinHook enable failed!", statusAsString);
                return 1;
            }
        }

        if (GetAsyncKeyState(VK_DELETE))
        {
            printf("Disabled!\n");

            status = MH_DisableHook(MH_ALL_HOOKS);
            if (status != MH_OK)
            {
                statusAsString = MH_StatusToString(status);
                shutdown0(fp, "MinHook disable failed!", statusAsString);
                return 1;
            }
        }

    }

    shutdown0(fp, "Test unhook", statusAsString);

    return 1;
}