#ifndef _MESH_IMPORTER_H_
#define _MESH_IMPORTER_H_

#include "Importer.h"

class ComponentMesh;
struct aiScene;
class aiMesh;
class Mesh;

class MeshImporter:public Importer 
{
public:
	MeshImporter() {};
	~MeshImporter() {};

	bool Init();
	bool CleanUp();

	Mesh* Import(const aiScene* scene, const aiMesh* aimesh);
	bool SaveMesh(Mesh* mesh, const char* name = "mesh");

	ComponentMesh* Load(const char* exported_file);
	//ComponentMesh* LoadPrimitive(PRIMITIVE_TYPE type);

	void CalculateNormalFace(Mesh* mesh, const aiMesh* aimesh);

};

#endif


