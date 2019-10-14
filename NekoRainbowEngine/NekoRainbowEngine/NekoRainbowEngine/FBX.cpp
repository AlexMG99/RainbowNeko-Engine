
//bool FBX::LoadTextures(Mesh* mesh, const char * path)
//{
//	bool ret = true;
//	if (path != "") {
//		ILuint devil_id = 0;
//
//		ilGenImages(1, &devil_id);
//		ilBindImage(devil_id);
//		ilutRenderer(ILUT_OPENGL);
//
//		ILuint Size;
//		FILE *File;
//		ILubyte *Lump;
//
//		File = fopen(path, "rb");
//		fseek(File, 0, SEEK_END);
//		Size = ftell(File);
//
//		texture = new Texture();
//
//		Lump = (ILubyte*)malloc(Size);
//		fseek(File, 0, SEEK_SET);
//		fread(Lump, 1, Size, File);
//		fclose(File);
//
//		if (!ilLoadL(IL_DDS, Lump, Size)) {
//			auto error = ilGetError();
//			LOG("Failed to load texture with path: %s. Error: %s", path, ilGetString(error));
//			ret = false;
//		}
//		else {
//			mesh->image_id = ilutGLBindTexImage();
//			texture->height = ilGetInteger(IL_IMAGE_HEIGHT) / 2;
//			texture->width = ilGetInteger(IL_IMAGE_WIDTH) / 2;
//
//			texture->GenerateTexture(mesh->image_id);
//		}
//
//		free(Lump);
//		ilDeleteImages(1, &devil_id);
//	}
//	else
//	{
//		LOG("The FBX doesn't have a texture path or path %s is incorrect", path);
//		delete texture;
//		texture = nullptr;
//	}
//
//	return ret;
//}
//
//Mesh::~Mesh()
//{
//	delete index;
//	index = nullptr;
//
//	delete vertices;
//	vertices = nullptr;
//}
//
//FBX::~FBX()
//{
//	for (auto it_mesh = mesh_list.begin(); it_mesh != mesh_list.end(); ++it_mesh)
//	{
//		ilDeleteImages(1, &(*it_mesh)->image_id);
//		delete (*it_mesh);
//		(*it_mesh) = nullptr;
//	}
//
//	delete texture;
//	texture = nullptr;
//}
//
//void Texture::GenerateTexture(uint& id)
//{
//	image_id = id;
//	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//	glBindTexture(GL_TEXTURE_2D, image_id);
//
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//
//	glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT),
//		0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());
//
//	glBindTexture(GL_TEXTURE_2D, 0);
//}
//
//void Texture::Render()
//{
//	glEnable(GL_TEXTURE_2D);
//	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
//	glBindTexture(GL_TEXTURE_2D, image_id);
//
//	glBegin(GL_QUADS);
//	glTexCoord2i(0, 0); glVertex2i(0, 0);
//	glTexCoord2i(0, 1); glVertex2i(0, height);
//	glTexCoord2i(1, 1); glVertex2i(width, height);
//	glTexCoord2i(1, 0); glVertex2i(width, 0);
//	glEnd();
//
//	glEnd();
//	glFlush();
//	glDisable(GL_TEXTURE_2D);
//}