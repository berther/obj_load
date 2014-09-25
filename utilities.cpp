#include "utilities.h"
#include <algorithm>

namespace baseutis
{
	void trim_right_ws(string& name)
	{
		name.erase(name.find_last_not_of(" \t\n")+1);
	}

	void trim_left_ws(string& name)
	{
		name.erase(0, name.find_first_not_of(' '));
	}

	void normalize_path(string &path)
	{
		std::replace(path.begin(), path.end(), '\\', '/');
	}

	string get_title_name(const string& name)
	{
		string name2 = name;
		normalize_path(name2);

		if (name2.rfind('/') != string::npos)
			name2 = name2.substr(name2.rfind('/')+1);
		name2.erase(name2.rfind('.'));

		return name2;
	}

	string get_file_path(const string& path)
	{
		string path2 = path;
		normalize_path(path2);

		return path2.substr(0, path2.rfind('/'));
	}
}
