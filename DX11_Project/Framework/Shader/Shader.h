#pragma once

class Shader {
protected:
	Shader() = default;
	virtual ~Shader() = default;
	wstring file;
public:
	static class VertexShader  * AddVS(wstring file);
	static class PixelShader   * AddPS(wstring file);
	static class ComputeShader * AddCS(wstring file);
	static class GeometryShader* AddGS(wstring file);
	static void Delete();

	wstring GetFile() { return file; };
private:
	static unordered_map<wstring, Shader*> shaders;
};