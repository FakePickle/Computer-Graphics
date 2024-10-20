/******************************************************************************
 *                                                                            *
 *  Copyright (c) 2023 Ojaswa Sharma. All rights reserved.                    *
 *                                                                            *
 *  Author: Ojaswa Sharma                                                     *
 *  E-mail: ojaswa@iiitd.ac.in                                                *
 *                                                                            *
 *  This code is provided solely for the purpose of the CSE 333/533 course    *
 *  at IIIT Delhi. Unauthorized reproduction, distribution, or disclosure     *
 *  of this code, in whole or in part, without the prior written consent of   *
 *  the author is strictly prohibited.                                        *
 *                                                                            *
 *  This code is provided "as is", without warranty of any kind, express      *
 *  or implied, including but not limited to the warranties of                *
 *  merchantability, fitness for a particular purpose, and noninfringement.   *
 *                                                                            *
 ******************************************************************************/

//structs.h
#ifndef _STRUCTS_H_
#define _STRUCTS_H_

#include "vector3D.h"
#include "color.h"
#include "material.h"

#define MAX_LIGHTS 10
struct renderViewInfo
{
    Vector3D eye;
    double screenWidth;
    double screenHeight;
    double screenZ;
    int windowWidth;
    int windowHeight;
 
    Vector3D light[MAX_LIGHTS];
    Color lightColor[MAX_LIGHTS];
    Color lightAmbient;
    int levelOfRecursion;
    bool doJitter;
    bool disableSpecular;    
    int numLights;
};

#endif
