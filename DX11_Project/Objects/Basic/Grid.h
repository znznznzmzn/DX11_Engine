#pragma once
class Grid {
private:
	const int MAX_VALUE = 100;
	Material* material;
	VertexBuffer* vBuffer;
	MatrixBuffer* wBuffer;
	vector<VertexColor> vertices;
	UINT width, height;

public:
	Grid();
	~Grid();

	void Render();

};