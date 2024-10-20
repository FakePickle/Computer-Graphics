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

#ifndef _POINTLIGHTSOURCE_H_
#define _POINTLIGHTSOURCE_H_

#include "lightsource.h"

class PointLightSource : public LightSource
{
private:
	Vector3D position;
public:
	PointLightSource(World *_world, const Vector3D& _pos, const Color& _intensity):
		LightSource(_world, _intensity), position(_pos) {}
	Vector3D getPosition() const {return position;}
};
#endif
