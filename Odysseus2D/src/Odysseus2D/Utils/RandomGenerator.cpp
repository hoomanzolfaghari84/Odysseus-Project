#include <random>

namespace Odysseus2D {

    int RandInt(int min, int max) {
        std::random_device rd; // Obtain a random number from hardware or OS
        std::mt19937 gen(rd()); // Seed the Mersenne Twister engine
        
        // Create a uniform integer distribution for numbers between 1 and 6 (inclusive)
        std::uniform_int_distribution<> distrib(min, max); 
        
        return distrib(gen); // Generate a random number
        
    }
}