/* some macros to avoid typing the same exiting code 100 times */
#define ERR_RET(errTxt) _ftprintf(stderr, _T(errTxt));  \
                        system("pause");                \
                        TerminateProcess(GetCurrentProcess(),1);
  
#define ERR_RET_STR_PARAM(errTxt, str) _ftprintf(stderr, _T(errTxt),str);   \
                        system("pause");                \
                        TerminateProcess(GetCurrentProcess(),1); 

#define UNICODE
#define _UNICODE
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS
#define RECORD_LEN 512

#include <windows.h>
#include <tchar.h>
#include <stdio.h>

int _tmain(int argc, LPTSTR argv[])
{
	HANDLE theFile;
	TCHAR aLine[RECORD_LEN];
	DWORD bOut;

	if(argc != 2)
	{
		ERR_RET("put a correct file\n");
	}

	/* PRODUCT FILE */
	theFile = CreateFile(argv[1], GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
	printf("%d\n", GetLastError());

	if(theFile == NULL)
	{
		ERR_RET("invalid handle\n");
	}

	_stprintf(aLine, _T("FIAT 500 file500.txt\n"));
	if(!WriteFile(theFile, (LPVOID) aLine,RECORD_LEN*sizeof(TCHAR),&bOut, NULL))
	{
		ERR_RET("can't write\n");
	}

	_stprintf(aLine, _T("ALFA MiTo fileMito.txt\n"));
	if (!WriteFile(theFile, (LPVOID)aLine, RECORD_LEN*sizeof(TCHAR), &bOut, NULL))
	{
		ERR_RET("can't write\n");
	}

	_stprintf(aLine, _T("BMW serie1 bmwS1.txt\n"));
	if(!WriteFile(theFile, (LPVOID) aLine,RECORD_LEN*sizeof(TCHAR),&bOut, NULL))
	{
		ERR_RET("can't write\n");
	}
	_stprintf(aLine, _T("PEUGEOT 206 file206.txt\n"));
	if(!WriteFile(theFile, (LPVOID) aLine,RECORD_LEN*sizeof(TCHAR),&bOut, NULL))
	{
		ERR_RET("can't write\n");
	}
	CloseHandle(theFile);


	/* file500.txt FILE */
	theFile = CreateFile(_T("file500.txt"), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
	printf("%d\n", GetLastError());

	if(theFile == NULL)
	{
		ERR_RET("invalid handle\n");
	}

	_stprintf(aLine, _T("fiatIt001 04.01.2014 15131\n"));
	if(!WriteFile(theFile, (LPVOID) aLine,RECORD_LEN*sizeof(TCHAR),&bOut, NULL))
	{
		ERR_RET("can't write\n");
	}

	_stprintf(aLine, _T("fiatIt006 04.01.2014 17000\n"));
	if(!WriteFile(theFile, (LPVOID) aLine,RECORD_LEN*sizeof(TCHAR),&bOut, NULL))
	{
		ERR_RET("can't write\n");
	}

	_stprintf(aLine, _T("dddXYZ 21.01.2014 13245\n"));
    if(!WriteFile(theFile, (LPVOID) aLine,RECORD_LEN*sizeof(TCHAR),&bOut, NULL))
	{
		ERR_RET("can't write\n");
	}
    
    _stprintf(aLine, _T("dddXYZ 21.01.2014 16245\n"));
    if(!WriteFile(theFile, (LPVOID) aLine,RECORD_LEN*sizeof(TCHAR),&bOut, NULL))
	{
		ERR_RET("can't write\n");
	}
    
	CloseHandle(theFile);

	/* fileMito.txt FILE */
	theFile = CreateFile(_T("fileMito.txt"), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
	printf("%d\n", GetLastError());

	if (theFile == NULL)
	{
		ERR_RET("invalid handle\n");
	}

	_stprintf(aLine, _T("alfa1 04.01.2014 10\n"));
	if (!WriteFile(theFile, (LPVOID)aLine, RECORD_LEN*sizeof(TCHAR), &bOut, NULL))
	{
		ERR_RET("can't write\n");
	}

	_stprintf(aLine, _T("alfa1 04.01.2014 11\n"));
	if (!WriteFile(theFile, (LPVOID)aLine, RECORD_LEN*sizeof(TCHAR), &bOut, NULL))
	{
		ERR_RET("can't write\n");
	}

	_stprintf(aLine, _T("alfa1 12.11.2014 31\n"));
	if (!WriteFile(theFile, (LPVOID)aLine, RECORD_LEN*sizeof(TCHAR), &bOut, NULL))
	{
		ERR_RET("can't write\n");
	}

	_stprintf(aLine, _T("alfa1 21.01.2014 12\n"));
	if (!WriteFile(theFile, (LPVOID)aLine, RECORD_LEN*sizeof(TCHAR), &bOut, NULL))
	{
		ERR_RET("can't write\n");
	}

	_stprintf(aLine, _T("alfa1 21.01.2014 15\n"));
	if (!WriteFile(theFile, (LPVOID)aLine, RECORD_LEN*sizeof(TCHAR), &bOut, NULL))
	{
		ERR_RET("can't write\n");
	}

	_stprintf(aLine, _T("alfa2 21.01.2014 2\n"));
	if (!WriteFile(theFile, (LPVOID)aLine, RECORD_LEN*sizeof(TCHAR), &bOut, NULL))
	{
		ERR_RET("can't write\n");
	}

	_stprintf(aLine, _T("alfa2 21.01.2014 23\n"));
	if (!WriteFile(theFile, (LPVOID)aLine, RECORD_LEN*sizeof(TCHAR), &bOut, NULL))
	{
		ERR_RET("can't write\n");
	}

	_stprintf(aLine, _T("alfa3 11.01.2013 56\n"));
	if (!WriteFile(theFile, (LPVOID)aLine, RECORD_LEN*sizeof(TCHAR), &bOut, NULL))
	{
		ERR_RET("can't write\n");
	}

	_stprintf(aLine, _T("alfa4 12.11.2014 21\n"));
	if (!WriteFile(theFile, (LPVOID)aLine, RECORD_LEN*sizeof(TCHAR), &bOut, NULL))
	{
		ERR_RET("can't write\n");
	}

	_stprintf(aLine, _T("alfa3 12.11.2014 11\n"));
	if (!WriteFile(theFile, (LPVOID)aLine, RECORD_LEN*sizeof(TCHAR), &bOut, NULL))
	{
		ERR_RET("can't write\n");
	}


	CloseHandle(theFile);

	/* bmwS1.txt FILE */
	theFile = CreateFile(_T("bmwS1.txt"), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
	printf("%d\n", GetLastError());

	if(theFile == NULL)
	{
		ERR_RET("invalid handle\n");
	}

	_stprintf(aLine, _T("bmwIt001 04.01.2014 30131\n"));
	if(!WriteFile(theFile, (LPVOID) aLine,RECORD_LEN*sizeof(TCHAR),&bOut, NULL))
	{
		ERR_RET("can't write\n");
	}

	_stprintf(aLine, _T("bmwIt001 09.01.2014 35131\n"));
	if(!WriteFile(theFile, (LPVOID) aLine,RECORD_LEN*sizeof(TCHAR),&bOut, NULL))
	{
		ERR_RET("can't write\n");
	}

	_stprintf(aLine, _T("bmwIt006 04.01.2014 85000\n"));
	if(!WriteFile(theFile, (LPVOID) aLine,RECORD_LEN*sizeof(TCHAR),&bOut, NULL))
	{
		ERR_RET("can't write\n");
	}

	_stprintf(aLine, _T("agnagnaw 21.01.2014 1000245\n"));
	if(!WriteFile(theFile, (LPVOID) aLine,RECORD_LEN*sizeof(TCHAR),&bOut, NULL))
	{
		ERR_RET("can't write\n");
	}
	CloseHandle(theFile);

	
	/* file206.txt FILE */
	theFile = CreateFile(_T("file206.txt"), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
	printf("%d\n", GetLastError());

	if(theFile == NULL)
	{
		ERR_RET("invalid handle\n");
	}

	_stprintf(aLine, _T("peugeotIt001 04.04.2014 15131\n"));
	if(!WriteFile(theFile, (LPVOID) aLine,RECORD_LEN*sizeof(TCHAR),&bOut, NULL))
	{
		ERR_RET("can't write\n");
	}

	_stprintf(aLine, _T("peugeotIt001 01.04.2014 18131\n"));
	if (!WriteFile(theFile, (LPVOID)aLine, RECORD_LEN*sizeof(TCHAR), &bOut, NULL))
	{
		ERR_RET("can't write\n");
	}

	_stprintf(aLine, _T("peugeotIt001 01.04.2014 35131\n"));
	if (!WriteFile(theFile, (LPVOID)aLine, RECORD_LEN*sizeof(TCHAR), &bOut, NULL))
	{
		ERR_RET("can't write\n");
	}

	_stprintf(aLine, _T("peugeotIt011 04.05.2014 45131\n"));
	if (!WriteFile(theFile, (LPVOID)aLine, RECORD_LEN*sizeof(TCHAR), &bOut, NULL))
	{
		ERR_RET("can't write\n");
	}

	_stprintf(aLine, _T("peugeotIt006 04.05.2014 22131\n"));
	if (!WriteFile(theFile, (LPVOID)aLine, RECORD_LEN*sizeof(TCHAR), &bOut, NULL))
	{
		ERR_RET("can't write\n");
	}

	_stprintf(aLine, _T("peugeotIt006 04.12.2014 10000\n"));
	if(!WriteFile(theFile, (LPVOID) aLine,RECORD_LEN*sizeof(TCHAR),&bOut, NULL))
	{
		ERR_RET("can't write\n");
	}

	_stprintf(aLine, _T("PSA_centre 29.01.2014 25245\n"));
	if(!WriteFile(theFile, (LPVOID) aLine,RECORD_LEN*sizeof(TCHAR),&bOut, NULL))
	{
		ERR_RET("can't write\n");
	}

	_stprintf(aLine, _T("peugeotIt006 04.11.2014 12345\n"));
	if(!WriteFile(theFile, (LPVOID) aLine,RECORD_LEN*sizeof(TCHAR),&bOut, NULL))
	{
		ERR_RET("can't write\n");
	}

	_stprintf(aLine, _T("peugeotIt006 01.12.2014 85426\n"));
	if(!WriteFile(theFile, (LPVOID) aLine,RECORD_LEN*sizeof(TCHAR),&bOut, NULL))
	{
		ERR_RET("can't write\n");
	}
	CloseHandle(theFile);

	system("pause");
	return 0;
}