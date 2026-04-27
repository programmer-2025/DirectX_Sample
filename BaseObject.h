#pragma once
#include <string>

class BaseObject {
private:
protected:
    std::string name_;
    bool isAlive_;
public:
    virtual void Init() = 0;
    virtual void Update() = 0;
    virtual void Draw() = 0;
    virtual void Release() = 0;

    BaseObject(std::string name, bool isAlive) {
        name_ = name;
        isAlive_ = isAlive;
    };
    virtual ~BaseObject() {}

    std::string GetName() { return name_; };
    bool IsAlive() { return isAlive_; };
};