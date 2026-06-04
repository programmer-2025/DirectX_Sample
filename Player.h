#pragma once
#include "BaseObject.h"

class Player : public BaseObject {
private:
public:
	Player();
	~Player() {};
	void Init() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};
