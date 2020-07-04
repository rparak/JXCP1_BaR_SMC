
#include <bur/plctypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifdef __cplusplus
	extern "C"
	{
#endif
	#include "rp_funcx.h"
#ifdef __cplusplus
	};
#endif

// ************************************************************************************************************************************ //
// -------------------------------------------------------------  DECLARATIONS -------------------------------------------------------- //
// ************************************************************************************************************************************ //
// INITIALIZATION // RESET //
void initialization_jxcp(struct jxcp_smc_fb* jxcp_s);

DINT convert_bin_to_dec(LREAL n);

// ************************************************************************************************************************************ //
// --------------------------------------------------------------  MAIN --------------------------------------------------------------- //
// ************************************************************************************************************************************ //
void jxcp_smc_fb(struct jxcp_smc_fb* jxcp_s)
{
	// *********************************************************//
	// ---------------- INITIALIZATION  ----------------------- //
	// *********************************************************//
	if(jxcp_s->Enable != 1){
		initialization_jxcp(jxcp_s);
	}
	
	// ******************************************************************************************************************//
	// --------------------------------------------- JXCP MAIN STATE  -------------------------------------------------- //
	// ******************************************************************************************************************//
	
	// declaration index {all cycles)
	int i, i_w;
	
	// Read
	struct multiplne_usint_to_dec m_u2d_cP;
	struct multiplne_usint_to_dec m_u2d_cS;
	struct multiplne_usint_to_dec m_u2d_cPF;
	struct multiplne_usint_to_dec m_u2d_TP;
	
	// Write
	struct dec_to_multiple_usint dec2u_m_S;
	struct dec_to_multiple_usint dec2u_m_P;
	struct dec_to_multiple_usint dec2u_m_Acc;
	struct dec_to_multiple_usint dec2u_m_Dec;
	struct dec_to_multiple_usint dec2u_m_PF;
	struct dec_to_multiple_usint dec2u_m_TLV;
	struct dec_to_multiple_usint dec2u_m_PS;
	struct dec_to_multiple_usint dec2u_m_MF;
	struct dec_to_multiple_usint dec2u_m_AR1;
	struct dec_to_multiple_usint dec2u_m_AR2;
	struct dec_to_multiple_usint dec2u_m_IN_P;
	
	
	switch(jxcp_s->Internal.actual_state){
		case JXCP_INITIALIZATION_STATE:
			{
				/*************************************** INITIALIZATION STATE ******************************************/
				jxcp_s->Internal.before_state = JXCP_INITIALIZATION_STATE;
				
				if(jxcp_s->ModuleOK == 1){
					jxcp_s->Active = 1;
					
					jxcp_s->Internal.actual_state = JXCP_WAIT_STATE;
				}
			}
			break;
		case JXCP_WAIT_STATE:
			{
				/*************************************** WAIT STATE ******************************************/
				jxcp_s->Internal.before_state = JXCP_WAIT_STATE;
				
				jxcp_s->Set_Parameters.Stop = 0;
				
				if(jxcp_s->Set_Parameters.Start == 1){
					if(jxcp_s->Set_Parameters.Mode == NORMAL_MOVE_MODE){
						
						jxcp_s->Internal.actual_state = JXCP_NORMAL_MODE_STATE;
					}else if(jxcp_s->Set_Parameters.Mode == CYCLE_MOVE_MODE){
						// Write Byte 2 - 3 { SET - 1 }
						jxcp_s->Input.BYTE_2.Speed_Restriction = 1;
						jxcp_s->Input.BYTE_2.Movement_Mode     = 1;
						jxcp_s->Input.BYTE_2.Speed             = 1;
						jxcp_s->Input.BYTE_2.Position          = 1;
						jxcp_s->Input.BYTE_2.Acceleration      = 1;
		
						jxcp_s->Input.BYTE_3.Deceleration      = 1;
						jxcp_s->Input.BYTE_3.Pushing_Force     = 1;
						jxcp_s->Input.BYTE_3.Trigger_LV        = 1;
						jxcp_s->Input.BYTE_3.Pushing_Speed     = 1;
						jxcp_s->Input.BYTE_3.Moving_Force      = 1;
						jxcp_s->Input.BYTE_3.Area_out_1        = 1;
						jxcp_s->Input.BYTE_3.Area_out_2        = 1;
						jxcp_s->Input.BYTE_3.IN_Pos_Tolerance  = 1;
						
						jxcp_s->Internal.actual_state = JXCP_CYCLE_MODE_STATE;
					}
				}
				
				if(jxcp_s->ModuleOK != 1){
					jxcp_s->Active = 0;
					jxcp_s->Internal.actual_state = JXCP_ERROR_MODE_STATE;
				}
			}
			break;
		case JXCP_NORMAL_MODE_STATE:
			{
				/*************************************** NORMAL MODE STATE ******************************************/
				jxcp_s->Internal.before_state = JXCP_NORMAL_MODE_STATE;
				
				if(jxcp_s->Set_Parameters.Stop == 1){
					jxcp_s->Set_Parameters.Start = 0;
					
					jxcp_s->Internal.actual_state = JXCP_WAIT_STATE;
				}
				
				if(jxcp_s->ModuleOK != 1){
					jxcp_s->Active = 0;
					
					jxcp_s->Internal.actual_state = JXCP_ERROR_MODE_STATE;
				}
			}
			break;
		case JXCP_CYCLE_MODE_STATE:
			{
				/*************************************** CYCLE MODE STATE ******************************************/
				jxcp_s->Internal.before_state = JXCP_CYCLE_MODE_STATE;
				
				jxcp_s->Input.Start_Flag 			 = 1;
				jxcp_s->Input.Movement_Mode.Absolute = 1;
				
				// Speed
				dec2u_m_S.Count_of_VAR = 2;
				dec2u_m_S.Decimal = jxcp_s->Input.Speed;
				
				dec_to_multiple_usint(&dec2u_m_S);
				
				jxcp_s->PROFINET_Mapping_OUT.Output_Area[6] = dec2u_m_S.Result_USINT_1;
				jxcp_s->PROFINET_Mapping_OUT.Output_Area[7] = dec2u_m_S.Result_USINT_2;
				
				// Position
				dec2u_m_P.Count_of_VAR = 4;
				
				dec2u_m_P.Decimal = jxcp_s->Input.Position;
				
				dec_to_multiple_usint(&dec2u_m_P);
				
				jxcp_s->PROFINET_Mapping_OUT.Output_Area[8] = dec2u_m_P.Result_USINT_1;
				jxcp_s->PROFINET_Mapping_OUT.Output_Area[9] = dec2u_m_P.Result_USINT_2;
				jxcp_s->PROFINET_Mapping_OUT.Output_Area[10] = dec2u_m_P.Result_USINT_3;
				jxcp_s->PROFINET_Mapping_OUT.Output_Area[11] = dec2u_m_P.Result_USINT_4;
				
				// Acceleration
				dec2u_m_Acc.Count_of_VAR = 2;
				
				dec2u_m_Acc.Decimal = jxcp_s->Input.Acceleration;
				
				dec_to_multiple_usint(&dec2u_m_Acc);
				
				jxcp_s->PROFINET_Mapping_OUT.Output_Area[12] = dec2u_m_Acc.Result_USINT_1;
				jxcp_s->PROFINET_Mapping_OUT.Output_Area[13] = dec2u_m_Acc.Result_USINT_2;
				
				// Deceleration
				dec2u_m_Dec.Count_of_VAR = 2;
				
				dec2u_m_Dec.Decimal = jxcp_s->Input.Deceleration;
				
				dec_to_multiple_usint(&dec2u_m_Dec);
				
				jxcp_s->PROFINET_Mapping_OUT.Output_Area[14] = dec2u_m_Dec.Result_USINT_1;
				jxcp_s->PROFINET_Mapping_OUT.Output_Area[15] = dec2u_m_Dec.Result_USINT_2;
				
				// Pushing Force
				dec2u_m_PF.Count_of_VAR = 2;
				
				dec2u_m_PF.Decimal = jxcp_s->Input.Pushing_Force;
				
				dec_to_multiple_usint(&dec2u_m_PF);
				
				jxcp_s->PROFINET_Mapping_OUT.Output_Area[16] = dec2u_m_PF.Result_USINT_1;
				jxcp_s->PROFINET_Mapping_OUT.Output_Area[17] = dec2u_m_PF.Result_USINT_2;
				
				// Trigger LV
				dec2u_m_TLV.Count_of_VAR = 2;
				
				dec2u_m_TLV.Decimal = jxcp_s->Input.Trigger_LV;
				
				dec_to_multiple_usint(&dec2u_m_TLV);
				
				jxcp_s->PROFINET_Mapping_OUT.Output_Area[18] = dec2u_m_TLV.Result_USINT_1;
				jxcp_s->PROFINET_Mapping_OUT.Output_Area[19] = dec2u_m_TLV.Result_USINT_2;
				
				// Pushing Speed
				dec2u_m_PS.Count_of_VAR = 2;
				
				dec2u_m_PS.Decimal = jxcp_s->Input.Pushing_Speed;
				
				dec_to_multiple_usint(&dec2u_m_PS);
				
				jxcp_s->PROFINET_Mapping_OUT.Output_Area[20] = dec2u_m_PS.Result_USINT_1;
				jxcp_s->PROFINET_Mapping_OUT.Output_Area[21] = dec2u_m_PS.Result_USINT_2;
				
				// Moving Force
				dec2u_m_MF.Count_of_VAR = 2;
				
				dec2u_m_MF.Decimal = jxcp_s->Input.Moving_Force;
				
				dec_to_multiple_usint(&dec2u_m_MF);
				
				jxcp_s->PROFINET_Mapping_OUT.Output_Area[22] = dec2u_m_MF.Result_USINT_1;
				jxcp_s->PROFINET_Mapping_OUT.Output_Area[23] = dec2u_m_MF.Result_USINT_2;
				
				// Area 1
				dec2u_m_AR1.Count_of_VAR = 4;
				
				dec2u_m_AR1.Decimal = jxcp_s->Input.AREA_1;
				
				dec_to_multiple_usint(&dec2u_m_AR1);
				
				jxcp_s->PROFINET_Mapping_OUT.Output_Area[24] = dec2u_m_AR1.Result_USINT_1;
				jxcp_s->PROFINET_Mapping_OUT.Output_Area[25] = dec2u_m_AR1.Result_USINT_2;
				jxcp_s->PROFINET_Mapping_OUT.Output_Area[26] = dec2u_m_AR1.Result_USINT_3;
				jxcp_s->PROFINET_Mapping_OUT.Output_Area[27] = dec2u_m_AR1.Result_USINT_4;
				
				// Area 2
				dec2u_m_AR2.Count_of_VAR = 4;
				
				dec2u_m_AR2.Decimal = jxcp_s->Input.AREA_2;
				
				dec_to_multiple_usint(&dec2u_m_AR2);
				
				jxcp_s->PROFINET_Mapping_OUT.Output_Area[28] = dec2u_m_AR2.Result_USINT_1;
				jxcp_s->PROFINET_Mapping_OUT.Output_Area[29] = dec2u_m_AR2.Result_USINT_2;
				jxcp_s->PROFINET_Mapping_OUT.Output_Area[30] = dec2u_m_AR2.Result_USINT_3;
				jxcp_s->PROFINET_Mapping_OUT.Output_Area[31] = dec2u_m_AR2.Result_USINT_4;
				
				// In-Position
				dec2u_m_IN_P.Count_of_VAR = 4;
				
				dec2u_m_IN_P.Decimal = jxcp_s->Input.IN_Position;
				
				dec_to_multiple_usint(&dec2u_m_IN_P);
				
				jxcp_s->PROFINET_Mapping_OUT.Output_Area[32] = dec2u_m_IN_P.Result_USINT_1;
				jxcp_s->PROFINET_Mapping_OUT.Output_Area[33] = dec2u_m_IN_P.Result_USINT_2;
				jxcp_s->PROFINET_Mapping_OUT.Output_Area[34] = dec2u_m_IN_P.Result_USINT_3;
				jxcp_s->PROFINET_Mapping_OUT.Output_Area[35] = dec2u_m_IN_P.Result_USINT_4;
				
				if(jxcp_s->Set_Parameters.Stop == 1){
					jxcp_s->Set_Parameters.Start = 0;
					
					jxcp_s->Input.Start_Flag 			   = 0;
					
					// Write Byte 2 - 3 { SET - 0 }
					jxcp_s->Input.BYTE_2.Speed_Restriction = 0;
					jxcp_s->Input.BYTE_2.Movement_Mode     = 0;
					jxcp_s->Input.BYTE_2.Speed             = 0;
					jxcp_s->Input.BYTE_2.Position          = 0;
					jxcp_s->Input.BYTE_2.Acceleration      = 0;
		
					jxcp_s->Input.BYTE_3.Deceleration      = 0;
					jxcp_s->Input.BYTE_3.Pushing_Force     = 0;
					jxcp_s->Input.BYTE_3.Trigger_LV        = 0;
					jxcp_s->Input.BYTE_3.Pushing_Speed     = 0;
					jxcp_s->Input.BYTE_3.Moving_Force      = 0;
					jxcp_s->Input.BYTE_3.Area_out_1        = 0;
					jxcp_s->Input.BYTE_3.Area_out_2        = 0;
					jxcp_s->Input.BYTE_3.IN_Pos_Tolerance  = 0;
					
					jxcp_s->Internal.actual_state = JXCP_WAIT_STATE;
				}
				
				if(jxcp_s->ModuleOK != 1){
					jxcp_s->Active = 0;
					
					jxcp_s->Internal.actual_state = JXCP_ERROR_MODE_STATE;
				}
			}
			break;
		case JXCP_ALARM_MODE_STATE:
			{
				/*************************************** ALARM STATE ******************************************/
				jxcp_s->Internal.before_state = JXCP_ALARM_MODE_STATE;
				
				if(jxcp_s->ModuleOK != 1){
					jxcp_s->Active = 0;
					
					jxcp_s->Internal.actual_state = JXCP_ERROR_MODE_STATE;
				}
			}
			break;
		case JXCP_ERROR_MODE_STATE:
			{
				/*************************************** ERROR STATE ******************************************/
				if(jxcp_s->ModuleOK == 1){
					jxcp_s->Active = 1;
					
					jxcp_s->Internal.actual_state = jxcp_s->Internal.before_state;
				}
			}
			break;
	}
	
	// ******************************************************************************************************************//
	// ---------------------------------------------------- JXCP R/W STATE  -------------------------------------------- //
	// ******************************************************************************************************************//
	switch(jxcp_s->Internal.actual_r_state){
		case JXCP_R_INITIALIZATION_STATE:
			{
				/*************************************** INITIALIZATION STATE ******************************************/
				jxcp_s->Internal.before_r_state = JXCP_R_INITIALIZATION_STATE;
				
				if(jxcp_s->ModuleOK == 1){
					jxcp_s->Active = 1;
					
					jxcp_s->Internal.actual_r_state = JXCP_R_RW_STATE;
				}
			}
			break;
		case JXCP_R_RW_STATE:
			{
				/*************************************** READ/WRITE STATE ******************************************/
				jxcp_s->Internal.before_r_state = JXCP_R_RW_STATE;
				
				// ---------------------------------------------- //
				// -------------------- READ -------------------- //
				// ---------------------------------------------- //
				
				// ----- Byte 0 ----- //
				USINT aux_get_byte_0;
				aux_get_byte_0 = jxcp_s->PROFINET_Mapping_IN.Input_Area[0];
				
				for(i = 0; i < (int)(sizeof(jxcp_s->Internal.Profinet.OUT_Aux.BYTE_0)/sizeof(jxcp_s->Internal.Profinet.OUT_Aux.BYTE_0[0])); i++){
					if(aux_get_byte_0 > 0){
						jxcp_s->Internal.Profinet.OUT_Aux.BYTE_0[i] = aux_get_byte_0 % 2;
						aux_get_byte_0 = aux_get_byte_0/2;
					}else{
						jxcp_s->Internal.Profinet.OUT_Aux.BYTE_0[i] = 0;
					}
				}
				
				// ----- Byte 1 ----- //
				USINT aux_get_byte_1;
				aux_get_byte_1 = jxcp_s->PROFINET_Mapping_IN.Input_Area[1];
				
				for(i = 0; i < (int)(sizeof(jxcp_s->Internal.Profinet.OUT_Aux.BYTE_1)/sizeof(jxcp_s->Internal.Profinet.OUT_Aux.BYTE_1[0])); i++){
					if(aux_get_byte_1 > 0){
						jxcp_s->Internal.Profinet.OUT_Aux.BYTE_1[i] = aux_get_byte_1 % 2;
						aux_get_byte_1 = aux_get_byte_1/2;
					}else{
						jxcp_s->Internal.Profinet.OUT_Aux.BYTE_1[i] = 0;
					}
				}
				
				// ----- Byte 2 ----- //
				if(jxcp_s->PROFINET_Mapping_IN.Input_Area[2] == 16){
					jxcp_s->Internal.Profinet.OUT_Aux.BYTE_2 = 1;
				}else{
					jxcp_s->Internal.Profinet.OUT_Aux.BYTE_2 = 0;
				}

				// Current Position
				m_u2d_cP.Count_of_VAR = 4;
				
				m_u2d_cP.USINT_1 = jxcp_s->PROFINET_Mapping_IN.Input_Area[4];
				m_u2d_cP.USINT_2 = jxcp_s->PROFINET_Mapping_IN.Input_Area[5];
				m_u2d_cP.USINT_3 = jxcp_s->PROFINET_Mapping_IN.Input_Area[6];
				m_u2d_cP.USINT_4 = jxcp_s->PROFINET_Mapping_IN.Input_Area[7];
				
				multiplne_usint_to_dec(&m_u2d_cP);
				
				jxcp_s->Output.Current_Position = m_u2d_cP.Result_Decimal;
				
				// Current Speed
				m_u2d_cS.Count_of_VAR = 2;
				
				m_u2d_cS.USINT_1 = jxcp_s->PROFINET_Mapping_IN.Input_Area[8];
				m_u2d_cS.USINT_2 = jxcp_s->PROFINET_Mapping_IN.Input_Area[9];
				
				multiplne_usint_to_dec(&m_u2d_cS);
				
				jxcp_s->Output.Current_Speed = m_u2d_cS.Result_Decimal;
				
				// Current Pushing Force
				m_u2d_cPF.Count_of_VAR = 2;
				
				m_u2d_cPF.USINT_1 = jxcp_s->PROFINET_Mapping_IN.Input_Area[10];
				m_u2d_cPF.USINT_2 = jxcp_s->PROFINET_Mapping_IN.Input_Area[11];
				
				multiplne_usint_to_dec(&m_u2d_cPF);
				
				jxcp_s->Output.Current_Pushing_Force = m_u2d_cPF.Result_Decimal;
				
				// Target Position
				m_u2d_TP.Count_of_VAR = 4;
				
				m_u2d_TP.USINT_1 = jxcp_s->PROFINET_Mapping_IN.Input_Area[12];
				m_u2d_TP.USINT_2 = jxcp_s->PROFINET_Mapping_IN.Input_Area[13];
				m_u2d_TP.USINT_3 = jxcp_s->PROFINET_Mapping_IN.Input_Area[14];
				m_u2d_TP.USINT_4 = jxcp_s->PROFINET_Mapping_IN.Input_Area[15];
				
				multiplne_usint_to_dec(&m_u2d_TP);
				
				jxcp_s->Output.Target_Position = m_u2d_TP.Result_Decimal;
				
				// ----- Byte 16 - 19 {ALARM} ----- //
				jxcp_s->Output.Warning.Alarm_1 = jxcp_s->PROFINET_Mapping_IN.Input_Area[16];
				jxcp_s->Output.Warning.Alarm_2 = jxcp_s->PROFINET_Mapping_IN.Input_Area[17];
				jxcp_s->Output.Warning.Alarm_3 = jxcp_s->PROFINET_Mapping_IN.Input_Area[18];
				jxcp_s->Output.Warning.Alarm_4 = jxcp_s->PROFINET_Mapping_IN.Input_Area[19];
				
				// ---------------------------------------------- //
				// -------------------- WRITE ------------------- //
				// ---------------------------------------------- //
				
				// ----- Byte 0 ----- //
				USINT aux_set_byte_0;
				
				aux_set_byte_0 = 0;
				
				for(i = 0; i < (int)(sizeof(jxcp_s->Internal.Profinet.IN_Aux.BYTE_0)/sizeof(jxcp_s->Internal.Profinet.IN_Aux.BYTE_0[0])); i++){
					if(jxcp_s->Internal.Profinet.IN_Aux.BYTE_0[i] == 1){
						aux_set_byte_0 += pow(2, i);
					}
				}
				jxcp_s->PROFINET_Mapping_OUT.Output_Area[0] = aux_set_byte_0;
				
				// ----- Byte 1 ----- //
				USINT aux_set_byte_1;
				
				aux_set_byte_1 = 0;
				
				for(i = 0; i < (int)(sizeof(jxcp_s->Internal.Profinet.IN_Aux.BYTE_1)/sizeof(jxcp_s->Internal.Profinet.IN_Aux.BYTE_1[0])); i++){
					if(jxcp_s->Internal.Profinet.IN_Aux.BYTE_1[i] == 1){
						aux_set_byte_1 += pow(2, i);
					}
				}
				jxcp_s->PROFINET_Mapping_OUT.Output_Area[1] = aux_set_byte_1;
				
				// ----- Byte 2 ----- //
				USINT aux_set_byte_2;
				
				aux_set_byte_2 = 0;
				
				for(i = 0; i < (int)(sizeof(jxcp_s->Internal.Profinet.IN_Aux.BYTE_2)/sizeof(jxcp_s->Internal.Profinet.IN_Aux.BYTE_2[0])); i++){
					if(jxcp_s->Internal.Profinet.IN_Aux.BYTE_2[i] == 1){
						aux_set_byte_2 += pow(2, i);
					}
				}
				jxcp_s->PROFINET_Mapping_OUT.Output_Area[2] = aux_set_byte_2;
				
				// ----- Byte 3 ----- //
				USINT aux_set_byte_3;
				
				aux_set_byte_3 = 0;
				
				for(i = 0; i < (int)(sizeof(jxcp_s->Internal.Profinet.IN_Aux.BYTE_3)/sizeof(jxcp_s->Internal.Profinet.IN_Aux.BYTE_3[0])); i++){
					if(jxcp_s->Internal.Profinet.IN_Aux.BYTE_3[i] == 1){
						aux_set_byte_3 += pow(2, i);
					}
				}
				jxcp_s->PROFINET_Mapping_OUT.Output_Area[3] = aux_set_byte_3;
				
				// ----- Byte 4 ----- //
				if(jxcp_s->Input.Start_Flag == 1){
					jxcp_s->PROFINET_Mapping_OUT.Output_Area[4] = 1;
				}else{
					jxcp_s->PROFINET_Mapping_OUT.Output_Area[4] = 0;	
				}
				// ----- Byte 5 ----- //
				if(jxcp_s->Input.Movement_Mode.Absolute == 1){
					jxcp_s->PROFINET_Mapping_OUT.Output_Area[5] = 1;
				}else if(jxcp_s->Input.Movement_Mode.Relative == 1){
					jxcp_s->PROFINET_Mapping_OUT.Output_Area[5] = 2;
				}else{
					jxcp_s->PROFINET_Mapping_OUT.Output_Area[5] = 0;
				}

				if(jxcp_s->ModuleOK != 1){
					jxcp_s->Active = 0;
					
					jxcp_s->Internal.actual_r_state = JXCP_R_ERROR_MODE_STATE;
				}
			}
			break;
		case JXCP_R_ERROR_MODE_STATE:
			{
				/*************************************** ERROR STATE ******************************************/
				if(jxcp_s->ModuleOK == 1){
					jxcp_s->Active = 1;
					
					jxcp_s->Internal.actual_r_state = jxcp_s->Internal.before_r_state;
				}
			}
			break;
	}
	
	/*************************************** R/W DATA ******************************************/
	if(jxcp_s->Active == 1){
		// Read Data
		jxcp_s->Internal.Profinet.IN_Aux.BYTE_0[0] = jxcp_s->Input.BYTE_0.IN_0;
		jxcp_s->Internal.Profinet.IN_Aux.BYTE_0[1] = jxcp_s->Input.BYTE_0.IN_1;
		jxcp_s->Internal.Profinet.IN_Aux.BYTE_0[2] = jxcp_s->Input.BYTE_0.IN_2;
		jxcp_s->Internal.Profinet.IN_Aux.BYTE_0[3] = jxcp_s->Input.BYTE_0.IN_3;
		jxcp_s->Internal.Profinet.IN_Aux.BYTE_0[4] = jxcp_s->Input.BYTE_0.IN_4;
		jxcp_s->Internal.Profinet.IN_Aux.BYTE_0[5] = jxcp_s->Input.BYTE_0.IN_5;
		
		jxcp_s->Internal.Profinet.IN_Aux.BYTE_1[0] = jxcp_s->Input.BYTE_1.HOLD;
		jxcp_s->Internal.Profinet.IN_Aux.BYTE_1[1] = jxcp_s->Input.BYTE_1.SVON;
		jxcp_s->Internal.Profinet.IN_Aux.BYTE_1[2] = jxcp_s->Input.BYTE_1.DRIVE;
		jxcp_s->Internal.Profinet.IN_Aux.BYTE_1[3] = jxcp_s->Input.BYTE_1.RESET;
		jxcp_s->Internal.Profinet.IN_Aux.BYTE_1[4] = jxcp_s->Input.BYTE_1.SETUP;
		jxcp_s->Internal.Profinet.IN_Aux.BYTE_1[5] = jxcp_s->Input.BYTE_1.JOG_Neg;
		jxcp_s->Internal.Profinet.IN_Aux.BYTE_1[6] = jxcp_s->Input.BYTE_1.JOG_Pos;
		jxcp_s->Internal.Profinet.IN_Aux.BYTE_1[7] = jxcp_s->Input.BYTE_1.FLHGT;
		
		jxcp_s->Internal.Profinet.IN_Aux.BYTE_2[0] = 0;
		jxcp_s->Internal.Profinet.IN_Aux.BYTE_2[1] = jxcp_s->Input.BYTE_2.Speed_Restriction;
		jxcp_s->Internal.Profinet.IN_Aux.BYTE_2[2] = 0;
		jxcp_s->Internal.Profinet.IN_Aux.BYTE_2[3] = 0;
		jxcp_s->Internal.Profinet.IN_Aux.BYTE_2[4] = jxcp_s->Input.BYTE_2.Movement_Mode;
		jxcp_s->Internal.Profinet.IN_Aux.BYTE_2[5] = jxcp_s->Input.BYTE_2.Speed;
		jxcp_s->Internal.Profinet.IN_Aux.BYTE_2[6] = jxcp_s->Input.BYTE_2.Position;
		jxcp_s->Internal.Profinet.IN_Aux.BYTE_2[7] = jxcp_s->Input.BYTE_2.Acceleration;
		
		jxcp_s->Internal.Profinet.IN_Aux.BYTE_3[0] = jxcp_s->Input.BYTE_3.Deceleration;
		jxcp_s->Internal.Profinet.IN_Aux.BYTE_3[1] = jxcp_s->Input.BYTE_3.Pushing_Force;
		jxcp_s->Internal.Profinet.IN_Aux.BYTE_3[2] = jxcp_s->Input.BYTE_3.Trigger_LV;
		jxcp_s->Internal.Profinet.IN_Aux.BYTE_3[3] = jxcp_s->Input.BYTE_3.Pushing_Speed;
		jxcp_s->Internal.Profinet.IN_Aux.BYTE_3[4] = jxcp_s->Input.BYTE_3.Moving_Force;
		jxcp_s->Internal.Profinet.IN_Aux.BYTE_3[5] = jxcp_s->Input.BYTE_3.Area_out_1;
		jxcp_s->Internal.Profinet.IN_Aux.BYTE_3[6] = jxcp_s->Input.BYTE_3.Area_out_2;
		jxcp_s->Internal.Profinet.IN_Aux.BYTE_3[7] = jxcp_s->Input.BYTE_3.IN_Pos_Tolerance;
		
		jxcp_s->Output.BYTE_0.OUT_0 = jxcp_s->Internal.Profinet.OUT_Aux.BYTE_0[0];
		jxcp_s->Output.BYTE_0.OUT_1 = jxcp_s->Internal.Profinet.OUT_Aux.BYTE_0[1];
		jxcp_s->Output.BYTE_0.OUT_2 = jxcp_s->Internal.Profinet.OUT_Aux.BYTE_0[2];
		jxcp_s->Output.BYTE_0.OUT_3 = jxcp_s->Internal.Profinet.OUT_Aux.BYTE_0[3];
		jxcp_s->Output.BYTE_0.OUT_4 = jxcp_s->Internal.Profinet.OUT_Aux.BYTE_0[4];
		jxcp_s->Output.BYTE_0.OUT_5 = jxcp_s->Internal.Profinet.OUT_Aux.BYTE_0[5];
		
		jxcp_s->Output.BYTE_1.BUSY = jxcp_s->Internal.Profinet.OUT_Aux.BYTE_1[0];
		jxcp_s->Output.BYTE_1.SVRE = jxcp_s->Internal.Profinet.OUT_Aux.BYTE_1[1];
		jxcp_s->Output.BYTE_1.SETON = jxcp_s->Internal.Profinet.OUT_Aux.BYTE_1[2];
		jxcp_s->Output.BYTE_1.INP = jxcp_s->Internal.Profinet.OUT_Aux.BYTE_1[3];
		jxcp_s->Output.BYTE_1.AREA = jxcp_s->Internal.Profinet.OUT_Aux.BYTE_1[4];
		jxcp_s->Output.BYTE_1.WAREA = jxcp_s->Internal.Profinet.OUT_Aux.BYTE_1[5];
		jxcp_s->Output.BYTE_1.ESTOP = jxcp_s->Internal.Profinet.OUT_Aux.BYTE_1[6];
		jxcp_s->Output.BYTE_1.ALARM = jxcp_s->Internal.Profinet.OUT_Aux.BYTE_1[7];
		
		jxcp_s->Output.BYTE_2.READY = jxcp_s->Internal.Profinet.OUT_Aux.BYTE_2;
		
	}
}

