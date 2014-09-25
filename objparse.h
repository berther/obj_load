#ifndef OBJ_PARSE_H
#define OBJ_PARSE_H

#include "types.h"
#include "vertex.h"
#include "mesh.h"

namespace objparse
{
	// read the line beginning with the "v" or "vn"
	// store the following values as a ObjVector3 variable
	ObjBool obj_parse_vector3(fstream& obj_stream, vector<ObjVector3>& xyz);

	// read the line beginning with the "vt"
	// store the following values as a ObjVector2 variable
	// line of "vt" may contain ".#QNAN", so we must filter them;
	ObjBool obj_parse_vector2(fstream& obj_stream, vector<ObjVector2>& uv);

	// given that the face index has three types
	void obj_parse_face_index(const string& szface, ObjVertexIndex& vertex_idx);

	// 1. the face index is one-based indices in .obj file
	// 		while the vector is zero-based indices, so we should convert the indices.
	// 2. the face index may have a value counted from bottom to up, such as f -4/-2/-3
	// 		so, we convert the value to a number counted from up to bottom
	void obj_vertex_index_normalize(ObjVertexIndex& vertex_idx, const vector<ObjVector3>& positions, 
		const vector<ObjVector2>& textcoords, const vector<ObjVector3>& normals);

	// read the face index line beginning with "f"
	// .obj file supports four-side face, that is f v/vt/vn v/vt/vn v/vt/vn v/vt/vn
	// we should read and store all face index
	// we get the shape of this face by the number of space   
	ObjBool obj_parse_face(fstream& obj_stream, vector<ObjVertexIndex>& faces, 
		const vector<ObjVector3>& positions, const vector<ObjVector2>& texcoords, const vector<ObjVector3>& normals);

	// read the line beginning with "mtllib"
	// .obj file may uses two or more material library files,
	// that is, mtllib filename1 filename2 . . .
	// so, we should read and save all material library files.
	// Notice: filename can contain space character
	ObjBool obj_parse_mtllib(fstream& obj_stream, vector<string>& materialnames);

	// read the line beginning with "usemtl"
	// we employ map<materialName, materialIndex> structure to store every material one time
	// here, the materialName is the words following "usemtl", materialIndex is a index value of that material in m_Materials
	ObjIntd obj_parse_usemtl(fstream& obj_stream, Mesh& tmpmesh, const map<string, int>& mtlMap);

	// read the line beginning with "g"
	// the line may be 'g'
	// or 'g (null)'
	// or 'g bmw', we should consider all cases.
	ObjBool obj_parse_group(fstream& obj_stream, string& name);
}

#endif	// OBJ_PARSE_H
