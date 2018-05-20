#pragma once
#include<fbxsdk\fbxsdk.h>
#include<vector>
#include<unordered_map>
#include<string>
#include"K_Math\MyMathFanctions.h"
#include"ModelData.h"

namespace K_Loader {

	//FBX�t�@�C�����璸�_�A�}�e���A���A�{�[������ǂݎ���ĕێ����A�ق��̃N���X�ɓn��
	//�ǂݍ��݂��ł��Ȃ������f�[�^��delete���邱�Ƃɂ���āA�������Ƃ��Ĉ�����iNULL�j
	//���̃N���X�ł͉���ӔC�𕉂�Ȃ�
	//�{�[���͂S�ȉ��̃E�F�C�g�Ō��ߑł��Ƃ���@(�T�ȏ�ł͕s����o��)�@�̂Ń��f�����ŋC��t����
	class FbxModelLoader {
	private:
		struct Vector4i {
			Vector4i() : data{ 0 } {}
			int data[4];
		};
		struct Vertex {
			K_Math::Vector3 position = K_Math::Vector3::Zero();
			K_Math::Vector2 texcoord = K_Math::Vector2::Zero();
			K_Math::Vector3 normal = K_Math::Vector3::Zero();
			K_Math::Vector4 boneWeight = K_Math::Vector4::Zero();
			Vector4i boneIndex;
		};

		//���_����|���S�����t��������e�[�u��
		struct PolygonTable {
			//�|���S���ԍ�
			int polygonIndex[255] = {};
			//�|���S�����C���f�b�N�X
			int polygon123[255] = {};
			int numPolygon = 0;
		};

		struct NewVertexData {
			//UV���
			FbxArray<FbxVector2> uv;
			FbxArray<int> index;
		};

		//���_������UV�z��ƐV���Ȓ��_���W�ɑΉ�����C���f�b�N�X�����e�[�u��
		typedef FbxMap<int, NewVertexData> VertexUVs;

	public:
		FbxModelLoader();
		~FbxModelLoader();
		bool LoadFBX(const std::string& fileName, K_Graphics::TextureList* list);

		K_Graphics::FbxData*       PassFbxData();
		K_Graphics::VertexData*    PassVertexBuffer();
		K_Graphics::MaterialData*  PassMaterialData();
		K_Graphics::AnimationData* PassAnimationData();
		K_Graphics::BoneData*      PassBoneData();


	private:
		void          Finalize();
		bool          InitializeFBX(const std::string& fileName);
		bool          RecursiveNode(FbxNode* node);
		bool          LoadFbxMesh(FbxMesh* mesh);
		void          LoadVertex(FbxMesh* mesh, Vertex* vertex);
		void          LoadMaterial(FbxMesh* mesh, VertexUVs& vertexData, std::vector<K_Graphics::Material>& material, std::vector<GLuint>& IBOs);
		bool          LoadBones(FbxMesh* mesh, Vertex* vertex, PolygonTable *table);
		void          CalcCurrentBoneMatrix(std::vector<K_Graphics::Bone>& bone);

		PolygonTable* CreatePolygonTable(FbxMesh *mesh, int numVertex, int numFace);
		int CreateUVBaseVertex(FbxMesh* mesh, VertexUVs& uvMap);

	private:
		bool loaded;

		K_Graphics::FbxData       *fbxData;
		K_Graphics::VertexData    *bufferData;
		K_Graphics::MaterialData  *materialData;
		K_Graphics::AnimationData *animationData;
		K_Graphics::BoneData      *boneData;

		//�N���X���ŃA�N�Z�X���邽�߂̂��́Bdelete����ӔC�͂Ȃ�
		K_Graphics::TextureList *textureList;

		//FBX�t�@�C���̏ꏊ�����[�g�ɂ���
		char* fileRoot;
		Vertex* vertexData;
		int numVertex;
		int numUV;
		int numFace;
	};
}