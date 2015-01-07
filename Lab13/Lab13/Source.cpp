/*
Fabrizio Demaria - s207063
Politecnico di Torino - System and Device Programming 2014
Lab 13
*/

#define UNICODE
#define _UNICODE
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>
#include <stdlib.h>

#define BUF_SIZE 512	//fixed known size of each record of the input files
#define MAX_STR 48		//Arbitrary limit  lenght of names

//Global variables definition
CRITICAL_SECTION readInputSection, writeOutputSection;

//Prototypes definitions
DWORD WINAPI computeStats(LPVOID);
LPVOID freeDataList(struct dataNode *);

//Handles passed to the threads to access input/output files
typedef struct hArgs{
	HANDLE readHandle;
	HANDLE outputHandle;
} hArgs;

//The following struct is used to build lists needed for statistics computation
typedef struct dataNode{
	struct dataNode *next;
	TCHAR context[MAX_STR];
	DWORD contextAmount;
} dataNode;

int _tmain(int argc, LPTSTR argv[])
{
	HANDLE *hThreadArray;
	HANDLE readHandle, outputHandle;
	int n;
	hArgs myArgs;

	//Initializing critical sections
	InitializeCriticalSection(&readInputSection);
	InitializeCriticalSection(&writeOutputSection);

	if (argc != 4){
		_tprintf(_T("Use 3 input arguments\n"));
		Sleep(3000);
		exit(1);
	}

	//Number of threads to be created
	n = _wtoi(argv[2]);

	if (n == 0){
		_tprintf(_T("Select N to be at least 1\n"));
		Sleep(3000);
		exit(1);
	}

	//Dynamic memory allocation
	hThreadArray = (HANDLE *)malloc(n*sizeof(HANDLE));

	//Creating input file handle
	readHandle = CreateFile(argv[1], GENERIC_READ, FILE_SHARE_READ, 
			NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (readHandle == INVALID_HANDLE_VALUE) {
		fprintf(stderr, "Cannot open input file. Error: %x\n", GetLastError());
		Sleep(3000);
		CloseHandle(readHandle);
		return 2;
	}

	//Creating output file handle
	outputHandle = CreateFile(argv[3], GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (outputHandle == INVALID_HANDLE_VALUE) {
		fprintf(stderr, "Cannot open output file. Error: %x\n", GetLastError());
		CloseHandle(outputHandle);
		Sleep(3000);
		return 3;
	}

	//Setting the struct to be passed to the threads
	myArgs.outputHandle = outputHandle;
	myArgs.readHandle = readHandle;

	//Creating the N threads
	for (int i = 0; i < n; i++){
		hThreadArray[i] = CreateThread(
			NULL,
			0,
			computeStats,
			&myArgs,
			0,
			NULL
			);
	}

	//Waiting all the threads to complete 
	WaitForMultipleObjects(n, hThreadArray, TRUE, INFINITE);

	//Memory release
	free(hThreadArray);
	DeleteCriticalSection(&readInputSection);
	DeleteCriticalSection(&writeOutputSection);
	CloseHandle(readHandle);
	CloseHandle(outputHandle);
	
	return 0;
}

DWORD WINAPI computeStats(LPVOID lpParam){

	hArgs *myArgs = (hArgs *)lpParam;
	HANDLE outputHanlde = myArgs->outputHandle;
	HANDLE readHandle = myArgs->readHandle;
	HANDLE tradeFile;
	DWORD nIn;
	_TCHAR buffer[BUF_SIZE], manufacturer[MAX_STR], model[MAX_STR], filename[MAX_STR];

	while (TRUE){

		//Setting the critical section to be sure that each line is read once by a single thread
		EnterCriticalSection(&readInputSection);
		if(ReadFile(readHandle, buffer, BUF_SIZE*sizeof(TCHAR), &nIn, NULL) && nIn > 0) {
			_stscanf(buffer, _T("%s %s %s"), manufacturer, model, filename);
			LeaveCriticalSection(&readInputSection);
		}
		else {
			//Leave the critical section before breaking the 'while' in case the reading of the file has been terminated
			//otherwise the thread will never release it
			LeaveCriticalSection(&readInputSection);
			break;
		}

		//If here, I need to process the row 
		tradeFile = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, 
			NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (tradeFile == INVALID_HANDLE_VALUE) {
			fprintf(stderr, "Cannot open output file. Error: %x\n", GetLastError());
			CloseHandle(tradeFile);
			return 4;
		}

		//Compute statistics START
		int carsSold = 0, totAmount = 0, cost, found, maxDateAmount = 0, maxBranchAmount = 0;
		TCHAR date[MAX_STR], branch[MAX_STR], maxDate[MAX_STR], maxBranch[MAX_STR];
		dataNode *dateHead, *dateP, *branchHead, *branchP;

		dateHead = (dataNode *)malloc(sizeof(dataNode));
		branchHead = (dataNode *)malloc(sizeof(dataNode));

		//Setting sentinels for the linked lists with amount____ -1
		dateHead->contextAmount = -1;
		dateHead->next = NULL;
		branchHead->contextAmount = -1;
		branchHead->next = NULL;


		while (ReadFile(tradeFile, buffer, BUF_SIZE*sizeof(TCHAR), &nIn, NULL) && nIn > 0){
			_stscanf(buffer, _T("%s %s %d"), branch, date, &cost);
			
			//Each file's line correspond to a car sold
			carsSold++;
			//Total revenue to be cumulatively added
			totAmount = totAmount + cost;

			//Setting the moving pointer at the head of the initialized list
			dateP = dateHead;
			branchP = branchHead;

			found = 0;

			//Check maxDate: date of highest cars sold number
			while (true){
				//Skipping the check over the sentinel with contextAmount set to -1
				if (dateP->contextAmount != -1){
					//If there is a node with the read date, the amount of cars sold has to be updated
					if (!lstrcmp(dateP->context, date)){
						dateP->contextAmount = dateP->contextAmount + 1;
						found = 1;
						break;
					}			
				}
				if (dateP->next == NULL)
					break;
				dateP = dateP->next;
			}

			if (!found){
				//If here the date found in the line is not present in the list and a new node is created
				//with the number of cars (contextAmount) set to 1
				dateP->next = (dataNode *)malloc(sizeof(dataNode));
				dateP = dateP->next;
				lstrcpy(dateP->context, date);
				dateP->contextAmount = 1;
				dateP->next = NULL;
			}

			//The maxDate and maxDateAmount are updated on the fly at each line read from file
			if (dateP->contextAmount > maxDateAmount){
				lstrcpy(maxDate, dateP->context);
				maxDateAmount = dateP->contextAmount;
			}

			//Check maxBranch with same specifications of the maxDate
			found = 0;
			while (true){
				if (branchP->contextAmount != -1){
					if (!lstrcmp(branchP->context, branch)){
						branchP->contextAmount = branchP->contextAmount + 1;
						found = 1;
						break;
					}
				}
				if (branchP->next == NULL)
					break;
				branchP = branchP->next;
			}

			if (!found){
				branchP->next = (dataNode *)malloc(sizeof(dataNode));
				branchP = branchP->next;
				lstrcpy(branchP->context, branch);
				branchP->contextAmount = 1;
				branchP->next = NULL;
			}

			if (branchP->contextAmount > maxBranchAmount){
				lstrcpy(maxBranch, branchP->context);
				maxBranchAmount = branchP->contextAmount;
			}
		}
		//Compute statistics ENDED

		//Memory release
		freeDataList(dateHead);
		freeDataList(branchHead);

		CloseHandle(tradeFile);

		//Write to output (using the related critical section to avoid threads to mix their results)
		EnterCriticalSection(&writeOutputSection);
		DWORD tmpWrite;
		_stprintf(buffer, _T("Thread:%d - %s %s stats: \r\nCars sold: %d\r\nTotal amount: %d\r\nBest date: %s with %d cars\r\nBest branch: %s with %d cars\r\n-----------------------\r\n"), GetCurrentThreadId(), manufacturer, model, carsSold, totAmount, maxDate, maxDateAmount, maxBranch, maxBranchAmount);
		WriteFile(outputHanlde, buffer, (_tcslen(buffer)*sizeof(TCHAR)), &tmpWrite, NULL);
		//Writing concluded, exiting the critical section
		LeaveCriticalSection(&writeOutputSection);
	}

	return 0;
}

LPVOID freeDataList(dataNode *pdata){

	if (pdata == NULL) return 0;

	freeDataList(pdata->next);
	free(pdata);
	return 0;
}
