#ifndef MTL_PARSE_H
#define MTL_PARSE_H

#include "types.h"

namespace mtlparse
{
	extern inline ObjBool mtl_parse_color(fstream& mtl_stream, ObjRgb& rgb);

	template <typename T>
	extern inline ObjBool mtl_parse_light(fstream& mtl_stream, T& light)
	{
		mtl_stream >> light;

		return mtl_stream.good();
	}

	extern inline ObjBool mtl_parse_texture(fstream& mtl_stream, string& texturename);
}

#endif	// MTL_PARSE_H
