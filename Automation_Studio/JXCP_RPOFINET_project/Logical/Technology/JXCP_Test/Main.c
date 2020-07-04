// ************************************************************************************************************************************ //
// Title: JXCP1 (SMC) - Control {PROFINET}
// Autor: Roman Parak
// Date : 8.5.2020
// Email: Roman.Parak@outlook.com
// ************************************************************************************************************************************ //

// ************************************************************************************************************************************ //
// -------------------------------------------------------------  LIBRARIES ----------------------------------------------------------- //
// ************************************************************************************************************************************ //
#include <bur/plctypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "rp_funcx.h"

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

// ************************************************************************************************************************************ //
// -------------------------------------------------------------  DECLARATIONS -------------------------------------------------------- //
// ************************************************************************************************************************************ //
// JXCP1 {SMC} // STRUCT // #include "rp_funcx.h" -> jxcp_smc_fb.c
_LOCAL struct jxcp_smc_fb jxcp_smc_fb_1;

// ************************************************************************************************************************************ //
// -------------------------------------------------------------  INITIALIZATION ------------------------------------------------------ //
// ************************************************************************************************************************************ //
void _INIT ProgramInit(void)
{
	// -------------------- INITIALIZATION JXCP1 (SMC) {PROFINET} -------------------- //
	// NODE 1 // LESH16RK - 100
	jxcp_smc_fb_1.Enable = 1;
	// Boundaries (MAX/MIN)
	// Position
	jxcp_smc_fb_1.Set_Parameters.Boundary.Position.max = 100;
	jxcp_smc_fb_1.Set_Parameters.Boundary.Position.min = 0;
	// Speed
	jxcp_smc_fb_1.Set_Parameters.Boundary.Speed.max = 200;
	jxcp_smc_fb_1.Set_Parameters.Boundary.Speed.min = 10;
	// Acceleration
	jxcp_smc_fb_1.Set_Parameters.Boundary.Acceleration.max = 3000;
	jxcp_smc_fb_1.Set_Parameters.Boundary.Acceleration.min = 100;
	// Decelration
	jxcp_smc_fb_1.Set_Parameters.Boundary.Deceleration.max = 3000;
	jxcp_smc_fb_1.Set_Parameters.Boundary.Deceleration.min = 100;
	// Set Mode - Normal/Cycle
	jxcp_smc_fb_1.Set_Parameters.Mode = CYCLE_MOVE_MODE;
	// Set Default Parameters
	jxcp_smc_fb_1.Input.Speed 		 = 50;
	jxcp_smc_fb_1.Input.Acceleration = 1000;
	jxcp_smc_fb_1.Input.Deceleration = 1000;
	jxcp_smc_fb_1.Input.Moving_Force = 100;
	jxcp_smc_fb_1.Input.IN_Position  = 0.5;
}

