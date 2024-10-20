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

#ifndef _LIGHTSOURCE_H_
#define _LIGHTSOURCE_H_

#include "world.h"
#include "color.h"
#include "vector3D.h"

class LightSource
{	
protected:
	World *world;
	Color intensity;
public:
	LightSource(World *_world, const Color _intensity):
		world(_world), intensity(_intensity)   {}
	virtual Vector3D getPosition() const = 0;
	Color getIntensity() const {return intensity; }

};
#endif
