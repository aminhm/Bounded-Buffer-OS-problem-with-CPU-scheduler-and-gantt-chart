#include <iostream>
#include <stdio.h>
#include <mutex>
#include <condition_variable>

using namespace std;

class Semaphore {

private:
    std::mutex mtx;
    std::condition_variable cv;
    int max;
    int count;


public:
    Semaphore(int count_, int max_)
            : count(count_) , max(max_) {}

    int get_count() {
        return this->count;
    }

    inline void notify()
    {
        unique_lock<mutex> lock(mtx);
        if (count < this->max) count++;
        cv.notify_one();
    }

    inline void wait()
    {
        unique_lock<mutex> lock(mtx);

        while (count == 0) {
            cv.wait(lock);
        }
        count--;
    }

};




