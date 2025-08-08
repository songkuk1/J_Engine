#include <iostream>
#include <memory>
#include <windows.h>

#include "TestApp.h"

using namespace std;

int main()
{
	JEngine::TestApp testapp;

	if (!testapp.Initialize())
	{
		cout << "Failed to initialize main window." << endl;
		return -1;
	}
	return testapp.Run();
}