#ifndef LOADINGSCREEN_H_
#define LOADINGSCREEN_H_

#include <functional>
#include <future>

namespace NSEngine {

    extern void nsLoadScreen(std::atomic_bool& run, std::atomic_bool& done);
    extern void blackLoadScreen(std::atomic_bool& run, std::atomic_bool& done);

    class LoadingScreen {

        public:
            static void Load(std::function<void(void)> load, std::function<void(std::atomic_bool&, std::atomic_bool&)> draw);

        private:
            static std::future<void> drawing_task;
            static std::atomic_bool loading;
            static std::atomic_bool done;
    };

}


#endif // LOADINGSCREEN_H_
