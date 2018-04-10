#pragma once

#include<unordered_map>
#include<string>
#include"CameraClass.h"

namespace K_Graphics {

	///@brief �J�����ɖ��O��t���ĊǗ�����N���X
	class CameraList {
	public:
		///@brief Initialize()���Ă�
		CameraList();
		///@brief Initialize()���Ă�
		~CameraList();
		///@brief ���X�g���̃J�����N���X��S�J�����Ă���
		void Initialize();

		///@brief �ˉe���e�̃J������ǉ�����A�ˉe���e�����s���e���͌�ŕύX�ł���\n�������O�͓o�^�ł��Ȃ�
		///@param[in] name �o�^����J�����̃��[�U�[��`��
		///@param[in] position �J�����ʒu
		///@param[in] target �����_�ʒu
		///@param[in] screenWidth �r���[�|�[�g�̕��i�A�X�y�N�g����o���̂Ɏg�p����j
		///@param[in] screenHeight �r���[�|�[�g�̍����i�A�X�y�N�g����o���̂Ɏg�p����j
		///@param[in] cameraNeer �N���b�s���O���ʂ̍ŋߖ�
		///@param[in] cameraFar �N���b�s���O���ʂ̍ŉ���
		///@param[in] fieldOfView �c�̎���p
		void AddPerspectiveCamera(const std::string& name, const K_Math::Vector3& position, const K_Math::Vector3& target, int screenWidth, int screenHeight, float cameraNeer, float cameraFar, float fieldOfView);

		///@brief ���s���e�̃J������ǉ�����A�ˉe���e�����s���e���͌�ŕύX�ł���\n�������O�͓o�^�ł��Ȃ�
		///@param[in] name �o�^����J�����̃��[�U�[��`��
		///@param[in] position �J�����ʒu
		///@param[in] target �����_�ʒu
		///@param[in] screenWidth �r���[�|�[�g�̕��i�A�X�y�N�g����o���̂Ɏg�p����j
		///@param[in] screenHeight �r���[�|�[�g�̍����i�A�X�y�N�g����o���̂Ɏg�p����j
		///@param[in] cameraNeer �N���b�s���O���ʂ̍ŋߖ�
		///@param[in] cameraFar �N���b�s���O���ʂ̍ŉ���
		void AddOrthoCamera(const std::string& name, const K_Math::Vector3& position, const K_Math::Vector3& target, int screenWidth, int screenHeight, float cameraNeer, float cameraFar);
		///@brief ���O����J�������擾����
		///@param[in] name �J�����̃��[�U�[��`��
		///@return ���̖��O�̃J�����ւ̃|�C���^�A���s�����ꍇ��nullptr
		CameraClass* GetCamera(const std::string& name);

	private:
		std::unordered_map<std::string, CameraClass*> cameraList;
	};
}
