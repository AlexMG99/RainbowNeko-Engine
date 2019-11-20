#ifndef _RESOURCE_MODEL_H_
#define _RESOURCE_MODEL_H_

#include "Resource.h"
#include "Random.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/Quat.h"
#include <vector>

class aiNode;
class aiScene;

class string;

class ResourceModel : public Resource
{
public:

	struct Node
	{
		std::string		name;
		float3			position = float3::zero;
		Quat			rotation = Quat::identity;
		float3			scale = float3::zero;
		uint				id = 0;
		uint				parent = 0;
		Random          mesh = 0;
		Random          texture = 0;
	};

public:
	ResourceModel();
	ResourceModel(uint32 id);
	~ResourceModel() {};

	bool ImportModel(const char* path, std::string& output_file, bool fromMeta, std::vector<Random> meshes, std::vector<Random> textures);
	bool Load();
	void ReorganizeHierarchy();
private:
	void GenerateTexture(const aiScene * scene, const char * file, std::vector<Random>& materials);
	void GenerateMetaTexture(const aiScene * scene, const char * file, std::vector<Random> textures);

	void GenerateMeshes(const aiScene * scene, const char * file, std::vector<Random>& meshes);
	void GenerateMetaMeshes(const aiScene * scene, const char * file, std::vector<Random> meshes);

	void GenerateNodes(const aiScene* model, const aiNode* node, int parent, const std::vector<Random>& meshes, const std::vector<Random>& materials, bool fromMeta);

	bool Save(ResourceModel model, std::string& output) const;

public:
	std::vector<Node> nodes;
};

void LogCallback(const char * text, char * data);
#endif // !_RESOURCE_SCENE_H_
