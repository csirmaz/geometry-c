
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

void vprint(vector* v) {
    printf("x=%lf y=%lf z=%lf\n", v->x, v->y, v->z);
}

void vzero(vector* target) {
    target->x = 0.;
    target->y = 0.;
    target->z = 0.;
}

void bvzero(boolvec* target) {
    target->x = 0;
    target->y = 0;
    target->z = 0;
}

void vcopy(vector* target, vector* other) {
    target->x = other->x;
    target->y = other->y;
    target->z = other->z;
}

void bvcopy(boolvec* target, boolvec* other) {
    target->x = other->x;
    target->y = other->y;
    target->z = other->z;
}

int veq(vector* a, vector* b) {
    return (a->x == b->x && a->y == b->y && a->z == b->z);
}

void vscale(vector* target, T_DATA s) {
    target->x *= s;
    target->y *= s;
    target->z *= s;
}

void vadd(vector* target, vector* other) {
    target->x += other->x;
    target->y += other->y;
    target->z += other->z;
}

void vsub(vector* target, vector* other) {
    target->x -= other->x;
    target->y -= other->y;
    target->z -= other->z;
}

T_DATA vlen(vector* v) {
    return sqrt(v->x*v->x + v->y*v->y + v->z*v->z);
}

void vnorm(vector* target) {
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

char*** alloc_3d_bool_array(int sizex, int sizey, int sizez) {
    int j, k;
    char*** arr = (char***)malloc(sizez * sizeof(char**));
    if(arr == NULL) { return NULL; }
    for (k = 0; k < sizez; k++) {
        arr[k] = (char**)malloc(sizey * sizeof(char*));
        if(arr[k] == NULL) { return NULL; }
        for (j = 0; j < sizey; j++) {
            arr[k][j] = (char*)malloc(sizex * sizeof(char));
            if(arr[k][j] == NULL) { return NULL; }
        }
    }
    return arr;
}


int test_no = 0;

void assert(int b) {
    test_no++;
    if(!b) { printf("Test %d failed\n", test_no); exit(1); }
}

void geometry_test(void) {
    vector vx, vy, vz;
    vzero(&vx);
    vzero(&vy);
    vzero(&vz);
    assert(veq(&vx, &vy)); // #1

    vx.x = 1.; // vx=(1,0,0)
    assert(!veq(&vx, &vy)); // #2
    assert(vlen(&vx) == 1.); // #3
    assert(vlen(&vy) == 0.); // #4

    vy.x = 1.; // vy=(1,0,0)
    assert(veq(&vx, &vy)); // #5

    vz.x = 2.; // vz=(2,0,0)
    vadd(&vx, &vy); // vx=(2,0,0)
    assert(veq(&vx, &vz)); // #6
    vscale(&vz, .5); // vz=(1,0,0)
    assert(veq(&vz, &vy)); // #7
    vsub(&vx, &vy); // vx=(1,0,0)
    assert(vlen(&vx) == 1.); // #8
    
    vx.y = 1.; // vx=(1,1,0)
    vcopy(&vz, &vx); // vz=(1,1,0)
    assert(vz.y == 1.); // #9
    assert(vlen(&vz) > 1.4); // #10
    assert(vlen(&vz) < 1.5); // #11
    vnorm(&vz);
    assert(fabs(vlen(&vz) - 1.) < .00001); // #12
}