// ************************************************************************************************************************************ //
// ------------------------------------------------------------  FUNCTIONS ------------------------------------------------------------ //
// ************************************************************************************************************************************ //

// ****************INITIALIZATION PARAMETERS **********************//
void initialization_jxcp(struct jxcp_smc_fb* jxcp_s)
{
	jxcp_s->Internal.actual_state = JXCP_INITIALIZATION_STATE;
	jxcp_s->Internal.before_state = JXCP_INITIALIZATION_STATE;
	
	jxcp_s->Internal.actual_r_state = JXCP_R_INITIALIZATION_STATE;
	jxcp_s->Internal.before_r_state = JXCP_R_INITIALIZATION_STATE;
	
	jxcp_s->Set_Parameters.Mode = NORMAL_MOVE_MODE;
	jxcp_s->Set_Parameters.Start = 0;
	jxcp_s->Set_Parameters.Stop = 0;
	
	jxcp_s->Input.BYTE_0.IN_0 = 0;
	jxcp_s->Input.BYTE_0.IN_1 = 0;
	jxcp_s->Input.BYTE_0.IN_2 = 0;
	jxcp_s->Input.BYTE_0.IN_3 = 0;
	jxcp_s->Input.BYTE_0.IN_4 = 0;
	jxcp_s->Input.BYTE_0.IN_5 = 0;
	
	jxcp_s->Input.BYTE_1.DRIVE = 0;
	jxcp_s->Input.BYTE_1.FLHGT = 0;
	jxcp_s->Input.BYTE_1.HOLD = 0;
	jxcp_s->Input.BYTE_1.JOG_Neg = 0;
	jxcp_s->Input.BYTE_1.JOG_Pos = 0;
	jxcp_s->Input.BYTE_1.RESET = 0;
	jxcp_s->Input.BYTE_1.SETUP = 0;
	jxcp_s->Input.BYTE_1.SVON = 0;
	
	jxcp_s->Input.BYTE_2.Acceleration = 0;
	jxcp_s->Input.BYTE_2.Movement_Mode = 0;
	jxcp_s->Input.BYTE_2.Position = 0;
	jxcp_s->Input.BYTE_2.Speed = 0;
	jxcp_s->Input.BYTE_2.Speed_Restriction = 0;
	
	jxcp_s->Input.BYTE_3.Area_out_1 = 0;
	jxcp_s->Input.BYTE_3.Area_out_2 = 0;
	jxcp_s->Input.BYTE_3.Deceleration = 0;
	jxcp_s->Input.BYTE_3.IN_Pos_Tolerance = 0;
	jxcp_s->Input.BYTE_3.Moving_Force = 0;
	jxcp_s->Input.BYTE_3.Pushing_Force = 0;
	jxcp_s->Input.BYTE_3.Pushing_Speed = 0;
	jxcp_s->Input.BYTE_3.Trigger_LV = 0;
	
	jxcp_s->Input.Acceleration = 0;
	jxcp_s->Input.AREA_1 = 0;
	jxcp_s->Input.AREA_2 = 0;
	jxcp_s->Input.Deceleration = 0;
	jxcp_s->Input.IN_Position = 0;
	jxcp_s->Input.Moving_Force = 0;
	jxcp_s->Input.Position = 0;
	jxcp_s->Input.Pushing_Force = 0;
	jxcp_s->Input.Pushing_Speed = 0;
	jxcp_s->Input.Speed = 0;
	jxcp_s->Input.Start_Flag = 0;
	jxcp_s->Input.Trigger_LV = 0;
	
	jxcp_s->Input.Movement_Mode.Absolute = 0;
	jxcp_s->Input.Movement_Mode.Relative = 0;
	
	jxcp_s->Active = 0;
	
	jxcp_s->Output.BYTE_0.OUT_0 = 0;
	jxcp_s->Output.BYTE_0.OUT_1 = 0;
	jxcp_s->Output.BYTE_0.OUT_2 = 0;
	jxcp_s->Output.BYTE_0.OUT_3 = 0;
	jxcp_s->Output.BYTE_0.OUT_4 = 0;
	jxcp_s->Output.BYTE_0.OUT_5 = 0;
	
	jxcp_s->Output.BYTE_1.ALARM = 0;
	jxcp_s->Output.BYTE_1.AREA  = 0;
	jxcp_s->Output.BYTE_1.BUSY  = 0;
	jxcp_s->Output.BYTE_1.ESTOP = 0;
	jxcp_s->Output.BYTE_1.INP   = 0;
	jxcp_s->Output.BYTE_1.SETON = 0;
	jxcp_s->Output.BYTE_1.SVRE  = 0;
	jxcp_s->Output.BYTE_1.WAREA = 0;
	
	jxcp_s->Output.BYTE_2.READY = 0;
	
	jxcp_s->Output.Current_Position = 0;
	jxcp_s->Output.Current_Pushing_Force = 0;
	jxcp_s->Output.Current_Speed = 0;
	jxcp_s->Output.Target_Position = 0;
	
	jxcp_s->Output.Warning.Alarm_1 = 0;
	jxcp_s->Output.Warning.Alarm_2 = 0;
	jxcp_s->Output.Warning.Alarm_3 = 0;
	jxcp_s->Output.Warning.Alarm_4 = 0;
	
	jxcp_s->PROFINET_Mapping_OUT.Output_Area[0] = 0;
	jxcp_s->PROFINET_Mapping_OUT.Output_Area[1] = 0;
	jxcp_s->PROFINET_Mapping_OUT.Output_Area[2] = 0;
	jxcp_s->PROFINET_Mapping_OUT.Output_Area[3] = 0;
	jxcp_s->PROFINET_Mapping_OUT.Output_Area[4] = 0;
	jxcp_s->PROFINET_Mapping_OUT.Output_Area[5] = 0;
	jxcp_s->PROFINET_Mapping_OUT.Output_Area[6] = 0;
	jxcp_s->PROFINET_Mapping_OUT.Output_Area[7] = 0;
	jxcp_s->PROFINET_Mapping_OUT.Output_Area[8] = 0;
	jxcp_s->PROFINET_Mapping_OUT.Output_Area[9] = 0;
	jxcp_s->PROFINET_Mapping_OUT.Output_Area[10] = 0;
	jxcp_s->PROFINET_Mapping_OUT.Output_Area[11] = 0;
	jxcp_s->PROFINET_Mapping_OUT.Output_Area[12] = 0;
	jxcp_s->PROFINET_Mapping_OUT.Output_Area[13] = 0;
	jxcp_s->PROFINET_Mapping_OUT.Output_Area[14] = 0;
	jxcp_s->PROFINET_Mapping_OUT.Output_Area[15] = 0;
	jxcp_s->PROFINET_Mapping_OUT.Output_Area[16] = 0;
	jxcp_s->PROFINET_Mapping_OUT.Output_Area[17] = 0;
	jxcp_s->PROFINET_Mapping_OUT.Output_Area[18] = 0;
	jxcp_s->PROFINET_Mapping_OUT.Output_Area[19] = 0;
	jxcp_s->PROFINET_Mapping_OUT.Output_Area[20] = 0;
	jxcp_s->PROFINET_Mapping_OUT.Output_Area[21] = 0;
	jxcp_s->PROFINET_Mapping_OUT.Output_Area[22] = 0;
	jxcp_s->PROFINET_Mapping_OUT.Output_Area[23] = 0;
	jxcp_s->PROFINET_Mapping_OUT.Output_Area[24] = 0;
	jxcp_s->PROFINET_Mapping_OUT.Output_Area[25] = 0;
	jxcp_s->PROFINET_Mapping_OUT.Output_Area[26] = 0;
	jxcp_s->PROFINET_Mapping_OUT.Output_Area[27] = 0;
	jxcp_s->PROFINET_Mapping_OUT.Output_Area[28] = 0;
	jxcp_s->PROFINET_Mapping_OUT.Output_Area[29] = 0;
	jxcp_s->PROFINET_Mapping_OUT.Output_Area[30] = 0;
	jxcp_s->PROFINET_Mapping_OUT.Output_Area[31] = 0;
	jxcp_s->PROFINET_Mapping_OUT.Output_Area[32] = 0;
	jxcp_s->PROFINET_Mapping_OUT.Output_Area[33] = 0;
	jxcp_s->PROFINET_Mapping_OUT.Output_Area[34] = 0;
	jxcp_s->PROFINET_Mapping_OUT.Output_Area[35] = 0;
}

