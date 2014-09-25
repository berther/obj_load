#ifndef OBJ_LOAD_H
#define OBJ_LOAD_H

#include "types.h"
#include "vertex.h"
#include "mesh.h"
#include "objparse.h"
#include "mtlload.h"

namespace objload
{
	class LibObjFile
	{
	public:
		LibObjFile();
		~LibObjFile();

		ObjBool obj_file_load(const string& objname);

	private:
		void obj_file_free(void);
		ObjBool mtl_file_load(const string& mtlname);
		ObjIntd mtl_index(string usemtl) const;
		ObjBool obj_save_mesh(const vector<ObjVector3>& positions,
							const vector<ObjVector2>& texcoords, 
							const vector<ObjVector3>& normals,
							const vector<vector<ObjVertexIndex> >& faceGroup,
							const string& meshname, Mesh& mesh);

	private:
		vector<ObjMaterial*> m_materials;
		vector<Mesh> m_meshes;
	};
}

#endif	// OBJ_LOAD_H
