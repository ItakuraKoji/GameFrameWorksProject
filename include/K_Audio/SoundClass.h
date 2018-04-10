#pragma once

#include"K_DLL_DEFINE.h"
#include<AL\al.h>
#include<AL\alc.h>
#include<unordered_map>
#include"SoundSource.h"

namespace K_Audio {

	///@brief OpenALをラッピングしたクラス
	class DLL_DECLSPEC SoundClass {
	public:
		///@brief OpenALの初期化
		SoundClass();
		///@brief サウンドソースを全て破棄
		~SoundClass();

		///@brief サウンドソース作成
		///@param[in] sourceName サウンドソースのユーザー定義名
		///@param[in] filePass サウンドのファイルパス（OGG と WAVE のみ）
		///@return 成功するとtrue
		bool CreateSource(const char* sourceName, const char* filePass, SoundSource::LoadMode mode);
		///@brief サウンドソースを削除
		///@param[in] 削除するサウンドソースの名前
		void DeleteSource(const char* sourceName);

		///@param[in] サウンドソースの名前
		///@return サウンドソースへのポインタ
		SoundSource* GetSource(const char* sourceName);

	private:
		ALCdevice * device;
		ALCcontext* context;

		std::unordered_map<std::string, SoundSource*> source;
	};
}