void multiplne_usint_to_dec(struct multiplne_usint_to_dec* m_u2d){
	
	// Declaration {2 - variables}
	int i_2;
	
	int result_bin_byte_2[16];
	int result_bin_byte_21[8], result_bin_byte_22[8];
	
	USINT aux_get_byte_21, aux_get_byte_22;
	
	LREAL aux_set_dec_2 = 0;
	
	int n_2 = (int)(sizeof(result_bin_byte_2)/sizeof(result_bin_byte_2[0]));
	
	// Declaration {4 - variables}
	int i_4;
	
	int result_bin_byte_4[32];
	int result_bin_byte_41[8], result_bin_byte_42[8], result_bin_byte_43[8], result_bin_byte_44[8];
	
	USINT aux_get_byte_41, aux_get_byte_42, aux_get_byte_43, aux_get_byte_44;
	
	LREAL aux_set_dec_4 = 0;

	int n_4 = (int)(sizeof(result_bin_byte_4)/sizeof(result_bin_byte_4[0])); 
	
	switch(m_u2d->Count_of_VAR){
		case 2:
			{
				aux_get_byte_21 = m_u2d->USINT_1;
				aux_get_byte_22 = m_u2d->USINT_2;
				
				for(i_2 = 0; i_2 < (int)(sizeof(result_bin_byte_21)/sizeof(result_bin_byte_21[0])); i_2++){
					
					if(aux_get_byte_21 > 0){
						result_bin_byte_21[i_2] = fmod(aux_get_byte_21, 2);
						aux_get_byte_21 = aux_get_byte_21/2;
					}else{
						result_bin_byte_21[i_2] = 0;
					}
					
					if(aux_get_byte_22 > 0){
						result_bin_byte_22[i_2] = fmod(aux_get_byte_22, 2);
						aux_get_byte_22 = aux_get_byte_22/2;
					}else{
						result_bin_byte_22[i_2] = 0;
					}
				}
				
				for(i_2 = 0; i_2 < n_2; i_2++){
					if(i_2 >= 0 && i_2 < 8){
						result_bin_byte_2[i_2] = result_bin_byte_22[i_2];
					}else if(i_2 >= 8 && i_2 < 16){
						result_bin_byte_2[i_2] = result_bin_byte_21[i_2 - 8];
					}
				}
				
				
				for(i_2 = 0; i_2 < n_2; i_2++){
					if(result_bin_byte_2[i_2] == 1){
						aux_set_dec_2 = aux_set_dec_2 + pow(10, i_2);
					}
				}
				
				m_u2d->Result_Decimal = (LREAL)convert_bin_to_dec(aux_set_dec_2);
			}
			break;
		case 4:
			{
			
				aux_get_byte_41 = m_u2d->USINT_1;
				aux_get_byte_42 = m_u2d->USINT_2;
				aux_get_byte_43 = m_u2d->USINT_3;
				aux_get_byte_44 = m_u2d->USINT_4;
				
				for(i_4 = 0; i_4 < (int)(sizeof(result_bin_byte_41)/sizeof(result_bin_byte_41[0])); i_4++){
					
					if(aux_get_byte_41 > 0){
						result_bin_byte_41[i_4] = fmod(aux_get_byte_41, 2);
						aux_get_byte_41 = aux_get_byte_41/2;
					}else{
						result_bin_byte_41[i_4] = 0;
					}
					
					if(aux_get_byte_42 > 0){
						result_bin_byte_42[i_4] = fmod(aux_get_byte_42, 2);
						aux_get_byte_42 = aux_get_byte_42/2;
					}else{
						result_bin_byte_42[i_4] = 0;
					}
					
					if(aux_get_byte_43 > 0){
						result_bin_byte_43[i_4] = fmod(aux_get_byte_43, 2);
						aux_get_byte_43 = aux_get_byte_43/2;
					}else{
						result_bin_byte_43[i_4] = 0;
					}
					
					if(aux_get_byte_44 > 0){
						result_bin_byte_44[i_4] = fmod(aux_get_byte_44, 2);
						aux_get_byte_44 = aux_get_byte_44/2;
					}else{
						result_bin_byte_44[i_4] = 0;
					}
				}
				
				for(i_4 = 0; i_4 < n_4; i_4++){
					if(i_4 >= 0 && i_4 < 8){
						result_bin_byte_4[i_4] = result_bin_byte_44[i_4];
					}else if(i_4 >= 8 && i_4 < 16){
						result_bin_byte_4[i_4] = result_bin_byte_43[i_4 - 8];
					}else if(i_4 >= 16 && i_4 < 24){
						result_bin_byte_4[i_4] = result_bin_byte_42[i_4 - 16];
					}else if(i_4 >= 24 && i_4 < 32){
						result_bin_byte_4[i_4] = result_bin_byte_41[i_4 - 24];
					}
				}

				for(i_4 = 0; i_4 < n_4; i_4++){
					if(result_bin_byte_4[i_4] == 1){
						aux_set_dec_4 = aux_set_dec_4 + pow(10,i_4);
					}
				}
				
				m_u2d->Result_Decimal = ((LREAL)convert_bin_to_dec(aux_set_dec_4))*0.01;
			}
			break;
	}
}

