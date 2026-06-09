#pragma once
#include "BaseObject.h"
class FontText : public BaseObject {
private:
	std::wstring text_;
public:
	FontText(std::wstring text);
	~FontText();
	void Init() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};

