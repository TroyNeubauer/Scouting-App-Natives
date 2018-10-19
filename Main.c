
/*
uint64_t last = 0;
for (uint64_t i = 0; i < 900000000; i++) {
	if (last > i) {
		printf("One cycle complete!");
		return 0;
	}
	if (is_prime(i)) printf("%i, ", i);
}
bool is_prime(uint64_t n) {
	if (n <= 1)
		return false;
	else if (n <= 3)
		return true;
	else if (n % 2 == 0 || n % 3 == 0) {
		return false;
	}
	uint64_t i = 5;
	while (i * i <= n) {
		if (n % i == 0 || n % (i + 2) == 0) {
			return false;
		}
		i += 6;
	}
	return true;
}
*/













#include <stdio.h>
#include <time.h>
#include <windows.h>

#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


char getRemovableDisk();

JNIEXPORT jboolean JNICALL Java_com_jt_scoutserver_utils_SystemUtils_hasNewDevices(JNIEnv * env, jclass class) {
	char driveLetter = getRemovableDisk();
	if (driveLetter != '0') {
		return JNI_TRUE;
	}
	return JNI_FALSE;
}

char lastDrives[MAX_PATH];

JNIEXPORT void JNICALL Java_com_jt_scoutserver_utils_SystemUtils_nativeInit(JNIEnv * env, jclass class) {
	GetLogicalDriveStrings(MAX_PATH, lastDrives);
}
/*
int main(void) {
	char driveLetter;
	
	while (1) {
		driveLetter = getRemovableDisk();
		if (driveLetter != '0') {
			printf("returning: %c \n", driveLetter);
		}

		Sleep(10);
	}

	return 0;
}*/

char getRemovableDisk() {
	char drive = '0';

	char drives[MAX_PATH];
	ZeroMemory(drives, MAX_PATH);

	DWORD count = GetLogicalDriveStrings(MAX_PATH, drives);
	for (int i = 0; i < count; i++)
	{
		char current = drives[i];
		char isContained = 0;
		for (int j = 0; j < MAX_PATH; j++) {
			if (current == lastDrives[j]) {
				isContained = 1;
			}
		}
		if (isContained == 0) {//It wasnt in the last one, but is in this one
			if (current >= 'A' && current <= 'Z' || current >= 'a' && current <= 'z') {//make sure its a drive letter...
				drive = current;
			}
		}
		else {
		}
	}
	memcpy(lastDrives, drives, MAX_PATH);
	return drive;
}