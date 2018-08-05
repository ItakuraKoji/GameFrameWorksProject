#pragma once
#include"K3MDLoader.h"
#include"K_Math\MyMathFanctions.h"
#include<fbxsdk\fbxsdk.h>
#include<vector>
#include<unordered_map>
#include<string>
#include"ModelData.h"

namespace K_Loader {

	//FBX�t�@�C�����璸�_�A�}�e���A���A�{�[������ǂݎ���āAK3MD�`���̃��f���ւƃf�R�[�h���ă��f���f�[�^���쐬
	//�{�[���͂S�ȉ��̃E�F�C�g�Ō��ߑł��Ƃ���@(�T�ȏ�ł͕s����o��)�@�̂Ń��f�����ŋC��t����
	class FbxModelLoader {
	private:
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
		K3MDHierarchy*             PassModelData();

	private:
		void          Finalize();
		void          InitializeFBX(const std::string& fileName);
		void          RecursiveNode(FbxNode* node);
		void          LoadFbxMesh(FbxMesh* mesh);
		void          LoadVertex(FbxMesh* mesh, K_Graphics::Vertex* vertex);
		void          LoadMaterial(FbxMesh* mesh, VertexUVs& vertexData, std::vector<K3MDMaterial>& material);
		void          LoadBones(FbxMesh* mesh, K_Graphics::Vertex* vertex, std::vector<K3MDBone>& bone, PolygonTable *table);
		void          CalcCurrentBoneMatrix(std::vector<K3MDBone>& bone, std::vector<FbxCluster*>& cluster);

		PolygonTable* CreatePolygonTable(FbxMesh *mesh, int numVertex, int numFace);
		int CreateUVBaseVertex(FbxMesh* mesh, VertexUVs& uvMap);

	private:
		bool loaded;

		K3MDHierarchy* modelData;

		K_Graphics::FbxData       *fbxData;

		//�N���X���ŃA�N�Z�X���邽�߂̂��́Bdelete����ӔC�͂Ȃ�
		K_Graphics::TextureList *textureList;

		//FBX�t�@�C���̏ꏊ�����[�g�ɂ���
		char* fileRoot;

		K_Graphics::Vertex* vertexData;
		int numVertex;
		int numUV;
		int numFace;
	};
}