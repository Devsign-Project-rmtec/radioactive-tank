#include <Servo.h>

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

//주행모드의 정의.
#define CAR_DIR_FW 0		//전진
#define CAR_DIR_BK 1		//후진
#define CAR_DIR_LF 2		//좌선회
#define CAR_DIR_RF 3		//우선회
#define CAR_DIR_ST 4		//정지

//Defalult value 정의.
int g_carDirection = CAR_DIR_ST;	//주행모드 변수 및 정지를 기본값으로 함
int g_carSpeed_l = 0;				//좌측모터 속도 아날로그(0~255) 변수
int g_carSpeed_r = 0;				//우측모터 속도 아날로그(0~255) 변수

//전원 관련 변수 선언
int   iBatt_Adc = 0;     		//리듐이온전지 전압의 아날로그 ADC값
float fVolt_Lvl = 0.0f;			//ADC를 전압으로 변경 값
bool  bPwrLED_Status = false;	//LED On/Off 상태를 저장
int iPwrBlink_Cnt = 0;

//Bzuzzer 음계
int iNote[] = {2093,2349,2637,2793,3136,3520,3951,4186}; //도레미파솔라시도
int iFreq = 150;				//가변주파수 저장 변수
bool bAlram_Flag = false;       //알람용 플레그 변수

//LED 관련 변수
bool bFront_Led2 = false;		//White LED On/Off 플레그 변수

//HC-12 433MHz Serial Wireless Communication Module.(for receiver pasing)
int iActPos = 80;		    	//Analog 출력값이 80이상이 되여야만 모터로 출력.
int first_idx = 99;				//수신된 문자열중 "$RTx"이 몇번째 인덱스에 있는지 확인
int second_idx = 99;			//수신된 문자열중 "$END"이 몇번째 인텍스에 있는지 확인
bool rcv_state = 0;  			//수신된 문자열이 있다면...
int rcv_cnt = 0;				//수신상태 체크 카운터.
int act_sen_cnt = 0;			//버튼등의 수동신호의 입력 지연.
int act_sen_val = 10;			//act_sen_cnt를 초기화 후 10이하여야 입력값으로 함.

int X1v, Y1v, Z1v;				//Left Joystick value - X, Y, Z
int X2v, Y2v, Z2v;				//Right Joystick value - X, Y, Z
int LPOTv, RPOTv;				//Left Potentiometer, Rright Potentiometer.
int LBUv, RBUv, LBDv, RBDv;		//Left Btn UP, Right Btn UP, Left Btn Down, Right Btn Down
int LTOGv, RTOGv;				//Left Toggle S/W, Right Toggle S/W
int iMode = 1; 					////1-Drive Mode, 0-Other Mode. ex)RobotArm, Auto Drive...
String Rcv_Str;					//HC-12로부터 수신문자열을 저장
int get_val[20] = {0};			//HC-12로부터 수신문자열의 정보를 분리하여 숫자정수로 저장

int set_val;					//strotok에서으 토큰의 갯수를 세기위한 변수
char rx_buf[64] = {0}; 			//Rcv_Str의 문자열을 문자열형 변수로 저장
char ms[64] = {0};				//rx_buf배열의 값으 메모리로 출력하기 위한 변수
char cs[16] = {0};				//strtok()에 의해 추출된 값을 저장
char *p;                        //strtok()에 의해 추출된 값의 포인터 변수
char str[64] = {0};				//디버깅을 위한 문자열형 변수

int i, j;						//for general loop

//////////////////////////////////////////////////////////////////
//추가적인 장치를 이용시 추가선언한다.
//////////////////////////////////////////////////////////////////
int iOptDevice1 = 1;            //AddDevice 1 : Light
int iOptDevice2 = 1;            //AddDevice 2 : Pan/Tilt Servo motor
int iOptDevice3 = 0;            //AddDevice 3 : FPV RELAY1
int iOptDevice4 = 0;		//AddDevice 4 : SMART Drive(with UltraSoinic, Infrared) 
int iOptDevice5 = 0;		//AddDevice 5 : RobotArm1/RobotArm2
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

/////////////////////////////////////////////////////////////////////

//A0 입력 전압을 ADC로 저장 및 ADC를 Voltage로 계산하는 함수 선언
float fCheck_Batt_Lvl();		
 
