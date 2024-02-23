#include "Framework.h"

namespace Utility {
	string ToString(wstring value) {
		string tmp;
		tmp.assign(value.begin(), value.end());
		return tmp;
	}
	wstring ToWString(string value) {
		wstring tmp;
		tmp.assign(value.begin(), value.end());
		return tmp;
	}

	void Replace(OUT string& str, IN const string& compare, IN const string& replace) {
		size_t at = 0;
		while ((at = str.find(compare, at)) != string::npos) {
			str.replace(at, compare.length(), replace);
			at += replace.length();
		}
	}

	wstring GetExtension(wstring path) { return path.substr(path.find_last_of('.') + 1, path.length()); }
	string GetFileName(string path) {
		Replace(path, "\\", "/");
		return path.substr(path.find_last_of('/') + 1);
	}
	string GetFileNameWidthoutExtension(string path) {
		string filename = GetFileName(path);
		size_t idx = filename.find_last_of('.');
		return filename.substr(0, idx);
	}

	bool ExistDirectory(string path) {
		DWORD fileValue = GetFileAttributesA(path.c_str());
		BOOL tmp = (fileValue != INVALID_FILE_ATTRIBUTES && (fileValue & FILE_ATTRIBUTE_DIRECTORY));
		return tmp == TRUE;
	}
	bool ExistFile(string file_path) {
		DWORD fileValue = GetFileAttributesA(file_path.c_str());
		return fileValue < 0xffffffff;
	}
	void CreateFolders(string path) {
		vector<string> folders = SplitString(path, "/");

		string tmp = "";

		for (string& elem : folders) {
			tmp += elem + '/';
			if (!ExistDirectory(tmp)) CreateDirectoryA(tmp.c_str(), 0);
		}
	}
	vector<string> SplitString(string origin, const string& tok, const bool& includeLast) {
		vector<string> result;
		size_t cutAt = 0;

		while ((cutAt = origin.find_first_of(tok)) != origin.npos) {
			if (cutAt > 0) result.push_back(origin.substr(0, cutAt));
			origin = origin.substr(cutAt + 1);
		}
		if (includeLast && origin.length() > 0)
			result.push_back(origin);

		return result;
	}

}