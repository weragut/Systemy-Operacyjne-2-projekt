#include "DiningPhilosophers.h"

using namespace std;

DiningPhilosophers::DiningPhilosophers(size_t n)
    : philosophers_number(n), state(n, State::THINKING)  // first initialize all philosophers as THINKING
{
    // create a semaphore for every philosopher
    for (size_t i = 0; i < philosophers_number; ++i) {
        both_forks_available.push_back(make_unique<binary_semaphore>(0));
        // initial value 0 shows no access to forks
        // 0 - no forks available
        // 1 - both forks available
    }
}

void DiningPhilosophers::run() {
    // create a thread for each philosopher
    for (size_t i = 0; i < philosophers_number; ++i) {
        threads.emplace_back(&DiningPhilosophers::activities, this, i);
    }

    for (size_t i = 0; i < threads.size(); ++i) {
        if (threads[i].joinable()) {
            threads[i].join();
        }
    }
}

void DiningPhilosophers::activities(size_t i) {
    while (true) {
        think(i);
        take_forks(i);
        eat(i);
        put_down_forks(i);
    }
}

void DiningPhilosophers::think(size_t i) {
    size_t duration = get_random_time(1000, 3000); // thinking time between 1000 and 3000 ms
    {
        lock_guard<mutex> lk(out_mtx); // lock output mutex
        cout << "State: THINKING  Philosopher: " << i << " for " << duration << " ms" << endl; // printing status
    }
    // sleeping for the thinking time
    this_thread::sleep_for(chrono::milliseconds(duration));
}

void DiningPhilosophers::take_forks(size_t i) {
    {
        lock_guard<mutex> lk(critical_region_mtx); // lock the critical region to prevent race conditions
        state[i] = State::HUNGRY; // change state to HUNGRY
        {
            lock_guard<mutex> lk(out_mtx); // lock output mutex
            cout << "State: HUNGRY    Philosopher: " << i << endl;
        }
        test(i); // check if neighbors can eat
    }
    both_forks_available[i]->acquire(); // try to take both forks
                                        // if not available than wait
}

void DiningPhilosophers::eat(size_t i) {
    size_t duration = get_random_time(1000, 3000);  // eating time between 1000 and 3000 ms
    {
        lock_guard<mutex> lk(out_mtx); // lock output mutex
        cout << "State: \033[35mEATING\033[0m    Philosopher: \033[35m" << i << "\033[0m for " << duration << " ms" << endl;

    }
    // sleeping for the eating time
    this_thread::sleep_for(chrono::milliseconds(duration));
}

void DiningPhilosophers::put_down_forks(size_t i) {
    {
        lock_guard<mutex> lk(critical_region_mtx);
        state[i] = State::THINKING; // change to thinking
        // check both neighbors if they can eat
        test(left_neighbor(i));
        test(right_neighbor(i));
    }

}

// check if philosopher is hungry and both neighbors are not eating
void DiningPhilosophers::test(size_t i) {
    if ((state[i] == State::HUNGRY) && (state[left_neighbor(i)] != State::EATING) && (state[right_neighbor(i)] != State::EATING)) {
        state[i] = State::EATING; // change state to EATING
        both_forks_available[i]->release(); // release the forks
    }
}

// return the index of the left neighbor
size_t DiningPhilosophers::left_neighbor(size_t i) const {
    return (i + philosophers_number - 1) % philosophers_number; // modulo operation for round table
}


// return the index of the right neighbor
size_t DiningPhilosophers::right_neighbor(size_t i) const {
    return (i + 1) % philosophers_number;
}

// random number generator for eating and thinking
size_t DiningPhilosophers::get_random_time(size_t min, size_t max) {
    static thread_local mt19937 rand_time(random_device{}());
    // because of 'thread_local' each thread has its own copy of rand_time
    return uniform_int_distribution<size_t>(min, max)(rand_time);
    // uniform_int_distribution - every integer in this interval has an equal chance of being generated
}
