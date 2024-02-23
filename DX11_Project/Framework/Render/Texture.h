#pragma once

class Texture {
private:
	Texture(ID3D11ShaderResourceView* srv, ScratchImage& image);
	~Texture();
public:
	static Texture* Add(wstring file);
	static Texture* Add(wstring file, wstring key);
	static void Clear();

	void PSSet(UINT slot);

	UINT Width()  { return static_cast<UINT>(image.GetMetadata().width ); }
	UINT Height() { return static_cast<UINT>(image.GetMetadata().height); }

	void ReadPixels(vector<float4>& outPixels);
	wstring GetPath();
	ID3D11ShaderResourceView*& GetSRV() { return srv; }
private:
	wstring file = L"";
	ScratchImage image;
	ID3D11ShaderResourceView* srv;
	static unordered_map<wstring, Texture*> textures;
};