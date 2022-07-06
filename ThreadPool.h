#pragma once

#include <future>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <memory>
#include <functional>
#include <condition_variable>
#include <atomic>
#include <type_traits>

#include "../MySTL/wkSTL.h"

using namespace std;

class ThreadPool
{
public:
	ThreadPool(ThreadPool const&) = delete;
	ThreadPool(ThreadPool&&) = delete;
	ThreadPool& operator=(ThreadPool const&) = delete;
	ThreadPool& operator=(ThreadPool&&) = delete;

public:
	ThreadPool();
	virtual ~ThreadPool();

	void Start(size_t _num);
	void Terminate();

    void AddTask(const function<void()>& task);
    size_t TaskConut();

private:
    void Run(int _id);
    void TerminateAll();

private:
    atomic<bool> isStop_;
    atomic<bool> isTerminated_;

    size_t threadCount_;

    vector<thread*>     threadVec_;
    mutex               threadMutex_;
    condition_variable  threadCondition_;

    wkSTL::Queue<function<void()>> taskQueue_;

    once_flag   callFlag_;

};

