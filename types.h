#ifndef LIBOBJ_TYPES_H
#define LIBOBJ_TYPES_H

#ifdef _MSC_VER
#ifdef LIBOBJ_EXPORTS
#define LIBOBJAPI __declspec(dllexport)
#else               
#define LIBOBJAPI __declspec(dllimport)
#endif           
#else
#define LIBOBJAPI
#endif

#define LIBOBJ_TRUE 1
#define LIBOBJ_FALSE 0


// Visual Studio 6 and Embedded Visual C++ 4 doesn't
// realize that, e.g. char has the same size as __int8
// so we give up on __intX for them.
#if	defined(_MSC_VER)
	#if _MSC_VER < 1300
	   typedef signed char       int8_t;
	   typedef signed short      int16_t;
	   typedef signed int        int32_t;
	   typedef unsigned char     uint8_t;
	   typedef unsigned short    uint16_t;
	   typedef unsigned int      uint32_t;
	#else
	   typedef signed __int8     int8_t;
	   typedef signed __int16    int16_t;
	   typedef signed __int32    int32_t;
	   typedef unsigned __int8   uint8_t;
	   typedef unsigned __int16  uint16_t;
	   typedef unsigned __int32  uint32_t;
	#endif
#else
// #elif defined(__BORLANDC__) && defined(__WIN32__)
	  typedef signed char		int8_t;
	  typedef signed short		int16_t;
	  typedef signed int		int32_t;
	  typedef unsigned char		uint8_t;
	  typedef unsigned short	uint16_t;
	  typedef unsigned int		uint32_t;
#endif

typedef int32_t ObjBool;
typedef uint8_t ObjByte;
typedef uint16_t ObjWord;
typedef uint32_t ObjDword;
typedef int8_t ObjIntb;
typedef int16_t ObjIntw;
typedef int32_t ObjIntd;

typedef float ObjFloat;
typedef double ObjDouble;

#include <map>
#include <vector>
#include <string>
#include <fstream>

using std::map;
using std::vector;
using std::string;
using std::fstream;

// typedef float ObjVector3[3];
// typedef float ObjVector2[2];
// typedef float ObjTexture[2];
typedef struct ObjVector3
{
	ObjFloat x;
	ObjFloat y;
	ObjFloat z;
} ObjVector3;

typedef struct ObjVector2
{
	ObjFloat x;
	ObjFloat y;
} ObjVector2;

typedef float ObjRgb[3];
typedef float ObjRgba[4];

typedef struct Mesh Mesh;
typedef struct ObjVertex ObjVertex;
typedef struct ObjVertexIndex ObjVertexIndex;

typedef struct ObjMaterial ObjMaterial;

#endif	// LIBOBJ_TYPES_H
