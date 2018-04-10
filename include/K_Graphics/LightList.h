#pragma once

#include<unordered_map>
#include"Light.h"
#include"K_Math\MyMathFanctions.h"

namespace K_Graphics {

	///@brief ���������܂Ƃ߂��N���X�A���������̓R�X�g�������̂Ő����i��
	///�A���r�G���g�F�P��
	///���̑��F�S�i�܂��\��j
	class LightList {
	public:
		///@brief Initialize()���Ă�
		LightList();
		///@brief Initialize()���Ă�
		~LightList();
		///@brief ���X�g������
		void Initialize();

		///@brief �V�F�[�_�[�ɃA���r�G���g���C�g����n��
		///@param[in] lightName �A���r�G���g���C�g�̖��O
		///@param[in] shader ����n���V�F�[�_�[�ւ̃|�C���^
		void SetAmbient(const std::string& lightName, ShaderClass* shader);
		///@brief �V�F�[�_�[�Ƀf�B���N�V���i�����C�g����n��
		///@param[in] lightName �f�B���N�V���i�����C�g�̖��O
		///@param[in] shader ����n���V�F�[�_�[�ւ̃|�C���^
		void SetDirectional(const std::string& lightName, ShaderClass* shader);

		///@brief �V�����A���r�G���g���C�g��ǉ�
		///@param[in] lightName �A���r�G���g���C�g�̃��[�U�[��`��
		///@param[in] power ���̋���
		///@param[in] color ���̐F
		void AddAmbient(const std::string& lightName, float power, const K_Math::Vector4& color);
		///@brief �V�����f�B���N�V���i�����C�g��ǉ�
		///@param[in] lightName �f�B���N�V���i�����C�g�̃��[�U�[��`��
		///@param[in] power ���̋���
		///@param[in] color ���̐F
		///@param[in] direction ���̕���
		void AddDirectional(const std::string& lightName, float power, const K_Math::Vector4& color, const K_Math::Vector3& direction);

	private:
		std::unordered_map<std::string, AmbientLight> ambient;
		std::unordered_map<std::string, DirectionalLight> directional;
	};
}
