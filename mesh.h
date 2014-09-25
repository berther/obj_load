#ifndef MESH_H
#define MESH_H

#include "types.h"

struct Mesh
{
	string name;		// name
	ObjIntd mtl_idx;	// the index of material used by mesh 

	vector<ObjVector3> positions;	// "v" flag
	vector<ObjVector2> texcoords;	// "vt" flag
	vector<ObjVector3> normals;		// "vn" flag
	vector<ObjDword> indices;	// the indices of points of face in positions

	void reset()
	{ 
		mtl_idx = -1;

		name.clear();
		positions.clear();
		texcoords.clear();
		normals.clear();
		indices.clear();
	}
};

#endif	// MESH_H
