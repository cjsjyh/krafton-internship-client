////////////////////////////////////////////////////////////////////////////////
// Filename: main.cpp
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "systemclass.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	SystemClass* System;
	bool result;
	
	
	// Create the system object.
	System = new SystemClass;
	if(!System)
	{
		return 0;
	}

	AllocConsole();
	freopen("CONOUT$", "wb", stdout);

	// Initialize and run the system object.
	result = System->Initialize();
	if(result)
	{
		System->Run();
	}
	

	// Shutdown and release the system object.
	System->Shutdown();
	delete System;
	System = 0;

	cout << "done2" << endl;

	return 0;
}