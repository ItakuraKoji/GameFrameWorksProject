#pragma once
#include"FontGenerator.h"
#include"MeshModel.h"
#include"K_Math\MyMathFanctions.h"
#include<map>

namespace K_Graphics {

	///@brief �t�H���g�𐶐����A�`�悷��N���X(���C�h������)
	class FontRenderer {
	private:
		struct FontDrawData {
			wchar_t* buffer;//�o�͂��镶����
			int fontSize;//�T�C�Y
			float posX;//X���W
			float posY;//Y���W
			float posZ;//Z���W�i3D�`��ɗ��p�j
		};

	public:

		///@brief �`��Ɏg�p����X�v���C�g��������
		FontRenderer();
		///@brief �X�v���C�g�̃|�C���^���J��
		~FontRenderer();

		///@brief ���O�ɌĂ΂ꂽ�`�施�߂������ōs��
		///@param[in] camera �g�p����J����
		///@param[in] shader �g�p����V�F�[�_�[�i�X�v���C�g���`��ł�����́j
		void Draw(CameraClass* camera, ShaderClass* shader);

		///@brief �t�H���g��ǂݍ���
		///@param[in] fontName �t�H���g�̃��[�U�[��`��
		///@param[in] filePass �t�H���g�̃t�@�C���p�X
		bool LoadFont(const char* fontName, const char* filePass);
		///@brief �g�p����t�H���g��ݒ�A���s�����false��Ԃ��A�Z�b�g����Ȃ�
		///@param[in] fontName �t�H���g��
		///@return ���������true
		bool SetCullentFont(const char* fontName);

		///@brief �w����W����e�L�X�g�̕`��\��i�ʒu�̓X�N���[���̍��W�j
		///@param[in] text �`�悷��e�L�X�g
		///@param[in] fontSize �t�H���g�̃T�C�Y
		///@param[in] posX �`����W
		///@param[in] posY �`����W
		void DrawString2D(const std::string& text, int fontSize, int posX, int posY);
		///@brief �w����W����e�L�X�g�̕`��\��i�ʒu��3D��Ԃ̍��W�j
		///@param[in] text �`�悷��e�L�X�g
		///@param[in] fontSize �t�H���g�̃T�C�Y
		///@param[in] posX �`����W
		///@param[in] posY �`����W
		///@param[in] posZ �`����W
		void DrawString3D(const std::string& text, int fontSize, float posX, float posY, float posZ);

		///@brief �t�H���g�`��ɗ��p���Ă���e�N�X�`�����폜����
		///@param[in] size �폜����t�H���g�̃T�C�Y
		void ClearFontTexture(int size);
		///@brief �t�H���g�`��ɗ��p���Ă���e�N�X�`�������ׂč폜����
		void ClearAllFontTexture();

	private:
		bool CreateFontBuffer(wchar_t* result, const char* src, int numChar);
		void DrawBuffers3D(FontDrawData& data, CameraClass* camera, ShaderClass* shader);

	private:
		FontGenerator * cullentFont;
		std::unordered_map<std::string, FontGenerator*> generators;
		K_Graphics::SpriteObject* sprite;

		//�Ă΂ꂽ�`�施�߂ɕK�v�ȏ����i�[���Ă����āADraw()�̎��Ɏg�p
		std::vector<FontDrawData> drawData2D;
		std::vector<FontDrawData> drawData3D;
	};

}
