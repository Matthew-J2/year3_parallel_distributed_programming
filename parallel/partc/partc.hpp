#ifndef PARTC_HPP
#define PARTC_HPP

#include <iostream>
#include <vector>
#include <array>

// Structure containing an array of vectors to represent the particles
struct Particle_Vec_Arr {
    std::array<std::vector<double>, 10> arr;  // std::array of 10 vectors of doubles
};

// Overload << operator to print a Particle_Vec_Arr
std::ostream& operator<<(std::ostream& os, const Particle_Vec_Arr& arrayOfVec);

// Calculate the average distance of the particles to a given centroid
std::vector<double> centroid_distance(Particle_Vec_Arr& pva, std::vector<double> centroid);

#endif