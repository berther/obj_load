#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>

namespace baseutis
{
	using std::string;
	// trim the space characters from right to left 
	// white space: " \t\n"
	// why do have a trim act? 
	// the face index line may contain space characters at the end
	// say "f 1//1 2//3 4//5 ", so it will have an affection when
	// the face index line goes into faceLineParse() function 
	//  which determines the face is whether a tri or polygon by space character
	extern inline void trim_right_ws(string& name);

	// this function is employed by fileLineParseMtlLib() function
	// left white space: ' '
	extern inline void trim_left_ws(string& name);

	// normalize the path consisted of \..\..\
	// to make it be accessed both for Windows and Linux
	extern inline void normalize_path(string &path);

	// get the title of the file
	extern string get_title_name(const string& name);

	// get the path of the file
	extern inline string get_file_path(const string& path);
}

#endif	// UTILITIES_H
