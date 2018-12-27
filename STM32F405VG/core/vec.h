#ifndef __VEC_H
#define __VEC_H

#include "stm32f4xx.h"
typedef struct{
	s32 n[3];
}vec3;

typedef struct{
	s32 n[9];
}mat3;
// [mat[0] mat[1] mat[2];mat[3] mat[4] mat[5];mat[6] mat[7] mat[8]];

s32 det(s32 a , s32 b, s32 c,s32 d);


vec3 cross_prod(vec3 first,vec3 second);

s32 dot_prod(vec3 first,vec3 second);

vec3 vec_add(vec3 a,vec3 b);

vec3 vec_sub(vec3 a,vec3 b);

s32 vec_length(vec3 a);

s32 vec_length2(vec3 a);

mat3 mat_prod(mat3 a, mat3 b);

s32 mat_det(mat3 a);

mat3 transpose(mat3 a);

vec3 mat_vec_prod(mat3 a,vec3 b);

void sca_vec_mult(s32 a,vec3* b);

void sca_vec_div(s32 a,vec3* b);

void sca_mat_prod(s32 a,mat3* b);

void sca_mat_div(s32 a,mat3* b);

mat3 inverse(const mat3 a);

u8 in_triangle (vec3 v1,vec3 v2 ,vec3 v0 , vec3 v);

void assign_col(mat3* m, vec3 v, u8 index);

u8 in_rectangle(vec3 v,vec3* x);

#endif
