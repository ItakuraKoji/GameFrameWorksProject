#pragma once
#include<string>
#include"ModelData.h"
#include"TextureList.h"
#include"FbxModelLoader.h"

namespace K_Graphics {

	//!@brief モデルクラスの初期化に必要なパラメーターの製作を担当するクラス
	class ModelDataFactory {
	public:
		//!@brief FBX形式の3Dモデルを読み込む
		//!@param[in] fileName FBXモデルのファイルパス
		//!@param[in] textureList テクスチャリストへのポインタ（FBXで使われているテクスチャを読み込むのに使う）
		//!@return モデルのデータクラスへのポインタ
		ModelDatas* LoadFBXModel(const std::string& fileName, TextureList* textureList);
		//!モデルデータのリソース作成（返すポインタの開放責任がある）
		ModelResource* CreateModelResourceFromFBX(const std::string& fileName, TextureList* textureList);
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
