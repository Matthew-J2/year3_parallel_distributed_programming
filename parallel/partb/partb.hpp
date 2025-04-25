#ifndef PARTB_HPP
#define PARTB_HPP

#include <iostream>
#include <vector>
#include <array>

// Structure containing array of vectors to represent the particles
struct Particle_Vec_Arr {
    std::array<std::vector<int>, 10> arr;  // std::array of 10 vectors of integers
};

// Overload << operator to print a Particle_Vec_Arr
std::ostream& operator<<(std::ostream& os, const Particle_Vec_Arr& arrayOfVec);

#endif