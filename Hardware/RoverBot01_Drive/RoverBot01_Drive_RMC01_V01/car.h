#pragma once

//�������� ����.
#define CAR_DIR_FW 0		//����
#define CAR_DIR_BK 1		//����
#define CAR_DIR_LF 2		//�¼�ȸ
#define CAR_DIR_RF 3		//�켱ȸ
#define CAR_DIR_ST 4		//����

//Defalult value ����.
int g_carDirection = CAR_DIR_ST;	//������ ���� �� ������ �⺻������ ��
int g_carSpeed_l = 0;				//�������� �ӵ� �Ƴ��α�(0~255) ����
int g_carSpeed_r = 0;				//�������� �ӵ� �Ƴ��α�(0~255) ����

void car_update();	//������ �ݿ�(MotorA, MotorB)
void car_forward();     //IN1-HIGH, IN2-LOW, IN3-HIGH, IN4-LOW
void car_backward();    //IN1-LOW, IN2-HIGH, IN3-LOW, IN4-HIGH 
void car_left();        //IN1-HIGH, IN2-LOW, IN3-LOW, IN4-HIGH
void car_right();       //IN1-LOW, IN2-HIGH, IN3-HIGH, IN4-LOW
void car_stop();         //ENA-0, ENB-0

						 //Motor ���� �� ������Ʈ
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

