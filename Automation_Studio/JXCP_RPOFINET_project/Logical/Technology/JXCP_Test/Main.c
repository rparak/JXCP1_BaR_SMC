#include <bur/plctypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "rp_funcx.h"

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

_LOCAL struct jxcp_smc_fb jxcp_smc_fb_1;

void _INIT ProgramInit(void)
{
	
	//jxcp_smc_fb_1.Enable = 1;
	
	jxcp_smc_fb_1.Set_Parameters.Mode = CYCLE_MOVE_MODE;
	
	jxcp_smc_fb_1.Input.Speed 		 = 50;
	jxcp_smc_fb_1.Input.Acceleration = 1000;
	jxcp_smc_fb_1.Input.Deceleration = 1000;
	jxcp_smc_fb_1.Input.Moving_Force = 100;
	jxcp_smc_fb_1.Input.IN_Position  = 0.5;
}

void _CYCLIC ProgramCyclic(void)
{
	jxcp_smc_fb(&jxcp_smc_fb_1);
}
