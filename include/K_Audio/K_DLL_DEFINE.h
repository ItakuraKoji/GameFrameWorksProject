#pragma once

//DLLファイルのインポートエクスポート設定
#ifdef DLL_EXP
#define DLL_DECLSPEC __declspec(dllexport)
#else
#define DLL_DECLSPEC __declspec(dllimport)
#endif //DLL_EXP