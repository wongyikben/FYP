/*
 * @file		define.h
 * @brief 	definition of constants for three encoder system
 * @author	Ryusuke SUGIMOTO (rsugimoto@ust.hk)
 * @date		January 6, 2018
 */

#ifndef __DEFINE_H
#define __DEFINE_H

//Shift of the center of positioning system from the center of the robot
#define SHIFT_X 0
#define SHIFT_Y 0

//Encoder count per spin
#define ENC_RES1 4096
#define ENC_RES2 4096
#define ENC_RES3 4096

#define ORIENTATION_SETTING 2

//The encoder value needs to be positive in clockwise direction. Otherwise, flip the sign. (1 or -1)
#define ENC_FLIP1 (-1)
#define ENC_FLIP2 (-1)
#define ENC_FLIP3 (-1)

//radiuses of the three encoders (unit: mm)

#define ENC_RADIUS1 53.0
#define ENC_RADIUS2 53.0
#define ENC_RADIUS3 53.0


//distances of the encoders from the center (unit: mm)
#define ENC_DISTANCE1 304.73
#define ENC_DISTANCE2 304.73
#define ENC_DISTANCE3 304.73

//Angles of the encoders in anticlockwise direction relative to y axis(unit: degree)
#define ENC_ANGLE1 0
#define ENC_ANGLE2 -120
#define ENC_ANGLE3 120

//Angles the encoders make with the center
//The definition od the angle is illustrated below (Usually it should be 90)
//This setting is independent from ENC_ANGLE constants
/*
 * ------------- <- Encoder
 *    (  /
 *      /
 *     /
 *    .          <- Center
 */
#define ENC_ANGLE_CENTER1 90
#define ENC_ANGLE_CENTER2 90
#define ENC_ANGLE_CENTER3 90

#endif
