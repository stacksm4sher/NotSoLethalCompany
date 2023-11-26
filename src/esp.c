#include "esp.h"

int cnt = 0;

// todo finish with rendering
void
renderEsp(void *entityThis, vec3 *entityV3, void **scrapThisArray, vec3 **scrapV3Array, unsigned int scrapV3ArraySize)
{
    if (cnt % 512 == 0)
    {
        printf("Entity: { %llu }; Position: { x: %f, y: %f, z: %f }\n", (unsigned long long) entityThis, entityV3->x, entityV3->y,
               entityV3->z);
    }

    cnt++;
}

void
detourUpdate(void *this)
{
    vec3 *serverPosition = GetFieldValue("EnemyAI", "serverPosition", "", GAME_ASSEMBLY, this);

//    renderEsp(this, serverPosition, objectArray, scrapVec3Array, arraySize);
    renderEsp(this, serverPosition, NULL, NULL, 0);

    free(serverPosition);

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
