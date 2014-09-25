#include "objinterface.h"

ObjLoader::ObjLoader()
{
	pobj = new LibObjFile;
}

ObjLoader::ObjLoader(const string& objfile)
{
	pobj = new LibObjFile;

	obj_load(objfile);
}

ObjLoader::~ObjLoader()
{
	if (pobj)
		delete pobj;
}

ObjBool ObjLoader::obj_load(const string& objfile)
{
	if (!pobj)
		return LIBOBJ_FALSE;

	return pobj->obj_file_load(objfile);
}
