#pragma once

//HC-12 433MHz Serial Wireless Communication Module.(for receiver pasing)
int iActPos = 80;		    	//Analog ��°��� 80�̻��� �ǿ��߸� ���ͷ� ���.
int first_idx = 99;				//���ŵ� ���ڿ��� "$RTx"�� ���° �ε����� �ִ��� Ȯ��
int second_idx = 99;			//���ŵ� ���ڿ��� "$END"�� ���° ���ؽ��� �ִ��� Ȯ��
bool rcv_state = 0;  			//���ŵ� ���ڿ��� �ִٸ�...
int rcv_cnt = 0;				//���Ż��� üũ ī����.
int act_sen_cnt = 0;			//��ư���� ������ȣ�� �Է� ����.
int act_sen_val = 10;			//act_sen_cnt�� �ʱ�ȭ �� 10���Ͽ��� �Է°����� ��.

int X1v, Y1v, Z1v;				//Left Joystick value - X, Y, Z
int X2v, Y2v, Z2v;				//Right Joystick value - X, Y, Z
int LPOTv, RPOTv;				//Left Potentiometer, Rright Potentiometer.
int LBUv, RBUv, LBDv, RBDv;		//Left Btn UP, Right Btn UP, Left Btn Down, Right Btn Down
int LTOGv, RTOGv;				//Left Toggle S/W, Right Toggle S/W
int iMode = 1; 					////1-Drive Mode, 0-Other Mode. ex)RobotArm, Auto Drive...
String Rcv_Str;					//HC-12�κ��� ���Ź��ڿ��� ����
int get_val[20] = { 0 };		//HC-12�κ��� ���Ź��ڿ��� ������ �и��Ͽ� ���������� ����

int set_val;					//strotok������ ��ū�� ������ �������� ����
char rx_buf[64] = { 0 }; 		//Rcv_Str�� ���ڿ��� ���ڿ��� ������ ����
char ms[64] = { 0 };			//rx_buf�迭�� ���� �޸𸮷� ����ϱ� ���� ����
char cs[16] = { 0 };			//strtok()�� ���� ����� ���� ����
char *p;                        //strtok()�� ���� ����� ���� ������ ����
char str[64] = { 0 };			//������� ���� ���ڿ��� ����



