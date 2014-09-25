#include "mtlparse.h"

namespace mtlparse
{
	ObjBool mtl_parse_color(fstream& mtl_stream, ObjRgb& rgb)
	{
		mtl_stream >> rgb[0] >> rgb[1] >> rgb[2];

		return mtl_stream.good();
	}

	ObjBool mtl_parse_texture(fstream& mtl_stream, string& texturename)
	{
		getline(mtl_stream, texturename);

		// if the line is: 	map_Kd Map__16_Gradient Ramp.bmp(the texture name),
		// we should ignore the characters before texture name.
		texturename = texturename.substr(texturename.rfind(' ')+1);

		return mtl_stream.good();
	}
}
