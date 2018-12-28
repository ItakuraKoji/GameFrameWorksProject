#pragma once

#include "K_Graphics/ModelData.h"
#include"K_Graphics/ShaderClass.h"
#include"K_Graphics/LIGHT.H"
#include"K_Graphics/FrameBufferList.h"
#include"K_Graphics/CameraClass.h"
#include<unordered_set>

namespace K_Graphics {
	class DrawableObject;

	//null�ł��炩���߂Ƃ��Ă������̈�̋󂫂�T���ēo�^���郊�X�g�B����Ȃ�������ǉ�
	//�^�w�肵�����̂̃|�C���^�[�ŊǗ�����
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

		//���X�g�̋󂫂ɓo�^���A����ID�����炤
		int EntryValue(T* value) {
			int index = FindIndex();
			this->vectorList[index] = value;
			return index;
		}

		//���X�g�̃T�C�Y���l��
		int GetSize() {
			return this->size;
		}

		//�w�肵��ID�̗v�f���l��
		T* const Get(int index) {
			if (index < 0 || index >= this->size) {
				return nullptr;
			}
			return this->vectorList[index];
		}

		//�w�肵��ID�̗v�f��null�ɂ��ď��O�A�߂�l�ŗv�f�̃|�C���^��Ԃ�
		T* const PopValue(int index) {
			T* result = Get(index);
			this->vectorList[index] = nullptr;
			return result;
		}


		//�w�肳�ꂽID�̎��ȍ~����L���Ȓl��Ԃ�
		int GetNextIndex(int index) const{
			//�����Ȓl
			if (index + 1 < 0 || index + 1 >= this->size) {
				return -1;
			}
			for (int i = index + 1; i < this->size; ++i) {
				//�L���ȗv�f���l�����ꂽ
				if (this->vectorList[i] != nullptr) {
					return i;
				}
			}
			//�L���l�͑��݂��Ă��Ȃ�
			return -1;
		}

		int GetFirstIndex() const {
			return GetNextIndex(-1);
		}

		void Clear() {
			//�S��null�ɂ���
			for (int i = 0; i < this->size; ++i) {
				this->vectorList[i] = nullptr;
			}
		}

		bool IsEnable(int index) {
			return Get(index) == nullptr;
		}


	private:
		int FindIndex() {
			//null������
			auto itr = std::find(this->vectorList.begin(), this->vectorList.end(), nullptr);
			size_t index = std::distance(this->vectorList.begin(), itr);
			if (index != this->vectorList.size()) {
				// �����ł����Ƃ�
				return (int)index;
			}
			// �����ł��Ȃ������Ƃ�
			this->vectorList.push_back(nullptr);
			++this->size;
			return this->size - 1;
		}

	private:
		std::vector<T*> vectorList;
		int size;
	};





	//�`���񕪂̐ݒ���l�ߍ��񂾃N���X�A��ɓo�^���ꂽ�������Ƃɂ����`�揈���̗�����J�v�Z��������
	//�Ǝ��̃f�[�^��n�����@�́A�p���悪��`���āA�Ȃ�Ƃ��O���������n�����Ƃ��銴��
	class DrawPass {
	public:
		struct DrawData {
			//�`�掞�̏��
			std::string renderTargetName;
			CameraClass* useCamera;
			ShaderClass* useShader;
			AmbientLight* ambientLight;
			DirectionalLight* directionalLight;
		};

	public:
		DrawPass(const DrawData& drawData);
		virtual ~DrawPass();

		//�R�s�[�֎~
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
		//ID�ł̃A�N�Z�X�\
		IndexVector<DrawableObject> drawObjects;

		//�`�掞�̏��
		DrawData drawData;
	};



	//�`��p�X���܂Ƃ߂ĊǗ�����A�����ɓo�^���Ă��낢��g��
	//���Ԃ́A���͓o�^��
	class DrawPassManager {
	public:
		DrawPassManager();
		virtual ~DrawPassManager();

		//�R�s�[�֎~
		DrawPassManager(const DrawPassManager&) = delete;
		void operator = (const DrawPassManager&) = delete;

		//�S�Ẵp�X��`��
		void Draw();

		//�`��̏��Ԃ͓o�^��
		void AddPass(DrawPass* pass);

		//�p�X�̓o�^��S����
		void DeleteAllPass();

		//�p�X�͓o�^����Ă��邩
		bool IsEnablePass(int drawPassID);

		//�`��p�X�ɃI�u�W�F�N�g��ǉ�
		int JoinDrawableObject(int drawPassID, DrawableObject* obj);

		//�`��p�X����I�u�W�F�N�g������
		void RemoveDrawableObject(int drawPassID, int objectHandle);


	private:
		void FinalizeList(DrawPass* pass, int passID);

	private:
		std::vector<DrawPass*> drawPass;
	};


}



