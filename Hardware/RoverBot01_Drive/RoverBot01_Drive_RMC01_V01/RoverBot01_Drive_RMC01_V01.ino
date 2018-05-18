#include <Servo.h>

#include "pin_definition.h"
#include "car.h"
#include "OptDevice.h"
#include "hc12.h"
#include "buzzer.h"
#include "PowerManager.h"


bool bFront_Led2 = false;		//White LED On/Off 플레그 변수
int i, j;						//for general loop
/////////////////////////////////////////////////////////////////////


//전원인가 후 loop() 함수 실행 에 한번만 실행 되는 초기화 목적의 함수.
void setup() {
  
	//Communication Uart initialize
	{
		Serial.begin(9600);     //Using for debug
		Serial2.begin(9600);    //Using for HC-12 433MHz 
		delay(100);
	}
  
    //디지털 출력모드 지정
	{
		pinMode(FRONT_LED1, OUTPUT);
		pinMode(FRONT_LED2, OUTPUT);
		pinMode(REAR_LED1, OUTPUT);
		pinMode(STATUS_LED1, OUTPUT);
		pinMode(STATUS_LED2, OUTPUT);
		pinMode(STATUS_LED3, OUTPUT);
		pinMode(ENA, OUTPUT);
		pinMode(ENB, OUTPUT);
		pinMode(IN1, OUTPUT);
		pinMode(IN2, OUTPUT);
		pinMode(IN3, OUTPUT);
		pinMode(IN3, OUTPUT);
		pinMode(BUZZER, OUTPUT);
	}

	if ( iOptDevice1 == 1 ) {
		//디지털 출력모드 지정
		pinMode(LIGHT_LED, OUTPUT);
	}
	if (iOptDevice2 == 1 ) {
		//서모모터의 디지털 출력핀을 OUTPUT으로 설정
		pinMode(CAM_SERVO_V,OUTPUT);
		pinMode(CAM_SERVO_H,OUTPUT);
		//servo.h 라이브러리에 서모모터를 할당
		ServoV.attach(CAM_SERVO_V);
		ServoH.attach(CAM_SERVO_H);
		
		//서모모터 제어 변수들의 초기화.
		Servo_Hor_Pos = 0;	//수평위치 변수
		Servo_Ver_Pos = 0;	//수직위치 변수
  
		ServoH.write(90); //Horizon-Center
		ServoV.write(90); //Vertical-Center 
		Servo_Hor_Pos = 90;  //수평 센터 위치
		Servo_Ver_Pos = 90;  //수직 센터 위치
		
	}
	if (iOptDevice6 == 1) {
		Serial3.begin(9600);
	}

	//LED 전부 켜기
	{
		digitalWrite(FRONT_LED1, HIGH);	//RED
		digitalWrite(FRONT_LED2, HIGH);	//WHITE
		digitalWrite(REAR_LED1, HIGH);	//RED
		digitalWrite(STATUS_LED1, HIGH);	//RED
		digitalWrite(STATUS_LED2, HIGH);	//BLUE
		digitalWrite(STATUS_LED3, HIGH); //YELLOW  
	}

	if ( iOptDevice1 == 1 ) {
		digitalWrite(LIGHT_LED,HIGH); //LIGHT
	}
		
	//Emergency 알람 3번 울리기
	for ( i = 0; i < 3; i ++ ){
		vAlram(0);	
	};

	//LED 전부 끄기
	{
		digitalWrite(FRONT_LED1, LOW);
		digitalWrite(FRONT_LED2, LOW);
		digitalWrite(REAR_LED1, LOW);
		digitalWrite(STATUS_LED1, LOW);
		digitalWrite(STATUS_LED2, LOW);
		digitalWrite(STATUS_LED3, LOW);
	}
	if ( iOptDevice1 == 1 ) {
		digitalWrite(LIGHT_LED,LOW); //LIGHT
	}
  
}

int i_led0 = 0;		//LED blink 일정 간격으로 On/Off하기 위한 카운터

//setup() 함수 실행 후에 무한반복 수행되는 함수.
void loop() {
    
	//For Using Blink LED  
	i_led0 += 1;
	//Serial.println(i_led0);
	if ( i_led0 == 64 ) {    
		  digitalWrite(FRONT_LED1,HIGH);
		  digitalWrite(REAR_LED1,HIGH);
		  if ( iMode == 0 ) { 			//Drving Mode
			digitalWrite(STATUS_LED2,HIGH); 
		  } else if ( iMode == 1 ) {		//Another Mode(RobotArm or ChivyRadar)
			digitalWrite(STATUS_LED2,HIGH);
		  }
		  //digitalWrite(STATUS_LED3,HIGH);
	}
	else if (i_led0 == 1 ) {
		  digitalWrite(FRONT_LED1,LOW);
		  digitalWrite(REAR_LED1,LOW);
		  if ( iMode == 0 ) { 			//Drving Mode
			digitalWrite(STATUS_LED2,HIGH); 
		  } else if ( iMode == 1) {		//Another Mode(RobotArm or ChivyRadar)
			digitalWrite(STATUS_LED2,LOW);
		  }
		  //digitalWrite(STATUS_LED3,LOW);
	} 
	if ( i_led0 > 128 ) { i_led0 = 0;}	//일정한 LED의 Blink를 위한 카운터

	//HC-12 Receiving & Parsing.
	HC12_RECEIVE();

	//Get Voltage
	fVolt_Lvl = fCheck_Batt_Lvl(); 
	
	//Power LED blink.	
	vPwrLed_Blink(fVolt_Lvl, STATUS_LED1);
}