#pragma once

//DLL�t�@�C���̃C���|�[�g�G�N�X�|�[�g�ݒ�
#ifdef DLL_EXP
#define DLL_DECLSPEC __declspec(dllexport)
#else
#define DLL_DECLSPEC __declspec(dllimport)
#endif //DLL_EXP