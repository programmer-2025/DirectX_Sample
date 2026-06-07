#include "ObjectManager.h"

void ObjectManager::InitManager() {
    for (int n = 0; n < objList.size(); n++) {
        BaseObject* obj = objList[n];
        if (obj == nullptr) continue;
        obj->Init();
    }
}

void ObjectManager::AddObject(BaseObject* obj) {
    objList.push_back(obj);
}

void ObjectManager::RemoveObject(BaseObject* obj) {
    for (auto it = objList.begin(); it != objList.end(); ) {
        if ((*it) == obj) {
            delete* it;
            it = objList.erase(it);
        }
        else {
            it++;
        }
    }
}

void ObjectManager::ClearObject() {
    objList.clear();
}

void ObjectManager::UpdateManager() {
    for (auto& obj : objList) {
        if (obj == nullptr) continue;
        if (obj->IsAlive()) {
            obj->Update();
            if (!obj->IsShowing()) return;
            obj->Draw();
        }
        else {
            RemoveObject(obj);
        }
    }
}
