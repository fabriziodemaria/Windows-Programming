#define UNICODE
#define _UNICODE
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS
#define RECORD_LEN 512

#include <windows.h>
#include <tchar.h>
#include <stdio.h>

LPVOID CreateBuffer(HANDLE f, DWORD bufSize); // called for an open file
BOOL ReadFileBuffered(HANDLE f, LPVOID buf,DWORD nBytesToRead, LPDWORD nBytesRead);
LPVOID FreeBuffer(LPVOID buf);


int _tmain(int argc, LPTSTR argv[])
{
	HANDLE fileH;
	fileH = CreateFile(argv[1],GENERIC_READ,0,NULL,OPEN_EXISTING,0,NULL);

	CreateBuffer(fileH, 10);
	

}


LPVOID CreateBuffer(HANDLE f, DWORD bufSize){
	LPVOID buffer;
	DWORD nRd;
	buffer = (LPVOID)malloc(bufSize);
	
	ReadFileBuffered(f, buffer, 1, &nRd);

	return;
}

BOOL ReadFileBuffered(HANDLE f, LPVOID buf, DWORD nBytesToRead, LPDWORD nBytesRead){


}