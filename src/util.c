#include "util.h"
#include <logger.h>

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
t_mono_class_vtable mono_class_vtable;
t_mono_field_static_get_value mono_field_static_get_value;
t_mono_runtime_class_init mono_runtime_class_init;
t_mono_class_get_property_from_name mono_class_get_property_from_name;

UpdateA pUpdateA;
UpdateA pUpdateATarget;

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
    mono_class_vtable = (t_mono_class_vtable) GetProcAddress(hMono, "mono_class_vtable");
    mono_field_static_get_value = (t_mono_field_static_get_value) GetProcAddress(hMono, "mono_field_static_get_value");
    mono_runtime_class_init = (t_mono_runtime_class_init) GetProcAddress(hMono, "mono_runtime_class_init");
    mono_class_get_property_from_name = (t_mono_class_get_property_from_name) GetProcAddress(hMono, "mono_class_get_property_from_name");

    void *domain = mono_get_root_domain();
    mono_thread_attach(domain);
}

const char *getMethodLogMsgFormat = "%s returned NULL;\n{\n\tclazzName: %s\n\tnamespace: %s\n\tmethodName: %s\n\tassembly: %s\n\targsCnt: %d\n}";

void *
GetMethod(const char *clazzName, const char *namespace, const char *methodName, const char *assembly, const int argsCnt)
{
    void *pDomain = mono_get_root_domain();

    if (pDomain == NULL)
    {
        log_error(getMethodLogMsgFormat, "GetMethod::GetRootDomain", clazzName, namespace, methodName, assembly, argsCnt);
        return NULL;
    }

    void *pAssembly = mono_domain_assembly_open(pDomain, assembly);
    if (pAssembly == NULL)
    {
        log_error(getMethodLogMsgFormat, "GetMethod::DomainAssemblyOpen", clazzName, namespace, methodName, assembly, argsCnt);
        return NULL;
    }

    void *pImage = mono_assembly_get_image(pAssembly);
    if (pImage == NULL)
    {
        log_error(getMethodLogMsgFormat, "GetMethod::AssemblyGetImage", clazzName, namespace, methodName, assembly, argsCnt);
        return NULL;
    }

    void *pClazz = mono_class_from_name(pImage, namespace, clazzName);
    if (pClazz == NULL)
    {
        log_error(getMethodLogMsgFormat, "GetMethod::GetClassFromName", clazzName, namespace, methodName, assembly, argsCnt);
        return NULL;
    }

    void *pMethod = mono_class_get_method_from_name(pClazz, methodName, argsCnt);
    if (pMethod == NULL)
    {
        log_error(getMethodLogMsgFormat, "GetMethod::GetMethodFromName", clazzName, namespace, methodName, assembly, argsCnt);
        return NULL;
    }

    void *pCompileMethod = mono_compile_method(pMethod);

    log_trace(
            "GetMethod::CompileMethod returned address: 0x%llx;\n{\n\tclazzName: %s\n\tnamespace: %s\n\tmethodName: %s\n\tassembly: %s\n\targsCnt: %d\n}",
            (unsigned long long) pCompileMethod,
            clazzName,
            namespace,
            methodName,
            assembly,
            argsCnt
    );
    return pCompileMethod;
}

const char *getFieldLogMsgFormat = "%s returned NULL;\n{\n\tclazzName: %s\n\tfieldName: %s\n\tnamespace: %s\n\tassembly: %s\n\tobject address: 0x%llx\n\tfield size: %d\n}";

