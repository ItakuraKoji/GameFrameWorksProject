#pragma once

#include "K_Graphics/ModelData.h"
#include"K_Graphics/ShaderClass.h"
#include"K_Graphics/LIGHT.H"
#include"K_Graphics/FrameBufferList.h"
#include"K_Graphics/CameraClass.h"
#include<unordered_set>

namespace K_Graphics {
	class DrawableObject;

	//nullであらかじめとっておいた領域の空きを探して登録するリスト。足りなかったら追加
	//型指定したもののポインターで管理する
	template<class T>
	class IndexVector {
	public:
		IndexVector(int size) {
			this->vectorList.resize(size);
			this->size = size;
		}
		IndexVector() {
			this->vectorList.resize(0);
			this->size = 0;
		}

		//リストの空きに登録し、そのIDをもらう
		int EntryValue(T* value) {
			int index = FindIndex();
			this->vectorList[index] = value;
			return index;
		}

		//リストのサイズを獲得
		int GetSize() {
			return this->size;
		}

		//指定したIDの要素を獲得
		T* const Get(int index) {
			if (index < 0 || index >= this->size) {
				return nullptr;
			}
			return this->vectorList[index];
		}

		//指定したIDの要素をnullにして除外、戻り値で要素のポインタを返す
		T* const PopValue(int index) {
			T* result = Get(index);
			this->vectorList[index] = nullptr;
			return result;
		}


		//指定されたIDの次以降から有効な値を返す
		int GetNextIndex(int index) const{
			//無効な値
			if (index + 1 < 0 || index + 1 >= this->size) {
				return -1;
			}
			for (int i = index + 1; i < this->size; ++i) {
				//有効な要素が獲得された
				if (this->vectorList[i] != nullptr) {
					return i;
				}
			}
			//有効値は存在していない
			return -1;
		}

		int GetFirstIndex() const {
			return GetNextIndex(-1);
		}

		void Clear() {
			//全部nullにする
			for (int i = 0; i < this->size; ++i) {
				this->vectorList[i] = nullptr;
			}
		}

		bool IsEnable(int index) {
			return Get(index) == nullptr;
		}


	private:
		int FindIndex() {
			//nullを検索
			auto itr = std::find(this->vectorList.begin(), this->vectorList.end(), nullptr);
			size_t index = std::distance(this->vectorList.begin(), itr);
			if (index != this->vectorList.size()) {
				// 発見できたとき
				return (int)index;
			}
			// 発見できなかったとき
			this->vectorList.push_back(nullptr);
			++this->size;
			return this->size - 1;
		}

	private:
		std::vector<T*> vectorList;
		int size;
	};





	//描画一回分の設定を詰め込んだクラス、主に登録された情報をもとにした描画処理の流れをカプセル化する
	//独自のデータを渡す方法は、継承先が定義して、なんとか外部から情報を渡そうとする感じ
	class DrawPass {
	public:
		struct DrawData {
			//描画時の情報
			std::string renderTargetName;
			CameraClass* useCamera;
			ShaderClass* useShader;
			AmbientLight* ambientLight;
			DirectionalLight* directionalLight;
		};

	public:
		DrawPass(const DrawData& drawData);
		virtual ~DrawPass();

		//コピー禁止
		DrawPass(const DrawPass&) = delete;
		void operator = (const DrawPass&) = delete;

		void Draw();
		int JoinDrawableObject(DrawableObject* obj);
		void RemoveDrawableObject(int id);
		IndexVector<DrawableObject>& GetDrawList();

		const DrawData& GetDrawData();

	private:
		virtual void ShaderSettings() = 0;
		virtual void EndDraw() = 0;
		void DrawObject();

	private:
		//IDでのアクセス可能
		IndexVector<DrawableObject> drawObjects;

		//描画時の情報
		DrawData drawData;
	};



	//描画パスをまとめて管理する、ここに登録していろいろ使う
	//順番は、今は登録順
	class DrawPassManager {
	public:
		DrawPassManager();
		virtual ~DrawPassManager();

		//コピー禁止
		DrawPassManager(const DrawPassManager&) = delete;
		void operator = (const DrawPassManager&) = delete;

		//全てのパスを描画
		void Draw();

		//描画の順番は登録順
		void AddPass(DrawPass* pass);

		//パスの登録を全解除
		void DeleteAllPass();

		//パスは登録されているか
		bool IsEnablePass(int drawPassID);

		//描画パスにオブジェクトを追加
		int JoinDrawableObject(int drawPassID, DrawableObject* obj);

		//描画パスからオブジェクトを消す
		void RemoveDrawableObject(int drawPassID, int objectHandle);


	private:
		void FinalizeList(DrawPass* pass, int passID);

	private:
		std::vector<DrawPass*> drawPass;
	};


}