void dec_to_multiple_usint(struct dec_to_multiple_usint* dec2u_m){
	
	// Declaration {2 - variables}
	int i_2;
	
	int result_bin_byte_2[16];
	
	int result_bin_byte_21[8], result_bin_byte_22[8];
	
	LREAL aux_get_dec_2;
	
	USINT aux_set_byte_21 = 0, aux_set_byte_22 = 0;
	

	int n_2 = (int)(sizeof(result_bin_byte_2)/sizeof(result_bin_byte_2[0])); 
	
	// Declaration {4 - variables}
	int i_4;
	
	int result_bin_byte_4[32];
	
	int result_bin_byte_41[8], result_bin_byte_42[8], result_bin_byte_43[8], result_bin_byte_44[8];
	
	LREAL aux_get_dec_4;
	
	USINT aux_set_byte_41 = 0, aux_set_byte_42 = 0, aux_set_byte_43 = 0, aux_set_byte_44 = 0;
	

	int n_4 = (int)(sizeof(result_bin_byte_4)/sizeof(result_bin_byte_4[0])); 
	
	switch(dec2u_m->Count_of_VAR){
		case 2:
			{
				aux_get_dec_2 = dec2u_m->Decimal;
			
				for(i_2 = 0; i_2 < n_2; i_2++){
					result_bin_byte_2[i_2] = fmod(aux_get_dec_2, 2);	
					aux_get_dec_2		   = aux_get_dec_2/2;
				}
			
				for(i_2 = 0; i_2 < n_2; i_2++){
					if(i_2 >= 0 && i_2 < 8){
						result_bin_byte_22[i_2]      = result_bin_byte_2[i_2];
					}else if(i_2 >= 8 && i_2 < 16){
						result_bin_byte_21[i_2 - 8]  = result_bin_byte_2[i_2];
					}
				}
			
				// ----- Byte 1 - 2 ----- //
				for(i_2 = 0; i_2 < (int)(sizeof(result_bin_byte_21)/sizeof(result_bin_byte_21[0])); i_2++){
					
					if(result_bin_byte_21[i_2] == 1){
						aux_set_byte_21 += pow(2, i_2);
					}
					
					if(result_bin_byte_22[i_2] == 1){
						aux_set_byte_22 += pow(2, i_2);
					}
					
				}
			
				dec2u_m->Result_USINT_1 = aux_set_byte_21;
				dec2u_m->Result_USINT_2 = aux_set_byte_22;	
			}
			break;
		case 4:
			{
				aux_get_dec_4 = dec2u_m->Decimal * 100;
			
				for(i_4 = 0; i_4 < n_4; i_4++){
					result_bin_byte_4[i_4] = fmod(aux_get_dec_4, 2);	
					aux_get_dec_4		   = aux_get_dec_4/2;
				}
			
				for(i_4 = 0; i_4 < n_4; i_4++){
					if(i_4 >= 0 && i_4 < 8){
						result_bin_byte_44[i_4]      = result_bin_byte_4[i_4];
					}else if(i_4 >= 8 && i_4 < 16){
						result_bin_byte_43[i_4 - 8]  = result_bin_byte_4[i_4];
					}else if(i_4 >= 16 && i_4 < 24){
						result_bin_byte_42[i_4 - 16] = result_bin_byte_4[i_4];
					}else if(i_4 >= 24 && i_4 < 32){
						result_bin_byte_41[i_4 - 24] = result_bin_byte_4[i_4];
					}
				}
			
				// ----- Byte 1 - 4 ----- //
				for(i_4 = 0; i_4 < (int)(sizeof(result_bin_byte_41)/sizeof(result_bin_byte_41[0])); i_4++){
					
					if(result_bin_byte_41[i_4] == 1){
						aux_set_byte_41 += pow(2, i_4);
					}
					
					if(result_bin_byte_42[i_4] == 1){
						aux_set_byte_42 += pow(2, i_4);
					}
					
					if(result_bin_byte_43[i_4] == 1){
						aux_set_byte_43 += pow(2, i_4);
					}
					
					if(result_bin_byte_44[i_4] == 1){
						aux_set_byte_44 += pow(2, i_4);
					}
				}
			
				dec2u_m->Result_USINT_1 = aux_set_byte_41;
				dec2u_m->Result_USINT_2 = aux_set_byte_42;
				dec2u_m->Result_USINT_3 = aux_set_byte_43;
				dec2u_m->Result_USINT_4 = aux_set_byte_44;	
			}
			break;
	}
}

DINT convert_bin_to_dec(LREAL variable) {
	DINT decimal_num = 0, i = 0, remainder;
	
	for(i = 0; variable > 0; i++)
	{
		remainder    = fmod(variable, 10);
		decimal_num += remainder * pow(2,i);
		variable    /= 10;
	}

	return decimal_num;
}
