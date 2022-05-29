#ifndef TESTUTIL_RAND_GEN_HPP_
#define TESTUTIL_RAND_GEN_HPP_

#include <cstdlib>
#include <random>

namespace testutil {

/** Class to simplify random number generation using C++ random library */
class rand_gen {
private:
    /** A Mersenne Twister pseudo-random generator of 64-bit numbers */
    std::mt19937_64 gen;
    /** Initial random seed */
    uint32_t seed;
public:
    /** Default constructor */
    rand_gen();
    /** Constructor that takes a specific seed */
    rand_gen(const uint32_t seed);
    /** Get generator seed value */
    uint32_t get_seed();
    /** Get a random u64 number */
    uint64_t get_rnd_u64();
    /** Get a random u64 number in a specified range (inclusive) */
    uint64_t get_rnd_u64_range(uint64_t min, uint64_t max);
    /** Get a random boolean value */
    bool get_rnd_bool();
};
}

#endif // TESTUTIL_RAND_GEN_HPP_
