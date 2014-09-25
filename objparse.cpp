#include "utilities.h"
#include "objparse.h"
#include <cstdlib>

using namespace baseutis;
namespace objparse
{
	using std::atof;
	using std::atof;

	// read the line beginning with the "v" or "vn"
	// store the following values as a Vector3 variable
	ObjBool obj_parse_vector3(fstream& obj_stream, vector<ObjVector3>& xyz)
	{
		ObjVector3 vector3;

		obj_stream >> vector3.x >> vector3.y >> vector3.z;
		xyz.push_back(vector3);

		return obj_stream.good();
	}

	// read the line beginning with the "vt"
	// store the following values as a Vector2 variable
	// line of "vt" may contain ".#QNAN", so we must filter them;
	ObjBool obj_parse_vector2(fstream& obj_stream, vector<ObjVector2>& uv)
	{
		string filter;
		ObjVector2 vector2;

		getline(obj_stream, filter);

		vector2.x = atof(filter.c_str());
		vector2.y = atof(filter.substr(filter.find_first_not_of(' ', filter.find_first_not_of(' ')+1)).c_str());

		uv.push_back(vector2);

		return obj_stream.good();
	}

	// given that the face index has three types
	void obj_parse_face_index(const string& szface, ObjVertexIndex& vertex_idx)
	{
		if (szface.find('/') == string::npos)			// the first type: only has position index, say f 2
		{
			vertex_idx.v_idx = atoi(szface.c_str());		
			vertex_idx.vt_idx = 0;				
			vertex_idx.vn_idx = 0;			
		}
		else if (szface.find("//") != string::npos)	// the second type: no texture index, say f 1//2
		{
			vertex_idx.v_idx = atoi(szface.c_str());		
			vertex_idx.vt_idx = 0;					
			vertex_idx.vn_idx = atoi(szface.substr(szface.rfind('/')+1).c_str());			
		}
		else 												// the third type: f 2/3/4
		{
			vertex_idx.v_idx = atoi(szface.c_str());
			vertex_idx.vt_idx = atoi(szface.substr(szface.find('/')+1).c_str());
			vertex_idx.vn_idx = atoi(szface.substr(szface.rfind('/')+1).c_str());
		}
	}

	// 1. the face index is one-based indices in .obj file
	// 		while the vector is zero-based indices, so we should convert the indices.
	// 2. the face index may have a value counted from bottom to up, such as f -4/-2/-3
	// 		so, we convert the value to a number counted from up to bottom
	void obj_vertex_index_normalize(ObjVertexIndex& vertex_idx, const vector<ObjVector3>& positions, 
		const vector<ObjVector2>& textcoords, const vector<ObjVector3>& normals)
	{
		if (vertex_idx.v_idx > 0)                   // 1.
			vertex_idx.v_idx -= 1;					 // positions must exist, so v_idx==0 is not needed
		else if (vertex_idx.v_idx < 0)              // 2.
			vertex_idx.v_idx += positions.size();

		if (vertex_idx.vt_idx > 0)                  // 1.
			vertex_idx.vt_idx -= 1;
		else if (vertex_idx.vt_idx == 0)            // no texture coordinate
			vertex_idx.vt_idx = -1;
		else if (vertex_idx.vt_idx < 0)             // 2.
			vertex_idx.vt_idx += textcoords.size();

		if (vertex_idx.vn_idx > 0)                  // 1.
			vertex_idx.vn_idx -= 1;
		else if (vertex_idx.vn_idx == 0)            // no normal
			vertex_idx.vn_idx = -1;
		else if (vertex_idx.vn_idx < 0)             // 2.
			vertex_idx.vn_idx += normals.size();
	}

	// read the face index line beginning with "f"
	// .obj file supports four-side face, that is f v/vt/vn v/vt/vn v/vt/vn v/vt/vn
	// we should read and store all face index
	// we get the shape of this face by the number of space
	ObjBool obj_parse_face(fstream& obj_stream, vector<ObjVertexIndex>& faces,
		const vector<ObjVector3>& positions, const vector<ObjVector2>& texcoords, const vector<ObjVector3>& normals)
	{
		int delimiter_pos = 0;		// delimiter position in string
		string szface;
		ObjVertexIndex face;

		getline(obj_stream, szface);

		trim_right_ws(szface);
		
		while (!szface.empty())
		{
			trim_left_ws(szface);

			delimiter_pos = szface.find(' ');
			string sub_face_idx = szface.substr(0, delimiter_pos);
			szface.erase(0, delimiter_pos);

			obj_parse_face_index(sub_face_idx, face);
			obj_vertex_index_normalize(face, positions, texcoords, normals);

			faces.push_back(face);
		}

		return obj_stream.good();
	}

	// read the line beginning with "mtllib"
	// .obj file may uses two or more material library files,
	// that is, mtllib filename1 filename2 . . .
	// so, we should read and save all material library files.
	// Notice: filename can contain space character
	ObjBool obj_parse_mtllib(fstream& obj_stream, vector<string>& materialnames)
	{
		int delimiter_pos = 0;
		string mtlname;;

		getline(obj_stream, mtlname);
		trim_right_ws(mtlname);

		while (!mtlname.empty())
		{
			delimiter_pos = mtlname.find(".mtl");
			string szmtl_file_name = mtlname.substr(0, delimiter_pos+4);
			mtlname.erase(0, delimiter_pos+4);		// 4: the length of ".mtl", that is strlen(".mtl")

			trim_left_ws(szmtl_file_name);
			materialnames.push_back(szmtl_file_name);
		}
	
		return obj_stream.good();
	}

	// read the line beginning with "usemtl"
	// we employ map<materialName, materialIndex> structure to store every material one time
	// here, the materialName is the words following "usemtl", materialIndex is a index value of that material in m_Materials
	ObjIntd obj_parse_usemtl(fstream& obj_stream, Mesh& tmpmesh, const map<string, int>& mtlMap)
	{
		string usemtl;

		obj_stream >> usemtl;

		if (mtlMap.find(usemtl) != mtlMap.end())
			tmpmesh.mtl_idx = mtlMap.find(usemtl)->second;
		else
			tmpmesh.mtl_idx = -1;

		return tmpmesh.mtl_idx;
	}

	// read the line beginning with "g"
	// the line may be 'g'
	// or 'g (null)'
	// or 'g bmw', we should consider all cases.
	ObjBool obj_parse_group(fstream& obj_stream, string& name)
	{
		getline(obj_stream, name);
		
		trim_right_ws(name);
		trim_left_ws(name);

		return obj_stream.good();
	}
}
