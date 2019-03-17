#pragma once
#include<string>
#include"ModelData.h"
#include"TextureList.h"
#include"FbxModelLoader.h"
#include"K3MDLoader.h"

namespace K_Graphics {

	//!@brief モデルクラスの初期化に必要なパラメーターの製作を担当するクラス
	class ModelDataFactory {
	public:
		//K3MDモデルへコンバートする、rootPathはモデルデータの位置
		void CreateK3MDModelFromHierarchy(K_Loader::K3MDHierarchy* hierarchy, const std::string& rootPath, const std::string& outputPath);
		//K3MDモデルへコンバートする
		void CreateK3MDModelFromFBX(const std::string& fbxFilePath, const std::string& outputPath, const std::string& outputFileName);


		//!@brief FBX形式の3Dモデルを読み込む
		//!@param[in] fileName FBXモデルのファイルパス
		//!@param[in] textureList テクスチャリストへのポインタ（FBXで使われているテクスチャを読み込むのに使う）
		//!@return モデルのデータクラスへのポインタ
		ModelDatas* LoadFBXModel(const std::string& fileName, TextureList* textureList);
		ModelDatas* LoadK_3DModel(const std::string& fileName, TextureList* textureList);
		//!モデルデータのリソース作成（返すポインタの開放責任がある）
		ModelResource* CreateModelResourceFromFBX(const std::string& fileName, TextureList* textureList);
		ModelResource* CreateModelResourceFromK3MD(const std::string& fileName, TextureList* textureList);
		//!リソースからデータを借りてモデルを作成（返すポインタの開放責任がある）
		ModelDatasCopy* CreateModelDataCopy(ModelResource* resource);
		//!@brief 四角形の板ポリゴンを作成
		//!@param[in] width 板ポリゴンの幅
		//!@param[in] height 板ポリゴンの高さ
		//!@param[in] texture 使用するテクスチャ（nullptr可）
		//!@param[in] isCenter コントロールポイントを中央にするか（省略時true）\nスプライトではコントロールポイントが左上にある
		ModelDatas* CreateSquareModel(float width, float height, Texture* texture, bool isCenter = true);
	};

}
