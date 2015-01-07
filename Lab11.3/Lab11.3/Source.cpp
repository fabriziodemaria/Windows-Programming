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

typedef struct myParam {
	LPTSTR path;
	HANDLE outputfile;
} parstruct;


DWORD WINAPI doOperarations(LPVOID lpParam);
static void TraverseDirectoryRecursive(LPTSTR, DWORD, HANDLE);
DWORD FileType(LPWIN32_FIND_DATA);
void convertInputArgs(parstruct *inputs, DWORD num);



int _tmain(int argc, LPTSTR argv[])
{
	int i, j;
	HANDLE  *hThreadArray, *account;
	DWORD   *dwThreadIdArray;
	parstruct *myParam;
	HANDLE *hOut, outFile;

	j = argc-1;

	hThreadArray = (HANDLE *)malloc(j*sizeof(HANDLE));
	dwThreadIdArray = (DWORD *)malloc(j*sizeof(DWORD));
	hOut = (HANDLE *)malloc(j*sizeof(HANDLE));
	myParam = (parstruct *)malloc(j*sizeof(parstruct));

	for (i = 0; i < j; i++){	
		myParam[i].outputfile = hOut[i];
		myParam[i].path = argv[i + 1];
	}
	
	//Convert realtive path inputs in absoulte path in the struct passed to the threads
	convertInputArgs(myParam, (argc - 1));

	//Creating the directory where the threads' output files will be save
	CreateDirectory(_T("C:/Users/Fabrizio/Desktop/Temp"), NULL);


	for (i = 0; i<j; i++){
		hThreadArray[i] = CreateThread(
			NULL,                   // default security attributes
			0,                      // use default stack size  
			doOperarations,         // thread function name
			&myParam[i],              // argument to thread function 
			0,                      // use default creation flags 
			&dwThreadIdArray[i]);   // returns the thread identifier
	}

	WaitForMultipleObjects(j, hThreadArray, TRUE, INFINITE);

	//Creating the result file in the Desktop
	TCHAR fileLoc[MAX_PATH];
	lstrcpy(fileLoc, _T("C:/Users/Fabrizio/Desktop/"));
	TCHAR tmp[20];
	wsprintf(tmp, _T("Result.txt"));
	LPTSTR tmp2 = (LPTSTR)tmp;
	lstrcat(fileLoc, tmp2);
	outFile = CreateFile(fileLoc, GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	
	//Writing the result file according to the threads' output files
	for (i = 0; i < j; i++){
		//_tprintf(_T("New file - %d\n"), i);
		TCHAR buffer[10];
		DWORD tmpRead, tmpWrite;
		SetFilePointer(myParam[i].outputfile, 0, NULL, FILE_BEGIN);
		while (ReadFile(myParam[i].outputfile, buffer, 10, &tmpRead, NULL) && tmpRead > 0){
			//int m=0;
			//while (m < tmpRead/2) //Note that tmpRead must be divided by two because of UNICODE file reading
			//{
			//	_tprintf(_T("%c"), buffer[m++]);
			//	
			//}
			WriteFile(outFile, buffer, tmpRead, &tmpWrite, NULL);
			/*_tprintf(_T("%s"), buffer);
			memset(buffer, 0, MAX_PATH + 100);*/

		}
	}

	_tprintf (_T("End\n")); 

	return 0;
}


DWORD WINAPI doOperarations(LPVOID lpParam)
{
	parstruct *a = (parstruct *)lpParam;
	LPTSTR PathName = a->path;
	TCHAR fileLoc[MAX_PATH];

	lstrcpy(fileLoc, _T("C:/Users/Fabrizio/Desktop/Temp/"));
	TCHAR tmp[20];
	wsprintf(tmp, _T("Thread%d.txt"), GetCurrentThreadId());
	LPTSTR tmp2 = (LPTSTR)tmp;
	lstrcat(fileLoc, tmp2);
	a->outputfile = CreateFile(fileLoc, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL,
		OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (a->outputfile == INVALID_HANDLE_VALUE) {
		printf("Cannot open output file. Error: %x\n", GetLastError());
		return 3;
	}
	
	_tprintf(_T("Test --- %d ---- %s\n"), GetCurrentThreadId(), PathName);

	TraverseDirectoryRecursive(PathName, 1, a->outputfile);

	return 0;

}


static void
TraverseDirectoryRecursive(
LPTSTR PathName,
DWORD level,
HANDLE file
)
{
	HANDLE SearchHandle;
	WIN32_FIND_DATA FindData;
	DWORD FType, i;

	TCHAR tmp[MAX_PATH];
	TCHAR tmp2[MAX_PATH];

	lstrcpy(tmp, PathName);
	lstrcat(tmp, _T("/*"));

	SearchHandle = FindFirstFile(tmp, &FindData);

	do {
		FType = FileType(&FindData);

		if (FType == TYPE_FILE) {
			/* Printing 
			for (i = 0; i<level; i++)
				_tprintf(_T("  "), level);
			_tprintf(_T("level=%d FILE: %s\n"), level, FindData.cFileName);
			*/
		}


		if (FType == TYPE_DIR) {
			TCHAR buffer[MAX_PATH+100];
			TCHAR buffer2[MAX_PATH + 100];
			memset(buffer, 0, (MAX_PATH + 100));
			/* Printing */
			lstrcpy(buffer, _T(""));
			for (i = 0; i < level; i++){
				lstrcat(buffer, _T(" "));
		//		_tprintf(_T("  "), level);
		
			}

			wsprintf(buffer2, _T("THREAD: %d - level=%d DIR : %s \r\n"), GetCurrentThreadId(), level, FindData.cFileName);
			lstrcat(buffer,buffer2);
			
		//	_tprintf(_T("THREAD: %d - level=%d DIR : %s\n"), GetCurrentThreadId(), level, FindData.cFileName);

			DWORD size = lstrlen(buffer)*sizeof(TCHAR);
			DWORD written;
			WriteFile(file, buffer, size, &written, NULL);

			lstrcpy(tmp2, PathName);
			lstrcat(tmp2, _T("/"));
			lstrcat(tmp2, FindData.cFileName);
			/* Recur */

			TraverseDirectoryRecursive(tmp2, level + 1, file);

			/* Move backward one level in the tree hierarchy */
			//SetCurrentDirectory(_T(".."));
		}
	} while (FindNextFile(SearchHandle, &FindData));

	FindClose(SearchHandle);

	return;
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


void convertInputArgs(parstruct *inputs, DWORD num){

	int i = 0;

	for (i = 0; i < num; i++) {
		TCHAR tmp[MAX_PATH];
		TCHAR tmp2[MAX_PATH];
		
		GetCurrentDirectory(MAX_PATH, tmp);
		SetCurrentDirectory(inputs[i].path);
		GetCurrentDirectory(MAX_PATH, tmp2);
		SetCurrentDirectory(tmp);
		_tprintf(_T("%s\n"),tmp2);
	}
}