#ifndef INTERPOLATOR
#define INTERPOLATOR

#include <iostream>
#include "NSlist.h"

#define ITP_ NSEngine::InterpolateManager::Interpolate

#include "Interpolator_impl.h"

namespace NSEngine {

extern bool GameIsPaused();

class InterpolateManager
{

    private:
        class InterpParent {
            public:
                virtual void Update() {}
                virtual ~InterpParent() {}
        };
        template<typename T>
        class InterpolatorBezier : public InterpParent {
            public:
                InterpolatorBezier(T* val, T start, T finish, T extra_1, T extra_2, uint16_t duration, bool updateOnPause)
                {
                    this->val      = val;
                    *this->val     = start;
                    this->start    = start;
                    this->finish   = finish;
                    this->extra_1  = extra_1;
                    this->extra_2  = extra_2;
                    this->duration = duration-1;
                    this->updateOnPause = updateOnPause;
                }
                T* val;
                T start;
                T finish;
                T extra_1;
                T extra_2;
                bool updateOnPause = false;
                uint16_t time = 0;
                uint16_t duration;
                void Update() override
                {
                    if (!updateOnPause && GameIsPaused()) return;
                    if (time == duration) { *val = finish; InterpolateManager::DeleteCurrent(); return; }
                    T sp0 = extra_1/*/T(duration)*//T(2),sp1 = extra_2/*/T(duration)*//T(2);
                    T a0 = start;
                    T a1 = sp0;
                    T a2 = -sp1 + T(3) * (finish - start) - T(2) * sp0;
                    T a3 = sp1 + sp0 - T(2) * (finish - start);
                    T timeIn = T((float)time/(float)duration);
                    *val = a0 + a1 * timeIn + a2 * timeIn * timeIn + a3 * timeIn * timeIn * timeIn;
                    time++;
                }
                ~InterpolatorBezier() override {};

        };
        template<typename T>
        class Interpolator : public InterpParent {
            public:
                Interpolator(T* val, T start, T finish, uint8_t mode, uint16_t duration, bool updateOnPause)
                {
                    this->val      = val;
                    *this->val     = start;
                    this->start    = start;
                    this->finish   = finish;
                    this->mode     = mode;
                    this->duration = duration-1;
                    this->updateOnPause = updateOnPause;
                }
                Interpolator(T* val, T start, T finish, T extra_1, T extra_2, uint8_t mode, uint16_t duration, bool updateOnPause)
                {
                    this->val      = val;
                    *this->val     = start;
                    this->start    = start;
                    this->finish   = finish;
                    this->extra_1  = extra_1;
                    this->extra_2  = extra_2;
                    this->mode     = mode;
                    this->duration = duration-1;
                    this->updateOnPause = updateOnPause;
                }
                T* val;
                T start;
                T finish;
                T extra_1;
                T extra_2;
                bool updateOnPause = false;
                uint16_t time = 0;
                uint16_t duration;
                uint8_t mode;
                void Update() override
                {
                    if (!updateOnPause && GameIsPaused()) return;
                    if (time == duration) { *val = finish; InterpolateManager::DeleteCurrent(); return; }
                    if (mode == 7)
                        *val+=finish;
                    else if (mode == 17)
                    {
                        *val+=extra_2;
                        extra_2+=finish;
                    }
                    else
                        *val = interpolate(mode, start, finish, duration, time);
                    time++;
                }
                ~Interpolator() override {};
        };

        static int current;
        static bool delcurrent;
        static NSlist<InterpParent> interpolators;

    public:
        template<typename T>
        static void Interpolate(T* val, T start, T finish, uint8_t mode, uint16_t duration, bool updateOnPause = false)
        {
            RemoveReference(val);
            interpolators.insert_back(new Interpolator<T>(val, start, finish, mode, duration, updateOnPause));
        }
        template<typename T>
        static void Interpolate(T* val, T start, T finish, T extra_1, T extra_2, uint8_t mode, uint16_t duration, bool updateOnPause = false)
        {
            RemoveReference(val);
            interpolators.insert_back(new Interpolator<T>(val, start, finish, extra_1, extra_2, mode, duration, updateOnPause));
        }
        template<typename T>
        static void InterpolateBezier(T* val, T start, T finish, T extra_1, T extra_2, uint16_t duration, bool updateOnPause = false)
        {
            RemoveReference(val);
            interpolators.insert_back(new InterpolatorBezier<T>(val, start, finish, extra_1, extra_2, duration, updateOnPause));
        }
        static void Update_All();
        static void Clean_All() { interpolators.clear(); }
        static void DeleteCurrent();
        static int NumOfInterpolators();
        template<typename T>
        static void RemoveReference(T* val)
        {
            int ctr = 0;
            for (auto n = interpolators.front->next; n != interpolators.back; n = n->next)
            {
                if (((Interpolator<T>*)n->object)->val == val)
                {
                    n = n->previous;
                    interpolators.del(ctr);
                    continue;
                }
                ctr++;
            }
        }

};

}

#endif
