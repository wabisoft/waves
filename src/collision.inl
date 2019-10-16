// collision.hpp

// Thanks javidx9!
// NOTE: this only determines if poly1 is specifically overlaping poly2
// Also NOTE: poly1 and poly2 must be CONVEX
template <int N, int M>
Collision collision(const wabi::Polygon<N>& poly1, const wabi::Polygon<M>& poly2) {
	// check the diagonals of one polygon ...
	std::vector<Collision> collisions;
	for (int i = 0; i < poly1.size; ++i) {
		glm::vec2 a = poly1.position;
		glm::vec2 b = poly1.vertices[i];
		// ...against the edges of the other
		for (int j = 0; j < poly2.size; ++j) {
			glm::vec2 c = poly2.vertices[j];
			glm::vec2 d = poly2.vertices[(j+1) % poly2.size];
			Collision col;
			if(lineSegmentIntersection(a,b,c,d,col.intersection)) {
				col.collides = true;
				col.penetration = glm::length(b-col.intersection);
				col.normal = glm::normalize(findNormal(c, d, a));
				col.surfaceStart = c;
				col.surfaceEnd = d;
				collisions.push_back(col);
			}
		}
	}
	if (!collisions.empty()) {
		Collision col = collisions[0];
		glm::vec2 rel = poly1.position - col.intersection;
		float min = glm::dot(rel, rel);
		for(int i = 1; i < collisions.size(); ++i) {
			glm::vec2 rel_i = poly1.position - collisions[i].intersection;
			float sqrDst = glm::dot(rel_i, rel_i);
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
Collision collision(const wabi::Circle& circle, const wabi::Polygon<N>& polygon) {
	std::vector<Collision> collisions;
	// Start with the circle position
	glm::vec2 c = circle.position;
	float circleRadiusSquared = circle.radius * circle.radius;
	for (int i = 0; i < polygon.size; ++i) {
		glm::vec2 a = polygon.vertices[i];
		glm::vec2 b = polygon.vertices[(i+1)%polygon.size];
		glm::vec2 normal = glm::normalize(findNormal(a, b, c)); // normal of a b in direction of c
		// Find the rel pos between circle and line segment start ...
		glm::vec2 ca = c - a;
		// ... and the rel pos between line segment end and start
		glm::vec2 ba = b - a;
		// Find the point of circle line relpos(ca) projected onto line relpos (ba)
		glm::vec2 ba_hat = glm::normalize(ba);
		glm::vec2 d = a + glm::dot(ca,ba_hat) * ba_hat;
		Collision col;
		// determine the distance between point on line and circle
		glm::vec2 dc = d-c;
		// if the point is closer than the radius then we collide with that line
		if (glm::dot(dc, dc) < circleRadiusSquared) {
			// Now we know that we collide with a point on the infinite line between a and b
			// We still need to check if our collision is bounded on the line segment
			// then there is a collision
			if (bounded(a, b, d)) {
				col.collides = true;
				col.intersection = d; // the poin projection onto ba is our collision point
				col.penetration = std::abs(glm::length(dc) - circle.radius);
				col.normal = normal;
				col.surfaceStart = a;
				col.surfaceEnd = b;
				collisions.push_back(col);
			} else {
				// if the circle is on the very edge of our line the vertex can be within the circle and
				// we wont detect it as a collision, a literal edge case
				// The reason we wait till the last minute to check this edge case is
				// that we don't want to prematurely take this action, only do this if we are not colliding
				// in the ordinary way
				glm::vec2 vertex = a; // start by looking at a
				glm::vec2 other_vertex = b; // keep track of this for use in collision object
				glm::vec2 relpos = ca; // since we already calulated c-a above let's not waste any cycles
				for (int i = 0; i < 2; ++i) {
					if (i == 1) {
						vertex = b; // look at b second time round
						other_vertex = a;
						relpos = c - vertex; // this time we must do the math
					}
					// if the vertex is closer than our radius is long
					if (glm::dot(relpos, relpos) < circleRadiusSquared) {
						// collision!
						col.collides = true;
						col.intersection = vertex;
						float relpos_mag = glm::length(relpos);
						col.penetration = std::abs(relpos_mag - circle.radius);
						col.surfaceStart = vertex;
						col.surfaceEnd = other_vertex;
						col.normal = relpos / relpos_mag; // since we are on a corner we just push out away from vertex;
						// col.normal = normal;
						collisions.push_back(col);
					}
				}
			}
		}
	}
	if (!collisions.empty()) {
		Collision col = collisions[0];
		glm::vec2 rel = circle.position - col.intersection;
		float min = glm::dot(rel, rel);
		for(int i = 1; i < collisions.size(); ++i) {
			glm::vec2 rel_i = circle.position - collisions[i].intersection;
			float sqrDst = glm::dot(rel_i, rel_i);
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
