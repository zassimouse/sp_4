#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>

#include "TaskQueue.h"
#include "ThreadProcessor.h"

using namespace std;

#define SRC_FILEPATH "..\\src.txt"
#define OUT_FILEPATH "../out.txt"
#define THREAD_COUNT 8

vector<string> mergeArrays(vector<string> v1, vector<string> v2);
vector<string> readFile(string filePath);
void writeFile(vector<string> sortedData);
void Sorting(void* content);
vector<Task*> initTasks(vector<string> allWords);

vector<vector<string>*> slices;

int main()
{
    auto words = readFile(SRC_FILEPATH);
    auto tasks = initTasks(words);
    TaskQueue taskQueue;
    for (Task* task : tasks) {
        taskQueue.EnqueueTask(task);
    }
    ThreadProcessor processor(&taskQueue);
    processor.ExecuteTasks(THREAD_COUNT);
    vector<string> result((slices[0])->begin(), (slices[0])->end());
    for (int i = 1; i < slices.size(); i++)
    {
        result = mergeArrays(result, *(slices[i]));
    }

    writeFile(result);
    return 0;
}

void writeFile(vector<string> sortedData)
{
    std::ofstream file(OUT_FILEPATH);
    for (string line : sortedData) {
        file << line << "\n";
    }
}

vector<string> readFile(string filePath)
{
    ifstream file(filePath);
    string word;
    vector<string> res;
    while (file >> word)
    {
        res.push_back(word);
    }
    return res;
}

void Sorting(void* content)
{
    vector<string>* obj = (vector<string>*)content;
    sort(obj->begin(), obj->end());
}


vector<Task*> initTasks(vector<string> allWords) {
    int wordsPerThread = ceil((double)allWords.size() / THREAD_COUNT);
    vector<Task*> tasks;
    for (int i = 0; i < THREAD_COUNT; ++i) {
        vector<string>* slice = new vector<string>(allWords.begin() + i * wordsPerThread,
            allWords.size() < (i + 1) * wordsPerThread ?
            allWords.end() : allWords.begin() + (i + 1) * wordsPerThread);
        Task* t = new Task(Sorting, (void*)slice);
        slices.push_back(slice);
        tasks.push_back(t);
    }
    return tasks;

}

vector<string> mergeArrays(vector<string> v1, vector<string> v2)
{
    int i = 0, j = 0;
    vector<string> res{};
    while (i < v1.size() && j < v2.size()) {
        if (v1[i] < v2[j])
            res.push_back(v1[i++]);
        else
            res.push_back(v2[j++]);
    }
    while (i < v1.size())
        res.push_back(v1[i++]);
    while (j < v2.size())
        res.push_back(v2[j++]);
    return res;
}