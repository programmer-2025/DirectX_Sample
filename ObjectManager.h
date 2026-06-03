#pragma once
#include <vector>
#include "BaseObject.h"

namespace {
    std::vector<BaseObject*> objList;
}

namespace ObjectManager {

    void InitManager();
    void AddObject(BaseObject* obj);
    void RemoveObject(BaseObject* obj);
    void ClearObject();
    void UpdateManager();

    template<class C> C* GetDrawObject() {
        for (BaseObject* obj : objList) {
            if (obj == nullptr) continue;
            C* instance = dynamic_cast<C*>(obj);
            if (instance != nullptr) {
                return instance;
            }
        }
        return nullptr;
    }
}