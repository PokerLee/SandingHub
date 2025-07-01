//#############################################################################
//
// FILE:   sv_gen.h
//
//#############################################################################

#ifndef SVGEN_H
#define SVGEN_H

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

//*****************************************************************************
//
//! \defgroup SVGEN SVGEN
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
// the structures
//
//*****************************************************************************

//*****************************************************************************
//
//! \brief Defines the Space Vector Generator (SVGEN) object
//
//*****************************************************************************
typedef struct _SVGEN_Obj_
{
	float32_t	Ts;
	float32_t	period;

	int			N;

	float32_t	u1;
	float32_t	u2;
	float32_t	u3;

	float32_t	Tx;
	float32_t	Ty;

	float32_t	Tcm1;
	float32_t	Tcm2;
	float32_t	Tcm3;

	float32_t	udc;
} SVGEN_Obj;

//*****************************************************************************
//
//! \brief Defines the SVGEN handle
//
//*****************************************************************************
typedef struct _SVGEN_Obj_ *SVGEN_Handle;

//*****************************************************************************
//
// the functions
//
//*****************************************************************************

//*****************************************************************************
//
//! \brief  Implements a SVM that saturates at the level of MaxModulation.
//!
//! \param[in] handle    The space vector generator (SVGEN) handle
//!
//! \param[in] pVab_V    The pointer to the alpha/beta voltages, V
//!
//! \param[in] pVabc_pu  The pointer to the three phase voltages, pu
//!
//! \return    None
//
//*****************************************************************************
static __UNUSED ALWAYSINLINE void
SVPWM_run(SVGEN_Handle handle, const MATH_Vec2 *pVab_V, MATH_Vec3 *pVabc_pu)
{
	// Sector Judgment
	handle->u1 = pVab_V->value[1];
	handle->u2 = pVab_V->value[0]*MATH_SQRTTHREE_OVER_TWO - pVab_V->value[1]/2;
	handle->u3 = -pVab_V->value[0]*MATH_SQRTTHREE_OVER_TWO - pVab_V->value[1]/2;

	int a = 0;
	int b = 0;
	int c = 0;

    (handle->u1 > 0)?(a = 1):(a = 0);
    (handle->u2 > 0)?(b = 1):(b = 0);
    (handle->u3 > 0)?(c = 1):(c = 0);

    handle->N = 4*c + 2*b + a;

    // Gets the duration of vector action
    float32_t Umr = MATH_SQRT_THREE * handle->Ts /handle->udc;
    float32_t X = Umr * handle->u1;
    float32_t Y = Umr * handle->u2;
    float32_t Z = Umr * handle->u3;

	switch(handle->N)
	{
	    case 3:
	    	handle->Tx = Y; handle->Ty = X;
	        break;
	    case 1:
	    	handle->Tx = -Y; handle->Ty = -Z;
	        break;
	    case 5:
	    	handle->Tx = X; handle->Ty = Z;
	        break;
	    case 4:
	    	handle->Tx = -X; handle->Ty = -Y;
	        break;
	    case 6:
	    	handle->Tx = Z; handle->Ty = Y;
	        break;
	    case 2:
	    	handle->Tx = -Z; handle->Ty = -X;
	        break;
	}

	if(handle->Tx + handle->Ty > handle->Ts)
	{
		handle->Tx = (handle->Tx/(handle->Tx + handle->Ty))*handle->Ts;
		handle->Ty = (handle->Ty/(handle->Tx + handle->Ty))*handle->Ts;
	}

	float32_t Ta = (handle->Ts-handle->Tx-handle->Ty)/4.0;
	float32_t Tb = Ta+handle->Tx/2.0;
	float32_t Tc = Tb+handle->Ty/2.0;

	switch(handle->N)
	{
	    case 3:
	    	handle->Tcm1 = Ta; handle->Tcm2 = Tb; handle->Tcm3 = Tc;
	    	break;
	    case 1:
	    	handle->Tcm1 = Tb; handle->Tcm2 = Ta; handle->Tcm3 = Tc;
	    	break;
	    case 5:
	    	handle->Tcm1 = Tc; handle->Tcm2 = Ta; handle->Tcm3 = Tb;
	    	break;
	    case 4:
	    	handle->Tcm1 = Tc; handle->Tcm2 = Tb; handle->Tcm3 = Ta;
	    	break;
	    case 6:
	    	handle->Tcm1 = Tb; handle->Tcm2 = Tc; handle->Tcm3 = Ta;
	    	break;
	    case 2:
	    	handle->Tcm1 = Ta; handle->Tcm2 = Tc; handle->Tcm3 = Tb;
	    	break;
	}

	// SVPWM generates periodic functions
	pVabc_pu->value[0] = (float32_t)(0.5 - (handle->Tcm1 * 2 / handle->Ts));
	pVabc_pu->value[1] = (float32_t)(0.5 - (handle->Tcm2 * 2 / handle->Ts));
	pVabc_pu->value[2] = (float32_t)(0.5 - (handle->Tcm3 * 2 / handle->Ts));
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // end of SVGEN_H definition
