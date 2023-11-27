#include "esp.h"
#include <logger.h>

int pCnt = 0;

// todo finish with rendering
void
renderEsp(void *entityThis, vec3 *pEntityV3, vec3 *pLocalPlayerV3)
{
    if (pCnt % 512 == 0)
    {

        log_trace(
                "Entity address: { 0x%llx }; Position: { x: %f, y: %f, z: %f } Position struct address: { 0x%llx }\n",
                (unsigned long long) entityThis,
                pEntityV3->x,
                pEntityV3->y,
                pEntityV3->z,
                (unsigned long long) pEntityV3
        );

        log_trace(
                "Player: { 0x%llx }; Position: { x: %f, y: %f, z: %f } Position struct address: { 0x%llx }\n",
                (unsigned long long) entityThis,
                pLocalPlayerV3->x,
                pLocalPlayerV3->y,
                pLocalPlayerV3->z,
                (unsigned long long) pLocalPlayerV3
        );

    }

//    printf("log2\n");
    pCnt++;
//    printf("log3\n");
}

//void *parentClazz;
//const char *name;
//void *getMethod;
//void *setMethod;
//unsigned int attrs;

void
detourUpdate(void *this)
{
    void *clazz = GetClazz("HUDManager", "", GAME_ASSEMBLY);
    monoProperty *property = GetProperty(clazz, "Instance");

//    log_trace("Property address: 0x%llx", (unsigned long long) property);

    log_trace(
            "Property address: 0x%llx\nStructure:\n{\n\tparenClazz address: 0x%llx\n\tname: %s\n\tgetter address: 0x%llx\n\tsetter address: 0x%llx\n\tattrs: %d\n}",
            (unsigned long long) property,
            (unsigned long long) property->parentClazz,
            property->name,
            (unsigned long long) property->getMethod,
            (unsigned long long) property->setMethod,
            property->attrs
    );
    void *hudManagerInstance = GetStaticFieldValue("HUDManager", "k__BackingField;", "", GAME_ASSEMBLY, 8);

    if (hudManagerInstance == NULL) {
        log_error("Skipping detourUpdate for EnemyAI after received NULL from GetStaticFieldValue for HudManager#Instance");
        pUpdateATarget(this);
        return;
    }

    void *localPlayer = GetFieldValue("HUDManager", "localPlayer", "", GAME_ASSEMBLY, hudManagerInstance, 8);

    if (localPlayer == NULL) {
        log_error("Skipping detourUpdate for EnemyAI after received NULL from GetFieldValue for HudManager.localPlayer");
        pUpdateATarget(this);
        return;
    }

    vec3 *localPlayerServerPosition = GetFieldValue(
            "PlayerControllerB", "serverPlayerPosition", "", GAME_ASSEMBLY, localPlayer, sizeof(struct Vector3));

    if (localPlayerServerPosition == NULL) {
        log_error("Skipping detourUpdate for EnemyAI after received NULL from GetFieldValue for PlayerControllerB.serverPlayerPosition");
        pUpdateATarget(this);
        return;
    }

    vec3 *enemyServerPosition = GetFieldValue("EnemyAI", "enemyServerPosition", "", GAME_ASSEMBLY, this, sizeof(struct Vector3));

    if (enemyServerPosition == NULL) {
        log_error("Skipping detourUpdate for EnemyAI after received NULL from GetFieldValue for EnemyAI enemyServerPosition");
        pUpdateATarget(this);
        return;
    }

//    renderEsp(this, enemyServerPosition, objectArray, scrapVec3Array, arraySize);
//    renderEsp(this, enemyServerPosition, NULL, NULL, 0);
    renderEsp(this, enemyServerPosition, localPlayerServerPosition);

    free(hudManagerInstance);
    free(localPlayer);
    free(localPlayerServerPosition);
    free(enemyServerPosition);

    pUpdateATarget(this);
}

// todo: remove or use

//        for (unsigned int i = 0; i < scrapV3ArraySize; ++i) {
//
//            printf("log6 + iter: %d\n", i);
//            vec3 *scrapV3 = *(scrapV3Array + i);
//            printf("log7 + iter: %d\n", i);
//            void *scrapThis = *(scrapThisArray + i);
//            printf("log8 + iter: %d\n", i);
//            printf(
//                    "Scrap: { %llu }; Position: { x: %f, y: %f, z: %f }\n",
//                    (unsigned long long) scrapThis,
//                    scrapV3->x,
//                    scrapV3->y,
//                    scrapV3->z
//            );
//        }

//    void *(*FindObjectsOfType)(void *type, BOOL includeInactive) =
//        GetMethod("Object", "UnityEngine", "FindObjectsOfType", UNITY_ENGINE, 2);
//    void *(*GetType)(void *this) =
//        GetMethod("Object", "System", "GetType", MSCORLIB, -1);
//    void *randomScrapSpawnClazz = GetClazz("RandomScrapSpawn", "", GAME_ASSEMBLY);
//    void *randomScrapSpawnType = GetType(randomScrapSpawnClazz);
//
//    void *objectArray = FindObjectsOfType(randomScrapSpawnType, FALSE);
//
////    printf("log1\n");
//    void *arraySize0 = mono_array_length(objectArray);
//
////    printf("log1_1\n");
//    unsigned long long arraySize = (unsigned long long) ((unsigned int *) arraySize0);
//
////    printf("log2\n");
//    vec3 **scrapVec3Array = malloc(arraySize * 8);
//    for (unsigned int i = 0; i < arraySize; ++i) {
//        *(scrapVec3Array + i) = malloc(8);
//    }
//
//    for (unsigned int i = 0; i < arraySize; ++i) {
//
////        printf("log3 + iter: %d\n", i);
//        void *randomScrapV = *((void **)objectArray + i);
//
//        if (randomScrapV == NULL) {
//            printf("PANIC3\n");
//        }
//
//        printf("addr: %llu\n", (unsigned long long) randomScrapV);
//
//        void *scrapTransform = GetFieldValue(
//                "Component",
//                "transform",
//                "UnityEngine",
//                UNITY_ENGINE,
//                randomScrapV
//        );
//
//        if (scrapTransform == NULL) {
//            printf("PANIC1\n");
//        }
//
////        printf("log4 + iter: %d\n", i);
//        vec3 *scrapPosition = GetFieldValue(
//                "Transform",
//                "position",
//                "UnityEngine",
//                UNITY_ENGINE,
//                scrapTransform
//        );
//        if (scrapPosition == NULL) {
//            printf("PANIC2\n");
//        }
//
////        printf("log5 + iter: %d\n", i);
//        *(scrapVec3Array + i) = scrapPosition;
//    }
