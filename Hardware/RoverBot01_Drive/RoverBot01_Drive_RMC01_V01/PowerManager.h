#pragma once
//���� ���� ���� ����
int   iBatt_Adc = 0;     		//�����̿����� ������ �Ƴ��α� ADC��
float fVolt_Lvl = 0.0f;			//ADC�� �������� ���� ��
bool  bPwrLED_Status = false;	//LED On/Off ���¸� ����
int iPwrBlink_Cnt = 0;

//A0 �Է� ������ ADC�� ���� �� ADC�� Voltage�� ����ϴ� �Լ� ����
float fCheck_Batt_Lvl();

//Get sensing for li-Po battery
float fCheck_Batt_Lvl() {

	float fTemp_Lvl = 0.0f;
	iBatt_Adc = analogRead(VOLT_CHK); 		//A0, �Ƴ��αװ� ����(sensing)
	delay(3);								//�����ð� 10ms
	fTemp_Lvl = (iBatt_Adc / 40.92f);        //Resolution = 1023/25V = 40,92
	//Serial.print(iBatt_Adc);                //for debug
	//Serial.print("      ");                 //for debug
	//Serial.println(fTemp_Lvl);              //for debug 
	return fTemp_Lvl;						//���� ���а��� ȣ���� �ʿ� ����

}

//���а��� ���� LED ���ڰŸ� ���� �Լ�.
void vPwrLed_Blink(float fVolt, int ledPin)
{

	//Fullcharge voltate : 12.6V, Discharge voltage - 8.4V
	//11.0V - always on Power LED.
	//10.5V - lower blink Power LED.
	// 9.6V - Quickly blink Power LED & Sounding buzzer.

	if (fVolt > 11.0) {                               //Always ON
		digitalWrite(ledPin, HIGH);
		iPwrBlink_Cnt = 0;
	}
	else if ((fVolt <= 11.0) && (fVolt >10.5)) {    //Lower Blink

		if (iPwrBlink_Cnt == 20) {
			if (bPwrLED_Status == false) {
				digitalWrite(ledPin, HIGH);
				bPwrLED_Status = true;
			}
			else if (bPwrLED_Status == true) {
				digitalWrite(ledPin, LOW);
				bPwrLED_Status = false;
			}
			iPwrBlink_Cnt = 0;
		}

	}
	else if ((fVolt <= 10.5) && (fVolt > 9.6)) {  	//fast Blink
		if (iPwrBlink_Cnt == 2) {
			if (bPwrLED_Status == false) {
				digitalWrite(ledPin, HIGH);
				bPwrLED_Status = true;
			}
			else if (bPwrLED_Status == true) {
				digitalWrite(ledPin, LOW);
				bPwrLED_Status = false;
			}
			iPwrBlink_Cnt = 0;
		}
		vAlram(2);
	}
	else {                                         	//Alywas OFF
		digitalWrite(ledPin, LOW);
		vAlram(0);
	}

	iPwrBlink_Cnt += 1;
}


