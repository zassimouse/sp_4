#pragma once
#include <Windows.h>
#include <queue>
#include <string>
#include "Task.h"


class TaskQueue
{
private:
	CRITICAL_SECTION criticalSection;
	std::queue<Task*> tasks;
public:
	TaskQueue();
	void EnqueueTask(Task* task);
	Task* DequeueTask();
	bool isEmpty();
	~TaskQueue();
};