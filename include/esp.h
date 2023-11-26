#ifndef NOTSOLETHALCOMPANY_ESP_H
#define NOTSOLETHALCOMPANY_ESP_H

#include <windows.h>
#include <stdio.h>

#include "util.h"

typedef void(*UpdateA)(void *this);

static UpdateA pUpdateA = NULL;
static UpdateA pUpdateATarget;

struct Vector3 {
    float x;
    float y;
    float z;
};

typedef struct Vector3 vec3;

void detourUpdate(void *this);

#endif //NOTSOLETHALCOMPANY_ESP_H
