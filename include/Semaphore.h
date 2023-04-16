#include <mutex>
#include <condition_variable>
using namespace std;

#ifndef __SEMAPHORE_H__
#define __SEMAPHORE_H__

class Semaphore
{
private:
    mutex mutex_;
    condition_variable condition_;
    unsigned long count_ = 0;
public:
    Semaphore();
    void release();
    void acquire();
    bool try_acquire();
};

#endif