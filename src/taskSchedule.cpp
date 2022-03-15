#include "taskSchedule.h"
#include "NSEngine.h"

template<typename T>
bool findInVector(std::vector<T> vec, T val)
{
    for (T t : vec) if (t == val) return true;
    return false;
}

namespace NSEngine {

    int taskSchedule::MaxTasks;
    int taskSchedule::currentTask;
    Task* taskSchedule::tasks;
    std::vector<int> taskSchedule::cancels;

    void taskSchedule::Update()
    {
        for (int i = 0; i < MaxTasks; i++)
        {
            if (tasks[i].frameUntil > 0)
            {
                if (!tasks[i].runifPaused && engineData::gameflags&0b10000000) continue;
                if (findInVector(cancels, tasks[i].cancelCode))
                {
                    tasks[i].frameUntil = 0;
                    continue;
                }
                if (--tasks[i].frameUntil == 0)
                {
                    tasks[i].fp();
                }
            }
        }
        cancels.clear();
    }

}
