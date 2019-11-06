#ifndef _RANDOM_H_
#define _RANDOM_H_

#include "Globals.h"
#include <vector>
#include <random>
#include "PCG/pcg_random.hpp"
#include "PCG/pcg_extras.hpp"

class Random {
public:
	Random() {};
	~Random() {};

	uint32 GetRandomInt()
	{
		pcg_extras::seed_seq_from<std::random_device> seed_source;
		pcg32 rng(seed_source);

		std::uniform_int_distribution<int> uniform_dist1(0, 4294967295);
		int num = number = uniform_dist1(rng);

		return num;
	}
private:
	uint32 number;

};
#endif // ! _RANDOM_H_
