// XXX: there is some duplicate code between polygonXpolygon and linesegmentXpolygon
// this is because I wrote polygonXpolygon first and I don't feel like refactoring it to use
// linesegmentXpolygon, if it becomes a maintainence issue I'll worry about it then
template <int N>
Collision collision(const Vector2 a, const Vector2 b, const Polygon<N> & polygon){ // this is actually not that usefull
	std::vector<Collision> collisions;
	for (int i=0; i < polygon.size; ++i) {
		Vector2 c = polygon.vertices[i];
		Vector2 d = polygon.vertices[(i+1) % polygon.size];
		Collision col;
		if (lineSegmentIntersection(a, b, c, d, col.intersection)) {
			col.collides = true;
			col.penetration = magnitude(b - col.intersection);
			col.normal = normalize(findNormal(c, d, a));
			collisions.push_back(col);
		}
	}
	if (!collisions.empty()) {
		Collision col = collisions[0];
		float min = squaredMagnitude(a - col.intersection);
		for(int i = 1; i < collisions.size(); ++i) {
			float sqrDst = squaredMagnitude(a - collisions[i].intersection); // get the intersection closest to a
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
	// Start with the circle position
	Vector2 c = circle.position;
	float circleRadiusSquared = circle.radius * circle.radius;
	for (int i = 0; i < polygon.size; ++i) {
		Vector2 a = polygon.vertices[i];
		Vector2 b = polygon.vertices[(i+1)%polygon.size];
		Vector2 normal = normalized(findNormal(a, b, c)); // normal of a b in direction of c
		// Find the rel pos between circle and line segment start...
		Vector2 ca = c - a;
		// ... and the rel pos between line segment end and start
		Vector2 ba = b - a;
		// Find the point of circle line relpos(ca) projected onto line relpos (ba)
		Vector2 ba_hat = normalized(ba);
		// Vector2 d = a + dot(ca,ba_hat) * normalized(ba_hat);
		Vector2 d = a + dot(ca,ba_hat) * ba_hat;
		Collision col;
		// determine the distance between point on line and circle
		Vector2 dc = d-c;
		// if the point is closer than the radius then we collide with that line
		if (squaredMagnitude(dc) < circleRadiusSquared) {
			// Now we know that we collide with a point on the infinite line between a and b
			// We still need to check if our collision is bounded on the line segment
			// then there is a collision
			if (std::abs(ba.x) > std::abs(ba.y)) { // line more horizontally oriented
				// we check that we are bounded on x
				if (ba.x > 0) { // positive dx means b.x > a.x
					col.collides = d.x >= a.x && d.x <=b.x;
				} else { // negative dx means that b.x < a.x
					col.collides = d.x >= b.x && d.x <=a.x;
				}
			} else { // line is more vertically oriented
				// we check that we are bounded on y
				if (ba.y > 0) { // positive dy means b.y > a.y
					col.collides = d.y >= a.y && d.y <= b.y;
				} else { // negative dy means that a.y > b.y
					col.collides = d.y >= b.y && d.y <= a.y;
				}
			}
			if (col.collides) {
				col.intersection = d; // the poin projection onto ba is our collision point
				col.penetration = std::abs(magnitude(dc) - circle.radius);
				col.normal = normal;
				collisions.push_back(col);
			} else {
				// if the circle is on the very edge of our line the vertex can be within the circle and
				// we wont detect it as a collision, a literal edge case
				// The reason we wait till the last minute to check this edge case is
				// that we don't want to prematurely take this action, only do this if we are not colliding
				// in the ordinary way
				Vector2 vertex = a; // start by looking at a
				Vector2 relpos = ca; // since we already calulated c-a above let's not waste any cycles
				for (int i = 0; i < 2; ++i) {
					if (i = 1) {
						vertex = b; // look at b second time round
						Vector2 relpos = c - vertex; // this time we must to the math
					}
					// if the vertex is closer than our radius is long
					if (squaredMagnitude(relpos) < circleRadiusSquared) {
						// collision!
						col.collides = true;
						col.intersection = vertex;
						float relpos_mag = magnitude(relpos);
						col.penetration = std::abs(relpos_mag - circle.radius);
						col.normal = relpos / relpos_mag; // since we are on a corner we just push out away from vertex;
						collisions.push_back(col);
					}
				}
			}
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
