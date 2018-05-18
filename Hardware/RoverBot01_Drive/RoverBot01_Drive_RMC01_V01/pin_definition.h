#pragma once

//전처리기를 이용하여 사용할 핀 번호를 문자열로 친환.
//LED 제어를 위한 디지털 출력핀 정의.
#define FRONT_LED1 		34   	//Active Status, RED_BLINK
#define FRONT_LED2 		36   	//Light, WHITE
#define REAR_LED1  		37   	//Active Status, RED_BLINK
#define STATUS_LED1 	38  	//MODE1, POWER ON&STATUS
#define STATUS_LED2 	39  	//MODE1, On-Drive Mode, Blink-RobotArm  
#define STATUS_LED3 	40  	//MODE2, RF Status

//Li-Po Batt의 전압 아날로그 입력핀 지정.
#define VOLT_CHK		A0	
//디지털 출력핀 지정, Buzzer
#define BUZZER 			10

//Motor driver(L298N) 제어를 위한 AnalogWrite및 DigitalWrite 핀 정의. 
#define ENA 11   	//Motor A Enable용 아날로그 출력핀(0~255)
#define ENB 12   	//Motor B Enable용 아날로그 출력핀(0~255)
#define IN1 46   	//Motor A IN1 디지털 출력핀(0,1)
#define IN2 47   	//Motor A IN2 디지털 출력핀(0,1)
#define IN3 48   	//Motor B IN3 디지털 출력핀(0,1)
#define IN4 49   	//Motor B IN3 디지털 출력핀(0,1)