#include "objload.h"

using namespace objload;

class LIBOBJAPI ObjLoader
{
public:
	ObjLoader();
	ObjLoader(const string& objfile);
	~ObjLoader();

	ObjBool obj_load(const string& objfile);

private:
	LibObjFile *pobj;
};
