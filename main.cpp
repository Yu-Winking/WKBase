#include <iostream>

#include <windows.h>
#include <signal.h>

#include "ThreadPool.h"

using namespace std;

void display(int _id)
{
	//cout << _id << endl;
	Sleep(500);
}

int main()
{
	cout << "Start" << endl;

	ThreadPool pool;
	pool.Start(10);

	for (int i=0; i < 1000; ++i)
	{
		const function<void()>& task = [i] { display(i); };
		pool.AddTask(task);
	}
		

	size_t count{ 0 };
	do
	{
		count = pool.TaskConut();
		cout << "Task Count:" << count << endl;
		Sleep(1000);
	}
	while (count > 0);
		

	pool.Terminate();
	
	Sleep(1000);

	cout << "End" << endl;

	return 0;
}