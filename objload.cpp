#include "objload.h"
#include "objparse.h"
#include "mtlparse.h"
#include "utilities.h"
#include <cstdlib>

using namespace objparse;
using namespace mtlparse;
using namespace baseutis;

namespace objload
{
	LibObjFile::LibObjFile() {}

	LibObjFile::~LibObjFile()
	{
		obj_file_free();
	}

	ObjBool LibObjFile::obj_file_load(const string& objname)
	{
		if (objname.empty())
			return LIBOBJ_FALSE;

		int mesh_count = 0;
		Mesh mesh_;
		string groupname;
		string usemtl;
		vector<ObjVector3> positions;
		vector<ObjVector3> normals;
		vector<ObjVector2> texcoords;
		vector<vector<ObjVertexIndex> > faces;
		map<string, ObjIntd> mtlMap;

		fstream obj_stream;	
		obj_stream.open(objname.c_str(), std::ios_base::in);
		if (!obj_stream.is_open())
			return LIBOBJ_FALSE;

		string obj_flag;
		while (obj_stream.good())
		{
			obj_stream >> obj_flag;

			if (obj_flag.empty() || (obj_flag[0] == '#'))
			{
				obj_stream.ignore(1024, '\n');			// skip line
				continue;
			}

			if (obj_flag == "v")			// position flag
				if (!obj_parse_vector3(obj_stream, positions))
					return LIBOBJ_FALSE;
			else if (obj_flag == "vt")	// texture coordinate flag
				if (!obj_parse_vector2(obj_stream, texcoords))
					return LIBOBJ_FALSE;
			else if (obj_flag == "vn")	// normal flag
				if (!obj_parse_vector3(obj_stream, normals))
					return LIBOBJ_FALSE;
			else if (obj_flag == "g")		//group name
				if (!obj_parse_group(obj_stream, groupname))
					return LIBOBJ_FALSE;
			else if (obj_flag == "f")		// face	flag
			{
				vector<ObjVertexIndex> face;

				obj_parse_face(obj_stream, face, positions, texcoords, normals);

				faces.push_back(face);
			}
			else if (obj_flag == "mtllib")	// Material library
			{
				vector<string> materialnames;

				obj_parse_mtllib(obj_stream, materialnames);

				for (unsigned i = 0; i < materialnames.size(); i++)
					if (!mtl_file_load(get_file_path(objname) + '/' + materialnames[i]))
						return LIBOBJ_FALSE;
			}
			else if (obj_flag == "usemtl")
			{
				if (obj_save_mesh(positions, texcoords, normals, faces, groupname+usemtl, mesh_))
				{
					m_meshes.back().mtl_idx = mtl_index(usemtl);
					
					faces.clear();
					mesh_.reset();
				}

				obj_parse_group(obj_stream, usemtl);
			}
			else
			{
				obj_stream.ignore(1024, '\n');			// skip line
			}
		}  

		if (!obj_stream.eof())
			return LIBOBJ_FALSE;

		//save mesh data
		if (obj_save_mesh(positions, texcoords, normals, faces, groupname+usemtl, mesh_))
		{
			m_meshes.back().mtl_idx = mtl_index(usemtl);

			faces.clear();
			mesh_.reset();
		}

		obj_stream.close();

		return LIBOBJ_TRUE;
	}

	ObjIntd LibObjFile::mtl_index(string materialname) const
	{
		for (ObjDword i = 0; i < m_materials.size(); i++)
			if (materialname == m_materials[i]->name)
				return i;

		return -1;
	}

	void LibObjFile::obj_file_free()
	{
		for (ObjDword i = 0; i < m_materials.size(); i++)
			delete m_materials[i];
	}

	// get vertex value consisted of position, normal, texture coordinate index (v/vt/vn)
	//		and store it in MeshData
	// we employ map<VertexIndex, unsigned> structure to ensure that
	//		every vertex value stored only one time 
	static ObjDword get_vertex_value(map<ObjVertexIndex, ObjDword>& vertexCache, Mesh& mesh,
					const vector<ObjVector3>& positions, const vector<ObjVector2>& texcoords,
					const vector<ObjVector3>& normals, const ObjVertexIndex& v_idx_finder)
	{
		std::map<ObjVertexIndex, ObjDword>::const_iterator it = vertexCache.find(v_idx_finder);

		if (it != vertexCache.end())
			return it->second;	// find the cache

		mesh.positions.push_back(positions[v_idx_finder.v_idx]);

		if (v_idx_finder.vt_idx >= 0)
			mesh.texcoords.push_back(texcoords[v_idx_finder.vt_idx]);

		if (v_idx_finder.vt_idx >= 0)
			mesh.normals.push_back(normals[v_idx_finder.vn_idx]);

		unsigned idx = mesh.positions.size() - 1;
		vertexCache[v_idx_finder] = idx;

		return idx;
	}

