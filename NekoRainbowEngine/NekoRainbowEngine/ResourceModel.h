#ifndef _RESOURCE_MODEL_H_
#define _RESOURCE_MODEL_H_

#include "Resource.h"
#include "Random.h"
#include <vector>

class aiNode;
class aiScene;

class ResourceModel : public Resource
{
public:
	ResourceModel() :Resource() {};
	ResourceModel(Random id) :Resource(id, type) {};
	~ResourceModel() {};


private:
	bool ImportModel(const char* path, std::string output_file);
	void GenerateTexture(const aiScene * scene, const char * file, std::vector<Random>& materials);
	void GenerateMeshes(const aiScene * scene, const char * file, std::vector<Random>& meshes);
	void LoadNode(const aiNode * node, const aiScene * scene, const char * path_fbx, uint parent, const std::vector<UID>& meshes, const std::vector<UID>& materials);
};

void LogCallback(const char * text, char * data);
#endif // !_RESOURCE_SCENE_H_
