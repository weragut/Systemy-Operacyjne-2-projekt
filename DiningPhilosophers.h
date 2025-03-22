#ifndef DININGPHILOSOPHERS_H
#define ININGPHILOSOPHERS_H
#include <iostream>
#include <thread>
#include <mutex>
#include <semaphore>
#include <random>
#include <vector>
#include <chrono>

using namespace std;

class DiningPhilosophers {
public:
    DiningPhilosophers(size_t n); // constructor, initializes the nr of philosophers
    void run(); // function to start the simulation

private:
    // possible states of philosophers
    enum class State { THINKING, HUNGRY, EATING };
    // number of philosophers and forks
    size_t philosophers_number;
    // vector to store the state of each philosopher
    vector<State> state;
    // a vector of semaphores to ensure they can pick up both forks
    vector<unique_ptr<binary_semaphore>> both_forks_available;
    // vector of threads each representing a philosopher in the simulation
    vector<jthread> threads;
    // mutex to protect the critical region
    mutex critical_region_mtx;
    // mutex to protect the output
    mutex out_mtx;

    void activities(size_t i);
    void think(size_t i);
    void take_forks(size_t i);
    void eat(size_t i);
    void put_down_forks(size_t i);
    // check if the philosopher can eat
    void test(size_t i);
    //  get the index of the left neighbor
    size_t left_neighbor(size_t i) const;
    //  get the index of the right neighbor
    size_t right_neighbor(size_t i) const;
    // random number generator for eating and thinking time
    size_t get_random_time(size_t min, size_t max);
};

#endif // DININGPHILOSOPHERS_H
