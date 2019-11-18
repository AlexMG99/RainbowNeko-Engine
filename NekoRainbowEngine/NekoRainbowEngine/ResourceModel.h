#ifndef _RESOURCE_MODEL_H_
#define _RESOURCE_MODEL_H_

#include "Resource.h"
#include "Random.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/Quat.h"
#include <vector>

class aiNode;
class aiScene;

class ResourceModel : public Resource
{
public:

	struct Node
	{
		std::string		name;
		float3			position = float3::zero;
		Quat			rotation = Quat::identity;
		float3			scale = float3::zero;
		uint			parent = 0;
		Random          mesh = 0;
		Random          texture = 0;
	};

public:
	ResourceModel() :Resource() {};
	ResourceModel(Random id) :Resource(id, type) {};
	~ResourceModel() {};

	bool ImportModel(const char* path, std::string output_file);
private:
	void GenerateTexture(const aiScene * scene, const char * file, std::vector<Random>& materials);
	void GenerateMeshes(const aiScene * scene, const char * file, std::vector<Random>& meshes);
	void GenerateNodes(const aiScene* model, const aiNode* node, uint parent, const std::vector<Random>& meshes, const std::vector<Random>& materials);

	bool Save(ResourceModel model, std::string& output) const;

private:
	std::vector<Node> nodes;
};

void LogCallback(const char * text, char * data);
#endif // !_RESOURCE_SCENE_H_
