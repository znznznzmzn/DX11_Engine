#include "Framework.h"

unordered_map<wstring, Texture*> Texture::textures;

Texture::Texture(ID3D11ShaderResourceView* srv, ScratchImage& image) : srv(srv), image(move(image)) { }
Texture::~Texture() { SAFE_RELEASE(srv); }

Texture* Texture::Add(wstring file) {
	if (textures.count(file) > 0) 
		return textures[file];
	
	wstring extension = GetExtension(file);

	ScratchImage image;

	if(extension.compare(L"tga") == 0)
		LoadFromTGAFile(file.c_str(), nullptr, image);
	else if (extension.compare(L"dds") == 0)
		LoadFromDDSFile(file.c_str(), DDS_FLAGS_NONE, nullptr, image);
	else 
		LoadFromWICFile(file.c_str(), WIC_FLAGS_NONE, nullptr, image);

	ID3D11ShaderResourceView* srv;

	CreateShaderResourceView(DEVICE, image.GetImages(), image.GetImageCount(), image.GetMetadata(), &srv);

	if (srv == nullptr) return nullptr;

	textures[file] = new Texture(srv, image);
	textures[file]->file = file;

	return textures[file];
}

Texture* Texture::Add(wstring file, wstring key) {
	if (textures.count(key) > 0)
		return textures[key];

	wstring extension = GetExtension(file);

	ScratchImage image;

	if (extension.compare(L"tga") == 0)
		LoadFromTGAFile(file.c_str(), nullptr, image);
	else if (extension.compare(L"dds") == 0)
		LoadFromDDSFile(file.c_str(), DDS_FLAGS_NONE, nullptr, image);
	else
		LoadFromWICFile(file.c_str(), WIC_FLAGS_NONE, nullptr, image);

	ID3D11ShaderResourceView* srv;

	CreateShaderResourceView(DEVICE, image.GetImages(), image.GetImageCount(), image.GetMetadata(), &srv);

	if (srv == nullptr) return nullptr;

	textures[key] = new Texture(srv, image);
	textures[key]->file = file;

	return textures[key];
}

void Texture::Clear() { for (pair<wstring, Texture*> elem : textures) { SAFE_DELETE(elem.second); } }

void Texture::PSSet(UINT slot) { DC->PSSetShaderResources(slot, 1, &srv); }

void Texture::ReadPixels(vector<float4>& outPixels) {
	uint8_t* colors = image.GetPixels();
	UINT size = static_cast<UINT>(image.GetPixelsSize());

	float scale = 1.0f / 255.0f;

	outPixels.resize(size / 4);
	for (UINT i = 0; i < outPixels.size(); i++) {
		outPixels[i].x = colors[i * 4 + 0] * scale;
		outPixels[i].y = colors[i * 4 + 1] * scale;
		outPixels[i].z = colors[i * 4 + 2] * scale;
		outPixels[i].w = colors[i * 4 + 3] * scale;
	}
}

wstring Texture::GetPath() { return file; }

