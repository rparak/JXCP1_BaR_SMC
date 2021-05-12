
TYPE
	jxcp_mv_alarm_str : 	STRUCT 
		Alarm_1 : USINT;
		Alarm_1_img : BOOL;
		Alarm_2 : USINT;
		Alarm_2_img : BOOL;
		Alarm_3 : USINT;
		Alarm_3_img : BOOL;
		Alarm_4 : USINT;
		Alarm_4_img : BOOL;
	END_STRUCT;
	jxcp_mv_get_param_str : 	STRUCT 
		Position : LREAL;
		Speed : LREAL;
		Force : LREAL;
		In_Pos : BOOL;
	END_STRUCT;
	jxcp_data_mv_str : 	STRUCT 
		Power : BOOL;
		Home : BOOL;
		isHome : BOOL;
		PowerOn : BOOL;
		Control : jxcp_mv_control_str;
		Write_Parameters : jxcp_mv_set_param_str;
		Read_Parameters : jxcp_mv_get_param_str;
		Alarm : jxcp_mv_alarm_str;
		Reset_Alarm : BOOL;
	END_STRUCT;
	jxcp_mv_cm_str : 	STRUCT 
		Start : BOOL;
		Stop : BOOL;
		Default_Param : BOOL;
	END_STRUCT;
	jxcp_mv_control_str : 	STRUCT 
		Jog_pos : BOOL;
		Jog_neg : BOOL;
		Cycle_Mode : jxcp_mv_cm_str;
	END_STRUCT;
	jxcp_mv_set_param_str : 	STRUCT 
		Position : LREAL;
		Speed : LREAL;
		Acceleration : LREAL;
		Deceleration : LREAL;
	END_STRUCT;
END_TYPE
