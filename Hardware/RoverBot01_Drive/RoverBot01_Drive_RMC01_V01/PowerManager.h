#pragma once
//전원 관련 변수 선언
int   iBatt_Adc = 0;     		//리듐이온전지 전압의 아날로그 ADC값
float fVolt_Lvl = 0.0f;			//ADC를 전압으로 변경 값
bool  bPwrLED_Status = false;	//LED On/Off 상태를 저장
int iPwrBlink_Cnt = 0;

//A0 입력 전압을 ADC로 저장 및 ADC를 Voltage로 계산하는 함수 선언
float fCheck_Batt_Lvl();

//Get sensing for li-Po battery
float fCheck_Batt_Lvl() {

	float fTemp_Lvl = 0.0f;
	iBatt_Adc = analogRead(VOLT_CHK); 		//A0, 아날로그값 센싱(sensing)
	delay(3);								//지연시간 10ms
	fTemp_Lvl = (iBatt_Adc / 40.92f);        //Resolution = 1023/25V = 40,92
	//Serial.print(iBatt_Adc);                //for debug
	//Serial.print("      ");                 //for debug
	//Serial.println(fTemp_Lvl);              //for debug 
	return fTemp_Lvl;						//계산된 전압값을 호출한 쪽에 리턴

}

//전압값에 따른 LED 깜박거림 제어 함수.
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


