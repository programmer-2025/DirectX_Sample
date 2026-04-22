#include "SceneBase.h"

SceneBase::SceneBase(std::string name) {
    name_ = name;
}

std::string SceneBase::GetName() {
    return name_;
}