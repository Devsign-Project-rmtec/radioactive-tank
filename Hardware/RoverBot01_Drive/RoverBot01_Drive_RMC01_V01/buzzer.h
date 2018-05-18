#pragma once



//Bzuzzer 음계
int iNote[] = { 2093,2349,2637,2793,3136,3520,3951,4186 }; //도레미파솔라시도
int iFreq = 150;				//가변주파수 저장 변수
bool bAlram_Flag = false;       //알람용 플레그 변수

//알람 제어용 함수 선언 : 0-Emegency, 1-도레미~, 2-50Hz, 3-400Hz, 4-1000Hz
void vAlram(int kind) {

	if (kind == 0) {			//Emergency Alram
		for (iFreq = 150; iFreq < 1800; iFreq++) {
			tone(BUZZER, iFreq, 10);
		}
		for (iFreq = 1800; iFreq >= 150; iFreq--) {
			tone(BUZZER, iFreq, 10);
		}
	}
	else if (kind == 1) {	//a note
		int elementCount = sizeof(iNote) / sizeof(int);
		for (i = 0; i < elementCount; i++)    //note를 play
		{
			tone(BUZZER, iNote[i], 500);
			delay(750);
		}
	}
	else if (kind == 2) {	//50Hz
		tone(BUZZER, 100, 10);
	}
	else if (kind == 3) {	//400Hz
		tone(BUZZER, 400, 1000);
	}
	else if (kind == 4) {   //1000Hz
		tone(BUZZER, 1000, 1000);
	}

}