#include "transmission_ctrl.h"

void transmission_ctrl_init(transmission_ctrl_t *ctrl){
  ctrl->gear = FIRST_GEAR;
  ctrl->GRT = 3.42;   // gear ratio transmission 
  ctrl->GRD[0] = 2.97;  // gear ratio differential
  ctrl->GRD[1] = 2.07;  // gear ratio differential
  ctrl->GRD[2] = 1.43;  // gear ratio differential
  ctrl->GRD[3] = 1;  // gear ratio differential
  ctrl->GRD[4] = 0.84;  // gear ratio differential
  ctrl->CT = 1.613679;  // wheal circumference in meters
}
void transmission_ctrl_update_state(transmission_ctrl_t *ctrl){
		int a, b =0;
	switch(ctrl->gear){
		case FIRST_GEAR: {
			*ctrl->rpm = speed_2_rpm(ctrl->GRD[0], *ctrl->speed);		//update RPM from speed
			
			//change to second gear
			if (*ctrl->rpm >= 2500) {
				ctrl->gear = SECOND_GEAR;		//change gear
				*ctrl->rpm = 1000;					//set rpm to 1000
				updateminmax(ctrl);					//update min/max values
				ctrl->counts[0][1] = ctrl->counts[0][1] + 1;		//update number of transitions
			}
			
			//stay in first gear
			else {
				ctrl->gear = FIRST_GEAR;		//change gear
				updateminmax(ctrl);					//update min/max values
				ctrl->counts[0][0] = ctrl->counts[0][0] + 1;		//update number of transitions
			}
			break;
		}
		case SECOND_GEAR: {
			*ctrl->rpm = speed_2_rpm(ctrl->GRD[1], *ctrl->speed);	//update RPM from speed
			
			//change to third gear
			if (*ctrl->rpm >= 2500) {
				ctrl->gear = THIRD_GEAR;		//change gear
				*ctrl->rpm = 1000;					//set rpm to 1000
				updateminmax(ctrl);					//update min/max values
				ctrl->counts[1][2] = ctrl->counts[1][2] + 1;		//update number of transitions
			}
			
			//change to first gear
			else if (*ctrl->rpm <= 700) {
				ctrl->gear = FIRST_GEAR;		//change gear
				*ctrl->rpm = 1000;					//set rpm to 1000
				updateminmax(ctrl);					//update min/max values
				ctrl->counts[1][0] = ctrl->counts[1][0] + 1;		//update number of transitions
			}
			
			//stay in second gear
			else {
				ctrl->gear = SECOND_GEAR;		//change gear
				updateminmax(ctrl);					//update min/max values
				ctrl->counts[1][1] = ctrl->counts[1][1] + 1;		//update number of transitions
			}
			break;
		}
		case THIRD_GEAR: {
			*ctrl->rpm = speed_2_rpm(ctrl->GRD[2], *ctrl->speed);	//update RPM from speed
			
			//change to fourth gear
			if (*ctrl->rpm >= 2500) {
				ctrl->gear = FOURTH_GEAR;		//change gear
				*ctrl->rpm = 1000;					//set rpm to 1000
				updateminmax(ctrl);					//update min/max values
				ctrl->counts[2][3] = ctrl->counts[2][3] + 1;		//update number of transitions
			}
			
			//change to second gear
			else if (*ctrl->rpm <= 700) {
				ctrl->gear = SECOND_GEAR;		//change gear
				*ctrl->rpm = 1000;					//set rpm to 1000v
				updateminmax(ctrl);					//update min/max values
				ctrl->counts[2][1] = ctrl->counts[2][1] + 1;		//update number of transitions
			}
			
			//stay in third gear
			else {
				ctrl->gear = THIRD_GEAR;		//change gear
				updateminmax(ctrl);					//update min/max values
				ctrl->counts[2][2] = ctrl->counts[2][2] + 1;		//update number of transitions
			}
			break;
		}
		case FOURTH_GEAR: {
			*ctrl->rpm = speed_2_rpm(ctrl->GRD[3], *ctrl->speed);	//update RPM from speed
			
			//change to fifth gear
			if (*ctrl->rpm >= 2500) {
				ctrl->gear = FIFTH_GEAR;		//change gear
				*ctrl->rpm = 1000;					//set rpm to 1000
				updateminmax(ctrl);					//update min/max values
				ctrl->counts[3][4] = ctrl->counts[3][4] + 1;		//update number of transitions
			}
			
			//change to third gear
			else if (*ctrl->rpm <= 700) {
				ctrl->gear = THIRD_GEAR;		//change gear
				*ctrl->rpm = 1000;					//set rpm to 1000
				updateminmax(ctrl);					//update min/max values
				ctrl->counts[3][2] = ctrl->counts[3][2] + 1;		//update number of transitions
			}
			
			//stay in fourth gear
			else {
				ctrl->gear = FOURTH_GEAR;		//change gear
				updateminmax(ctrl);					//update min/max values
				ctrl->counts[3][3] = ctrl->counts[3][3] + 1;		//update number of transitions
			}
			break;
		}
		case FIFTH_GEAR: {
			*ctrl->rpm = speed_2_rpm(ctrl->GRD[4], *ctrl->speed);	//update RPM from speed
			
			//change to fourth gear
			if (*ctrl->rpm <= 2000) {
				ctrl->gear = FOURTH_GEAR;		//change gear
				*ctrl->rpm = 1000;					//set rpm to 1000
				updateminmax(ctrl);					//update min/max values
				ctrl->counts[4][3] = ctrl->counts[4][3] + 1;		//update number of transitions
			}
			
			//stay in fifth gear
			else {
				ctrl->gear = FIFTH_GEAR;		//change gear
				updateminmax(ctrl);					//update min/max values
				ctrl->counts[4][4] = ctrl->counts[4][4] + 1;		//update number of transitions
			}
			break;
		}
	}
	
	// Prints additional task final counts as a table into console log
	printf ("Table:   Speed1   Speed2   Speed3   Speed4   Speed5   min spd    max spd    min rpm    max rpm  \n"); 
		for (a=0; a<5; a++){
			printf("Speed %d: ", a+1);
				for (b=0; b<9; b++){
				printf("%f ", ctrl->counts[a][b]);
				}
				printf("\n");
		}
}

