#pragma once
#include <string>

class SceneBase {
private:
    std::string name_;
public:
    SceneBase(std::string name);
    virtual void Update() = 0;
    virtual void Draw() = 0;
    virtual void Init() = 0;
    virtual void Release() = 0;

    std::string GetName();
};