#ifdef BUFFMATHFUNCSDLL_EXPORTS
#define BUFFMATHFUNCSDLL_API __declspec(dllexport) 
#else
#define BUFFMATHFUNCSDLL_API __declspec(dllimport) 
#endif

extern "C" {
	//BUFFMATHFUNCSDLL_API void sort(int *v, int n);
}