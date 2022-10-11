/*
 * entity.h
 *
 *  Created on: 29 avr. 2020
 *      Author: silverly
 */

#ifndef WORLD_ENTITY_IENTITY_H_
#define WORLD_ENTITY_IENTITY_H_

#include <mutex>
#include <glm/glm/vec3.hpp>
#include <glm/glm/ext/matrix_float4x4.hpp>
#include "../../utils/location.h"

#define FIXED_TICK_TIME (1/20.0f)
#define FIXED_RENDER_TIME (1/60.0f)

class Entity {
public:
	Entity();
	Entity(Location);
	virtual ~Entity() = default;

	/**
	 * Those are specific to each entity
	 */
	virtual void preload() = 0; // Loading phase by the client worker. No GL rights.
	virtual void load() = 0; // Loading phase by the main thread, just before the 1st render
	virtual void draw(glm::mat4&, const glm::vec3& light_dir, const glm::vec3& view_pos) = 0;
	virtual void unload() = 0; // Unloading by the main thread

	/**
	 * Both the render thread and the update thread will iterate over entities. However, there
	 * should not be a race over them. At last, those functions should only be called by the API.
	 */
	void lock();
	void unlock();

	/**
	 * Called by the world thread 20 time per sec. To be overriden for sentient entities.
	 */
	virtual void fixedUpdate();
	virtual void fastUpdate();

	/**
	 * Location is updated every fixed update, and extrapolated using speed during render. No
	 * teleportation from outside.
	 */
	const Location& getLocation() const;

	const glm::vec3& getPosition() const;
	const glm::vec3& getRotation() const;

	const glm::vec3& getSpeed() const;
	void setSpeed(glm::vec3 &speed);

	const glm::vec3& getRotationSpeed() const;
	void setRotationSpeed(glm::vec3 &rotationSpeed);

	// TODO: Add container/chunk system + local pos/global pos

    bool is_loaded();

protected:
    bool _is_loaded;
	Location _location;
	glm::vec3 _extraPosition, _extraRotation;

	glm::mat3 _normal_matrix;

private:
	std::mutex _mutex;

};

#endif /* WORLD_ENTITY_IENTITY_H_ */