void *
GetFieldValue(const char *clazzName,
              const char *fieldName,
              const char *namespace,
              const char *assembly,
              const void *object,
              unsigned int fieldSize)
{

    void *pDomain = mono_get_root_domain();
    if (pDomain == NULL)
    {
        log_error(
                getFieldLogMsgFormat,
                "GetFieldValue::GetRootDomain",
                clazzName,
                fieldSize,
                namespace,
                assembly,
                (unsigned long long) object,
                fieldSize
        );
        return NULL;
    }

    void *pAssembly = mono_domain_assembly_open(pDomain, assembly);
    if (pAssembly == NULL)
    {
        log_error(
                getFieldLogMsgFormat,
                "GetFieldValue::DomainAssemblyOpen",
                clazzName,
                fieldSize,
                namespace,
                assembly,
                (unsigned long long) object,
                fieldSize
        );
        return NULL;
    }

    void *pImage = mono_assembly_get_image(pAssembly);
    if (pImage == NULL)
    {
        log_error(
                getFieldLogMsgFormat,
                "GetFieldValue::AssemblyGetImage",
                clazzName,
                fieldSize,
                namespace,
                assembly,
                (unsigned long long) object,
                fieldSize
        );
        return NULL;
    }

    void *pClazz = mono_class_from_name(pImage, namespace, clazzName);
    if (pClazz == NULL)
    {
        log_error(
                getFieldLogMsgFormat,
                "GetFieldValue::GetClassFromName",
                clazzName,
                fieldSize,
                namespace,
                assembly,
                (unsigned long long) object,
                fieldSize
        );
        return NULL;
    }

    void *pField = mono_class_get_field_from_name(pClazz, fieldName);
    if (pField == NULL)
    {
        log_error(
                getFieldLogMsgFormat,
                "GetFieldValue::GetFieldFromName",
                clazzName,
                fieldSize,
                namespace,
                assembly,
                (unsigned long long) object,
                fieldSize
        );
        return NULL;
    }

    void *fieldValue = malloc(fieldSize);
    mono_field_get_value(object, pField, fieldValue);

    log_trace(
            "GetFieldValue::GetValue returned address: 0x%llx\n{\n\tclazzName: %s\n\tfieldName: %s\n\tnamespace: %s\n\tassembly: %s\n\tobject address: 0x%llx\n\tfield size: %d\n}",
            (unsigned long long) fieldValue,
            clazzName,
            fieldSize,
            namespace,
            assembly,
            (unsigned long long) object,
            fieldSize
    );

    return fieldValue;
}

const char *getStaticFieldLogMsgFormat = "%s returned NULL;\n{\n\tclazzName: %s\n\tfieldName: %s\n\tnamespace: %s\n\tassembly: %s\n\tfield size: %d\n}";

