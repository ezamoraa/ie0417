#include <stdexcept>

#include <testutil/rand_gen.hpp>

namespace testutil {

rand_gen::rand_gen()
{
    /* Use time as default seed value */
    uint32_t nseed = time(NULL);
    this->gen.seed(nseed);
    this->seed = nseed;
}

rand_gen::rand_gen(const uint32_t nseed)
{
    this->gen.seed(nseed);
    this->seed = nseed;
}

uint32_t rand_gen::get_seed()
{
    return this->seed;
}

uint64_t rand_gen::get_rnd_u64()
{
    return this->gen();
}

uint64_t rand_gen::get_rnd_u64_range(uint64_t min, uint64_t max)
{
    uint64_t range = 0;
    if (max < min) {
        throw std::invalid_argument("Max value is smaller than min value");
    }
    range = max - min;
    return (get_rnd_u64() % (range + 1)) + min;
}

bool rand_gen::get_rnd_bool()
{
    return (get_rnd_u64() % 2) == 0;
}

}
