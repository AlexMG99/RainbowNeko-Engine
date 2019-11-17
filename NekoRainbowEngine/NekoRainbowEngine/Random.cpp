#include "Random.h"
#include <random>
#include "PCG/pcg_random.hpp"
#include "PCG/pcg_extras.hpp"

Random::Random(uint32 num)
{
	number = num;
}

uint32 Random::GenerateRandomInt()
{
	pcg_extras::seed_seq_from<std::random_device> seed_source;
	pcg32 rng(seed_source);

	std::uniform_int_distribution<uint32> uniform_dist1(0, 4294967295);
	int num = number = uniform_dist1(rng);

	return num;
}

uint32 Random::GetNumber() const
{
	return number;
}

void Random::SetNumber(double num)
{
	number = (uint32)num;
}

