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

/*
Fabrizio Demaria - s207063
Lab11 - Politecnico di Torino
26-05-2014
*/

//Global data
HANDLE *allSynchEvent;
HANDLE *restartEvent;
LPTSTR *names;			//names to be compared are saved globaly 
bool *thended;          //bool array to know at each cycle which thread ended
HANDLE *hThreadArray;   //handles of scanning threads is global and can be reached by checkThread

//Parameters struct passed to the threads
typedef struct myParam {
	LPTSTR path;
	DWORD tmpId;
} parstruct;

//Prototypes
DWORD WINAPI doOperarations(LPVOID lpParam);
static void TraverseDirectoryRecursive(LPTSTR, DWORD, DWORD, bool);
DWORD FileType(LPWIN32_FIND_DATA);
void convertInputArgs(parstruct *inputs, DWORD num);
DWORD WINAPI checkOperation(LPVOID lpParam);

int _tmain(int argc, LPTSTR argv[])
{
	int i, j, g;
	HANDLE checkThread;
	DWORD   *dwThreadIdArray, threadIdCheck;
	parstruct *myParam;

	if (argc <= 2) {
		_tprintf(_T("Insert at least two paths as input arguments...\n\nExiting..."));
		exit(0);
	}

	j = argc - 1; //number of threads passed to the function

	//Allocation of the dynamic elements of the program 
	//according to the number of paths passed to the function
	allSynchEvent = (HANDLE *)malloc(j*sizeof(HANDLE));
	names = (LPTSTR *)malloc(j*sizeof(MAX_PATH * sizeof(TCHAR)));
	for (int y = 0; y < j; y++)
		names[y] = (TCHAR *)malloc(MAX_PATH * sizeof(TCHAR));
	thended = (bool *)malloc(j*sizeof(bool));
	restartEvent = (HANDLE *)malloc(j*sizeof(HANDLE));
	hThreadArray = (HANDLE *)malloc(j*sizeof(HANDLE));
	dwThreadIdArray = (DWORD *)malloc(j*sizeof(DWORD));
	myParam = (parstruct *)malloc(j*sizeof(parstruct));

	//Creating the events to synchronize the checking thread
	for (g = 0; g < j; g++){
		allSynchEvent[g] = CreateEvent(NULL, FALSE, FALSE, NULL);
		if (allSynchEvent[g] == NULL)
		{
			printf("CreateEvent failed (%d)\n", GetLastError());
		}
	}

	//Creating the events to let the threads start again after a check
	for (g = 0; g < j; g++){
		restartEvent[g] = CreateEvent(NULL, FALSE, FALSE, NULL);
		if (restartEvent[g] == NULL)
		{
			printf("CreateEvent failed (%d)\n", GetLastError());
		}
	}

	//Initialization of the struct and the bool array before creating the threads
	for (i = 0; i < j; i++){
		myParam[i].path = argv[i + 1];
		myParam[i].tmpId = i;			//this is used to assign a range of int from 0 to (argc-1) to the threads
		thended[i] = false;				//this initialise the array of bools to false, meaning no thread exited so far
	}

	//Creating the chacking thread plus the argc-1 scanning threads
	checkThread = CreateThread(NULL, 0, checkOperation, &j, 0, &threadIdCheck);
	for (i = 0; i<j; i++){
		hThreadArray[i] = CreateThread(
			NULL,                   // default security attributes
			0,                      // use default stack size  
			doOperarations,         // thread function name
			&myParam[i],            // argument to thread function 
			0,                      // use default creation flags 
			&dwThreadIdArray[i]);   // returns the thread identifier
	}
	
	//Waiting for all the threads completion
	WaitForMultipleObjects(j, hThreadArray, TRUE, INFINITE);
	WaitForSingleObject(checkThread, INFINITE);

	_tprintf(_T("\n\nExiting...\n"));
	Sleep(1000);
	return 0;
}


DWORD WINAPI doOperarations(LPVOID lpParam)
{
	parstruct *a = (parstruct *)lpParam;
	LPTSTR PathName = a->path;				//retrieve the path to be passed to the Traversing function

	TraverseDirectoryRecursive(PathName, 1, a->tmpId, true);

	return 0;
}


