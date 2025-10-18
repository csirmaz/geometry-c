
/* Simple memory-efficient vector functions */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define T_DATA double

typedef struct {
    T_DATA x;
    T_DATA y;
    T_DATA z;
} vector;

typedef struct {
    char x;
    char y;
    char z;
} boolvec;

void vzero(vector target) {
    target.x = 0.;
    target.y = 0.;
    target.z = 0.;
}

void bvzero(boolvec target) {
    target.x = 0;
    target.y = 0;
    target.z = 0;
}

void vcopy(vector target, vector other) {
    target.x = other.x;
    target.y = other.y;
    target.z = other.z;
}

void bvcopy(boolvec target, boolvec other) {
    target.x = other.x;
    target.y = other.y;
    target.z = other.z;
}

void vscale(vector target, T_DATA s) {
    target.x *= s;
    target.y *= s;
    target.z *= s;
}

void vadd(vector target, vector other) {
    target.x += other.x;
    target.y += other.y;
    target.z += other.z;
}

void vsub(vector target, vector other) {
    target.x -= other.x;
    target.y -= other.y;
    target.z -= other.z;
}

T_DATA vlen(vector v) {
    return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

void vnorm(vector target) {
    T_DATA len = vlen(target);
    vscale(target, 1. / len);
}


vector*** alloc_3d_vector_array(int sizex, int sizey, int sizez) {
    int j, k;
    vector*** arr = (vector***)malloc(sizez * sizeof(vector**));
    if(arr == NULL) { return NULL; }
    for (k = 0; k < sizez; k++) {
        arr[k] = (vector**)malloc(sizey * sizeof(vector*));
        if(arr[k] == NULL) { return NULL; }
        for (j = 0; j < sizey; j++) {
            arr[k][j] = (vector*)malloc(sizex * sizeof(vector));
            if(arr[k][j] == NULL) { return NULL; }
        }
    }
    return arr;
}

boolvec*** alloc_3d_boolvec_array(int sizex, int sizey, int sizez) {
    int j, k;
    boolvec*** arr = (boolvec***)malloc(sizez * sizeof(boolvec**));
    if(arr == NULL) { return NULL; }
    for (k = 0; k < sizez; k++) {
        arr[k] = (boolvec**)malloc(sizey * sizeof(boolvec*));
        if(arr[k] == NULL) { return NULL; }
        for (j = 0; j < sizey; j++) {
            arr[k][j] = (boolvec*)malloc(sizex * sizeof(boolvec));
            if(arr[k][j] == NULL) { return NULL; }
        }
    }
    return arr;
}