//HC-12 433MHz Serial Wireless Communication Module 
void HC12_RECEIVE()			    //receiving & data parsing.
{

	rcv_state = 0;				//���Ż��¸� �ʱ�ȭ.
	Rcv_Str = "";  				//���ŵ� ���ڿ� ������ �ʱ�ȭ.

	Serial2.setTimeout(30);						//Timeout�� 30ms�� �����Ѵ�.
	Rcv_Str = Serial2.readStringUntil('\n');  	//���ڿ��� "\n"�� �� ������ ��ٸ���.

	/*
	���Ź��ڿ��� �и��Ͽ� ������ ���� :
	X1v, Y1v, Z1v, X2v, Y2v, Z2v, LPOTv, RPOTv, LBUv, RBUv, LBDv, RBDv, LTOGv, RTOGv
	���Ź��ڿ� �� :
	$RTx,512,520,1,508,511,1,0,0,1,1,1,1,1,1,$END  - 43�ڸ� ���ڿ�
	���Ź��ڿ� �� :
	$RTx,5,6,1,7,8,1,0,0,1,1,1,1,1,1,$END  - 37�ڸ� �ּ� ���ڿ� ����
	���Ź��ڿ� �� :
	$RTx,1023,1023,1,1023,1023,1,1023,1023,1,1,1,1,1,1,$END  - 55�ڸ� ���ڿ�
	*/

	first_idx = 99;								//"$RTx"�� ���ڿ��߿��� ���°�� �ִ���...
	second_idx = 99;							//"$END"�� ���ڿ��߿��� ���°�� �ִ���...
	first_idx = Rcv_Str.indexOf("$RTx");		//���ŵ� ���ڿ��� "$RTx"�� �ִ��� Ȯ��
	second_idx = Rcv_Str.lastIndexOf("$END");	//���ŵ� ���ڿ��� "$END"�� �ִ��� Ȯ��


	set_val = 0;		//����� ������ ���� ���� �迭�� �ε����� ���

	/*
	���� ���ڿ��� "$RTx"�� ù��°��, "$END"�� ���ڿ��� 33������ ũ�ٸ� ���� �������� �Ǵ�
	�޸��� �������� ���ڸ� �и� �ϱ����� ���ڿ��� char�� ������ ����
	*/
	if ((first_idx == 0) && (second_idx >= 33)) {

		rcv_state = 1;  //���ڿ��� "$RTx"�� "$END"�� �ִٸ� ���� �������� �Ǵ�
		rcv_cnt = 0;    //���� ���� ī��Ʈ ����.

		for (i = 0; i < second_idx; i++) {
			rx_buf[i] = Rcv_Str[i];				//Rcv_Str�� ���ڿ��� ���ڿ��� ������ ����
		}
		sprintf(ms, rx_buf);		         		//rx_buf�� ���ڿ��� �޸𸮿� ����

		/*
		strtok �Լ� ���� : char *strtok(char *_Str, const char *_Delim);
		strtok() �Լ��� _Str�� _Delim�� �ɰ��� ��ū���� ���� ��, ���Ͽ� ���� ���ڸ� ������ ��,
		�� ���ڸ� NULL������ �ٲٴ� �Լ��̴�.
		*/

		//ù ȣ��ÿ��� ����ڿ�(_Str)�� �ڸ��⸦ ���� ����(_Delim)�� �μ��� ����
		p = strtok(ms, ",");                     //First Token�� ���� ����Ʈ p�� ����
		sprintf(cs, "%s", p);					//����Ʈ p�� ���� cs�� ����
		get_val[set_val] = atoi(cs);			//$RTx�� ���ڰ� �ƴϹǷ� 0���� ��ȯ��

		//���Ŀ��� _Str�� NULL����, Delim�� �ڸ��⸦ ���� ���ڸ� �μ��� �����Ѵ�.
		//���̻� ��ȯȰ ���� ���� ������ ���й��ڸ� �����Ѵ�.
		while ((p = strtok(NULL, ",")) != NULL) {  //another token ����	
			set_val++;							//����� ��������(token) �� ī��Ʈ
			sprintf(cs, "%s", p);				//����Ʈ p�� ���� ���ڿ��� ���� cs�� ����
			get_val[set_val] = atoi(cs);        //���ڿ��� cs�� ���� ������ ��ȯ�Ͽ� ����
			if (set_val == 14) { break; }      //���� �������ڰ� 14�̸� ��������
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
	else {//20�� ���� üũ ���и� �˶� �� ����̺� ������	
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

	//�������� ���Ż����̰� iMode drive mode���
	if ((rcv_state == 1) && (iMode == 0)) {


		//X1v(Joystick X�� ����) : Left(0)-Cent(512)-Right(1023)  		
		//Y1v(Joystick X�� ����) :   Up(0)-Cent(512)-Down(1023)			
		//�Ƴ��α� ��� ����(0~255)�� ���
		X1v = round((511 - X1v) / 2); //255 ~ 0, 0 ~ -255
		Y1v = round((511 - Y1v) / 2); //255 ~ 0, 0 ~ -255

		////////////////////////////////////////////////////////////
		//1.Forward(255~0)
		if (Y1v >= iActPos) {             //iActPos = 80 �̻� �Ǿ�� ���� ����          		

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

		car_update();	//������ �ݿ�


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

	//�����۽ű�κ��� ���ο� �޽����� �ް�, �ɼ��� ��ġ �Ǿ� �ִٸ�.
	if ((rcv_state == 1) && (iOptDevice2 == 1)) {

		//���̽�ƽ ��(0~1023)�� �������� ��(0~180)���� ����.
		X2v = int(map(X2v, 0, 1023, 0, 180));
		Y2v = int(map(Y2v, 0, 1023, 0, 180));

		//Horizontal(Pan) Servo Motor
		if (X2v > 100) { 			//���̽�ƽ�� ���� �������� �� 1�� ����
			Servo_Hor_Pos += 1;
			if (X2v > 170) { 		//���̽�ƽ�� ���� �������� �� �߰� 2�� ����
				Servo_Hor_Pos += 2;
			}
		}
		else if (X2v < 80) {
			Servo_Hor_Pos -= 1;		//���̽�ƽ�� ���� �������� �� 1�� ����
			if (X2v < 10) {
				Servo_Hor_Pos -= 2;	//���̽�ƽ�� ���� �������� �� �߰� 2�� ����
			}
		}
		if (Servo_Hor_Pos >= 180) { Servo_Hor_Pos = 180; }; 	//MAX180��
		if (Servo_Hor_Pos <= 0) { Servo_Hor_Pos = 0; };		//MIN0��
															//���� �������� �ٸ� ��쿡�� ����
		if (Servo_Hor_Pos != Servo_Hor_Pos_Old) {
			ServoH.write(180 - Servo_Hor_Pos);
		}

		//Vertical(Tilt) Servo Motor
		if (Y2v > 100) {
			Servo_Ver_Pos += 1;		//���̽�ƽ�� ���� �������� �� 1�� ����
			if (Y2v > 170) {
				Servo_Ver_Pos += 2;	//���̽�ƽ�� ���� �������� �� �߰� 2�� ����
			}
		}
		else if (Y2v < 80) {
			Servo_Ver_Pos -= 1;		//���̽�ƽ�� ���� �������� �� 1�� ����
			if (Y2v < 10) {
				Servo_Ver_Pos -= 2;	//���̽�ƽ�� ���� �������� �� �߰� 2�� ����
			}
		}
		if (Servo_Ver_Pos >= 180) { Servo_Ver_Pos = 180; };
		if (Servo_Ver_Pos <= 0) { Servo_Ver_Pos = 0; };
		if (Servo_Ver_Pos != Servo_Ver_Pos_Old) {
			ServoV.write(Servo_Ver_Pos);
		}

		//���������� ����ȭ�� ���ϱ� ���� ����.
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