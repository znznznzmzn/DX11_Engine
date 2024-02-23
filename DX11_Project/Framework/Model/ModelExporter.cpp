#include "Framework.h"

ModelExpoter::ModelExpoter(string name, string file) : name(name) {
	importer = new Assimp::Importer();
	importer->SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
	scene = importer->ReadFile(file, 
		aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_MaxQuality);
	assert(scene != nullptr);
}
ModelExpoter::~ModelExpoter() {
	SAFE_DELETE(importer);
	for (Material*& elem : materials) { SAFE_DELETE(elem); }
	for (MeshData*& elem : meshes   ) { SAFE_DELETE(elem); }
	materials.clear();
	meshes.clear();
}

void ModelExpoter::ExportMaterial() {
	ReadMaterial();
	WriteMaterial();
}
void ModelExpoter::ExportMesh() {
	ReadNode(scene->mRootNode, -1, -1);
	ReadMesh(scene->mRootNode);
	WriteMesh();
}
void ModelExpoter::ExportClip(string clipName) {
	for (UINT i = 0; i < scene->mNumAnimations; i++) {
		Clip* clip = ReadClip(scene->mAnimations[i]);
		WriteClip(clip, clipName, i);
	}
}
void ModelExpoter::ReadMaterial() {
	for (UINT i = 0; i < scene->mNumMaterials; i++) {
		aiMaterial* srcMaterial = scene->mMaterials[i];
		Material* material = new Material();
		
		string mat_Name = srcMaterial->GetName().C_Str(); 
		material->SetName(mat_Name);

		aiColor3D color;
		MaterialBuffer::Data& data = material->GetData();

		srcMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		data.diffuse = float4(color.r, color.g, color.b, 1.0f);

		srcMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color);
		data.specular = float4(color.r, color.g, color.b, 1.0f);

		srcMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color);
		data.ambient = float4(color.r, color.g, color.b, 1.0f);

		srcMaterial->Get(AI_MATKEY_SHININESS, data.shininess);

		aiString file;
		srcMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &file);
		material->SetDiffuseMap(ToWString(CreateTexture(file.C_Str())));
		file.Clear();

		srcMaterial->GetTexture(aiTextureType_SPECULAR, 0, &file);
		material->SetSpecularMap(ToWString(CreateTexture(file.C_Str())));
		file.Clear();

		srcMaterial->GetTexture(aiTextureType_NORMALS, 0, &file);
		material->SetNormalMap(ToWString(CreateTexture(file.C_Str())));
		file.Clear();
		
		materials.push_back(material);
	}
}

void ModelExpoter::WriteMaterial() {
	string savePath = "Models/Materials/" + name + "/";
	string file = name + ".mats";

	CreateFolders(savePath);

	BinaryWriter* w = new BinaryWriter(savePath + file);

	w->UInt(materials.size());

	for (Material* elem : materials) {
		string path = savePath + elem->GetName() + ".mat";
		elem->SaveMaterial(path);
		w->String(path);
		delete elem;
	}
	materials.clear();

	delete w;
}

void ModelExpoter::ReadMesh(aiNode* node) {
	for (UINT i = 0; i < node->mNumMeshes; i++) {
		MeshData* mesh = new MeshData();
		mesh->name = node->mName.C_Str();

		UINT idx = node->mMeshes[i];
		aiMesh* srcMesh = scene->mMeshes[idx];

		mesh->mererialIndex = srcMesh->mMaterialIndex;

		UINT startVertex = mesh->vertices.size();

		vector<VertexWeights> vertexWeights(srcMesh->mNumVertices);
		ReadBone(srcMesh, vertexWeights);

		mesh->vertices.resize(srcMesh->mNumVertices);
		for (UINT v = 0; v < srcMesh->mNumVertices; v++) {
			ModelVertex vertex;
			memcpy(&vertex.pos, &srcMesh->mVertices[v], sizeof(float3));
			if (srcMesh->HasTextureCoords(0))
				memcpy(&vertex.uv, &srcMesh->mTextureCoords[0][v], sizeof(float2));

			if (srcMesh->HasNormals())
				memcpy(&vertex.normal, &srcMesh->mNormals[v], sizeof(float3));

			if (srcMesh->HasTangentsAndBitangents())
				memcpy(&vertex.tangent, &srcMesh->mTangents[v], sizeof(float3));

			if (!vertexWeights.empty()) {
				vertexWeights[v].Normalize();

				vertex.indices.x = (float)vertexWeights[v].indices[0];
				vertex.indices.y = (float)vertexWeights[v].indices[1];
				vertex.indices.z = (float)vertexWeights[v].indices[2];
				vertex.indices.w = (float)vertexWeights[v].indices[3];

				vertex.weights.x = vertexWeights[v].weights[0];
				vertex.weights.y = vertexWeights[v].weights[1];
				vertex.weights.z = vertexWeights[v].weights[2];
				vertex.weights.w = vertexWeights[v].weights[3];
			}

			mesh->vertices[v] = vertex;
		}

		mesh->indices.resize(srcMesh->mNumFaces * 3);
		for (UINT f = 0; f < srcMesh->mNumFaces; f++) {
			aiFace& face = srcMesh->mFaces[f];
			for (UINT k = 0; k < face.mNumIndices; k++) {
				mesh->indices[f * 3 + k] = face.mIndices[k] + startVertex;
			}
		}

		meshes.push_back(mesh);
	}

	for (UINT i = 0; i < node->mNumChildren; i++) ReadMesh(node->mChildren[i]);
}

