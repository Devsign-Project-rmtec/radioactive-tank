#pragma once



//Bzuzzer ����
int iNote[] = { 2093,2349,2637,2793,3136,3520,3951,4186 }; //�������ļֶ�õ�
int iFreq = 150;				//�������ļ� ���� ����
bool bAlram_Flag = false;       //�˶��� �÷��� ����

//�˶� ����� �Լ� ���� : 0-Emegency, 1-������~, 2-50Hz, 3-400Hz, 4-1000Hz
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
		for (i = 0; i < elementCount; i++)    //note�� play
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