void *
GetStaticFieldValue(const char *clazzName, const char *fieldName, const char *namespace, const char *assembly, unsigned int fieldSize)
{
    void *pDomain = mono_get_root_domain();
    if (pDomain == NULL)
    {
        log_error(
                getStaticFieldLogMsgFormat, "GetStaticFieldValue::GetRootDomain", clazzName, fieldName, namespace, assembly, fieldSize
        );
        return NULL;
    }

    void *pAssembly = mono_domain_assembly_open(pDomain, assembly);
    if (pAssembly == NULL)
    {
        log_error(
                getStaticFieldLogMsgFormat, "GetStaticFieldValue::DomainAssemblyOpen", clazzName, fieldName, namespace, assembly, fieldSize
        );
        return NULL;
    }

    void *pImage = mono_assembly_get_image(pAssembly);
    if (pImage == NULL)
    {
        log_error(
                getStaticFieldLogMsgFormat, "GetStaticFieldValue::AssemblyGetImage", clazzName, fieldName, namespace, assembly, fieldSize
        );
        return NULL;
    }

    void *pClazz = mono_class_from_name(pImage, namespace, clazzName);
    if (pClazz == NULL)
    {
        log_error(
                getStaticFieldLogMsgFormat, "GetStaticFieldValue::ClassFromName", clazzName, fieldName, namespace, assembly, fieldSize
        );
        return NULL;
    }


    void *pField = mono_class_get_field_from_name(pClazz, fieldName);
    if (pField == NULL)
    {
        log_error(
                getStaticFieldLogMsgFormat,
                "GetStaticFieldValue::ClassGetFieldFromName",
                clazzName,
                fieldName,
                namespace,
                assembly,
                fieldSize
        );
        return NULL;
    }

    void *pVTable = mono_class_vtable(pDomain, pClazz);
    if (pVTable == NULL)
    {
        log_error(
                getStaticFieldLogMsgFormat, "GetStaticFieldValue::GetClassVTable", clazzName, fieldName, namespace, assembly, fieldSize
        );
        return NULL;
    }

    log_trace(
            "Invoking mono_runtime_class_init for:\n{\n\tvtable address: 0x%llx\n\tclassName: %s\n\tfieldName: %s\n\tnamespace: %s\n\tassembly: %s\n\tfieldSize: %d\n}",
            (unsigned long long) pVTable,
            clazzName,
            fieldName,
            namespace,
            assembly,
            fieldSize
    );
    mono_runtime_class_init(pVTable);
    log_trace(
            "Invoked mono_runtime_class_init for:\n{\n\tvtable address: 0x%llx\n\tclassName: %s\n\tfieldName: %s\n\tnamespace: %s\n\tassembly: %s\n\tfieldSize: %d\n}",
            (unsigned long long) pVTable,
            clazzName,
            fieldName,
            namespace,
            assembly,
            fieldSize
    );

    void *fieldValue = malloc(fieldSize);
    mono_field_static_get_value(pVTable, pField, fieldValue);

    log_trace(
            "GetStaticFieldValue::GetStaticFieldValue returned address: 0x%llx;\n{\n\tclazzName: %s\n\tfieldName: %s\n\tnamespace: %s\n\tassembly: %s\n\tfield size: %d\n}",
            (unsigned long long) fieldValue,
            clazzName,
            fieldName,
            namespace,
            assembly,
            fieldSize
    );

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

void *
GetProperty(void *clazz, const char *name)
{
    monoProperty *property = mono_class_get_property_from_name(clazz, name);
    if (property == NULL) {
        log_error("mono_class_get_property_from_name returned NULL for\n{\n\tclazz address: 0x%llx\n\tname: %s\n}");
        return NULL;
    }

    return property;
}

DWORD __stdcall
EjectThread(LPVOID lpParameter)
{
    Sleep(100);
    FreeLibraryAndExitThread(pHinstance, 0);
}

void
shutdown0(FILE *fpConsole, FILE *fpLog, const char *reason, const char *status)
{
    log_info("Reason: %s;\nStatus%s\n", reason, status);
    Sleep(5000);
    if (fpConsole != NULL)
    {
        fclose(fpConsole);
    }
    if (fpLog != NULL)
    {
        fclose(fpLog);
    }
    FreeConsole();
    CreateThread(0, 0, EjectThread, 0, 0, 0);
}

DWORD WINAPI
Hook(LPVOID lpParam)
{
    FreeConsole();
    AllocConsole();

    FILE *fpConsole;
    FILE *fpLog = NULL;
    freopen_s(&fpConsole, "CONOUT$", "w", stdout);
//    freopen_s(&fpLog, "D:\\IT\\C\\NotSoLethalCompany\\bin\\NotSoLethalCompany.log", "wa", stdout);

    setLogLevel(TRACE_LEVEL);

    InitializeMonoFuncs();

    MH_STATUS status = MH_Initialize();
    const char *statusAsString;

    if (status != MH_OK)
    {
        statusAsString = MH_StatusToString(status);
        shutdown0(fpConsole, fpLog, "MinHook init failed!", statusAsString);
        return 0;
    }

    pUpdateA = GetMethod("EnemyAI", "", "Update", GAME_ASSEMBLY, -1);
    status = MH_CreateHook(pUpdateA, detourUpdate, (LPVOID *) &pUpdateATarget);

    if (status != MH_OK)
    {
        statusAsString = MH_StatusToString(status);
        shutdown0(fpConsole, fpLog, "MinHook create failed!", statusAsString);
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
                shutdown0(fpConsole, fpLog, "MinHook enable failed!", statusAsString);
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
                shutdown0(fpConsole, fpLog, "MinHook disable failed!", statusAsString);
                return 1;
            }
        }

    }

    shutdown0(fpConsole, fpLog, "Test unhook", statusAsString);

    return 1;
}