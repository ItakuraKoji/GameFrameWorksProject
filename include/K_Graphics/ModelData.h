#pragma once
#include"K_Math\MyMathFanctions.h"

#include<string>
#include<vector>
#include<unordered_map>
#include<fbxsdk\fbxsdk.h>
#include<GLEW\glew.h>
#include"TextureList.h"

namespace K_Graphics {

	struct Material {
		std::string materialName;
		Texture* texture;
		int numFace;
		K_Math::Vector4 diffuse;
		K_Math::Vector4 ambient;
		K_Math::Vector4 specular;
		float power;
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	};
	struct VertexBuffer {
		GLuint VAO;
		GLuint VBO;
		std::vector<GLuint> IBOs;
		int numMaterial;
		int numFace;
	};
	struct Bone {
		FbxCluster* cluster;
		K_Math::Matrix4x4 bindMat;
		K_Math::Matrix4x4 currentMat;
		K_Math::Matrix4x4 interPolationMat;
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	};
	struct AnimType {
		std::string animName;
		int animID;
		int startTime;
		int endTime;
	};

	//�O������ǂݍ��܂ꂽ�A���邢�͍��ꂽ�}�e���A���̏���ێ�����
	//���̃N���X���ێ����Ă���e�N�X�`���̉���ӔC������
	class MaterialData {
	public:
		MaterialData();
		~MaterialData();

		void Add(std::vector<Material> material);
		void SetTexture(Texture* texture, int arrayIndex, int materialIndex);
		Material& GetMaterial(int arrayIndex, int materialIndex);

		int GetNumMaterial(int arrayIndex);
		int GetNumFace(int arrayIndex, int materialIndex);

	private:
		std::vector<std::vector<Material>> materialArray;
	};

	//�O������ǂݍ��܂ꂽ�A���邢�͍��ꂽ���_�o�b�t�@�̗ނ�ێ�����
	//���̃N���X�́AVAO�AVBO�AIBO�A�̂R�ɂ��ĉ���ӔC������
	class VertexData {
	public:
		VertexData();
		~VertexData();

		void Add(VertexBuffer& buffer);
		int GetNumBuffer();

		GLuint GetVAO(int arrayIndex);
		GLuint GetVBO(int arrayIndex);
		GLuint GetIBO(int arrayIndex, int materialIndex);
		int GetNumFace(int arrayIndex);

	private:
		std::vector<VertexBuffer> bufferArray;
	};

	//3D�A�j���[�V�������
	class AnimationData {
	public:
		AnimationData();
		~AnimationData();
		void UpdateAnimation();
		void SetSpeed(float speed);
		void Add(AnimType& animData);
		void SetAnimation(const std::string& animName, FbxScene* fbxScene, bool playOnce, bool isInterpolation, bool loop);
		float GetCurrentAnimTime();

		//BoneData��StartInterPolation()��ʒm����p
		bool IsStartInterpolation();

	private:
		std::unordered_map<std::string, AnimType> animList;
		float speed;
		int   currentAnimID;
		float currentAnimTime;
		int   maxAnimTime;
		bool  isLoop;
		bool  isInterpolation;
	};

	//�{�[������FBXCluster
	class BoneData {
	public:
		BoneData();
		~BoneData();
		void Add(std::vector<Bone>& boneData);
		void SetClurrentBoneData(int arrayIndex, int time);
		void SetMatrixTextureData(int arrayIndex, Texture* texture);
		void StartInterporation(int frameCount);
		int GetNumBone(int arrayIndex);

	private:
		K_Math::Matrix4x4 CalculateBoneMatrix(int arrayIndex, int boneIndex);
		void BoneInterporation(int arrayIndex, int boneIndex, float ratio);

	private:
		std::vector<std::vector<Bone>> boneData;
		float interporationCount;
		float interporationMaxCount;
	};

	//�{���͕����ł�����悩�����񂾂��AFBX�̏��ʂ��v�������������̂�FBXManager�����͂��̂܂܎����Ă������ق���������������Ȃ�
	//FBXManager�̉���ӔC�͂����ɂ���
	class FbxData {
	public:
		FbxData();
		~FbxData();
		void Add(FbxManager* manager, FbxImporter* importer, FbxScene* scene);
		FbxScene* GetScene();

	private:
		FbxManager * manager;
		FbxImporter* importer;
		FbxScene*    scene;
	};

	//���f���f�[�^���ЂƂ܂Ƃ߂ɂ������́i�����o�ɂ����|�C���^�͏��L�������̍\���̂Ɉڏ������j
	struct ModelDatas {
	public:
		ModelDatas();
		~ModelDatas();

	public:
		FbxData*       fbxData;
		VertexData*    vertexBuffer;
		MaterialData*  material;
		BoneData*      bone;
		AnimationData* animation;
	};
}