uint32_t speed_2_rpm(float grd, float speed){
  uint32_t _rpm;
  // speed = rpm * CT * 3.6 / 60 / GRT / GRD
  // rpm = speed * GRT * GRD * 60 / CT / 3.6 = speed * GRD * 35.32
  _rpm = (uint32_t) (35.32f * speed * grd);
  return _rpm;
}


void updateminmax(transmission_ctrl_t *ctrl) {
	int temp;				//holds row index
	
	// initializes row index value based on gear
	switch (ctrl->gear) {
		case FIRST_GEAR: {
			temp = 0;
			break;
		}
		case SECOND_GEAR: {
			temp = 1;
			break;
		}
		case THIRD_GEAR: {
			temp = 2;
			break;
		}
		case FOURTH_GEAR: {
			temp = 3;
			break;
		}
		case FIFTH_GEAR: {
			temp = 4;
			break;
		}
	}
	
	// min speed index = 5
	// max speed index = 6
	// min rpm index = 7
	// max rpm index = 8
	
	//update min speed
	if (*ctrl->speed < ctrl->counts[temp][5]) {
		ctrl->counts[temp][5] = *ctrl->speed;
	}
			
	//update max speed
	if (*ctrl->speed > ctrl->counts[temp][6]) {
		ctrl->counts[temp][6] = *ctrl->speed;
	}
			
	//update min rpm
	if (*ctrl->rpm < ctrl->counts[temp][7]) {
		ctrl->counts[temp][7] = *ctrl->rpm;
	}
			
	//update max rpm
	if (*ctrl->rpm > ctrl->counts[temp][8]) {
		ctrl->counts[temp][8] = *ctrl->rpm;
	}
}