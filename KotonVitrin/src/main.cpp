//|||||||||||||||||||||||||||||||||||||||||||||||
#include "StdAfx.h"
#include "KotonApp.hpp"
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
#if OGRE_PLATFORM == PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
int main(int argc, char **argv)
#endif
{

	HANDLE hMutex = CreateMutex(NULL, FALSE, _T("Local\\KotonMutex"));
	_ASSERTE(hMutex != NULL); // CreateMutex shouldn't fail.
	// Check if we created a new mutex or opened an existing.
	if (GetLastError() == ERROR_ALREADY_EXISTS)
		exit(0);
	// If we get here, then we're the only instance.

	KotonApp demo;
	try	
	{
		demo.startDemo();
	}
	catch(std::exception& e)
    { 
#if OGRE_PLATFORM == PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        MessageBoxA(NULL, e.what(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
        fprintf(stderr, "An exception has occurred: %s\n", e.what());
#endif
    }
 
    return 0;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||