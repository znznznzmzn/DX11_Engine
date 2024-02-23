#include "Framework.h"

BinaryReader::BinaryReader(wstring filePath) {
	file = CreateFile(filePath.c_str(), GENERIC_READ,
		FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
}
BinaryReader::BinaryReader(string filePath) {
	file = CreateFileA(filePath.c_str(), GENERIC_READ,
		FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
}
BinaryReader::~BinaryReader() { CloseHandle(file); }

int BinaryReader::Int() { 
	int tmp;
	ReadFile(file, &tmp, sizeof(int), &size, nullptr);
	return tmp;
}
UINT  BinaryReader::UInt() {
	UINT tmp;
	ReadFile(file, &tmp, sizeof(UINT), &size, nullptr);
	return tmp;
}
float BinaryReader::Float() {
	float tmp;
	ReadFile(file, &tmp, sizeof(float), &size, nullptr);
	return tmp;
}

string BinaryReader::String() {
	UINT strSize = UInt();
	char* tmp = new char[strSize + 1];
	ReadFile(file, tmp, sizeof(char) * strSize, &size, nullptr);
	tmp[strSize] = '\0';
	return tmp;
}
wstring BinaryReader::WString() {
	UINT strSize = UInt();
	WCHAR* tmp = new WCHAR[strSize + 1];
	ReadFile(file, tmp, sizeof(WCHAR) * strSize, &size, nullptr);
	tmp[strSize] = '\0';
	return tmp;
}
Vector3 BinaryReader::Vector() {
	Vector3 tmp;
	ReadFile(file, &tmp, sizeof(Vector3), &size, nullptr);
	return tmp;
}
XMMATRIX BinaryReader::Matrix() {
	XMMATRIX tmp;
	ReadFile(file, &tmp, sizeof(XMMATRIX), &size, nullptr);
	return tmp;
}
XMFLOAT4 BinaryReader::Float4() {
	XMFLOAT4 tmp;
	ReadFile(file, &tmp, sizeof(XMFLOAT4), &size, nullptr);
	return tmp;
}

void BinaryReader::Byte(void** data, UINT dataSize) 
{ ReadFile(file, *data, dataSize, &size, nullptr); }