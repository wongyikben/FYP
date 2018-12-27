#include "../core/vec.h"
#include "macro_math.h"

s32 det(s32 a , s32 b, s32 c,s32 d){
	return a*d-b*c;
}


vec3 cross_prod(vec3 first,vec3 second){
	vec3 result;
	result.n[0] = det(first.n[1],first.n[2],second.n[1],second.n[2]);
	result.n[1] = -det(first.n[0],first.n[2],second.n[0],second.n[2]);
	result.n[2] = det(first.n[0],first.n[1],second.n[0],second.n[1]);
	return result;
}

s32 dot_prod(vec3 first,vec3 second){
	s32 result = first.n[0]*second.n[0]+first.n[1]*second.n[1]+first.n[2]*second.n[2];
	return result;
}

vec3 vec_add(vec3 a,vec3 b){
	vec3 result;
	result.n[0]=a.n[0]+b.n[0];
	result.n[1]=a.n[1]+b.n[1];
	result.n[2]=a.n[2]+b.n[2];
	return result;
}

vec3 vec_sub(vec3 a,vec3 b){
	vec3 result;
	result.n[0]=a.n[0]-b.n[0];
	result.n[1]=a.n[1]-b.n[1];
	result.n[2]=a.n[2]-b.n[2];
	return result;
}

s32 vec_length(vec3 a){ //scaled by 128
			return Sqrt(a.n[0]*a.n[0]+a.n[1]*a.n[1]);
}

s32 vec_length2(vec3 a){ 
			return (a.n[0]*a.n[0]+a.n[1]*a.n[1]);
}

mat3 mat_prod(mat3 a, mat3 b){
	mat3 result;
	result.n[0] = a.n[0]*b.n[0]+a.n[1]*b.n[3]+a.n[2]*b.n[6];
	result.n[1] = a.n[0]*b.n[1]+a.n[1]*b.n[4]+a.n[2]*b.n[7];
	result.n[2] = a.n[0]*b.n[2]+a.n[1]*b.n[5]+a.n[2]*b.n[8];
	result.n[3] = a.n[3]*b.n[0]+a.n[4]*b.n[3]+a.n[5]*b.n[6];
	result.n[4] = a.n[3]*b.n[1]+a.n[4]*b.n[4]+a.n[5]*b.n[7];
	result.n[5] = a.n[3]*b.n[2]+a.n[4]*b.n[5]+a.n[5]*b.n[8];
	result.n[6] = a.n[6]*b.n[0]+a.n[7]*b.n[3]+a.n[8]*b.n[6];
	result.n[7] = a.n[6]*b.n[1]+a.n[7]*b.n[4]+a.n[8]*b.n[7];
	result.n[8] = a.n[6]*b.n[2]+a.n[7]*b.n[5]+a.n[8]*b.n[8];
	return result;
}

s32 mat_det(mat3 a){
	return a.n[0] * det(a.n[4], a.n[5], a.n[7], a.n[8]) - a.n[1] * det(a.n[3], a.n[5], a.n[6], a.n[8]) 
				+ a.n[2] * det(a.n[3], a.n[4], a.n[6], a.n[7]);
}

mat3 transpose(mat3 a){
	mat3 result;
	result.n[0] = a.n[0];
	result.n[1] = a.n[3];
	result.n[2] = a.n[6];
	result.n[3] = a.n[1];
	result.n[4] = a.n[4];
	result.n[5] = a.n[7];
	result.n[6] = a.n[2];
	result.n[7] = a.n[5];
	result.n[8] = a.n[8];
	return result;
}

vec3 mat_vec_prod(mat3 a,vec3 b){
	vec3 result;
	result.n[0] = a.n[0]*b.n[0]+a.n[1]*b.n[1]+a.n[2]*b.n[2];
	result.n[1] = a.n[3]*b.n[0]+a.n[4]*b.n[1]+a.n[5]*b.n[2];
	result.n[2] = a.n[6]*b.n[0]+a.n[7]*b.n[1]+a.n[8]*b.n[2];
	return result;
}

