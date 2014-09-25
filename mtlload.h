#ifndef MATERIAL_FILE_H
#define MATERIAL_FILE_H

#include "types.h"

struct ObjMaterial
{
	string name;
	
	ObjRgb ambient;
	ObjRgb diffuse;
	ObjRgb specular;

	ObjWord shininess;
	ObjByte illumination_model;
	ObjFloat transparency;

	string ambient_texture;
	string diffuse_texture;
	string specular_texture;
};

#endif	// MATERIAL_FILE_H
