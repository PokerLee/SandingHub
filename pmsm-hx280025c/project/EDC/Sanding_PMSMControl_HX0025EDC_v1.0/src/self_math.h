/*
 * math.h
 *
 *  Created on: 2023��1��31��
 *      Author: Kangaihong
 */

#ifndef SELF_MATH_H_
#define SELF_MATH_H_
#include "inc/hw_types.h"
#include "always.h"
#define MATH_Nm_TO_lbin_SF        ((float32_t)(8.8507457913f))

//*****************************************************************************
//
//! \brief Defines 2/3
//
//*****************************************************************************
#define MATH_TWO_OVER_THREE       ((float32_t)(0.6666666666666666666666666667f))

//*****************************************************************************
//
//! \brief Defines 1/3
//
//*****************************************************************************
#define MATH_ONE_OVER_THREE       ((float32_t)(0.3333333333333333333333333333f))

//*****************************************************************************
//
//! \brief Defines 1/(pi)
//
//*****************************************************************************
#define MATH_ONE_OVER_PI          ((float32_t)(0.318309886183791f))

//*****************************************************************************
//
//! \brief Defines sqrt(3)/2
//
//*****************************************************************************
#define MATH_SQRTTHREE_OVER_TWO       ((float32_t)(0.8660254038f))
//*****************************************************************************
//
//! \brief Defines sqrt(2)
//
//*****************************************************************************
#define MATH_SQRT_TWO             ((float32_t)(1.414213562373095f))


//*****************************************************************************
//
//! \brief Defines sqrt(3)
//
//*****************************************************************************
#define MATH_SQRT_THREE           ((float32_t)(1.73205080756887f))

//*****************************************************************************
//
//! \brief Defines 1/sqrt(3)
//
//*****************************************************************************
#define MATH_ONE_OVER_SQRT_THREE  ((float32_t)(0.5773502691896257645091487805f))

//*****************************************************************************
//
//! \brief Defines 1/(4*pi)
//
//*****************************************************************************
#define MATH_ONE_OVER_FOUR_PI     ((float32_t)(0.07957747154594767f))

//*****************************************************************************
//
//! \brief Defines 1/(2*pi)
//
//*****************************************************************************
#define MATH_ONE_OVER_TWO_PI      ((float32_t) (0.1591549430918954f))

//*****************************************************************************
//
//! \brief Defines pi
//
//*****************************************************************************
#define MATH_PI                   ((float32_t)(3.1415926535897932384626433832f))

//*****************************************************************************
//
//! \brief Defines pi per unit
//
//*****************************************************************************
#define MATH_PI_PU                ((float32_t)(0.5f))

//*****************************************************************************
//
//! \brief Defines 2*pi
//
//*****************************************************************************
#define MATH_TWO_PI               ((float32_t)(6.283185307179586f))

//*****************************************************************************
//
//! \brief Defines 2*pi per unit
//
//*****************************************************************************
#define MATH_TWO_PI_PU            ((float32_t)(1.0f))

//*****************************************************************************
//
//! \brief Defines 4*pi
//
//*****************************************************************************
#define MATH_FOUR_PI               ((float32_t)(12.56637061435917f))

//*****************************************************************************
//
//! \brief Defines 4*pi per unit
//
//*****************************************************************************
#define MATH_FOUR_PI_PU            ((float32_t)(2.0f))

//*****************************************************************************
//
//! \brief Defines pi/2
//
//*****************************************************************************
#define MATH_PI_OVER_TWO           ((float32_t)(1.570796326794897f))

//*****************************************************************************
//
//! \brief Defines pi/2 per unit
//
//*****************************************************************************
#define MATH_PI_OVER_TWO_PU        ((float32_t)(0.25f))


//*****************************************************************************
//
//! \brief Defines pi/3
//
//*****************************************************************************
#define MATH_PI_OVER_THREE         ((float32_t)(1.047197551196598f))

//*****************************************************************************
//
//! \brief Defines pi/4
//
//*****************************************************************************
#define MATH_PI_OVER_FOUR          ((float32_t)(0.785398163397448f))

//*****************************************************************************
//
//! \brief Defines pi/4 per unit
//
//*****************************************************************************
#define MATH_PI_OVER_FOUR_PU        ((float32_t)(0.125f))



typedef struct _MATH_Vec2_
{
	float32_t  value[2];
} MATH_Vec2;

typedef MATH_Vec2 MATH_vec2;

//*****************************************************************************
//
//! \brief Defines a three element vector
//
//*****************************************************************************
typedef struct _MATH_Vec3_
{
    float32_t value[3];
} MATH_Vec3;

typedef MATH_Vec3 MATH_vec3;

static __UNUSED ALWAYSINLINE float32_t
MATH_incrAngle(const float32_t angle_rad, const float32_t angleDelta_rad)
{
    float32_t angleNew_rad;

    angleNew_rad = angle_rad + angleDelta_rad;

    angleNew_rad = (angleNew_rad > MATH_PI)  ? angleNew_rad - MATH_TWO_PI : angleNew_rad;
    angleNew_rad = (angleNew_rad < -MATH_PI) ? angleNew_rad + MATH_TWO_PI : angleNew_rad;

    return(angleNew_rad);
} // end of MATH_incrAngle() function

static __UNUSED ALWAYSINLINE float32_t
MATH_incrAngle1(const float32_t angle_rad, const float32_t angleDelta_rad)
{
    float32_t angleNew_rad;
	angleNew_rad = angle_rad + angleDelta_rad;

    while((angleNew_rad > MATH_TWO_PI)||(angleNew_rad < 0))
    {
    	if(angleNew_rad > MATH_TWO_PI)
    	{
    		angleNew_rad -= MATH_TWO_PI;
    	}
    	if(angleNew_rad < 0)
    	{
    		angleNew_rad += MATH_TWO_PI;
    	}
    }

    return(angleNew_rad);
}

static __UNUSED ALWAYSINLINE float32_t
MATH_incrAngle2(const float32_t angle_rad, const float32_t angleDelta_rad, const uint32_t pairs)
{
    float32_t angleNew_rad;
	angleNew_rad = angle_rad + angleDelta_rad;

    while((angleNew_rad > MATH_TWO_PI * pairs)||(angleNew_rad < 0))
    {
    	if(angleNew_rad > MATH_TWO_PI * pairs)
    	{
    		angleNew_rad -= MATH_TWO_PI;
    	}
    	if(angleNew_rad < 0)
    	{
    		angleNew_rad += MATH_TWO_PI;
    	}
    }

    return(angleNew_rad);
}

#endif /* MATH_H_ */