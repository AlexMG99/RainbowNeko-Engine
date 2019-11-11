#ifndef _MESH_IMPORTER_H_
#define _MESH_IMPORTER_H_

#include "Importer.h"

class ComponentMesh;
struct aiScene;
class aiMesh;

class MeshImporter:public Importer 
{
public:
	MeshImporter() {};
	~MeshImporter() {};

	bool Init();
	bool CleanUp();

	ComponentMesh* Import(const aiScene* scene, const aiMesh* aimesh);
	bool SaveMesh(ComponentMesh* mesh);

	ComponentMesh* Load(const char* exported_file);
	//ComponentMesh* LoadPrimitive(PRIMITIVE_TYPE type);

	void CalculateNormalTriangle(ComponentMesh * m, vec3 triangle_p1, vec3 triangle_p2, vec3 triangle_p3);

};

#endif


