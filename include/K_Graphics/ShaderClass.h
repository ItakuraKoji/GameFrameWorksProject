#pragma once
#include<fstream>
#include<iostream>
#include<GLEW\glew.h>
#include"K_Math\MyMathFanctions.h"

namespace K_Graphics {

	//!@brief �V�F�[�_�[�v���O�����̊Ǘ���uniform�ϐ��̎󂯓n����S������N���X
	//!uniform�ϐ����͌Œ�I�ȋ@�\�������ƁA�֐����ɃL�����Ȃ��̂Ŕėp�I�Ȋ֐���p��
	class ShaderClass {
	public:
		//!@brief Initialize()���Ă�
		ShaderClass(GLuint vertexShader, GLuint fragmentShader);
		//!@brief Finalize()���Ă�
		~ShaderClass();

		//!@brief GLSL�V�F�[�_�[�v���O�������쐬
		//!@param[in] vertexShader ���_�V�F�[�_�[�̃C���f�b�N�X
		//!@param[in] fragmentShader �t���O�����g�V�F�[�_�[�̃C���f�b�N�X
		//!@return ���������true
		bool Initialize(GLuint vertexShader, GLuint fragmentShader);
		//!@brief �V�F�[�_�[���J������
		void Finalize();

		//!@brief �V�F�[�_�[��`��p�ɐݒ�
		void UseShader();
		//!@brief �V�F�[�_�[�� ���[���h x �r���[ x �v���W�F�N�V���� �̍s���n��
		//!@param[in] mat �n���ϊ��s��
		void SetMatrix(const K_Math::Matrix4x4& mat);
		//!@brief �V�F�[�_�[�Ƀ��[���h�s�񂾂���n��
		//!@param[in] mat �n���ϊ��s��
		void SetWorldMatrix(const K_Math::Matrix4x4& world);
		//!@brief �V�F�[�_�[�Ƀe�N�X�`����n���A���f���`�掞�Ɉȉ��̔ԍ����g���邱�Ƃ�����
		//!���C���[�O�ԁF���f���̃}�e���A���e�N�X�`��
		//!���C���[�P�ԁF���f���̃{�[���s��e�N�X�`��
		//!@param[in] uniformName GLSL�̃e�N�X�`�����w��Uniform�ϐ��̖��O
		//!@param[in] textureLayer �e�N�X�`���̃��C���[�A�������̃e�N�X�`����n���Ƃ��͐�����ς���
		//!@param[in] textureNumber �e�N�X�`���̃C���f�b�N�X
		void SetTexture(const std::string& uniformName, GLuint textureLayer, GLuint textureNumber);
		//!@brief �f�B���N�V���i�����C�g����n��
		//!@param[in] power ���̋���
		//!@param[in] color ���̐F
		//!@param[in] direction ���̌���
		void SetDirectionalLight(float power, const K_Math::Vector4& color, const K_Math::Vector3& direction);
		//!@brief �A���r�G���g���C�g����n��
		//!@param[in] power ���̋���
		//!@param[in] color ���̐F
		void SetAmbientLight(float power, const K_Math::Vector4& color);

