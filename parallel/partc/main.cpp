#include <iostream>
#include <vector>
#include <array>
#include <random>
#include <cmath>
#include "partc.hpp"

// Overload << operator to print a Particle_Vec_Arr
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

// Calculate the average distance of the particles to 
std::vector<double> centroid_distance(Particle_Vec_Arr& pva, std::vector<double> centroid) {
    size_t pva_cols = pva.arr[0].size();
    size_t pva_rows = pva.arr.size();
    
    std::vector<double> distance = {0,0,0};
    for (int axis = 0; axis < pva_cols; ++axis) {
        for (int particle = 0; particle < pva_rows; ++particle) {
            distance[axis] += abs(pva.arr[particle][axis] - centroid[axis]);
        }
        distance[axis] /= pva_rows;
    }
    
    return distance;
}

int main() {

    // Initialize the array of vectors with the values provided
    Particle_Vec_Arr pva = {
        std::vector<double>{-691, 552, 580},
        std::vector<double>{-1191, -786, -124},
        std::vector<double>{-1120, 101, 476},
        std::vector<double>{347, -988, -443},
        std::vector<double>{782, -304, -1111},
        std::vector<double>{-64, 392, 221},
        std::vector<double>{-434, 1, -585},
        std::vector<double>{751, -959, 340},
        std::vector<double>{1157, -1190, -133},
        std::vector<double>{-140, -1063, -2}
    };

    // Get number of axes and particles
    size_t pva_cols = pva.arr[0].size();
    size_t pva_rows = pva.arr.size();

    // Print the array of vectors
    std::cout << "Initial coordinates:\n" << pva << std::endl;

    // Set PPA to 2 and initialize the centroid
    double PPA = 2;
    std::vector<double> centroid = {0,0,0};

    // Calculate centroid
    for (int axis = 0; axis < pva_cols; ++axis) {
        for (int particle = 0; particle < pva_rows; ++particle) {
            centroid[axis] += pva.arr[particle][axis];
        }
        centroid[axis] /= pva_rows;
    }

    // Print centroid
    std::cout << "Centroid coordinates:" << std::endl;
    for (double i : centroid) {        
        std::cout << i << std::endl;
    }

    // Print initial average distance from centroid on each axis 
    std::cout << "Initial distances:" << std::endl;
    for (int axis=0; axis < pva_cols; ++axis){    
        std::cout << "Distance on axis " << axis << ": " << centroid_distance(pva, centroid)[axis] << std::endl;
    }

    for (int i = 0; i < 50; ++i) {
        // Parallelizing a structured block, one point of entry and one point of exit
        // Minimal inter-thread dependencies, each particle and axis does not depend on other
        // particles and axes.
        // Perfectly nested loops so we use collapse(2)
        // Thread count kept as default and schedule set to auto so the decision of which
        // scheduler to use is delegated to the compiler, this code will always be ran on 
        // Hamarabi so the auto option is appropriate.
        #pragma omp parallel for schedule(auto) collapse(2)
        // Move the particle closer to the centroid by the value of PPA
        for (int particle = 0; particle < pva_rows; ++particle) {
            for (int axis=0; axis < pva_cols; ++axis){
                if (pva.arr[particle][axis] == centroid[axis]) {
                    continue;
                }
                else if (pva.arr[particle][axis] > centroid[axis]) {
                    pva.arr[particle][axis] -= PPA;
                }
                else {
                    pva.arr[particle][axis] += PPA;
                }
            }
        }

        // Print position of each particle and average distance of the particles from the centroid
        if (i == 24 || i == 49){
            std::cout << "Iteration " << i+1 << ":" << std::endl;
            std::cout << pva << std::endl;
            for (int axis=0; axis < pva_cols; ++axis){
                std::cout << "Distance on axis " << axis << ": " << centroid_distance(pva, centroid)[axis] << std::endl;
            }
        }
    }
    return 0;
}
