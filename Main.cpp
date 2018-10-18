#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <string>

//#include <jni.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

string allDrives;/*

char getRemovableDisk();
JNIEXPORT jboolean JNICALL Java_com_jt_scoutserver_utils_SystemUtils_hasNewDevices
(JNIEnv * env, jclass class, jlong fd) {
	driveLetter = getRemovableDisk();
	if (driveLetter != '0') {
	}
	return true;
}

*/


bool is_prime(int n) {
	if (n <= 1)
		return false;
	else if (n <= 3)
		return true;
	else if (n % 2 == 0 || n % 3 == 0) {
		return false;
	}
	int i = 5;
	while (i * i <= n) {
		if (n % i == 0 or n % (i + 2) == 0) {
			return false;
		}
		i += 6;
	}
	return true;
}


int main(void) {
	for (int i = 0; i < 90000000; i++) {
		if (is_prime(i)) printf("%i, ", i);
	}
	
	
	return 0;/*
	char driveLetter;
	while (1) {
		driveLetter = getRemovableDisk();
		if (driveLetter != '0') {
			printf("%c \n", driveLetter);
		}

		Sleep(100);
	}

	return 0;*/
}

char getRemovableDisk() {
	char drive = '0';

	char szLogicalDrives[MAX_PATH];
	DWORD dwResult = GetLogicalDriveStrings(MAX_PATH, szLogicalDrives);

	string currentDrives = "";

	//cout << dwResult << endl;
	for (int i = 0; i<dwResult; i++)
	{
		if (szLogicalDrives[i]>64 && szLogicalDrives[i]< 90)
		{
			currentDrives.append(1, szLogicalDrives[i]);

			if (allDrives.find(szLogicalDrives[i]) > 100)
			{
				drive = szLogicalDrives[i];
			}
		}
	}

	allDrives = currentDrives;

	return drive;
}