#pragma once
#include<fstream>
#include"K_Math\MyMathFanctions.h"
#include"ModelData.h"

namespace K_Loader {
	//�}�e���A�����
	struct K3MDMaterial {
		//���̃}�e���A���Ɋ��蓖�Ă�ꂽ���_�C���f�b�N�X
		std::vector<unsigned int> vertexIndex;
		//�}�e���A�����
		std::string materialName;
		std::string texturePath;
		K_Math::Vector4 diffuse;
		K_Math::Vector4 ambient;
		float           ambientPower;
		K_Math::Vector4 specular;
		float           specurarShininess;
		float           specurarPower;
	};

	struct K3MDMat {
		//��x�[�X4x3�s��
		K_Math::Vector3 line[4];
	};

	struct K3MDBone {
		K3MDMat bindMat;
		//�A�j�����ƃt���[�����̔z��
		std::vector<std::vector<K3MDMat>> mat;
	};

	//���f���f�[�^�\���́A�K�w�̂��郂�f���͂��ꂪ��������
	struct K3MDData {
		std::vector<K_Graphics::Vertex> vertexData;
		std::vector<K3MDMaterial> materialData;
		//�{�[�����ɂ��z��
		std::vector<K3MDBone> boneData;
	};
	//�K�w���f���f�[�^
	struct K3MDHierarchy {
		//�ǂ݂��܂�Ă���ł��낤���f���̈ʒu
		std::string modelPath;
		std::vector<K3MDData> modelHierarchy;
		//�A�j���[�V�������
		std::unordered_map<std::string, K_Graphics::AnimType> animList;
		std::vector<std::string> animNameList;
	};

	//���f�����\�[�X�f�[�^����K3MD�`���̃o�C�i���t�@�C�����������AK3MD�`���t�@�C����ǂݍ��񂾂�ł���
	class K3MDLoader {
	public:
		K3MDLoader();
		~K3MDLoader();

		void CreateBinaryFile(K3MDHierarchy* data, const std::string& fileName, const std::string& outputFilePath);
		void CreateBinaryFile(K3MDHierarchy* data, const std::string& outputFilePath);
		
		K_Graphics::ModelResource* LoadModel(K3MDHierarchy* data, K_Graphics::TextureList* list);
		K_Graphics::ModelResource* LoadModel(const char* filepath, K_Graphics::TextureList* list);
		K3MDHierarchy* LoadK3MDHierarchy(const char* filepath);

		K3MDHierarchy* LoadK3MDHierarchyFromMemory(char* binaryData);

	private:
		void CreateVertex(K3MDData* modelData, GLuint VAO, const std::vector<GLuint>& IBOs);
		void CreateMaterial(const std::string& fileRoot, K3MDData* modelData, std::vector<GLuint>& IBOs);
		void CreateBone(K3MDHierarchy* data, K3MDData* modelData);

		void ReadBinary(char* dest, char* src, size_t* position, size_t size);

	private:
		std::string fileRoot;

		K_Graphics::TextureList* textureList;
		K_Graphics::VertexData* bufferData;
		K_Graphics::MaterialData* materialData;
		K_Graphics::BoneData* boneData;
	};
}
