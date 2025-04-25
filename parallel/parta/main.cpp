#include <iostream>

int main() {
    int sum = 0;
    int adder = 10;
    int num_list[8] = {1,2,3,4,5,6,7,8};
        // Parallelizing a structured block, one point of entry and one point of exit
        // Minimal inter-thread dependencies, each element in num_list does not depend on other elements
        // Thread count kept as default and schedule set to auto so the decision of which
        // scheduler to use is delegated to the compiler, this code will always be ran on 
        // Hamarabi so the auto option is appropriate.
        // Sum is shared between threads
    #pragma omp parallel for schedule (auto) shared (sum)
    for (int i = 0; i < sizeof(num_list)/sizeof(num_list[0]); ++i){
        num_list[i] += adder;
        // Prevent race condition by performing the sum operation atomically
        // Only applies to one operation so more efficient than #pragma omp critical
        #pragma omp atomic 
        sum += num_list[i];
        // Print statement to demonstrate that each array item is added to in parallel
        std::cout << num_list[i] << std::endl;
    } 
    std::cout << sum << std::endl;
    return 0;
}