void ModelExpoter::ReadNode(aiNode* node, int index, int parent) {
	NodeData* nodeData = new NodeData();
	nodeData->index = index;
	nodeData->parent = parent;
	nodeData->name = node->mName.C_Str();

	Matrix matrix(node->mTransformation[0]);
	nodeData->transform = XMMatrixTranspose(matrix);

	nodes.push_back(nodeData);
	for (UINT i = 0; i < node->mNumChildren; i++)
		ReadNode(node->mChildren[i], nodes.size(), index);
}

void ModelExpoter::ReadBone(aiMesh* mesh, vector<VertexWeights>& vertexWeights) {
	for (UINT i = 0; i < mesh->mNumBones; i++) {
		UINT boneIndex = 0;

		string name = mesh->mBones[i]->mName.C_Str();
		if (boneMap.count(name) == 0) {
			boneIndex = boneCount++;
			boneMap[name] = boneIndex;

			BoneData* boneData = new BoneData();
			boneData->name = name;
			boneData->index = static_cast<int>(boneIndex);

			Matrix matrix(mesh->mBones[i]->mOffsetMatrix[0]);
			boneData->offset = XMMatrixTranspose(matrix);

			bones.push_back(boneData);
		} else boneIndex = boneMap[name]; 

		for (UINT j = 0; j < mesh->mBones[i]->mNumWeights; j++) {
			UINT index = mesh->mBones[i]->mWeights[j].mVertexId;
			vertexWeights[index].Add(boneIndex, mesh->mBones[i]->mWeights[j].mWeight);
		}
	}
}

void ModelExpoter::WriteMesh() {
	string path = "Models/Meshes/" + name + ".mesh";

	CreateFolders(path);

	BinaryWriter* w = new BinaryWriter(path);

	w->UInt(meshes.size());
	for (MeshData* elem : meshes) {
		w->String(elem->name);
		w->UInt(elem->mererialIndex);
		
		w->UInt(elem->vertices.size());
		w->Byte(elem->vertices.data(), sizeof(ModelVertex) * elem->vertices.size());

		w->UInt(elem->indices.size());
		w->Byte(elem->indices.data(), sizeof(UINT) * elem->indices.size());
		delete elem;
	}
	meshes.clear();

	w->UInt(nodes.size());
	for (NodeData* elem : nodes) {
		w->Int(elem->index);
		w->String(elem->name);
		w->Int(elem->parent);
		w->Matrix(elem->transform);
		delete elem;
	}
	nodes.clear();

	w->UInt(bones.size());
	for (BoneData* elem : bones) {
		w->String(elem->name);
		w->Int(elem->index);
		w->Matrix(elem->offset);
		delete elem;
	}
	bones.clear();

	delete w;
}