	// save the faces to MeshData
	ObjBool LibObjFile::obj_save_mesh(const vector<ObjVector3>& positions,
									const vector<ObjVector2>& texcoords, 
									const vector<ObjVector3>& normals,
									const vector<vector<ObjVertexIndex> >& faces,
									const string& name, Mesh& tmpmesh)
	{
		if (faces.empty()) 
			return LIBOBJ_TRUE;

		map<ObjVertexIndex, ObjDword> vertexmap;			// save vertex only once to save memory

		for (size_t i = 0; i < faces.size(); i++)
		{
			const vector<ObjVertexIndex>& face_idx = faces[i];

			size_t face_count = face_idx.size();

			unsigned v0 = get_vertex_value(vertexmap, tmpmesh, positions, texcoords, normals, face_idx[0]);
			unsigned v1 = get_vertex_value(vertexmap, tmpmesh, positions, texcoords, normals, face_idx[1]);
			unsigned v2 = get_vertex_value(vertexmap, tmpmesh, positions, texcoords, normals, face_idx[2]);

			tmpmesh.indices.push_back(v0);
			tmpmesh.indices.push_back(v1);
			tmpmesh.indices.push_back(v2);

			// four side face, Polygon -> triangle conversion
			if (face_count == 4)
			{
				v0 = get_vertex_value(vertexmap, tmpmesh, positions, texcoords, normals, face_idx[0]);
				v1 = get_vertex_value(vertexmap, tmpmesh, positions, texcoords, normals, face_idx[2]);
				v2 = get_vertex_value(vertexmap, tmpmesh, positions, texcoords, normals, face_idx[3]);

				tmpmesh.indices.push_back(v0);
				tmpmesh.indices.push_back(v1);
				tmpmesh.indices.push_back(v2);
			}
		}

		tmpmesh.name = name;
		m_meshes.push_back(tmpmesh);

		return LIBOBJ_TRUE;
	}	

	ObjBool LibObjFile::mtl_file_load(const string& mtlname)
	{
		if (mtlname.empty())		// if obj has no material
			return LIBOBJ_FALSE;

		ObjMaterial* pmaterial = NULL;

		fstream mtlstream;
		mtlstream.open(mtlname.c_str(), std::ios_base::in);	//open strFileName file
		if (!mtlstream.is_open())
			return LIBOBJ_FALSE;
		
		string mtlflag;
		while (mtlstream.good())
		{
			mtlstream >> mtlflag;

			if (mtlflag == "newmtl")
			{
				string materialname;
				mtlstream >> materialname;
				if (mtl_index(materialname) != -1)
					continue;

				pmaterial = new ObjMaterial;
				pmaterial->name = materialname;
				m_materials.push_back(pmaterial);
			}
			else if (mtlflag == "Ka")		//Ambient color
				if (!mtl_parse_color(mtlstream, pmaterial->ambient))
					return LIBOBJ_FALSE;
			else if (mtlflag == "Kd")		//Diffuse color
				if (!mtl_parse_color(mtlstream, pmaterial->diffuse))
					return LIBOBJ_FALSE;
			else if (mtlflag == "Ks")		//Specular color
				if (!mtl_parse_color(mtlstream, pmaterial->specular))
					return LIBOBJ_FALSE;
			else if (mtlflag == "Tr"/*"d"*/)		// Alpha, that is transparent
				if (!mtl_parse_light(mtlstream, pmaterial->transparency))
					return LIBOBJ_FALSE;
			else if (mtlflag == "Ns")		//Shininess
				if (!mtl_parse_light(mtlstream, pmaterial->shininess))
					return LIBOBJ_FALSE;
			else if (mtlflag == "illum")		//Illumination type
				if (!mtl_parse_light(mtlstream, pmaterial->illumination_model))
					return LIBOBJ_FALSE;
			else if (mtlflag == "map_Ka")	//ambient Texture
				if (!mtl_parse_texture(mtlstream, pmaterial->ambient_texture))
					return LIBOBJ_FALSE;
			else if (mtlflag == "map_Kd")	//diffuse Texture
				if (!mtl_parse_texture(mtlstream, pmaterial->diffuse_texture))
					return LIBOBJ_FALSE;
			else if (mtlflag == "map_Ks")	//specular Texture
				if (!mtl_parse_texture(mtlstream, pmaterial->specular_texture))
					return LIBOBJ_FALSE;
			else
				mtlstream.ignore(1024, '\n');	// ignore this line, on the assumption that the max characters at this line is 1024.				
		}

		if (!mtlstream.eof())
			return LIBOBJ_FALSE;

		mtlstream.close();
		return LIBOBJ_TRUE;
	}
}
