#pragma once

#include <set>

using Entity = int;

class EntityManager {
public:
	static EntityManager* get();

	Entity create();

	void discard(Entity entity);
private:
	EntityManager() = default;

	Entity _next{ 0 };

	std::set<Entity> _discardedEntities{};

	inline static EntityManager* _instance{ nullptr };
};

class EntityResource {
public:
	explicit EntityResource(const Entity entity) : _entity{ entity } {}

	[[nodiscard]] Entity getEntity() const;

private:
	Entity _entity;
};