void sca_vec_mult(s32 a,vec3* b){
	(*b).n[0]*=a;
	(*b).n[1]*=a;
	(*b).n[2]*=a;
}

void sca_vec_div(s32 a,vec3* b){
	(*b).n[0]/=a;
	(*b).n[1]/=a;
	(*b).n[2]/=a;
}

void sca_mat_prod(s32 a,mat3* b){
	(*b).n[0]*=a;
	(*b).n[1]*=a;
	(*b).n[2]*=a;
	(*b).n[3]*=a;
	(*b).n[4]*=a;
	(*b).n[5]*=a;
	(*b).n[6]*=a;
	(*b).n[7]*=a;
	(*b).n[8]*=a;
}

void sca_mat_div(s32 a,mat3* b){
	(*b).n[0]/=a;
	(*b).n[1]/=a;
	(*b).n[2]/=a;
	(*b).n[3]/=a;
	(*b).n[4]/=a;
	(*b).n[5]/=a;
	(*b).n[6]/=a;
	(*b).n[7]/=a;
	(*b).n[8]/=a;
}

mat3 inverse(const mat3 a){ // multipled by 128
	s32 detA = mat_det(a);
	if (detA == 0) {return a;}
		
		mat3 result;
		
		result.n[0] = det(a.n[4], a.n[5], a.n[7], a.n[8]);
		result.n[1] =-det(a.n[3], a.n[5], a.n[6], a.n[8]);
		result.n[2] = det(a.n[3], a.n[4], a.n[6], a.n[7]);
		result.n[3] =-det(a.n[1], a.n[2], a.n[7], a.n[8]);
		result.n[4] = det(a.n[0], a.n[2], a.n[6], a.n[8]);
		result.n[5] =-det(a.n[0], a.n[1], a.n[6], a.n[7]);
		result.n[6] = det(a.n[1], a.n[2], a.n[4], a.n[5]);
		result.n[7] =-det(a.n[0], a.n[2], a.n[3], a.n[5]);
		result.n[8] = det(a.n[0], a.n[1], a.n[3], a.n[4]);
		sca_mat_prod(4096,&result);
		sca_mat_div(detA,&result);
		result = transpose(result);
		return result;
}



// v is the point user want to check whether v inside v0,v1,v2

u8 in_triangle (vec3 v1,vec3 v2 ,vec3 v0 , vec3 v){
	v1 = vec_sub(v1,v0);
	v2 = vec_sub(v2,v0);
	
	s16 a= (det(v.n[0],v2.n[0],v.n[1],v2.n[1])-det(v0.n[0],v2.n[0],v0.n[1],v2.n[1]))<<7;
	s16 b= (det(v.n[0],v1.n[0],v.n[1],v1.n[1])-det(v0.n[0],v1.n[0],v0.n[1],v1.n[1]))<<7;
	if((a-b)<=(det(v1.n[0],v2.n[0],v1.n[1],v2.n[1])<<7)+1){return 1;}
	return 0;
}

void assign_col(mat3* m, vec3 v, u8 index){
	(*m).n[index+0] = v.n[0];
	(*m).n[index+3] = v.n[1];
	(*m).n[index+6] = v.n[2];
}


u8 in_rectangle(vec3 v,vec3* x){
	vec3 first;
	vec3 second;
	vec3 result;
	
	first = vec_sub(x[2],x[0]);
	second = vec_sub(v,x[0]);
	result = cross_prod(first,second);
	if(result.n[2]>=0){return 0;}
	
	first = vec_sub(x[0],x[1]);
	second = vec_sub(v,x[1]);
	result = cross_prod(first,second);
	if(result.n[2]>=0){return 0;}
	
	first = vec_sub(x[3],x[2]);
	second = vec_sub(v,x[2]);
	result = cross_prod(first,second);
	if(result.n[2]>=0){return 0;}
	
	first = vec_sub(x[1],x[3]);
	second = vec_sub(v,x[3]);
	result = cross_prod(first,second);
	if(result.n[2]>=0){return 0;}

	return 1;
}
