#pragma once

#include<unordered_map>
#include"ShaderClass.h"

namespace K_Graphics {
	///@brief �V�F�[�_�[�N���X�����X�g�Ƃ��Ď����A����ӔC�����N���X
	class ShaderList {
	public:
		///@brief Initialize()���Ă�
		ShaderList();
		///@brief Initialize()���Ă�
		~ShaderList();
		///@brief ���X�g�̃V�F�[�_�[��S�ĊJ������
		void Initialize();

		///@brief �V�F�[�_�[�ւ̃|�C���^���擾
		///@param[in] shaderName �V�F�[�_�[�̖��O
		///@return ���O�ɑΉ������V�F�[�_�[
		ShaderClass* GetShader(const std::string& shaderName);
		///@brief �V�F�[�_�[��`��p�ɐݒ肵�A���̃V�F�[�_�[�ւ̃|�C���^��Ԃ�
		///@param[in] shaderName �V�F�[�_�[�̖��O
		///@return ���O�ɑΉ������V�F�[�_�[
		ShaderClass* UseShader(const std::string& shaderName);

		///@brief ���_�V�F�[�_�[��ǂݍ��݁A���s�����string�^�̃G���[���b�Z�[�W���O�ő��o����
		///@param[in] fileName �V�F�[�_�[�̃p�X
		void LoadVertexShader(const std::string& fileName);
		///@brief �t���O�����g�V�F�[�_�[��ǂݍ��݁A���s�����string�^�̃G���[���b�Z�[�W���O�ő��o����
		///@param[in] filename �V�F�[�_�[�̃p�X
		void LoadFragmentShader(const std::string& fileName);

		///@brief ���_�V�F�[�_�[�ƃt���O�����g�V�F�[�_�[��g�ݍ��킹�ăV�F�[�_�[���쐬
		///���ƂȂ�V�F�[�_�[���Ȃ��ꍇ��string�^�̃G���[���b�Z�[�W���O�ő��o����
		///@param[in] shaderName �쐬����V�F�[�_�[�̃��[�U�[��`��
		///@param[in] vertexShader ���ƂȂ钸�_�V�F�[�_�[�ւ̃p�X�iLoadVertexShader()�̎��Ƀp�X�ŃV�F�[�_�[�����o�^����Ă���j
		///@param[in] vertexShader ���ƂȂ�t���O�����g�V�F�[�_�[�ւ̃p�X�iLoadFragmentShader()�̎��Ƀp�X�ŃV�F�[�_�[�����o�^����Ă���j
		void CreateShaderProgram(const std::string& shaderName, const std::string& vertexShader, const std::string& fragmentShader);

	private:
		char* LoadTxtResource(const std::string& fileName);
		void ShowShaderErrors(GLuint shaderID);

	private:
		std::unordered_map<std::string, ShaderClass*> shaderList;
		std::unordered_map<std::string, GLuint> vertexShaders;
		std::unordered_map<std::string, GLuint> pixelShaders;

	};
}
