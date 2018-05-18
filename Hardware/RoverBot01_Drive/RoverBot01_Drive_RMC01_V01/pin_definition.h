#pragma once

//��ó���⸦ �̿��Ͽ� ����� �� ��ȣ�� ���ڿ��� ģȯ.
//LED ��� ���� ������ ����� ����.
#define FRONT_LED1 		34   	//Active Status, RED_BLINK
#define FRONT_LED2 		36   	//Light, WHITE
#define REAR_LED1  		37   	//Active Status, RED_BLINK
#define STATUS_LED1 	38  	//MODE1, POWER ON&STATUS
#define STATUS_LED2 	39  	//MODE1, On-Drive Mode, Blink-RobotArm  
#define STATUS_LED3 	40  	//MODE2, RF Status

//Li-Po Batt�� ���� �Ƴ��α� �Է��� ����.
#define VOLT_CHK		A0	
//������ ����� ����, Buzzer
#define BUZZER 			10

//Motor driver(L298N) ��� ���� AnalogWrite�� DigitalWrite �� ����. 
#define ENA 11   	//Motor A Enable�� �Ƴ��α� �����(0~255)
#define ENB 12   	//Motor B Enable�� �Ƴ��α� �����(0~255)
#define IN1 46   	//Motor A IN1 ������ �����(0,1)
#define IN2 47   	//Motor A IN2 ������ �����(0,1)
#define IN3 48   	//Motor B IN3 ������ �����(0,1)
#define IN4 49   	//Motor B IN3 ������ �����(0,1)