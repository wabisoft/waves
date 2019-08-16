#include "printing.hpp"

// Thanks javidx9!
// NOTE: this only determines if poly1 is specifically overlaping poly2
// Also NOTE: poly1 and poly2 must be CONVEX
template <int N, int M>
Collision collision(const Polygon<N>& poly1, const Polygon<M>& poly2) {// Vector2& intersection, float& depth) {
	// check the diagonals of one polygon ...
	std::vector<Collision> collisions;
	for (int i = 0; i < poly1.size; ++i) {
		Vector2 a = poly1.position;
		Vector2 b = poly1.vertices[i];
		// ...against the edges of the other
		for (int j = 0; j < poly2.size; ++j) {
			Vector2 c = poly2.vertices[j];
			Vector2 d = poly2.vertices[(j+1) % poly2.size];
			Collision col;
			if(lineSegmentIntersection(a,b,c,d,col.intersection)) {
				col.collides = true;
				col.penetration = magnitude(b-col.intersection);
				// get normal (how?) it's always either (-dy, dx) or (dy,-dx) but which
				// we want the one that is opposite polygon1's velocity (probably can also use relpos)
				col.normal = normalize(findNormal(c, d, a));
				collisions.push_back(col);
			}
		}
	}
	if (!collisions.empty()) {
		Collision col = collisions[0];
		float min = squaredMagnitude(poly1.position - col.intersection);
		for(int i = 1; i < collisions.size(); ++i) {
			float sqrDst = squaredMagnitude(poly1.position - collisions[i].intersection);
			if (sqrDst < min) {
				col = collisions[i];
				min = sqrDst;
			}
		}
		return col;
	} else {
		return Collision();
	}
}

template <int N>
Collision collision(const Circle& circle, const Polygon<N>& polygon) {//, Vector2& intersection, float& depth) {
	std::vector<Collision> collisions;
	// Check radius projected on the relative position vector...
	Vector2 a = circle.position;
	// Vector2 b = a + normalized(polygon.position - circle.position) * circle.radius;
	// ...against the edges of the polygon
	for (int i = 0; i < polygon.size; ++i) {
		Vector2 c = polygon.vertices[i];
		Vector2 d = polygon.vertices[(i+1)%polygon.size];
		Vector2 normal = normalized(findNormal(c, d, a));
		Vector2 b = a - normal * circle.radius; // furthest extend of circle anti the polygon normal
		Collision col;
		if(lineSegmentIntersection(a, b, c, d, col.intersection)) {
			col.collides = true;
			col.penetration = magnitude(b-col.intersection);
			// special case close to polygon vertex;
			if (squaredMagnitude(c - col.intersection) < 0.01) {
				col.normal = normalized(c - polygon.position); // set normal to normalized vertex diagonal
			} else if (squaredMagnitude(d - col.intersection) < 0.01) {
				col.normal = normalized(d - polygon.position); // set normal to normalized vertex diagonal
			} else {
				col.normal = normal;
			}
			std::cout << col.normal << std::endl;
			collisions.push_back(col);
		}
	}
	if (!collisions.empty()) {
		Collision col = collisions[0];
		float min = squaredMagnitude(circle.position - col.intersection);
		for(int i = 1; i < collisions.size(); ++i) {
			float sqrDst = squaredMagnitude(circle.position - collisions[i].intersection);
			if (sqrDst < min) {
				col = collisions[i];
				min = sqrDst;
			}
		}
		return col;
	} else {
		return Collision();
	}
}
