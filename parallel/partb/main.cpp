#include <iostream>
#include <vector>
#include <array>
#include <random>
#include "partb.hpp"

std::ostream& operator<<(std::ostream& os, const Particle_Vec_Arr& arrayOfVec) {
    for (int i = 0; i < 10; ++i) {
        os << "Vector " << i << ": ";
        for (int num : arrayOfVec.arr[i]) {
            os << num << " ";
        }
        os << std::endl;
    }
    return os;
}

int main() {
    // Declare a random device object rd (random number generator)
    // Instantiates a Mersenne Twister PRNG function with internal state size of 19937 bits using the seed from rd()
    
    std::random_device rd;
    std::mt19937 gen(rd());

    // Constructs a uniform integer distribution from an integer minimum to maximum
    // The first is used for the distance a particle moves, and the second for which direction in the axis
    std::uniform_int_distribution<> incr_dis(1, 5);
    std::uniform_int_distribution<> sign_dis(0, 1);

    // Initialize the array of vectors with the values provided
    Particle_Vec_Arr pva = {
        std::vector<int>{5, 14, 10},
        std::vector<int>{7, -8, -14},
        std::vector<int>{-2, 9, 8},
        std::vector<int>{15, -6, 3},
        std::vector<int>{12, 4, -5},
        std::vector<int>{4, 20, 17},
        std::vector<int>{-16, 5, -1},
        std::vector<int>{-11, 3, 16},
        std::vector<int>{3, 10, -19},
        std::vector<int>{-16, 7, 4}
    };

    // Get number of axes and particles
    size_t pva_cols = pva.arr[0].size();
    size_t pva_rows = pva.arr.size();

    // Print the array of vectors
    std::cout << "Intitial coordinates:" << std::endl;
    std::cout << pva << std::endl;

    int PPA = 2;

    // Cannot parallelise before the first for loop as each iteration is dependent on
    // the previous ones.
    // For each iteration, iterate over each particle and for each particle iterate over the x, y, z axes
    // Randomly either increment or decrement each particle on each axis by a random integer in the range
    // 1-5

    for (int i = 0; i < 50; ++i) {
        // Parallelizing a structured block, one point of entry and one point of exit
        // Minimal inter-thread dependencies, each particle and axis does not depend on other
        // particles and axes.
        // Perfectly nested loops so we use collapse(2)
        // Thread count kept as default and schedule set to auto so the decision of which
        // scheduler to use is delegated to the compiler, this code will always be ran on 
        // Hamarabi so the auto option is appropriate.
        PPA += incr_dis(gen);
        #pragma omp parallel for schedule(auto) collapse(2)
        for (int particle = 0; particle < pva_rows; ++particle) {
            for (int axis=0; axis < pva_cols; ++axis){
                sign_dis(gen) ? pva.arr[particle][axis] += PPA : pva.arr[particle][axis] -= PPA;
            }
        }
        if (i == 24 || i == 49){
            std::cout << "Iteration " << i + 1 << ":" << std::endl;
            std::cout << pva << std::endl;
        }
    }
    return 0;
}
