#include "ThreadProcessor.h"

ThreadProcessor::ThreadProcessor(TaskQueue* queue)
{
	this->queue = queue;
}

DWORD __stdcall ThreadProcessor::ProcessTask(void* vqueue)
{
	TaskQueue* queue = (TaskQueue*)vqueue;
	if (!queue->isEmpty())                // выполнение таски
	{
		Task* task = queue->DequeueTask();
		task->func(task->params);
	}

	return 0;
}

void ThreadProcessor::ExecuteTasks(int threadCount)
{
	HANDLE* threads = (HANDLE*)malloc(sizeof(HANDLE) * threadCount);
	for (int i = 0; i < threadCount; i++)
	{
		HANDLE t = CreateThread(NULL, 0, &ThreadProcessor::ProcessTask, (LPVOID)(this->queue), CREATE_SUSPENDED, NULL);
		threads[i] = t;
	}

	for (int i = 0; i < threadCount; i++)
	{
		ResumeThread(threads[i]);
	}

	WaitForMultipleObjects(threadCount, threads, TRUE, INFINITE);
}