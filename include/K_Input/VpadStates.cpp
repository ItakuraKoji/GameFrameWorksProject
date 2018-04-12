#include"VpadStates.h"

namespace K_Input {

	StickState::StickState() {
		this->x = nullptr;
		this->y = nullptr;
	}

	void StickState::SetAxis(float* xAxisPos, float* yAxisPos) {
		this->x = xAxisPos;
		this->y = yAxisPos;
	}

	K_Math::Vector2 StickState::GetPosition() {
		if (this->x == nullptr || this->y == nullptr) {
			return K_Math::Vector2(0, 0);
		}
		return K_Math::Vector2(*this->x, *this->y);
	}

	float StickState::GetRotation() {
		if (this->x == nullptr || this->y == nullptr) {
			return 0.0f;
		}
		return atan2(*this->y, *this->x);
	}

	float StickState::GetPower() {
		if (this->x == nullptr || this->y == nullptr) {
			return 0.0f;
		}
		return sqrtf(powf(*this->x, 2) + powf(*this->y, 2));
	}
}
