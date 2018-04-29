#pragma once
#define GLEW_STATIC

#include<chrono>

#include<GLEW/glew.h>
#include<GLFW/glfw3.h>

#include"K_Math\MyMathFanctions.h"
#include"K_Input\InputClass.h"

//���t�@�����X�p
///@brief �O���t�B�b�N�֘A�̖��O���\n
///@details�E��v�N���X\n
///3D���f���̕\���@:�uMeshObject�v�uSpriteObject�v\n
///�J�����@�@�@�@�@:�uCameraList�v���Ǘ�����uCameraClass�v\n
///�����@�@�@�@�@�@:�uLightList�v\n
///�摜�@�@�@�@�@�@:�uTextureList�v���Ǘ�����uTexture�v\n
///�V�F�[�_�[�@�@�@:�uShaderList�v���Ǘ�����uShaderClass�v\n
///�`��o�b�t�@�@�@:�uFrameBufferList�v\n
namespace K_Graphics {}
///@brief �����Փ˔���֘A�̖��O���
///@details�E��v�N���X\n
///�R���W�����쐬�E�Փ˔���@�F�uBulletPhysics�v
///�n�`�f�[�^�쐬�@�@�@�@�@�@�F�uMapPolygon�v
namespace K_Physics {}
///@brief �����֘A�̖��O���
///@details�E��v�N���X\n
///�����̍쐬�E���X�i�[�̈ړ��@�F�uSoundClass�v
///�����̍Đ��E�ړ��@�@�@�@�@�@�F�uSoundSource�v
namespace K_Audio {}
///@brief ���͊֘A�̖��O���
///@details�E��v�N���X
///���͂̎擾�uInputGLFW�v
namespace K_Input {}
///@brief �s��n�̊֐��ƃx�N�g����s��^��񋟂���
///@details��v�N���X�͂Ȃ��A�֐��ƌ^��������`����Ă���
namespace K_Math {}

///@brief ���SystemClass���g���A�A�v���P�[�V�����̏��������s��
namespace K_System {

	///
	///�A�v���P�[�V�����̓�����Ǘ�
	///@brief ��ԏ��߂ɏ��������AIsSystemEnd()��true�ɂȂ�܂ł̊ԃA�v���P�[�V�����𓮂���\n�Ȃ��A�u�L�[���́E�Q�[���p�b�h���́v�̏��͂��̃N���X�ɂ���
	///
	class SystemClass {
	public:
		///Initialize()���Ă�
		SystemClass(int windowWidth, int windowHeight, bool isFullScreen);
		///Finalize()���Ă�
		~SystemClass();
		///@brief �`�����̏�����
		///@param[in] windowWidth �쐬����E�B���h�E�̕�
		///@param[in] windowHeight �쐬����E�B���h�E�̍���
		///@param[in] isFullScreen �t���X�N���[���ɂ��邩�i�E�B���h�E�̃T�C�Y���𑜓x�ɂȂ�܂��j
		///@return �������ɐ��������true
		bool Initialize(int windowWidth, int windowHeight, bool isFullScreen);
		///@brief �I�������AInitialize()�̏��߂ɂ��Ă΂�Ă���
		void Finalize();
		///@brief �E�B���h�E�C�x���g�Ɠ��͂̍X�V�������s��
		void ProcessSystem();
		///@brief �`��𔽉f�A������ĂԂƃt���[�����[�g��60FPS�ɂȂ�
		void SwapBuffer();
		///@brief �E�B���h�E���t�H�[�J�X����Ă��邩
		///@return �E�B���h�E���t�H�[�J�X����Ă���Ƃ�true
		bool WindowForcus();
		///@brief �A�v���̏I���ʒm
		///@return �E�B���h�E������ꂽ��AEsc�L�[���������ꂽ�Ƃ���true
		bool IsSystemEnd();
		///@brief �A�N�Z�T�[
		///@return �E�B���h�E�̕�
		int GetWindowWidth();
		///@brief �A�N�Z�T�[
		///@return �E�B���h�E�̍���
		int GetWindowHeight();
		///@brief �A�N�Z�T�[
		///@return FramePerSecond�̒l
		float GetFPS();

		///@brief �A�N�Z�T�[
		///@return ���̓N���X�ւ̃|�C���^
		K_Input::InputClass* GetInput();

	private:
		bool CreateAppricationWindow(const char* windowName, int width, int height, bool fullScreen);
		bool InitializeOpenGL();

	private:
		int windowWidth, windowHeight;
		bool isFullScreen;
		bool isFocus;
		bool windowClosed;
		GLFWwindow* windowHandle;
		K_Input::InputClass* input;

		//FPS�v��
		float fps;
		std::chrono::system_clock::time_point cullentTime;
		std::chrono::system_clock::time_point startTime;
		int framecount;
	};
}
