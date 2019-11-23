#ifndef __PATH_H_
#define __PATH_H_

#include <string>
#include <vector>


struct Path
{
	Path() : path("") {};

	std::string path;
	std::string local_path;


	std::vector<Path> childs;

	bool file = true;
};





#endif // !_PATH_H_