Clip* ModelExpoter::ReadClip(aiAnimation* animation) {
	Clip* clip = new Clip();
	clip->name = animation->mName.C_Str();
	clip->tickPerSecond = (float)animation->mTicksPerSecond;
	clip->framecount = (UINT)(animation->mDuration) + 1;

	vector<ClipNode> clipNodes;
	for (UINT i = 0; i < animation->mNumChannels; i++) {
		aiNodeAnim* srcNode = animation->mChannels[i];
		
		ClipNode node;
		node.name = srcNode->mNodeName;

		UINT keyCount = max(srcNode->mNumPositionKeys, srcNode->mNumRotationKeys);
		keyCount = max(keyCount, srcNode->mNumScalingKeys);

		KeyTransform transform;

		for (UINT k = 0; k < keyCount; k++) {
			bool isFound = false;
			float t = node.transforms.size();

			if (k < srcNode->mNumPositionKeys && abs((float)srcNode->mPositionKeys[k].mTime - t) <= FLT_EPSILON) {
				aiVectorKey key = srcNode->mPositionKeys[k];
				memcpy_s(&transform.position, sizeof(float3), &key.mValue, sizeof(aiVector3D));
				isFound = true;
			}
			if (k < srcNode->mNumRotationKeys && abs((float)srcNode->mRotationKeys[k].mTime - t) <= FLT_EPSILON) {
				aiQuatKey key = srcNode->mRotationKeys[k];
				transform.rotation.x = (float)key.mValue.x;
				transform.rotation.y = (float)key.mValue.y;
				transform.rotation.z = (float)key.mValue.z;
				transform.rotation.w = (float)key.mValue.w;
				isFound = true;
			}
			if (k < srcNode->mNumScalingKeys && abs((float)srcNode->mScalingKeys[k].mTime - t) <= FLT_EPSILON) {
				aiVectorKey key = srcNode->mScalingKeys[k];
				memcpy_s(&transform.scale, sizeof(float3), &key.mValue, sizeof(aiVector3D));
				isFound = true;
			}
			if (isFound) node.transforms.push_back(transform);
		}
		if (node.transforms.size() < clip->framecount) {
			UINT count = clip->framecount - node.transforms.size();
			KeyTransform keyTransform = node.transforms.back();
			for (UINT n = 0; n < count; n++) node.transforms.push_back(keyTransform);
		}
		clipNodes.push_back(node);
	}

	ReadKeyFrame(clip, scene->mRootNode, clipNodes);

	return clip;
}

void ModelExpoter::ReadKeyFrame(Clip* clip, aiNode* node, vector<ClipNode>& clipNodes) {
	KeyFrame* keyFrame = new KeyFrame();
	keyFrame->boneName = node->mName.C_Str();
	for (UINT i = 0; i < clip->framecount; i++) {
		ClipNode* clipNode = nullptr;
		for (ClipNode& tmp : clipNodes) {
			if (tmp.name == node->mName) {
				clipNode = &tmp;
				break;
			}
		}
		KeyTransform keyTransform;
		if (clipNode == nullptr) {
			Matrix transform(node->mTransformation[0]);
			transform = XMMatrixTranspose(transform);

			Vector3 S, R, T;
			XMMatrixDecompose(S.GetData(), R.GetData(), T.GetData(), transform);
			keyTransform.scale = S;
			XMStoreFloat4(&keyTransform.rotation, R);
			keyTransform.position = T; 
		}
		else keyTransform = clipNode->transforms[i];

		keyFrame->transforms.push_back(keyTransform);
	}
	clip->keyFrames.push_back(keyFrame);

	for (UINT i = 0; i < node->mNumChildren; i++)
		ReadKeyFrame(clip, node->mChildren[i], clipNodes);
}

void ModelExpoter::WriteClip(Clip* clip, string clipName, UINT index) {
	string file = "Models/Clips/" + name + "/" + clipName + to_string(index) + ".clip";
	CreateFolders(file);

	BinaryWriter* w = new BinaryWriter(file);
	w->String(clip->name);
	w->UInt(clip->framecount);
	w->Float(clip->tickPerSecond);

	w->UInt(clip->keyFrames.size());
	for (KeyFrame* elem : clip->keyFrames) {
		w->String(elem->boneName);
		w->UInt(elem->transforms.size());
		w->Byte(elem->transforms.data(), sizeof(KeyTransform) * elem->transforms.size());

		delete elem;
	}
	delete clip;
	delete w;
}

string ModelExpoter::CreateTexture(string file) {
	if (file.length() == 0) return "";

	string fileName = GetFileNameWidthoutExtension(file) + ".png";
	string path = "Textures/Model/" + name + "/" + fileName;

	CreateFolders(path);
	if (ExistFile(path)) return path;

	const aiTexture* texture = scene->GetEmbeddedTexture(file.c_str());

	if (texture == nullptr) return "";

	if (texture->mHeight < 1) {
		BinaryWriter w(path);
		w.Byte(texture->pcData, texture->mWidth);
	} else {
		Image img;
		img.width = texture->mWidth;
		img.height = texture->mHeight;
		img.pixels = (uint8_t*)texture->pcData;
		img.rowPitch = img.width *4;
		img.slicePitch = img.width * img.height * 4;

		SaveToWICFile(img, WIC_FLAGS_NONE, 
			GetWICCodec(WIC_CODEC_PNG), ToWString(path).c_str());
	}

	return path;
}