		//!@brief �ėp�I��uniform�ϐ��ւ̎󂯓n��
		//!@param[in] uniformName GLSL�̃e�N�X�`�����w��Uniform�ϐ��̖��O
		//!@param[in] value �n���l
		void SetValue(const std::string& uniformName, int value);
		//!@brief �ėp�I��uniform�ϐ��ւ̎󂯓n��
		//!@param[in] uniformName GLSL�̃e�N�X�`�����w��Uniform�ϐ��̖��O
		//!@param[in] value �n���l
		void SetValue(const std::string& uniformName, float value);
		//!@brief �ėp�I��uniform�ϐ��ւ̎󂯓n��
		//!@param[in] uniformName GLSL�̃e�N�X�`�����w��Uniform�ϐ��̖��O
		//!@param[in] value �n���l
		void SetValue(const std::string& uniformName, const K_Math::Matrix4x4& value);
		//!@brief �ėp�I��uniform�ϐ��ւ̎󂯓n��
		//!@param[in] uniformName GLSL�̃e�N�X�`�����w��Uniform�ϐ��̖��O
		//!@param[in] value �n���l
		void SetValue(const std::string& uniformName, const K_Math::Vector4& value);
		//!@brief �ėp�I��uniform�ϐ��ւ̎󂯓n��
		//!@param[in] uniformName GLSL�̃e�N�X�`�����w��Uniform�ϐ��̖��O
		//!@param[in] value �n���l
		void SetValue(const std::string& uniformName, const K_Math::Vector3& value);
		//!@brief �ėp�I��uniform�ϐ��ւ̎󂯓n��
		//!@param[in] uniformName GLSL�̃e�N�X�`�����w��Uniform�ϐ��̖��O
		//!@param[in] value �n���l
		void SetValue(const std::string& uniformName, const K_Math::Vector2& value);

		//!@brief �ėp�I��uniform�ϐ��ւ̔z��̎󂯓n��
		//!@param[in] uniformName GLSL�̃e�N�X�`�����w��Uniform�ϐ��̖��O
		//!@param[in] value �n���z��
		//!@param[in] numArray �z��̗v�f��
		void SetValue(const std::string& uniformName, int* value, int numArray);
		//!@brief �ėp�I��uniform�ϐ��ւ̔z��̎󂯓n��
		//!@param[in] uniformName GLSL�̃e�N�X�`�����w��Uniform�ϐ��̖��O
		//!@param[in] value �n���z��
		//!@param[in] numArray �z��̗v�f��
		void SetValue(const std::string& uniformName, float* value, int numArray);
		//!@brief �ėp�I��uniform�ϐ��ւ̔z��̎󂯓n��
		//!@param[in] uniformName GLSL�̃e�N�X�`�����w��Uniform�ϐ��̖��O
		//!@param[in] value �n���z��
		//!@param[in] numArray �z��̗v�f��
		void SetValue(const std::string& uniformName, K_Math::Matrix4x4* value, int numArray);
		//!@brief �ėp�I��uniform�ϐ��ւ̔z��̎󂯓n��
		//!@param[in] uniformName GLSL�̃e�N�X�`�����w��Uniform�ϐ��̖��O
		//!@param[in] value �n���z��
		//!@param[in] numArray �z��̗v�f��
		void SetValue(const std::string& uniformName, K_Math::Vector4* value, int numArray);
		//!@brief �ėp�I��uniform�ϐ��ւ̔z��̎󂯓n��
		//!@param[in] uniformName GLSL�̃e�N�X�`�����w��Uniform�ϐ��̖��O
		//!@param[in] value �n���z��
		//!@param[in] numArray �z��̗v�f��
		void SetValue(const std::string& uniformName, K_Math::Vector3* value, int numArray);
		//!@brief �ėp�I��uniform�ϐ��ւ̔z��̎󂯓n��
		//!@param[in] uniformName GLSL�̃e�N�X�`�����w��Uniform�ϐ��̖��O
		//!@param[in] value �n���z��
		//!@param[in] numArray �z��̗v�f��
		void SetValue(const std::string& uniformName, K_Math::Vector2* value, int numArray);

		//!@brief subroutine�̐ݒ�(vertex shader)�A�w�肵�����O�̊֐����I�������
		//!@param subroutineFunctionName �g�p����T�u���[�`���֐��̖��O
		void SetVertexShaderSubroutine(const std::string& subroutineFunctionName);
		//!@brief subroutine�̐ݒ�(fragment shader)�A�w�肵�����O�̊֐����I�������
		//!@param subroutineFunctionName �g�p����T�u���[�`���֐��̖��O
		void SetFragmentShaderSubroutine(const std::string& subroutineFunctionName);

	private:
		char* LoadTxtResource(const std::string& fileName);
		void ShowShaderErrors(GLuint shaderID);
		void ShowLinkErrors(GLuint shaderID);

		GLuint shaderProgram;
	};
}
