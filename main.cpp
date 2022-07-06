#include <iostream>

#include <windows.h>
#include <signal.h>

#include "ThreadPool.h"

using namespace std;

int main()
{
	cout << "Start" << endl;

	ThreadPool pool;
	pool.Start(10);

	cout << pool.TaskConut() << endl;

	pool.Terminate();
	
	Sleep(1000);

	cout << "End" << endl;

	return 0;
}