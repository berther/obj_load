#ifndef OBJ_VERTEX_H
#define OBJ_VERTEX_H

#include "types.h"

struct ObjVertex
{
	ObjVector3 position;
	ObjVector3 normal;
	ObjVector2	texcoord;
};

struct ObjVertexIndex 
{
	ObjIntd v_idx;
	ObjIntd vn_idx;
	ObjIntd vt_idx;
};

inline bool operator< (const ObjVertexIndex& a, const ObjVertexIndex& b)
{
	if (a.v_idx != b.v_idx) return (a.v_idx < b.v_idx);
	if (a.vn_idx != b.vn_idx) return (a.vn_idx < b.vn_idx);
	if (a.vt_idx != b.vt_idx) return (a.vt_idx < b.vt_idx);

	return false;
}

#endif	// OBJ_VERTEX_H
