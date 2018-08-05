#pragma once
#include"K_Math\MyMathFanctions.h"

#include<string>
#include<vector>
#include<unordered_map>
#include<fbxsdk\fbxsdk.h>
#include<GLEW\glew.h>
#include"TextureList.h"

namespace K_Graphics {

	//�{�[���C���f�b�N�X�̂��߂̐����S�z��
	struct Vector4i {
		Vector4i() : data{ 0 } {}
		int data[4];
	};
	//���_���
	struct Vertex {
		K_Math::Vector3 position;
		K_Math::Vector2 texcoord;
		K_Math::Vector3 normal;
		K_Math::Vector4 boneWeight;
		Vector4i boneIndex;
	};

	struct Material {
		std::string materialName;
		Texture* texture;
		int numFace;
		K_Math::Vector4 diffuse;
		K_Math::Vector4 ambient;
		float           ambientPower;
		K_Math::Vector4 specular;
		float           specurarShininess;
		float           specurarPower;
	};
	struct VertexBuffer {
		GLuint VAO;
		GLuint VBO;
		std::vector<GLuint> IBOs;
		int numMaterial;
		int numFace;
	};

	struct Bone {
		//�A�j�����ƃt���[���������z��������Ă���
		std::vector<std::vector<K_Math::Matrix4x4>> mat;
		FbxCluster* cluster;
		K_Math::Matrix4x4 bindMat;
	};
	//�A�j���[�V�����̌��݂̃{�[�����
	struct BoneAnimation {
		K_Math::Matrix4x4 currentMat;
		K_Math::Matrix4x4 interPolationMat;
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
		void SetTexture(Texture* texture, int hierarchyIndex, int materialIndex);
		Material& GetMaterial(int hierarchyIndex, int materialIndex);

		int GetNumMaterial(int hierarchyIndex);
		int GetNumFace(int hierarchyIndex, int materialIndex);

	private:
		void CheckHierarchyRange(int index);
		void CheckMaterialArrayRange(int arrayIndex, int materialIndex);

	private:
		//���f���K�w�Ƃ��̃}�e���A���������z��������Ă���
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

		GLuint GetVAO(int hierarchyIndex);
		GLuint GetVBO(int hierarchyIndex);
		GLuint GetIBO(int hierarchyIndex, int materialIndex);
		int GetNumFace(int hierarchyIndex);

	private:
		void CheckHierarchyRange(int index);
		void CheckMaterialArrayRange(int arrayIndex, int materialIndex);

	private:
		//���f���K�w�������z��������Ă���
		std::vector<VertexBuffer> bufferArray;
	};

	//�{�[���s����
	class BoneData {
	public:
		BoneData();
		~BoneData();
		void AddBoneData(const std::vector<Bone>& boneData);
		void AddAnimData(const AnimType& animData);

		const K_Math::Matrix4x4& GetBindBoneMatrix(int hierarchyIndex, int boneIndex) const;
		const K_Math::Matrix4x4& GetCurrentBoneMatrix(int hierarchyIndex, int boneIndex, int animID, int time) const;
		int GetNumBone(int hierarchyIndex) const;
		const AnimType& GetAnimData(const std::string& animName) const;
		const AnimType& GetAnimData(int animIndex) const;

	private:
		//�A�j���[�V�������
		std::unordered_map<std::string, AnimType> animList;
		std::vector<std::string> animNameList;
		//�K�w�ƃ{�[�����ɂ��z��
		std::vector<std::vector<Bone>> boneData;
	};

	//3D�A�j���[�V�������
	class AnimationData {
	public:
		AnimationData(BoneData* bone, int numHirarchy);
		~AnimationData();
		void UpdateAnimation(float timeSpeed);
		void SetSpeed(float speed);
		void SetAnimation(const std::string& animName, bool playOnce, bool loop, int interpolationCount);
		void SetAnimation(int animIndex, bool playOnce, bool loop, int interpolationCount);
		void SetMatrixTextureData(int hierarchyIndex, Texture* texture);
		float GetCurrentAnimTime();
		int GetAnimationID();
		int GetNumBone(int hierarchyIndex) const;

	private:
		void SetCurrentAnimation(const AnimType& anim, bool playOnce, bool loop, int interpolationCount);
		void CalculateBoneMatrix(K_Math::Matrix4x4& resultMat, int hierarchyIndex, int boneIndex);
		void BoneInterporation(int hierarchyIndex, int boneIndex, float ratio);

	private:
		float speed;
		int   currentAnimID;
		float currentAnimTime;
		int   maxAnimTime;
		bool  isLoop;
		//�K�w�ƃ{�[�����ɂ�錻�݂̍s����z��
		std::vector<std::vector<BoneAnimation>> currentBone;
		float interporationCount;
		float interporationMaxCount;
		//�؂蕨�̃{�[���f�[�^���
		BoneData* bone;
	};




	//�{���͕����ł�����悩�����񂾂��AFBX�̏��ʂ��v�������������̂�FBXManager�����͂��̂܂܎����Ă������ق���������������Ȃ�
	//FBXManager�̉���ӔC�͂����ɂ���
	class FbxData {
	public:
		FbxData();
		~FbxData();
		void Add(FbxManager* manager, FbxImporter* importer, FbxScene* scene);
		FbxScene* GetScene();
		FbxImporter* GetInporter();

	private:
		FbxManager * manager;
		FbxImporter* importer;
		FbxScene*    scene;
	};

	//���f���f�[�^���ЂƂ܂Ƃ߂ɂ������́i���N���X�j
	struct ModelDataSuper {
	public:
		ModelDataSuper() = default;
		virtual ~ModelDataSuper() = 0;

	public:
		VertexData * vertexBuffer;
		MaterialData*  material;
		BoneData*      bone;
		AnimationData* animation;
	};

	//���f���f�[�^���ЂƂ܂Ƃ߂ɂ������́i�����o�ɂ����|�C���^�͏��L�������̍\���̂Ɉڏ������j
	struct ModelDatas : public ModelDataSuper {
	public:
		ModelDatas();
		virtual ~ModelDatas();
	};

	//���f���f�[�^���ЂƂ܂Ƃ߂ɂ������́i���\�[�X����̃R�s�[�łȂ�AnimationData*�ȊO�͊J�����Ȃ��j
	struct ModelDatasCopy : public ModelDataSuper {
	public:
		ModelDatasCopy();
		virtual ~ModelDatasCopy();
	};

	//3D���f�����\�[�X�A�����̏���݂��o����3D���f�����쐬�ł���
	struct ModelResource {
	public:
		ModelResource();
		~ModelResource();

	public:
		VertexData* vertexBuffer;
		MaterialData* material;
		BoneData* bone;
	};
}
