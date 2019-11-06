#ifndef _RANDOM_H_
#define _RANDOM_H_

#include "Globals.h"
#include <vector>
#include <random>
#include "PCG/pcg_random.hpp"
#include "PCG/pcg_extras.hpp"

#include <stdint.h>

struct Random {
public:
	Random() {};
	~Random() {};

	uint32 GenerateRandomInt()
	{
		pcg_extras::seed_seq_from<std::random_device> seed_source;
		pcg32 rng(seed_source);

		std::uniform_int_distribution<uint32> uniform_dist1(0, UINT32_MAX);
		int num = number = uniform_dist1(rng);

		return num;
	}

	uint32 GetNumber() const
	{
		return number;
	}

private:
	uint32 number;

};
#endif // ! _RANDOM_H_
