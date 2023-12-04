#include "TaskQueue.h"

TaskQueue::TaskQueue()
{
	InitializeCriticalSectionAndSpinCount(&criticalSection, 0x00000400);
}

void TaskQueue::EnqueueTask(Task* task)
{
	EnterCriticalSection(&criticalSection);
	tasks.push(task);
	LeaveCriticalSection(&criticalSection);
}

Task* TaskQueue::DequeueTask()
{
	while (tasks.empty()) 
	{
	}
	EnterCriticalSection(&criticalSection);
	Task* task = tasks.front();
	tasks.pop();
	LeaveCriticalSection(&criticalSection);
	return task;
}

bool TaskQueue::isEmpty()
{
	return tasks.empty();
}

TaskQueue::~TaskQueue()
{
	while (!isEmpty())
	{
		DequeueTask();
	}
	DeleteCriticalSection(&criticalSection);
}
