#ifndef _RANDOM_H_
#define _RANDOM_H_

#include "Globals.h"

struct Random {
public:
	Random() {};
	~Random() {};

	uint32 GenerateRandomInt();
	uint32 GetNumber() const;

private:
	uint32 number;

};
#endif // ! _RANDOM_H_