// ************************************************************************************************************************************ //
// ----------------------------------------------------------------  MAIN ------------------------------------------------------------- //
// ************************************************************************************************************************************ //
void _CYCLIC ProgramCyclic(void)
{
	// --------------------  Write Data - From MappView to JXCP (Function Block) -------------------- //
	// Power -> ON/OFF
	jxcp_smc_fb_1.Input.BYTE_1.SVON  = jxcp_data_mv.Power;
	// Initialization axis -> Home
	jxcp_smc_fb_1.Input.BYTE_1.SETUP = jxcp_data_mv.Home;
	// Resel Alarm -> ALL
	jxcp_smc_fb_1.Input.BYTE_1.RESET = jxcp_data_mv.Reset_Alarm;
	
	// Function Block -> Start/Stop Cycle Mode
	jxcp_smc_fb_1.Set_Parameters.Start = jxcp_data_mv.Control.Cycle_Mode.Start;
	jxcp_smc_fb_1.Set_Parameters.Stop  = jxcp_data_mv.Control.Cycle_Mode.Stop;
	
	// Jog -> Positive/Negative
	jxcp_smc_fb_1.Input.BYTE_1.JOG_Pos = jxcp_data_mv.Control.Jog_pos;
	jxcp_smc_fb_1.Input.BYTE_1.JOG_Neg = jxcp_data_mv.Control.Jog_neg;
	
	// Set Parameters -> s, v, acc, dec
	jxcp_smc_fb_1.Input.Position     = jxcp_data_mv.Write_Parameters.Position;
	jxcp_smc_fb_1.Input.Speed 		 = jxcp_data_mv.Write_Parameters.Speed;
	jxcp_smc_fb_1.Input.Acceleration = jxcp_data_mv.Write_Parameters.Acceleration;
	jxcp_smc_fb_1.Input.Deceleration = jxcp_data_mv.Write_Parameters.Deceleration;
	
	// Defaul Parameters
	if(jxcp_data_mv.Control.Cycle_Mode.Default_Param){
		jxcp_data_mv.Write_Parameters.Speed        = 50;
		jxcp_data_mv.Write_Parameters.Acceleration = 1000;
		jxcp_data_mv.Write_Parameters.Deceleration = 1000;
	}
	
	// --------------------  Read Data - From JXCP (Function Block) to MappView -------------------- //
	// Power On -> Successful
	jxcp_data_mv.PowerOn = jxcp_smc_fb_1.Output.BYTE_1.SVRE;
	// Is Home -> Successful
	jxcp_data_mv.isHome  = jxcp_smc_fb_1.Output.BYTE_1.SETON;
	
	// Read Data (Shown in a graph -> MappView)
	jxcp_data_mv.Read_Parameters.Force    = jxcp_smc_fb_1.Output.Current_Pushing_Force;
	jxcp_data_mv.Read_Parameters.Position = jxcp_smc_fb_1.Output.Current_Position;
	jxcp_data_mv.Read_Parameters.Speed    = jxcp_smc_fb_1.Output.Current_Speed;
	// In Position
	jxcp_data_mv.Read_Parameters.In_Pos   = jxcp_smc_fb_1.Output.BYTE_1.INP;
	
	// -------------------- Read Data {ALARM 1 - 4} -------------------- //
	// Alarm no. 1
	jxcp_data_mv.Alarm.Alarm_1 = jxcp_smc_fb_1.Output.Warning.Alarm_1;
	// Alarm Condition {Change Image}
	if(jxcp_smc_fb_1.Output.Warning.Alarm_1 > 0){
		jxcp_data_mv.Alarm.Alarm_1_img = 1;
	}else{
		jxcp_data_mv.Alarm.Alarm_1_img = 0;
	}
	
	// Alarm no. 2
	jxcp_data_mv.Alarm.Alarm_2 = jxcp_smc_fb_1.Output.Warning.Alarm_2;
	// Alarm Condition {Change Image}
	if(jxcp_smc_fb_1.Output.Warning.Alarm_2 > 0){
		jxcp_data_mv.Alarm.Alarm_2_img = 1;
	}else{
		jxcp_data_mv.Alarm.Alarm_2_img = 0;
	}
	
	// Alarm no. 3
	jxcp_data_mv.Alarm.Alarm_3 = jxcp_smc_fb_1.Output.Warning.Alarm_3;
	// Alarm Condition {Change Image}
	if(jxcp_smc_fb_1.Output.Warning.Alarm_3 > 0){
		jxcp_data_mv.Alarm.Alarm_3_img = 1;
	}else{
		jxcp_data_mv.Alarm.Alarm_3_img = 0;
	}
	
	// Alarm no. 4
	jxcp_data_mv.Alarm.Alarm_4 = jxcp_smc_fb_1.Output.Warning.Alarm_4;
	// Alarm Condition {Change Image}
	if(jxcp_smc_fb_1.Output.Warning.Alarm_4 > 0){
		jxcp_data_mv.Alarm.Alarm_4_img = 1;
	}else{
		jxcp_data_mv.Alarm.Alarm_4_img = 0;
	}

	// -------------------- CALL FUNCTION -------------------- //
	jxcp_smc_fb(&jxcp_smc_fb_1);
}
