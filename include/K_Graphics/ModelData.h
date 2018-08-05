#pragma once
#include"K_Math\MyMathFanctions.h"

#include<string>
#include<vector>
#include<unordered_map>
#include<fbxsdk\fbxsdk.h>
#include<GLEW\glew.h>
#include"TextureList.h"

namespace K_Graphics {

	//ボーンインデックスのための整数４つ配列
	struct Vector4i {
		Vector4i() : data{ 0 } {}
		int data[4];
	};
	//頂点情報
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
		//アニメ数とフレーム数だけ配列を持っている
		std::vector<std::vector<K_Math::Matrix4x4>> mat;
		FbxCluster* cluster;
		K_Math::Matrix4x4 bindMat;
	};
	//アニメーションの現在のボーン情報
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

	//外部から読み込まれた、あるいは作られたマテリアルの情報を保持する
	//このクラスが保持しているテクスチャの解放責任を持つ
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
		//モデル階層とそのマテリアル数だけ配列を持っている
		std::vector<std::vector<Material>> materialArray;
	};

	//外部から読み込まれた、あるいは作られた頂点バッファの類を保持する
	//このクラスは、VAO、VBO、IBO、の３つについて解放責任を持つ
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
		//モデル階層数だけ配列を持っている
		std::vector<VertexBuffer> bufferArray;
	};

	//ボーン行列情報
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
		//アニメーション情報
		std::unordered_map<std::string, AnimType> animList;
		std::vector<std::string> animNameList;
		//階層とボーン数による配列
		std::vector<std::vector<Bone>> boneData;
	};

	//3Dアニメーション情報
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
		//階層とボーン数による現在の行列情報配列
		std::vector<std::vector<BoneAnimation>> currentBone;
		float interporationCount;
		float interporationMaxCount;
		//借り物のボーンデータ情報
		BoneData* bone;
	};




	//本当は分離できたらよかったんだが、FBXの情報量が思ったよりも多いのでFBXManagerたちはそのまま持っておいたほうがいいかもしれない
	//FBXManagerの解放責任はここにある
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

	//モデルデータをひとまとめにしたもの（基底クラス）
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

	//モデルデータをひとまとめにしたもの（メンバにしたポインタは所有権がこの構造体に移譲される）
	struct ModelDatas : public ModelDataSuper {
	public:
		ModelDatas();
		virtual ~ModelDatas();
	};

	//モデルデータをひとまとめにしたもの（リソースからのコピーでないAnimationData*以外は開放しない）
	struct ModelDatasCopy : public ModelDataSuper {
	public:
		ModelDatasCopy();
		virtual ~ModelDatasCopy();
	};

	//3Dモデルリソース、ここの情報を貸し出して3Dモデルを作成できる
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
