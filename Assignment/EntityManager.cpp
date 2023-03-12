#include <stdexcept>
#include <format>

#include "EntityManager.h"

EntityManager* EntityManager::get() {
	if (!_instance) {
		_instance = new EntityManager{};
	}
	return _instance;
}


Entity EntityManager::create() {
	Entity id;
	do {
		id = _next++;
	} while (_discardedEntities.contains(id));
	return id;
}

void EntityManager::discard(const Entity entity) {
	if (entity >= _next) {
		throw std::invalid_argument(std::format("Entity {} is invalid.\n", entity));
	}
	_discardedEntities.insert(entity);
}

Entity EntityResource::getEntity() const {
	return _entity;
}
