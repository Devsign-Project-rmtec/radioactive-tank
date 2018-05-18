#pragma once

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

void car_update();	//설정값 반영(MotorA, MotorB)
void car_forward();     //IN1-HIGH, IN2-LOW, IN3-HIGH, IN4-LOW
void car_backward();    //IN1-LOW, IN2-HIGH, IN3-LOW, IN4-HIGH 
void car_left();        //IN1-HIGH, IN2-LOW, IN3-LOW, IN4-HIGH
void car_right();       //IN1-LOW, IN2-HIGH, IN3-HIGH, IN4-LOW
void car_stop();         //ENA-0, ENB-0

						 //Motor 방향 값 업데이트
void car_update()
{
	if (g_carDirection == CAR_DIR_FW) {
		car_forward();
	}
	else if (g_carDirection == CAR_DIR_BK) {
		car_backward();
	}
	else if (g_carDirection == CAR_DIR_LF) {
		car_left();
	}
	else if (g_carDirection == CAR_DIR_RF) {
		car_right();
	}
	else if (g_carDirection == CAR_DIR_ST) {
		car_stop();
	}
}

void car_forward()
{
	digitalWrite(IN1, HIGH);
	digitalWrite(IN2, LOW);
	analogWrite(ENA, g_carSpeed_r);

	digitalWrite(IN3, HIGH);
	digitalWrite(IN4, LOW);
	analogWrite(ENB, g_carSpeed_l);
}

void car_backward()
{
	digitalWrite(IN1, LOW);
	digitalWrite(IN2, HIGH);
	analogWrite(ENA, g_carSpeed_r);

	digitalWrite(IN3, LOW);
	digitalWrite(IN4, HIGH);
	analogWrite(ENB, g_carSpeed_l);
}

void car_left()
{
	digitalWrite(IN1, HIGH);
	digitalWrite(IN2, LOW);
	analogWrite(ENA, g_carSpeed_r);

	digitalWrite(IN3, LOW);
	digitalWrite(IN4, HIGH);
	analogWrite(ENB, g_carSpeed_l);
}

void car_right()
{
	digitalWrite(IN1, LOW);
	digitalWrite(IN2, HIGH);
	analogWrite(ENA, g_carSpeed_r);

	digitalWrite(IN3, HIGH);
	digitalWrite(IN4, LOW);
	analogWrite(ENB, g_carSpeed_l);
}

void car_stop()
{
	analogWrite(ENA, 0);
	analogWrite(ENB, 0);
}

