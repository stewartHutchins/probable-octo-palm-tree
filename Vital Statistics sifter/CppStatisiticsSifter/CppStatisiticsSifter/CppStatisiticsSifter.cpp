// CppStatisiticsSifter.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "CppStatisiticsSifter.h"


// This is an example of an exported variable
CPPSTATISITICSSIFTER_API int nCppStatisiticsSifter=0;

// This is an example of an exported function.
CPPSTATISITICSSIFTER_API int fnCppStatisiticsSifter(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see CppStatisiticsSifter.h for the class definition
CCppStatisiticsSifter::CCppStatisiticsSifter()
{
	return;
}
