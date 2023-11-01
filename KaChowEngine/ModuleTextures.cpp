#include "Application.h"
#include "ModuleTextures.h"

#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment(lib, "DevIL/libx86/DevIL.lib")
#pragma comment(lib, "DevIL/libx86/ILU.lib")
#pragma comment(lib, "DevIL/libx86/ILUT.lib")

//#include "SDL/include/SDL_opengl.h"

ModuleTextures::ModuleTextures(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	//textureID = 0;
}

bool ModuleTextures::Init()
{


	return true;
}

bool ModuleTextures::Start()
{
	texPath = "";
	textureID = 0;
	checkerID = 0;
	textureWidth = 0;
	textureHeight = 0;

	//Initialize checker image
	for (int i = 0; i < CHECKERS_HEIGHT; i++) {
		for (int j = 0; j < CHECKERS_WIDTH; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkerImage[i][j][0] = (GLubyte)c;
			checkerImage[i][j][1] = (GLubyte)c;
			checkerImage[i][j][2] = (GLubyte)c;
			checkerImage[i][j][3] = (GLubyte)255;
		}
	}

	glEnable(GL_TEXTURE_2D);
	//glActiveTexture(GL_TEXTURE0);

	//Generate and bind texture
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &checkerID);
	glBindTexture(GL_TEXTURE_2D, checkerID);

	//How texture behaves outside 0,1 range (S->x, T->y)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //GL_CLAMP
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Texture behaviour after resize (MIN->smaller , MAG->bigger)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	//Create Texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT,
		0, GL_RGBA, GL_UNSIGNED_BYTE, checkerImage);

	glGenerateMipmap(GL_TEXTURE_2D);

	//unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);

	ilInit();
	//ilEnable(IL_ORIGIN_SET);
	//ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

	/*texPath = "Assets/Textures/Baker_house.png";

	LoadTexture(texPath);*/

	return true;
}

bool ModuleTextures::CleanUp()
{
	//glDeleteBuffers(1, &checkerID);
	return true;
}


uint ModuleTextures::LoadTexture(const char* file_path)
{
	uint devilImageId;
	ilGenImages(1, &devilImageId);
	ilBindImage(devilImageId);

	bool success = ilLoadImage(file_path);

	/*if (!success) {
		LOG ERROR LOADING TEXTURE
		return 0;
	}*/

	success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

	//if (!success) {
	//	LOG converting texture
	//	return 0;
	//}

	//Extract loaded image data
	BYTE* data = ilGetData();
	ILuint imgWidth, imgHeight;
	imgWidth = ilGetInteger(IL_IMAGE_WIDTH);
	imgHeight = ilGetInteger(IL_IMAGE_HEIGHT);
	int const type = ilGetInteger(IL_IMAGE_TYPE);
	int const format = ilGetInteger(IL_IMAGE_FORMAT);

	uint imageId = ilutGLBindTexImage();
	glBindTexture(GL_TEXTURE_2D, imageId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, format, imgWidth, imgHeight, 0, format, type, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	ilDeleteImages(1, &devilImageId);

	glBindTexture(GL_TEXTURE_2D, 0);

	return imageId;
}


void ModuleTextures::FreeTexture(GLuint texId)
{
	//Delete texture
	if (texId != 0)
	{
		glDeleteTextures(1, &texId);
		texId = 0;
	}
}