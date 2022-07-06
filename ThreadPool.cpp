#include "ThreadPool.h"

ThreadPool::ThreadPool()
	:isStop_(false), isTerminated_(false), threadCount_(1u)
{}

ThreadPool::~ThreadPool()
{}

void ThreadPool::Start(size_t _num)
{
	if (_num < 1)
		threadCount_ = 1;
	else if (_num > 30)
		threadCount_ = 30;
	else
		threadCount_ = _num;

	for (size_t i = 0; i < threadCount_; ++i)
	{
		thread* t = new thread(std::bind(&ThreadPool::Run, this, i));//std::make_shared<std::thread>(std::bind(&WK_ThreadPool::run, this, i));
		threadVec_.emplace_back(t);
	}
}

void ThreadPool::Terminate()
{
	call_once(callFlag_, [this] { this->TerminateAll(); });
}

void ThreadPool::AddTask(const function<void()>& task)
{
	if (!isStop_)
	{
		taskQueue_.push(task);
		threadCondition_.notify_one();
	}
}

size_t ThreadPool::TaskConut()
{
	return taskQueue_.size();;
}

void ThreadPool::Run(int _id)
{
	while (true)
	{
		if (isStop_)
			break;

		if (taskQueue_.isEmpty())
		{
			unique_lock<mutex> locker(threadMutex_);
			if (threadCondition_.wait_for(locker, chrono::seconds(60)) == cv_status::timeout)
				continue;
		}

		if (isStop_)
			break;

		function<void()> task(nullptr);
		if (taskQueue_.pop(task) && task != nullptr)
			task();
	}
}

void ThreadPool::TerminateAll()
{
	isStop_ = true;

	threadCondition_.notify_all();

	for (auto& iter : threadVec_)
	{
		if (iter != NULL && iter->joinable())
			iter->join();

		delete iter;
	}

	threadVec_.clear();

	isTerminated_ = true;
}
