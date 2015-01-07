#define UNICODE
#define _UNICODE
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>
#include <stdlib.h>

typedef void(*SORT)(int *, int);

int _tmain(int argc, LPTSTR argv[])
{
	int *d;
	SORT sortme;
	HINSTANCE ff = LoadLibrary(_T("dll3.dll"));
	sortme = (SORT)GetProcAddress(ff,"sort");

	d = (int *)malloc(sizeof(int)*3);
	d[0] = 3;
	d[1] = 1;
	d[2] = 2;

	(sortme)(d,3);

	for (int i = 0; i < 3; i++)
		printf("%d", d[i]);
	Sleep(10000);
		return 0;
}