static void
TraverseDirectoryRecursive(
LPTSTR PathName,
DWORD level,
DWORD tmpId,
bool isroot
)
{
	HANDLE SearchHandle;
	WIN32_FIND_DATA FindData;
	DWORD FType, i;

	TCHAR tmp[MAX_PATH];

	lstrcpy(tmp, PathName);
	lstrcat(tmp, _T("\\*"));

	SearchHandle = FindFirstFile(tmp, &FindData);	//SearchHandle adopted by using absoulte paths 

	do {
		FType = FileType(&FindData);
		//_tprintf(_T("[%d] I'm in %s\nI found this: %s\n\n\n"), tmpId, tmp, FindData.cFileName);
		
		if (FType == TYPE_FILE) {	
			//The name of the file found is stored in the global array "names" 
			//at the position assigned to the current thread
			lstrcpy((LPTSTR)names[tmpId], FindData.cFileName);
			//Signaling the checkThread that the current thread has set the name of the file found
			SetEvent(allSynchEvent[tmpId]);
			//Waiting for the checkThread to signal the restart of the scanning operation
			DWORD dwWaitResult;
			dwWaitResult = WaitForSingleObject(
				restartEvent[tmpId], // event handle
				INFINITE);			 // indefinite wait
		}


		if (FType == TYPE_DIR) {	
			lstrcpy(tmp, PathName);
			lstrcat(tmp, _T("\\"));
			lstrcat(tmp, FindData.cFileName);

			//The name of the directory found is stored in the global array "names" 
			//at the position assigned to the current thread
			lstrcpy((LPTSTR)names[tmpId],FindData.cFileName);
			//Signaling the checkThread that the current thread has set the name of the directory found
			SetEvent(allSynchEvent[tmpId]);
			//Waiting for the checkThread to signal the restart of the scanning operation
			DWORD dwWaitResult;
			dwWaitResult = WaitForSingleObject(
				restartEvent[tmpId], // event handle
				INFINITE);		     // indefinite wait

			/* Recur - Indicating that we are no more in the "root" of the scanning procedure*/
			TraverseDirectoryRecursive(tmp, level + 1, tmpId, false);
		}
	} while (FindNextFile(SearchHandle, &FindData));

	/*
	*if here and "isroot" is true, then the thread has ended the scanning. Before returning, the thread
	*set the location of the bool array corresponing to its tmpId to true thus indicating to the checkThread that 
	*it is exiting and the value at names[tmpId] is not to be checked. Next it signals the checkThread and returns
	*/
	if (isroot){
		thended[tmpId] = true;
		SetEvent(allSynchEvent[tmpId]);
	}
+	FindClose(SearchHandle);

	return;
}

DWORD WINAPI checkOperation(LPVOID lpParam){

	DWORD *t = (DWORD *)lpParam;
	int i = *t, g;
	DWORD dwWaitResult;

	while (1)
	{
		//Wait for all the scanning thread to save the found names and signal
		dwWaitResult = WaitForMultipleObjects(i, allSynchEvent, TRUE, INFINITE);

		//setting "tmp" equal to the number of scanning threads that exited
		int tmp = 0;
		for (g = 0; g < i; g++){
			if (thended[g] == true) tmp++;
		}

		//all scanning threads exited, so they could complete their operation till the end
		//and the paths' contents are the same. This is printed before closing this thread
		if (tmp == i) break;

		for (g = 0; g < (i-1); g++){
			if (lstrcmp(names[g], names[g + 1]) || (tmp!=0 && tmp!=i))
			{
				//if here, either the names found by the scanning threads are different or
				//some threads ended the scan and others didn't. In both cases the contents
				//of the paths passed as argument to the program are different
				_tprintf(_T("\n\nThe content of the paths is NOT the same\n"));
				
				//close opened scanning threads before returning
				for (int c = 0; c < i; c++){
					if (thended[c] == false){
						TerminateThread(hThreadArray[c], 1);
					}
				}
				return 1;
			}			
		}

		//Signaling the scanning threads that the check has been completed and they can proceed
		for (g = 0; g < i; g++){
			SetEvent(restartEvent[g]);
		}
	}
	
	//If here the infinite while broke meaning that the contents match
	_tprintf(_T("\n\The content of the paths is the same\n"));
	return 0;
}

static DWORD FileType(LPWIN32_FIND_DATA pFileData)
{
	BOOL IsDir;
	DWORD FType;
	FType = TYPE_FILE;
	IsDir = (pFileData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
	if (IsDir)
		if (lstrcmp(pFileData->cFileName, _T(".")) == 0
			|| lstrcmp(pFileData->cFileName, _T("..")) == 0)
			FType = TYPE_DOT;
		else FType = TYPE_DIR;
		return FType;
}