//Voltage 수준(level) 따라, LED 상태를 제어하는 함수 선언
void vPwrLed_Blink(float fVolt);

//알람 제어용 함수 선언 : 0-Emegency, 1-도레미~, 2-50Hz, 3-400Hz, 4-1000Hz
void vAlram(int kind);

//HC-12 Receiving & Parsing.
void HC12_RECEIVE();

void car_update();	//설정값 반영(MotorA, MotorB)
void car_forward();     //IN1-HIGH, IN2-LOW, IN3-HIGH, IN4-LOW
void car_backward();    //IN1-LOW, IN2-HIGH, IN3-LOW, IN4-HIGH 
void car_left();        //IN1-HIGH, IN2-LOW, IN3-LOW, IN4-HIGH
void car_right();       //IN1-LOW, IN2-HIGH, IN3-HIGH, IN4-LOW
void car_stop();         //ENA-0, ENB-0

//전원인가 후 loop() 함수 실행 에 한번만 실행 되는 초기화 목적의 함수.
void setup() {
  
	//Communication Uart initialize
	Serial.begin(9600);     //Using for debug
	Serial2.begin(9600);    //Using for HC-12 433MHz 
	delay(100);
  
    //디지털 출력모드 지정
	pinMode(FRONT_LED1,  OUTPUT);
	pinMode(FRONT_LED2,  OUTPUT);	
	pinMode(REAR_LED1,   OUTPUT);
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

	//LED 전부 켜기
	digitalWrite(FRONT_LED1, HIGH);	//RED
	digitalWrite(FRONT_LED2, HIGH);	//WHITE
	digitalWrite(REAR_LED1,  HIGH);	//RED
	digitalWrite(STATUS_LED1,HIGH);	//RED
	digitalWrite(STATUS_LED2,HIGH);	//BLUE
	digitalWrite(STATUS_LED3,HIGH); //YELLOW  
	if ( iOptDevice1 == 1 ) {
		digitalWrite(LIGHT_LED,HIGH); //LIGHT
	}
		
	//Emergency 알람 3번 울리기
	for ( i = 0; i < 3; i ++ ){
		vAlram(0);	
	};

	//LED 전부 끄기
	digitalWrite(FRONT_LED1, LOW);
	digitalWrite(FRONT_LED2, LOW);
	digitalWrite(REAR_LED1,  LOW);
	digitalWrite(STATUS_LED1,LOW);
	digitalWrite(STATUS_LED2,LOW);
	digitalWrite(STATUS_LED3,LOW);
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
	} else if (i_led0 == 1 ) {
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
	vPwrLed_Blink(fVolt_Lvl);
}

//Get sensing for li-Po battery
float fCheck_Batt_Lvl(){

	float fTemp_Lvl = 0.0f;	
	iBatt_Adc = analogRead(VOLT_CHK); 		//A0, 아날로그값 센싱(sensing)
	delay(3);								//지연시간 10ms
    fTemp_Lvl = (iBatt_Adc/40.92f) ;        //Resolution = 1023/25V = 40,92
	//Serial.print(iBatt_Adc);                //for debug
	//Serial.print("      ");                 //for debug
	//Serial.println(fTemp_Lvl);              //for debug 
	return fTemp_Lvl;						//계산된 전압값을 호출한 쪽에 리턴
	
}

//전압값에 따른 LED 깜박거림 제어 함수.
void vPwrLed_Blink(float fVolt)
{
  
  //Fullcharge voltate : 12.6V, Discharge voltage - 8.4V
  //11.0V - always on Power LED.
  //10.5V - lower blink Power LED.
  // 9.6V - Quickly blink Power LED & Sounding buzzer.
  
  if ( fVolt > 11.0 ) {                               //Always ON
    digitalWrite(STATUS_LED1, HIGH);
    iPwrBlink_Cnt = 0;
  } else if ( (fVolt <= 11.0) && (fVolt >10.5 ) ){    //Lower Blink
  
    if ( iPwrBlink_Cnt == 20 ) {
      if ( bPwrLED_Status == false ) {
	digitalWrite(STATUS_LED1,HIGH); 
	bPwrLED_Status = true;
      } else if( bPwrLED_Status == true ) {
	digitalWrite(STATUS_LED1,LOW);	
	bPwrLED_Status = false;
      }
      iPwrBlink_Cnt = 0;
    }
    
  } else if ( (fVolt <= 10.5) && (fVolt > 9.6) ){  	//fast Blink
    if ( iPwrBlink_Cnt == 2 ) {
      if ( bPwrLED_Status == false ) {
	digitalWrite(STATUS_LED1,HIGH); 
	bPwrLED_Status = true;
      } else if( bPwrLED_Status == true ) {
	digitalWrite(STATUS_LED1,LOW);	
	bPwrLED_Status = false;
      }
      iPwrBlink_Cnt = 0;
    }
	vAlram(2);
  } else {                                         	//Alywas OFF
    digitalWrite(STATUS_LED1, LOW);
	vAlram(0);
  }
  
  iPwrBlink_Cnt += 1;
}

