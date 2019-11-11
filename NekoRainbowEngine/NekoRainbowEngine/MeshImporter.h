#ifndef _MESH_IMPORTER_H_
#define _MESH_IMPORTER_H_

#include "Importer.h"

class ComponentMesh;
class aiScene;
class aiMesh;

class MeshImporter:public Importer 
{
public:
	MeshImporter() {};
	~MeshImporter() {};

	bool Init();
	bool CleanUp();

	ComponentMesh* Import(const aiScene* scene, const aiMesh* mesh);
	bool SaveMesh(ComponentMesh* mesh);

	ComponentMesh* Load(const char* exported_file);
	//ComponentMesh* LoadPrimitive(PRIMITIVE_TYPE type);

};

#endif


