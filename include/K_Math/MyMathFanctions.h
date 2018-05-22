#pragma once

#include<glm/glm.hpp>
#include<glm/gtc/quaternion.hpp>
#include<glm/gtx/quaternion.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#define _USE_MATH_DEFINES
#include<math.h>

//Eigen�͂����Ԃ񕡎G�ȃw�b�_�̂悤�ŁA�����͍ŏ��̂ق��ɃC���N���[�h����ق����������ۂ�

namespace K_Math {
	//!float�^�Q�v�f�x�N�g��\n�v�f�̃A�N�Z�X�� "vector.x" �Ɗ֐��̌`���Ƃ�̂ɒ���
	typedef glm::vec2 Vector2;
	//!float�^�R�v�f�x�N�g��\n�v�f�̃A�N�Z�X�� "vector.x" �Ɗ֐��̌`���Ƃ�̂ɒ���
	typedef glm::vec3 Vector3;
	//!float�^�S�v�f�x�N�g��\n�v�f�̃A�N�Z�X�� "vector.x" �Ɗ֐��̌`���Ƃ�̂ɒ���
	typedef glm::vec4 Vector4;
	//!float�^�R�s�R��s��
	typedef glm::mat3x3 Matrix3x3;
	//!float�^�S�s�S��s��
	typedef glm::mat4x4 Matrix4x4;
	//!float�^�̃N�H�[�^�j�I��\n�����ł͉�]�s��̓N�H�[�^�j�I���𗘗p���Ă���
	typedef glm::quat Quaternion;

	//�A�t�B���ϊ��p
	//typedef Eigen::Transform<float, 3, 2, Eigen::DontAlign> Affine3;
	//typedef Eigen::AngleAxis<float> AngleAxis;
	//typedef Eigen::Translation<float, 3> Translation;
	//typedef Eigen::DiagonalMatrix<float, 3> DiagonalMatrix;

	//!@brief ���W��"x""y"�A��������"w""h"������\n�S��int�^�ŁA2D�`��p
	struct Box2D {
	public:
		Box2D();
		Box2D(int x, int y, int w, int h);
		Box2D& operator =(Box2D& box);
	public:
		int x, y, w, h;
	};

	//!@brief �u�x���@�@���@���W�A���v�̕ϊ�
	//!@param[in] deg �x���@�ł̊p�x
	//!@return ���W�A���ł̊p�x
	float DegToRad(float deg);
	//!@brief �u���W�A���@���@�x���@�v�̕ϊ�
	//!@param[in] rad ���W�A���ł̊p�x
	//!@return �x���@�ł̊p�x
	float RadToDeg(float rad);
	//!@brief ������W�n�̎ˉe�ϊ��s������
	//!@param[out] result ���ʂ�4x4�s����i�[����ϐ�
	//!@param[in] screenWidth �`��X�N���[���̕�\n�E�B���h�E�ł͂Ȃ��A�r���[�|�[�g�̕�������
	//!@param[in] screenWidth �`��X�N���[���̍���\n�E�B���h�E�ł͂Ȃ��A�r���[�|�[�g�̍���������
	//!@param[in] screenNear �߂��ق��́A������̃N���b�s���O�̈�
	//!@param[in] screenFar �����ق��́A������̃N���b�s���O�̈�
	//!@param[in] fieldOfView ��ʂ̏c�ɑ΂��鎋��p
	void  MatrixPerspectiveLH(Matrix4x4& result, float screenWidth, float screenHeight, float screenNear, float screenFar, float fieldOfView);
	//!@brief �E����W�n�̎ˉe�ϊ��s������\n�ڍׂ�MatrixPerspectiveLH()���Q�Ƃ̂���
	void  MatrixPerspectiveRH(Matrix4x4& result, float screenWidth, float screenHeight, float screenNear, float screenFar, float fieldOfView);
	//!@brief ������W�n�̕��s���e�s������
	//!@param[out] result ���ʂ�4x4�s����i�[����ϐ�
	//!@param[in] screenWidth �`��X�N���[���̕�\n�E�B���h�E�ł͂Ȃ��A�r���[�|�[�g�̕�������
	//!@param[in] screenWidth �`��X�N���[���̍���\n�E�B���h�E�ł͂Ȃ��A�r���[�|�[�g�̍���������
	//!@param[in] screenNear �߂��ق��́A������̃N���b�s���O�̈�
	//!@param[in] screenFar �����ق��́A������̃N���b�s���O�̈�
	void  MatrixOrthoLH(Matrix4x4& result, float screenWidth, float screenHeight, float screenNear, float screenFar);
	//!@brief �E����W�n�̕��s���e�s������\n�ڍׂ�MatrixOrthoLH()���Q�Ƃ̂���
	void  MatrixOrthoRH(Matrix4x4& result, float screenWidth, float screenHeight, float screenNear, float screenFar);
	//!@brief �r���[�s������
	//!@param[out] result ���ʂ�4x4�s����i�[����ϐ�
	//!@param[in] position �J�����̍��W
	//!@param[in] lookAt �����_�̍��W
	//!@param[in] up �J�����̏���w���P�ʃx�N�g��
	void  MatrixLookAt(Matrix4x4& result, Vector3& position, Vector3& lookAt, Vector3& up);

	//!@brief ��]��������N�H�[�^�j�I�����쐬����(Y����X����Z��)
	Quaternion RotationToQuaternion(const Vector3& rotation);
	//!@brief �N�H�[�^�j�I�������]�������쐬����(Y����X����Z��)
	Vector3 QuaternionToRotation(const Quaternion& quaternion);

	//!@brief
	float Norm(const Vector2& vector);
	float Norm(const Vector3& vector);
	float Norm(const Vector4& vector);

	//!@brief
	float Cross(const Vector2& vector1, const Vector2& vector2);
	Vector3 Cross(const Vector3& vector1, const Vector3& vector2);

	//!@brief
	float Dot(const Vector2& vector1, const Vector2& vector2);
	float Dot(const Vector3& vector1, const Vector3& vector2);
}