void vAlram(int kind){
	
	if ( kind == 0 ) {			//Emergency Alram
		for ( iFreq = 150; iFreq < 1800; iFreq++ ) {	
			tone(BUZZER, iFreq, 10);	
		} 
		for ( iFreq = 1800; iFreq >= 150; iFreq-- ) {	
			tone(BUZZER, iFreq, 10);	
		} 	
	} else if ( kind == 1 ) {	//a note
		int elementCount = sizeof(iNote) / sizeof(int);
		for ( i=0; i < elementCount; i++)    //note를 play
		{
			tone(BUZZER,iNote[i],500);
			delay(750);
		}
	} else if ( kind == 2 )	{	//50Hz
		tone(BUZZER,100,10);
	} else if ( kind == 3 )	{	//400Hz
		tone(BUZZER,400,1000);
	} else if ( kind == 4 ) {   //1000Hz
		tone(BUZZER,1000,1000);
	}

}

  
//HC-12 433MHz Serial Wireless Communication Module 
void HC12_RECEIVE()			    //receiving & data parsing.
{

	rcv_state = 0;				//수신상태를 초기화.
	Rcv_Str = "";  				//수신될 문자열 변수의 초기화.
	
	Serial2.setTimeout(30);						//Timeout을 30ms로 설정한다.
	Rcv_Str = Serial2.readStringUntil('\n');  	//문자열중 "\n"이 올 때까지 기다린다.
	
	/*
	수신문자열을 분리하여 저장할 변수 : 
	X1v, Y1v, Z1v, X2v, Y2v, Z2v, LPOTv, RPOTv, LBUv, RBUv, LBDv, RBDv, LTOGv, RTOGv
	수신문자열 예 : 
	$RTx,512,520,1,508,511,1,0,0,1,1,1,1,1,1,$END  - 43자리 문자열
	수신문자열 예 : 
	$RTx,5,6,1,7,8,1,0,0,1,1,1,1,1,1,$END  - 37자리 최소 문자열 길이
	수신문자열 예 : 
	$RTx,1023,1023,1,1023,1023,1,1023,1023,1,1,1,1,1,1,$END  - 55자리 문자열
	*/
	
	first_idx = 99;								//"$RTx"가 문자열중에서 몇번째에 있는지...
	second_idx = 99;							//"$END"가 문자열중에서 몇번째에 있는지...
	first_idx = Rcv_Str.indexOf("$RTx");		//수신된 문자열중 "$RTx"가 있는지 확인
	second_idx = Rcv_Str.lastIndexOf("$END");	//수신된 문자열중 "$END"가 있는지 확인
	
	
	set_val = 0;		//추출된 문자의 수를 세고 배열의 인덱스로 사용
	/*
	수신 문자열중 "$RTx"가 첫번째에, "$END"가 문자열중 33번보다 크다면 정상 수신으로 판단
	콤마를 기준으로 문자를 분리 하기위해 문자열을 char형 변수에 저장
	*/
	if ( (first_idx == 0) && (second_idx >= 33) ) {    
	
		rcv_state = 1;  //문자열중 "$RTx"와 "$END"가 있다면 수신 성공으로 판단
		rcv_cnt = 0;    //수신 상태 카운트 리셋.
		
		for ( i = 0; i < second_idx; i++) {
			rx_buf[i] = Rcv_Str[i];				//Rcv_Str의 문자열을 문자열형 변수로 저장
		}
		sprintf(ms,rx_buf);		         		//rx_buf의 문자열을 메모리에 저장
		
		/*
		strtok 함수 원형 : char *strtok(char *_Str, const char *_Delim);
		strtok() 함수는 _Str과 _Delim을 쪼개어 토큰으로 만든 후, 비교하여 같은 문자를 만났을 때,
		그 문자를 NULL값으로 바꾸는 함수이다. 
		*/

		//첫 호출시에는 대상문자열(_Str)과 자르기를 위한 문자(_Delim)를 인수로 전달
		p = strtok(ms,",");                     //First Token의 값을 포인트 p에 저장
		sprintf(cs,"%s",p);						//포인트 p의 값을 cs에 저장
		get_val[set_val] = atoi(cs);			//$RTx는 숫자가 아니므로 0으로 반환됨

		//이후에는 _Str을 NULL으로, Delim은 자르기를 위한 문자를 인수로 전달한다.
		//더이상 반환활 값이 없을 때까지 구분문자를 추출한다.
		while((p = strtok(NULL,",")) != NULL){  //another token 추출	
			set_val++;							//추출된 구문문자(token) 수 카운트
			sprintf(cs,"%s",p);					//포인트 p의 값을 문자열형 변수 cs에 저장
			get_val[set_val] = atoi(cs);        //문자열형 cs의 값을 정수로 변환하여 저장
			if ( set_val == 14 ) { break;}      //받을 구문문자가 14이면 빠져나감
		}
	  
		set_val = 0;
					
		//for debug
		/*
		sprintf(str,"OK %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
							get_val[1], get_val[2], get_val[3], 
							get_val[4], get_val[5], get_val[6], 
							get_val[7], get_val[8],
							get_val[9], get_val[10], get_val[11], get_val[12],
							get_val[13], get_val[14]);
		Serial.println(str);
		*/

		X1v   = get_val[1];		//Left Joystick X - Analog value
		Y1v   = get_val[2];		//Left Joystick Y - Analog value
		Z1v   = get_val[3];		//Left Joystick Z - Digital value
		X2v   = get_val[4];		//Right Joystick X - Analog value
		Y2v   = get_val[5];		//Right Joystick Y - Analog value
		Z2v   = get_val[6];		//Right Joystick Z - Digital value
		LPOTv = get_val[7];     //Left Potentiometer - Analog value
		RPOTv = get_val[8];     //Right Potentiometer - Analog value
		LBUv  = get_val[9];     //Left Up Button - Digital value 
		RBUv  = get_val[10];    //Right Up Button - Digital value   
		LBDv  = get_val[11];    //Left Down Button - Digital value   
		RBDv  = get_val[12];    //Right Down Button - Digital value   
		LTOGv = get_val[13];    //Left Toggle S/W - Digtal value  
		RTOGv = get_val[14];    //Right Toggle S/W - Digtal value  				
		
            digitalWrite(STATUS_LED3, HIGH);				
	} else {//20번 수신 체크 실패면 알람 및 드라이브 정지를	
	        digitalWrite(STATUS_LED3, LOW);				
		if( rcv_cnt > 20 ) {
			g_carDirection = CAR_DIR_ST;
			g_carSpeed_l = 0;
			g_carSpeed_r = 0; 		
			car_update();
			vAlram(3);			
		}
		rcv_cnt += 1;
	}
	
	//정상정인 수신상태이고 iMode drive mode라면
	if ( ( rcv_state == 1 ) && ( iMode == 0 ) ){ 


		//X1v(Joystick X값 범위) : Left(0)-Cent(512)-Right(1023)  		
		//Y1v(Joystick X값 범위) :   Up(0)-Cent(512)-Down(1023)			
		//아날로그 출력 범위(0~255)로 계산
		X1v = round((511 - X1v) / 2); //255 ~ 0, 0 ~ -255
		Y1v = round((511 - Y1v) / 2); //255 ~ 0, 0 ~ -255
		
		////////////////////////////////////////////////////////////
		//1.Forward(255~0)
		if ( Y1v >= iActPos ) {             //iActPos = 80 이상 되어야 모터 동작          		
		
			g_carDirection = CAR_DIR_FW;      			
			//1.1.turn left forward        
			if ( X1v >= iActPos ) {                     	
				g_carSpeed_l = Y1v - int(X1v);        
				g_carSpeed_r = Y1v + int(X1v); 				
			//1.2.turn rigit forward          
			} else if (X1v <= -iActPos ) {              	
				X1v =  int(abs(X1v));       //convert plus
				g_carSpeed_l = Y1v + int(X1v);        
				g_carSpeed_r = Y1v - int(X1v);        		
			//1.3.Direct forward	
			} else {                                		        
				g_carSpeed_l = Y1v;
				g_carSpeed_r = Y1v;
			}
		////////////////////////////////////////////////////////////	
		//2.Backward(0~-255)
		} else if ( Y1v <= -iActPos ) {               		
		
			g_carDirection = CAR_DIR_BK;      			
			Y1v = int(abs(Y1v));           //convert plus
			//2.1.turn left backward
			if ( X1v >= iActPos ) {                     		       
				g_carSpeed_l = Y1v - int(X1v);        
				g_carSpeed_r = Y1v + int(X1v);				
			//2.2.furn rigit backward	
			} else if (X1v <= -iActPos ) {              		                
				X1v = int(abs(X1v));            //convert plus    
				g_carSpeed_l = Y1v + int(X1v);        
				g_carSpeed_r = Y1v - int(X1v);        
			//2.3.Direct backward	
			} else {                                 		
				g_carSpeed_l = Y1v;
				g_carSpeed_r = Y1v;
			}   
		////////////////////////////////////////////////////////////
		//3.turn left or right and stop	
		} else if ( (Y1v < iActPos) && (Y1v > (0-iActPos)) ){   
			//3.1.turn left 
			if ( X1v >= iActPos ) {                      		         
				g_carDirection = CAR_DIR_LF;  
				g_carSpeed_l = int(X1v); 
				g_carSpeed_r = int(X1v);
			//3.1.turn right	
			} else if ( X1v <= (-iActPos) ) {               	              
				g_carDirection = CAR_DIR_RF;
				g_carSpeed_l = -int(X1v);
				g_carSpeed_r = -int(X1v);
			//3.2.stop
			} else {
				g_carDirection = CAR_DIR_ST;
				g_carSpeed_l = 0;
				g_carSpeed_r = 0;
			}
		////////////////////////////////////////////////////////////
		//4. Stop		
		} else {                                     		//Stop      
		
			g_carDirection = CAR_DIR_ST;
			g_carSpeed_l = 0;
			g_carSpeed_r = 0;    
			
		}    

		if ( g_carSpeed_l >= 255) { g_carSpeed_l = 255; };
		if ( g_carSpeed_r >= 255) { g_carSpeed_r = 255; };
		if ( g_carSpeed_l <= 0) { g_carSpeed_l = 0; };
		if ( g_carSpeed_r <= 0) { g_carSpeed_r = 0; };
		
		car_update();	//설정값 반영
		
				
	} else {
		//
	}
	
	 
	if (LTOGv == 1) {				//Drive Mode
		iMode = 0;
	} else if (LTOGv == 0) {		//Other mode, ex) RobotArm, Auto drive
		iMode = 1;
	}
	
	if (RTOGv == 1) {
		bFront_Led2 = false;
	} else if (RTOGv == 0) {
		bFront_Led2 = true;
	}
		
	if (bFront_Led2 == true) {
		digitalWrite(FRONT_LED2, HIGH);
	} else {
		digitalWrite(FRONT_LED2, LOW);
	}	
	
	/////////////////////////////////////////////
	//Add Option Part.
	//iOption 0-No option, 1-Light, 2-Pan/Tile Servo, 3-..... 
	if (iOptDevice1 == 1) {
		if (RTOGv == 1){
			digitalWrite(LIGHT_LED,LOW); 
		} else {
			digitalWrite(LIGHT_LED,HIGH);
		}
	}
	
	//무선송신기로부터 새로운 메시지를 받고, 옵션이 설치 되어 있다면.
	if ( (rcv_state == 1) && (iOptDevice2 == 1) ){
		
		//조이스틱 값(0~1023)을 서보모터 각(0~180)으로 맵핑.
		X2v = int(map(X2v,0,1023,0,180));
		Y2v = int(map(Y2v,0,1023,0,180));		
		
		//Horizontal(Pan) Servo Motor
		if ( X2v > 100 ) { 			//조이스틱을 조금 움직였을 때 1도 증가
			Servo_Hor_Pos += 1;		
			if ( X2v > 170 ) { 		//조이스틱을 많이 움직였을 때 추가 2도 증가
				Servo_Hor_Pos += 2; 
			}
		} else if ( X2v < 80 ) {
			Servo_Hor_Pos -= 1;		//조이스틱을 조금 움직였을 때 1도 감소
			if ( X2v < 10 ) {
				Servo_Hor_Pos -= 2;	//조이스틱을 많이 움직였을 때 추가 2도 감소
			}
		}						
		if ( Servo_Hor_Pos >= 180 ) { Servo_Hor_Pos = 180; }; 	//MAX180도
        if ( Servo_Hor_Pos <= 0 ) { Servo_Hor_Pos = 0; };		//MIN0도
		//이전 서보값과 다를 경우에만 적용
		if ( Servo_Hor_Pos != Servo_Hor_Pos_Old) {
			ServoH.write(180 - Servo_Hor_Pos);
		}
		  
		//Vertical(Tilt) Servo Motor
		if ( Y2v > 100 ) { 
			Servo_Ver_Pos += 1;		//조이스틱을 조금 움직였을 때 1도 증가
			if ( Y2v > 170 ) { 
				Servo_Ver_Pos += 2;	//조이스틱을 많이 움직였을 때 추가 2도 증가
			}
		} else if ( Y2v < 80 ) {
			Servo_Ver_Pos -= 1;		//조이스틱을 조금 움직였을 때 1도 감소
			if ( Y2v < 10 ) { 
				Servo_Ver_Pos -= 2;	//조이스틱을 많이 움직였을 때 추가 2도 감소
			}
		}		
		if ( Servo_Ver_Pos >= 180 ) { Servo_Ver_Pos = 180; };
        if ( Servo_Ver_Pos <= 0 ) { Servo_Ver_Pos = 0; };
		if ( Servo_Ver_Pos != Servo_Ver_Pos_Old) {
			ServoV.write(Servo_Ver_Pos);
		}
		
		//서보모터의 각변화를 비교하기 위해 저장.
		Servo_Hor_Pos_Old = Servo_Hor_Pos;
		Servo_Ver_Pos_Old = Servo_Ver_Pos;	  
		
	}
	
	if (iOptDevice3 == 1 ) {
		//
	}
	
	if (iOptDevice4 == 1 ) {
		//
	}
	
	if (iOptDevice5 == 1 ) {
		//
	}
	
}

