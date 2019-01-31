/*
 * @file		algorithm.c
 * @brief 	calculation of position based on three encoder system
 * @author	Ryusuke SUGIMOTO (rsugimoto@ust.hk)
 * @date		December 27, 2017
 */

#include "algorithm.h"

s16 real_X = 0;
s16 real_Y = 0;
s16 real_A = 0;

static double CONST_MATRIX[3][3];

/*
 * @brief 	calculate the constants needed
 * @param		none
 * @return	none
 */
void calculation_init(){
	double mat[3][3];
	double k;
	int i,j;
	
	mat[0][0] = ENC_FLIP1*ENC_RES1*cos(M_PI*ENC_ANGLE1/180)/(2*M_PI*ENC_RADIUS1);
	mat[0][1] = ENC_FLIP1*ENC_RES1*sin(M_PI*ENC_ANGLE1/180)/(2*M_PI*ENC_RADIUS1);
	mat[0][2] = -ENC_FLIP1*ENC_RES1*sin(M_PI*ENC_ANGLE_CENTER1/180)*ENC_DISTANCE1/(2*M_PI*ENC_RADIUS1);
	
	mat[1][0] = ENC_FLIP2*ENC_RES2*cos(M_PI*ENC_ANGLE2/180)/(2*M_PI*ENC_RADIUS2);
	mat[1][1] = ENC_FLIP2*ENC_RES2*sin(M_PI*ENC_ANGLE2/180)/(2*M_PI*ENC_RADIUS2);
	mat[1][2] = -ENC_FLIP2*ENC_RES2*sin(M_PI*ENC_ANGLE_CENTER2/180)*ENC_DISTANCE2/(2*M_PI*ENC_RADIUS2);
	
	mat[2][0] = ENC_FLIP3*ENC_RES3*cos(M_PI*ENC_ANGLE3/180)/(2*M_PI*ENC_RADIUS3);
	mat[2][1] = ENC_FLIP3*ENC_RES3*sin(M_PI*ENC_ANGLE3/180)/(2*M_PI*ENC_RADIUS3);
	mat[2][2] = -ENC_FLIP3*ENC_RES3*sin(M_PI*ENC_ANGLE_CENTER3/180)*ENC_DISTANCE3/(2*M_PI*ENC_RADIUS3);
	
	k =   mat[0][0]*mat[1][1]*mat[2][2]+mat[1][0]*mat[2][1]*mat[0][2]+mat[2][0]*mat[0][1]*mat[1][2]
			 -mat[0][0]*mat[2][1]*mat[1][2]-mat[2][0]*mat[1][1]*mat[0][2]-mat[1][0]*mat[0][1]*mat[2][2];

	for(i = 0; i<3; i++)
			for(j = 0; j<3; j++)
					CONST_MATRIX[i][j] = (mat[(j+1)%3][(i+1)%3]*mat[(j+2)%3][(i+2)%3]-mat[(j+1)%3][(i+2)%3]*mat[(j+2)%3][(i+1)%3])/k;
}

/*
 * @brief 	calculate the position
 * @param		none
 * @return	none
 */
void calculation(){
	
	static double curr_A = 0;
	static double curr_X = 0;
	static double curr_Y = 0;
	
	double cos_angle, sin_angle;
	double mid_A = curr_A;
	
	static s32 prev_enc1;
	static s32 prev_enc2;
	static s32 prev_enc3;
	
	s32 curr_enc1 = gun_can_get_encoder(MOTOR_1);
	s32 curr_enc2 = gun_can_get_encoder(MOTOR_2);
	s32 curr_enc3 = gun_can_get_encoder(MOTOR_3);
	
	//store the difference of encoder counts
	s16 enc1 = curr_enc1 - prev_enc1;
	s16 enc2 = curr_enc2 - prev_enc2;
	s16 enc3 = curr_enc3 - prev_enc3;
	
	prev_enc1 = curr_enc1;
	prev_enc2 = curr_enc2;
	prev_enc3 = curr_enc3;
	
	/*======================================Angle Calculation Start======================================*/
	
			curr_A = CONST_MATRIX[2][0] * curr_enc1 + CONST_MATRIX[2][1] * curr_enc2 + CONST_MATRIX[2][2] * curr_enc3;
			s32 temp_real_A = (s32)(curr_A*1800/M_PI);
			temp_real_A %= 3600;
			if(temp_real_A < 0 ) temp_real_A += 3600;
			real_A = (s16)temp_real_A;
			
	/*======================================Angle Calculation End======================================*/
	
	
	/*======================================XY Calculation Start======================================*/
			
			//map the encoder differences to each axes.
			//Unit of local_dx and local_dy is mm
			double local_dx = CONST_MATRIX[0][0] * enc1 + CONST_MATRIX[0][1] * enc2 + CONST_MATRIX[0][2] * enc3;
			double local_dy = CONST_MATRIX[1][0] * enc1 + CONST_MATRIX[1][1] * enc2 + CONST_MATRIX[1][2] * enc3;
			
			//mid_A is the angle we use to integrate dx or dy.
			mid_A = (mid_A + curr_A)/2;
			cos_angle = cos(mid_A);
			sin_angle = sin(mid_A);
			
			//map local_dx and local_dy to global system and integrate them.
			curr_X += local_dx*cos_angle - local_dy*sin_angle;
			curr_Y += local_dx*sin_angle + local_dy*cos_angle;
			
			//real_X and real_Y are the values we pass to the mainboard
			//SHIFT_X and SHIFT_Y calculation is done here.
			real_X = (s16)curr_X - SHIFT_X + (SHIFT_X*int_cos(real_A)+SHIFT_Y*int_sin(real_A))/10000;
			real_Y = (s16)curr_Y - SHIFT_Y + (SHIFT_Y*int_cos(real_A)-SHIFT_X*int_sin(real_A))/10000;
			
	/*======================================XY Calculation End======================================*/
	
}
