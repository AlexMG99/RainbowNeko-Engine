#include "Globals.h"
#include "MeshImporter.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#include "Assimp/include/mesh.h"

bool MeshImporter::Init()
{

	return true;
}

bool MeshImporter::CleanUp()
{
	return true;
}

ComponentMesh * MeshImporter::Import(const aiScene * scene, const aiMesh * mesh)
{
	return nullptr;
}

bool MeshImporter::SaveMesh(ComponentMesh * mesh)
{
	return true;
}

ComponentMesh * MeshImporter::Load(const char * exported_file)
{
	return nullptr;
}