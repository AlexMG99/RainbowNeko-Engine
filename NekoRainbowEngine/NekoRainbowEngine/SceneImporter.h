#ifndef _SCENE_IMPORTER_H_
#define _SCENE_IMPORTER_H_

#include "Importer.h"
#include "MathGeoLib/include/Math/float3.h"

class aiNode;
class aiScene;
class GameObject;

class SceneImporter :public Importer {
public:
	SceneImporter() {};
	~SceneImporter() {};

	bool Init();
	bool CleanUp();

	bool Import(const char* path);
	bool Load(const char* exported_file, GameObject* resource) {};

private:
	void LoadNode(const aiNode * node, const aiScene * scene, const char * path_fbx, GameObject * parent);
};

void LogCallback(const char * text, char * data);

#endif // !_SCENE_IMPORTER_H_

