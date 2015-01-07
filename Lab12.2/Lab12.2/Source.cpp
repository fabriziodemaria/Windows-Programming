#define UNICODE
#define _UNICODE

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>
#include <stdlib.h>

#define TYPE_FILE 1
#define TYPE_DIR 2
#define TYPE_DOT 3
#define BUFSIZE MAX_PATH
#define TH_NUMBER 4
#define MAX_SEM_COUNT 10

DWORD actualValue;
HANDLE singleThEvent[TH_NUMBER];
HANDLE barrierEvent[TH_NUMBER];

DWORD WINAPI SumThread(LPVOID lpParam);
DWORD WINAPI ProdThread(LPVOID lpParam);
DWORD WINAPI FactThread(LPVOID lpParam);
DWORD WINAPI HashThread(LPVOID lpParam);
DWORD factorialCalculation(DWORD);

int _tmain(int argc, LPTSTR argv[])
{
	int i, j;
	HANDLE hThreadArray[4];
	DWORD   dwThreadIdArray[4];
	HANDLE fileHandle;
	DWORD readBytes;

	if (argc != 2){
		_tprintf(_T("Use one argument...\n"));
	}

	for (i = 0; i < TH_NUMBER; i++){
		singleThEvent[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
	}
	for (i = 0; i < TH_NUMBER; i++){
		barrierEvent[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
	}

	hThreadArray[0] = CreateThread(
		NULL,
		0,
		SumThread,
		&actualValue,
		0,
		&dwThreadIdArray[0]);


	hThreadArray[1] = CreateThread(
		NULL,
		0,
		ProdThread,
		&actualValue,
		0,
		&dwThreadIdArray[1]);


	hThreadArray[2] = CreateThread(
		NULL,
		0,
		FactThread,
		&actualValue,
		0,
		&dwThreadIdArray[2]);


	hThreadArray[3] = CreateThread(
		NULL,
		0,
		HashThread,
		&actualValue,
		0,
		&dwThreadIdArray[3]);

	fileHandle = CreateFile(argv[1], GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (fileHandle == NULL){
		_tprintf(_T("Error opening the file to read...\n"));
	}

	while (ReadFile(fileHandle, &actualValue, sizeof(DWORD), &readBytes, NULL) && readBytes > 0){

		for (i = 0; i < TH_NUMBER; i++){
			SetEvent(singleThEvent[i]);
		}

		_tprintf(_T("\n"));

		DWORD dwWaitResult;
		dwWaitResult = WaitForMultipleObjects(TH_NUMBER, barrierEvent, TRUE, INFINITE);
	}

	Sleep(5000);

	return 0;
}


DWORD WINAPI SumThread(LPVOID lpParam){

	DWORD *tmp = (DWORD *)lpParam;
	DWORD sum = 0;

	while (TRUE){
		DWORD dwWaitResult;
		dwWaitResult = WaitForSingleObject(
			singleThEvent[0], // event handle
			INFINITE);			 // indefinite wait

		sum = sum + *tmp;
		_tprintf(_T("Sum: %d\n"), sum);

		SetEvent(barrierEvent[0]);
	}

	return 0;

}
DWORD WINAPI ProdThread(LPVOID lpParam){
	DWORD *tmp = (DWORD *)lpParam;
	DWORD prod = 1;

	while (TRUE){
		DWORD dwWaitResult;
		dwWaitResult = WaitForSingleObject(
			singleThEvent[1], // event handle
			INFINITE);			 // indefinite wait

		prod = prod * *tmp;
		_tprintf(_T("Prod: %d\n"), prod);

		SetEvent(barrierEvent[1]);
	}
	return 0;
}
DWORD WINAPI FactThread(LPVOID lpParam){
	DWORD *tmp = (DWORD *)lpParam;
	DWORD fact;
	while (TRUE){
		DWORD dwWaitResult;
		dwWaitResult = WaitForSingleObject(
			singleThEvent[2], // event handle
			INFINITE);			 // indefinite wait

		fact = factorialCalculation(*tmp);
		_tprintf(_T("Fact: %d\n"), fact);


		SetEvent(barrierEvent[2]);
	}
	return 0;
}
DWORD WINAPI HashThread(LPVOID lpParam){
	DWORD *tmp = (DWORD *)lpParam;

	while (TRUE){
		DWORD dwWaitResult;
		dwWaitResult = WaitForSingleObject(
			singleThEvent[3], // event handle
			INFINITE);			 // indefinite wait

		LPTSTR hashString;
		HANDLE hHeap;
		hHeap = GetProcessHeap();

		//hashString = (LPTSTR)malloc((*tmp + 2)*sizeof(_TCHAR));
		hashString = (LPTSTR)HeapAlloc(hHeap, NULL, (*tmp + 2)*sizeof(_TCHAR));

		lstrcpy(hashString, _T(""));

		for (int i = 0; i < *tmp; i++)
			lstrcat(hashString, _T("#"));
		lstrcat(hashString, _T("\n"));

		_tprintf(_T("%s"), hashString);

		//free(hashString);
		HeapFree(hHeap, NULL, hashString);

		SetEvent(barrierEvent[3]);
	}
	return 0;
}

DWORD factorialCalculation(DWORD tmp){
	if (tmp == 1)
		return tmp;

	return tmp * factorialCalculation(tmp - 1);
}