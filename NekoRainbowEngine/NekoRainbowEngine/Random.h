#ifndef _RANDOM_H_
#define _RANDOM_H_

#include "Globals.h"

struct Random {
public:
	Random() {};
	Random(uint32 num);
	~Random() {};

	uint32 GenerateRandomIntRange(int first, int last);
	uint32 GenerateRandomInt();
	uint32 GetNumber() const;

	void SetNumber(double num);

private:
	uint32 number;

};
#endif // ! _RANDOM_H_
