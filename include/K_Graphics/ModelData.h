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

	struct AnimMatrix {
		//フレーム数だけ配列を持っている
		std::vector<std::vector<K_Math::Matrix4x4>> animMatrix;
	};
	struct Bone {
		//アニメ数とフレーム数だけ配列を持っている
		std::vector<std::vector<K_Math::Matrix4x4>> mat;
		FbxCluster* cluster;
		K_Math::Matrix4x4 bindMat;
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
		std::vector<VertexBuffer> bufferArray;
	};

	//3Dアニメーション情報
	class AnimationData {
	public:
		AnimationData();
		~AnimationData();
		void UpdateAnimation();
		void SetSpeed(float speed);
		void Add(AnimType& animData);
		void SetAnimation(const std::string& animName, bool playOnce, bool isInterpolation, bool loop);
		float GetCurrentAnimTime();
		int GetAnimationID();

		//BoneDataにStartInterPolation()を通知する用
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

	//ボーン情報とFBXCluster
	class BoneData {
	public:
		BoneData();
		~BoneData();
		void Add(std::vector<Bone>& boneData);
		void SetClurrentBoneData(int hierarchyIndex, int animID, int time);
		void SetMatrixTextureData(int hierarchyIndex, Texture* texture);
		void StartInterporation(int frameCount);
		void UpdateInterporation();
		int GetNumBone(int hierarchyIndex);

	private:
		void CalculateBoneMatrix(K_Math::Matrix4x4& resultMat, int hierarchyIndex, int boneIndex);
		void BoneInterporation(int hierarchyIndex, int boneIndex, float ratio);

	private:
		std::vector<std::vector<Bone>> boneData;
		float interporationCount;
		float interporationMaxCount;
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

	//モデルデータをひとまとめにしたもの（メンバにしたポインタは所有権がこの構造体に移譲される）
	struct ModelDatas {
	public:
		ModelDatas();
		~ModelDatas();

	public:
		//FbxData*       fbxData;
		VertexData*    vertexBuffer;
		MaterialData*  material;
		BoneData*      bone;
		AnimationData* animation;
	};
}
