#include "world/world.h"

ThreadPool::ThreadPool(size_t numThreads) : stopFlag(false)
{
    for (size_t i = 0; i < numThreads; ++i)
    {
        workers.emplace_back(&ThreadPool::workerThread, this);
    }
}

ThreadPool::~ThreadPool()
{
    stop();
}

void ThreadPool::enqueueJob(std::function<void()> job)
{
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        jobs.push(job);
    }
    condition.notify_one(); // Notify one worker that a job is available
}

void ThreadPool::stop()
{
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        stopFlag.store(true);
    }
    condition.notify_all(); // Notify all threads to stop

    for (auto &thread : workers)
    {
        if (thread.joinable())
        {
            thread.join(); // Join all threads
        }
    }
}

void ThreadPool::workerThread()
{
    while (true)
    {
        std::function<void()> job;
        {
            std::unique_lock<std::mutex> lock(queueMutex);

            // Wait for a job or stop signal
            condition.wait(lock, [this]
                           { return !jobs.empty() || stopFlag.load(); });

            if (stopFlag.load() && jobs.empty())
            {
                return; // Stop the thread
            }

            job = jobs.front();
            jobs.pop();
        }
        job(); // Execute the job
    }
}