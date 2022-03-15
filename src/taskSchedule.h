#ifndef TASKSCHEDULE
#define TASKSCHEDULE

#include <vector>
#include <functional>

namespace NSEngine {

    struct Task {
        int frameUntil = 0;
        std::function<void(void)> fp;
        int cancelCode = -1;
        bool runifPaused = false;
    };

    class taskSchedule {
        public:

            static void Init(int maxTasks) { tasks = new Task[maxTasks]; MaxTasks = maxTasks;}

            static void Update();
            static void cancel(int cancelCode) { cancels.push_back(cancelCode); }
            static void newTask(int frameUntil, void (*fp)(void), int cancelCode = -1, bool runifPaused = false) { (++currentTask)%=MaxTasks; tasks[currentTask] = {frameUntil, fp, cancelCode, runifPaused}; }
            static void newTask(int frameUntil, std::function<void(void)> fp, int cancelCode = -1, bool runifPaused = false) { (++currentTask)%=MaxTasks; tasks[currentTask] = {frameUntil, fp, cancelCode, runifPaused}; }

            static void Clean() { delete [] tasks; }

        private:
            static Task* tasks;
            static int MaxTasks;
            static int currentTask;
            static std::vector<int> cancels;
    };

}

#endif
