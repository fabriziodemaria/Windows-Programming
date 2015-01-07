#ifdef BUBBLEMATHFUNCSDLL_EXPORTS
#define BUBBLEMATHFUNCSDLL_API __declspec(dllexport) 
#else
#define BUBBLEMATHFUNCSDLL_API __declspec(dllimport) 
#endif

extern "C" {
	BUBBLEMATHFUNCSDLL_API void sort(int *v, int n);
}