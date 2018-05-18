#pragma once

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
int get_val[20] = { 0 };		//HC-12로부터 수신문자열의 정보를 분리하여 숫자정수로 저장

int set_val;					//strotok에서으 토큰의 갯수를 세기위한 변수
char rx_buf[64] = { 0 }; 		//Rcv_Str의 문자열을 문자열형 변수로 저장
char ms[64] = { 0 };			//rx_buf배열의 값으 메모리로 출력하기 위한 변수
char cs[16] = { 0 };			//strtok()에 의해 추출된 값을 저장
char *p;                        //strtok()에 의해 추출된 값의 포인터 변수
char str[64] = { 0 };			//디버깅을 위한 문자열형 변수



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
	if ((first_idx == 0) && (second_idx >= 33)) {

		rcv_state = 1;  //문자열중 "$RTx"와 "$END"가 있다면 수신 성공으로 판단
		rcv_cnt = 0;    //수신 상태 카운트 리셋.

		for (i = 0; i < second_idx; i++) {
			rx_buf[i] = Rcv_Str[i];				//Rcv_Str의 문자열을 문자열형 변수로 저장
		}
		sprintf(ms, rx_buf);		         		//rx_buf의 문자열을 메모리에 저장

		/*
		strtok 함수 원형 : char *strtok(char *_Str, const char *_Delim);
		strtok() 함수는 _Str과 _Delim을 쪼개어 토큰으로 만든 후, 비교하여 같은 문자를 만났을 때,
		그 문자를 NULL값으로 바꾸는 함수이다.
		*/

		//첫 호출시에는 대상문자열(_Str)과 자르기를 위한 문자(_Delim)를 인수로 전달
		p = strtok(ms, ",");                     //First Token의 값을 포인트 p에 저장
		sprintf(cs, "%s", p);					//포인트 p의 값을 cs에 저장
		get_val[set_val] = atoi(cs);			//$RTx는 숫자가 아니므로 0으로 반환됨

		//이후에는 _Str을 NULL으로, Delim은 자르기를 위한 문자를 인수로 전달한다.
		//더이상 반환활 값이 없을 때까지 구분문자를 추출한다.
		while ((p = strtok(NULL, ",")) != NULL) {  //another token 추출	
			set_val++;							//추출된 구문문자(token) 수 카운트
			sprintf(cs, "%s", p);				//포인트 p의 값을 문자열형 변수 cs에 저장
			get_val[set_val] = atoi(cs);        //문자열형 cs의 값을 정수로 변환하여 저장
			if (set_val == 14) { break; }      //받을 구문문자가 14이면 빠져나감
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

		X1v = get_val[1];		//Left Joystick X - Analog value
		Y1v = get_val[2];		//Left Joystick Y - Analog value
		Z1v = get_val[3];		//Left Joystick Z - Digital value
		X2v = get_val[4];		//Right Joystick X - Analog value
		Y2v = get_val[5];		//Right Joystick Y - Analog value
		Z2v = get_val[6];		//Right Joystick Z - Digital value
		LPOTv = get_val[7];     //Left Potentiometer - Analog value
		RPOTv = get_val[8];     //Right Potentiometer - Analog value
		LBUv = get_val[9];		//Left Up Button - Digital value 
		RBUv = get_val[10];		//Right Up Button - Digital value   
		LBDv = get_val[11];		//Left Down Button - Digital value   
		RBDv = get_val[12];		//Right Down Button - Digital value   
		LTOGv = get_val[13];    //Left Toggle S/W - Digtal value  
		RTOGv = get_val[14];    //Right Toggle S/W - Digtal value  				

		digitalWrite(STATUS_LED3, HIGH);
	}
	else {//20번 수신 체크 실패면 알람 및 드라이브 정지를	
		digitalWrite(STATUS_LED3, LOW);
		if (rcv_cnt > 20) {
			g_carDirection = CAR_DIR_ST;
			g_carSpeed_l = 0;
			g_carSpeed_r = 0;
			car_update();
			vAlram(3);
		}
		rcv_cnt += 1;
	}

	//정상정인 수신상태이고 iMode drive mode라면
	if ((rcv_state == 1) && (iMode == 0)) {


		//X1v(Joystick X값 범위) : Left(0)-Cent(512)-Right(1023)  		
		//Y1v(Joystick X값 범위) :   Up(0)-Cent(512)-Down(1023)			
		//아날로그 출력 범위(0~255)로 계산
		X1v = round((511 - X1v) / 2); //255 ~ 0, 0 ~ -255
		Y1v = round((511 - Y1v) / 2); //255 ~ 0, 0 ~ -255

		////////////////////////////////////////////////////////////
		//1.Forward(255~0)
		if (Y1v >= iActPos) {             //iActPos = 80 이상 되어야 모터 동작          		

			g_carDirection = CAR_DIR_FW;
			//1.1.turn left forward        
			if (X1v >= iActPos) {
				g_carSpeed_l = Y1v - int(X1v);
				g_carSpeed_r = Y1v + int(X1v);
				//1.2.turn rigit forward          
			}
			else if (X1v <= -iActPos) {
				X1v = int(abs(X1v));       //convert plus
				g_carSpeed_l = Y1v + int(X1v);
				g_carSpeed_r = Y1v - int(X1v);
				//1.3.Direct forward	
			}
			else {
				g_carSpeed_l = Y1v;
				g_carSpeed_r = Y1v;
			}
		////////////////////////////////////////////////////////////	
		//2.Backward(0~-255)
		}
		else if (Y1v <= -iActPos) {

			g_carDirection = CAR_DIR_BK;
			Y1v = int(abs(Y1v));           //convert plus
										   //2.1.turn left backward
			if (X1v >= iActPos) {
				g_carSpeed_l = Y1v - int(X1v);
				g_carSpeed_r = Y1v + int(X1v);
				//2.2.furn rigit backward	
			}
			else if (X1v <= -iActPos) {
				X1v = int(abs(X1v));            //convert plus    
				g_carSpeed_l = Y1v + int(X1v);
				g_carSpeed_r = Y1v - int(X1v);
				//2.3.Direct backward	
			}
			else {
				g_carSpeed_l = Y1v;
				g_carSpeed_r = Y1v;
			}
		////////////////////////////////////////////////////////////
		//3.turn left or right and stop	
		}
		else if ((Y1v < iActPos) && (Y1v > (0 - iActPos))) {
			//3.1.turn left 
			if (X1v >= iActPos) {
				g_carDirection = CAR_DIR_LF;
				g_carSpeed_l = int(X1v);
				g_carSpeed_r = int(X1v);
				//3.1.turn right	
			}
			else if (X1v <= (-iActPos)) {
				g_carDirection = CAR_DIR_RF;
				g_carSpeed_l = -int(X1v);
				g_carSpeed_r = -int(X1v);
				//3.2.stop
			}
			else {
				g_carDirection = CAR_DIR_ST;
				g_carSpeed_l = 0;
				g_carSpeed_r = 0;
			}
		////////////////////////////////////////////////////////////
		//4. Stop		
		}
		else {                                     		//Stop      

			g_carDirection = CAR_DIR_ST;
			g_carSpeed_l = 0;
			g_carSpeed_r = 0;

		}

		if (g_carSpeed_l >= 255) { g_carSpeed_l = 255; };
		if (g_carSpeed_r >= 255) { g_carSpeed_r = 255; };
		if (g_carSpeed_l <= 0) { g_carSpeed_l = 0; };
		if (g_carSpeed_r <= 0) { g_carSpeed_r = 0; };

		car_update();	//설정값 반영


	}
	else {
		//
	}


	if (LTOGv == 1) {				//Drive Mode
		iMode = 0;
	}
	else if (LTOGv == 0) {		//Other mode, ex) RobotArm, Auto drive
		iMode = 1;
	}

	if (RTOGv == 1) {
		bFront_Led2 = false;
	}
	else if (RTOGv == 0) {
		bFront_Led2 = true;
	}

	if (bFront_Led2 == true) {
		digitalWrite(FRONT_LED2, HIGH);
	}
	else {
		digitalWrite(FRONT_LED2, LOW);
	}

	/////////////////////////////////////////////
	//Add Option Part.
	//iOption 0-No option, 1-Light, 2-Pan/Tile Servo, 3-..... 
	if (iOptDevice1 == 1) {
		if (RTOGv == 1) {
			digitalWrite(LIGHT_LED, LOW);
		}
		else {
			digitalWrite(LIGHT_LED, HIGH);
		}
	}

	//무선송신기로부터 새로운 메시지를 받고, 옵션이 설치 되어 있다면.
	if ((rcv_state == 1) && (iOptDevice2 == 1)) {

		//조이스틱 값(0~1023)을 서보모터 각(0~180)으로 맵핑.
		X2v = int(map(X2v, 0, 1023, 0, 180));
		Y2v = int(map(Y2v, 0, 1023, 0, 180));

		//Horizontal(Pan) Servo Motor
		if (X2v > 100) { 			//조이스틱을 조금 움직였을 때 1도 증가
			Servo_Hor_Pos += 1;
			if (X2v > 170) { 		//조이스틱을 많이 움직였을 때 추가 2도 증가
				Servo_Hor_Pos += 2;
			}
		}
		else if (X2v < 80) {
			Servo_Hor_Pos -= 1;		//조이스틱을 조금 움직였을 때 1도 감소
			if (X2v < 10) {
				Servo_Hor_Pos -= 2;	//조이스틱을 많이 움직였을 때 추가 2도 감소
			}
		}
		if (Servo_Hor_Pos >= 180) { Servo_Hor_Pos = 180; }; 	//MAX180도
		if (Servo_Hor_Pos <= 0) { Servo_Hor_Pos = 0; };		//MIN0도
															//이전 서보값과 다를 경우에만 적용
		if (Servo_Hor_Pos != Servo_Hor_Pos_Old) {
			ServoH.write(180 - Servo_Hor_Pos);
		}

		//Vertical(Tilt) Servo Motor
		if (Y2v > 100) {
			Servo_Ver_Pos += 1;		//조이스틱을 조금 움직였을 때 1도 증가
			if (Y2v > 170) {
				Servo_Ver_Pos += 2;	//조이스틱을 많이 움직였을 때 추가 2도 증가
			}
		}
		else if (Y2v < 80) {
			Servo_Ver_Pos -= 1;		//조이스틱을 조금 움직였을 때 1도 감소
			if (Y2v < 10) {
				Servo_Ver_Pos -= 2;	//조이스틱을 많이 움직였을 때 추가 2도 감소
			}
		}
		if (Servo_Ver_Pos >= 180) { Servo_Ver_Pos = 180; };
		if (Servo_Ver_Pos <= 0) { Servo_Ver_Pos = 0; };
		if (Servo_Ver_Pos != Servo_Ver_Pos_Old) {
			ServoV.write(Servo_Ver_Pos);
		}

		//서보모터의 각변화를 비교하기 위해 저장.
		Servo_Hor_Pos_Old = Servo_Hor_Pos;
		Servo_Ver_Pos_Old = Servo_Ver_Pos;

	}

	if (iOptDevice3 == 1) {
		//
	}

	if (iOptDevice4 == 1) {
		//
	}

	if (iOptDevice5 == 1) {
		//
	}

	if (iOptDevice6 == 1) {
		//
	}
}