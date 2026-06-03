#include "BaseObject.h"
#include "ObjectManager.h"

BaseObject::BaseObject(std::string name, bool isAlive) {
	name_ = name;
	isAlive_ = isAlive;
	ObjectManager::AddObject(this);
}
