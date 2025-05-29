#include "DiningPhilosophers.h"
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    size_t n;

    // check if exactly 1 argument is given
    if (argc != 2) {
        cerr << "Wrong number of arguments provided. \n";
        return 1;
    }

    try {
        string arg = argv[1];

        // try to convert the argument to int
        int temp = stoi(arg);

        //  check if the number is grater than 0
        if (temp <= 0) {
            cerr << "The number of philosophers must be greater than 0. \n";
            return 1;
        }

        // convert to size_t
        n = static_cast<size_t>(temp);

    } catch (const invalid_argument& e) {
        // if the argument can't be converted to an integer
        cerr << "The argument is not an integer.\n";
        return 1;
    }

    cout << "Simmualtion with " << n << " philosophers\n";
    DiningPhilosophers dinner(n); // create the simulation with provided nr of philosophers
    dinner.run();
}