//Motor 방향 값 업데이트
void car_update()
{  
	if ( g_carDirection == CAR_DIR_FW) {
		car_forward();
	} else if ( g_carDirection == CAR_DIR_BK) {
		car_backward();
	} else if ( g_carDirection == CAR_DIR_LF) {
		car_left();
	} else if ( g_carDirection == CAR_DIR_RF) {
		car_right();
	} else if ( g_carDirection == CAR_DIR_ST) {
		car_stop();
	}   
}

void car_forward()
{
	digitalWrite(IN1,HIGH);
	digitalWrite(IN2,LOW);
	analogWrite(ENA,g_carSpeed_r);

	digitalWrite(IN3,HIGH);
	digitalWrite(IN4,LOW);
	analogWrite(ENB,g_carSpeed_l);  
}

void car_backward()
{
	digitalWrite(IN1,LOW);
	digitalWrite(IN2,HIGH);
	analogWrite(ENA,g_carSpeed_r);

	digitalWrite(IN3,LOW);
	digitalWrite(IN4,HIGH);
	analogWrite(ENB,g_carSpeed_l);
}

void car_left()
{
	digitalWrite(IN1,HIGH);
	digitalWrite(IN2,LOW);
	analogWrite(ENA,g_carSpeed_r);

	digitalWrite(IN3,LOW);
	digitalWrite(IN4,HIGH);
	analogWrite(ENB,g_carSpeed_l);
} 

void car_right()
{
	digitalWrite(IN1,LOW);
	digitalWrite(IN2,HIGH);
	analogWrite(ENA,g_carSpeed_r);

	digitalWrite(IN3,HIGH);
	digitalWrite(IN4,LOW );
	analogWrite(ENB,g_carSpeed_l);
} 
  
void car_stop()
{
	analogWrite(ENA,0);
	analogWrite(ENB,0); 
}

