#pragma once

//////////////////////////////////////////////////////////////////
//추가적인 장치를 이용시 추가선언한다.
//////////////////////////////////////////////////////////////////
int iOptDevice1 = 1;            //AddDevice 1 : Light
int iOptDevice2 = 1;            //AddDevice 2 : Pan/Tilt Servo motor
int iOptDevice3 = 0;            //AddDevice 3 : FPV RELAY1
int iOptDevice4 = 0;		//AddDevice 4 : SMART Drive(with UltraSoinic, Infrared) 
int iOptDevice5 = 0;		//AddDevice 5 : RobotArm1/RobotArm2
int iOptDevice6 = 1;			//AddDevice 6 : GPS
							//		~  
							//////////////////////////////////////////////////////////////////

							/////////////////////////////////////
							//Add device, iOptDevice 1
#define LIGHT_LED       41	//Option Light

							/////////////////////////////////////
							//Add device, iOptDevice 2
#define CAM_SERVO_H   	8		//디지털 출력핀 지정, Vertical(Tilt) Servo
#define CAM_SERVO_V   	9		//디지털 출력핀 지정, Horizontal(Pan) Servo
							//For Cam tilt/pan Servo Motor 	
Servo ServoV;					//Tilt
Servo ServoH;					//Pan	
int Servo_Hor_Pos;
int Servo_Ver_Pos;
int Servo_Hor_Pos_Old;
int Servo_Ver_Pos_Old;
int Servo_Hor_Cent = 100;
int Servo_Ver_Cent = 80;

/////////////////////////////////////
//Add device, iOptDevice 3
//		~

/////////////////////////////////////
//Add device, iOptDevice 4
//		~

/////////////////////////////////////
//Add device, iPotDeviec 5
//		~

/////////////////////////////////////
//Add device, iPotDeviec 6
//	