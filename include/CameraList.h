#pragma once

#include<unordered_map>
#include<string>
#include"CameraClass.h"

namespace K_Graphics {

	//ƒJƒƒ‰‚É–¼‘O‚ğ•t‚¯‚ÄŠÇ—
	class CameraList {
	public:
		CameraList();
		~CameraList();
		void Initialize();

		void AddPerspectiveCamera(const std::string& name, const K_Math::Vector3& position, const K_Math::Vector3& target, int width, int height, float neer, float far, float fieldOfView);
		void AddOrthoCamera(const std::string& name, const K_Math::Vector3& position, const K_Math::Vector3& target, int width, int height, float neer, float far);
		CameraClass* GetCamera(const std::string& name);

	private:
		std::unordered_map<std::string, CameraClass*> cameraList;
	};
}
