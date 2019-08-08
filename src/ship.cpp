#include "aabb.hpp"
#include "entity.hpp"
#include "ship.hpp"


void fixedUpdate(Ship& ship){ }

AABB aabb(const Ship& ship) {
	Vector2 diag = {ship.width/2, ship.height/2};
	return AABB(ship.position - diag, ship.position + diag, SHIP, ship.id);
}
