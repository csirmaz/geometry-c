
/* Simple memory-efficient vector functions and STL export helpers */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define T_DATA double
#define _ABS(x) fabs(x)
#define EPSILON 1e-7

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

int vapprox_zero(vector* v) {
    return (_ABS(v->x) < EPSILON && _ABS(v->y) < EPSILON && _ABS(v->z) < EPSILON);
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

void vcross(vector* result, vector* a, vector* b) {
    result->x = a->y * b->z - a->z * b->y;
    result->y = a->z * b->x - a->x * b->z;
    result->z = a->x * b->y - a->y * b->x;
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

FILE* stl_open(char* filename) {
    FILE* fptr;
    
    fptr = fopen(filename, "w");
    if(fptr == NULL) {
        printf("Cannot open file %s\n", filename);
        return NULL;
    }
    fprintf(fptr, "solid geomc\n");
    return fptr;
}

void stl_write_vector(FILE* fptr, vector* v) {
    fprintf(fptr, "%lf %lf %lf", v->x, v->y, v->z);
}

void stl_write_triangle(FILE* fptr, vector* p1, vector* p2, vector* p3) {
    vector p21, p31, norm;
    
    vcopy(&p21, p2);
    vsub(&p21, p1);
    vcopy(&p31, p3);
    vsub(&p31, p1);
    vcross(&norm, &p21, &p31);
    if(vapprox_zero(&norm)) {
        // degenerate triange
        return;
    }
    vnorm(&norm);
    fprintf(fptr, "facet normal "); stl_write_vector(fptr, &norm);
    fprintf(fptr, "\nouter loop");
    fprintf(fptr, "\nvertex "); stl_write_vector(fptr, p1);
    fprintf(fptr, "\nvertex "); stl_write_vector(fptr, p2);
    fprintf(fptr, "\nvertex "); stl_write_vector(fptr, p3);
    fprintf(fptr, "\nendloop\nendfacet\n");    
}

void stl_write_tetragon(FILE* fptr, vector* p1, vector* p2, vector* p3, vector* p4) {
    vector d1, d2;
    
    // Decide which diagonal is best to break on
    vcopy(&d1, p1);
    vsub(&d1, p3);
    vcopy(&d2, p2);
    vsub(&d2, p4);
    if(vlen(&d1) < vlen(&d2)) {
        stl_write_triangle(fptr, p1, p2, p3);
        stl_write_triangle(fptr, p1, p3, p4);
    }
    else {
        stl_write_triangle(fptr, p1, p2, p4);
        stl_write_triangle(fptr, p2, p3, p4);
    }
}

void stl_close(FILE* fptr) {
    fprintf(fptr, "endsolid geomc\n");
    fclose(fptr);
}

// Unit tests

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
    assert(_ABS(vlen(&vz) - 1.) < EPSILON); // #12
}
