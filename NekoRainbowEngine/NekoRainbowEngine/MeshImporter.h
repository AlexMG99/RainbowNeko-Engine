#ifndef _MESH_IMPORTER_H_
#define _MESH_IMPORTER_H_

#include "Importer.h"

class ComponentMesh;
struct aiScene;
class aiMesh;
class ResourceMesh;

class MeshImporter:public Importer 
{
public:
	MeshImporter() {};
	~MeshImporter() {};

	bool Init();
	bool CleanUp();

	ResourceMesh* Import(const aiMesh* aimesh, ResourceMesh* res_mesh);
	bool SaveMesh(ResourceMesh* mesh);

	ResourceMesh* Load(const char* exported_file);
	//ComponentMesh* LoadPrimitive(PRIMITIVE_TYPE type);

	void CalculateNormalFace(ResourceMesh* mesh, const aiMesh* aimesh);

};

#endif


