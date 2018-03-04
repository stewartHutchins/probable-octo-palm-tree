// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the CPPSTATISITICSSIFTER_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// CPPSTATISITICSSIFTER_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef CPPSTATISITICSSIFTER_EXPORTS
#define CPPSTATISITICSSIFTER_API __declspec(dllexport)
#else
#define CPPSTATISITICSSIFTER_API __declspec(dllimport)
#endif

// This class is exported from the CppStatisiticsSifter.dll
class CPPSTATISITICSSIFTER_API CCppStatisiticsSifter {
public:
	CCppStatisiticsSifter(void);
	// TODO: add your methods here.
};

extern CPPSTATISITICSSIFTER_API int nCppStatisiticsSifter;

CPPSTATISITICSSIFTER_API int fnCppStatisiticsSifter(void);
