#include "shapes.hpp"

using namespace glm;

namespace wabi {

Rectangle::Rectangle(vec2 p, float w, float h, float r) : width(w), height(h) {
	position = p;
	rotation = r;
	float halfWidth = w/2.f;
	float halfHeight = h/2.f;
	model[0] = {-halfWidth, halfHeight}; // top left
	model[1] = {halfWidth, halfHeight}; // top right
	model[2] = {halfWidth, -halfHeight}; // bottom right
	model[3] = {-halfWidth, -halfHeight}; // bottom left
	vertices[0] = position + model[0];
	vertices[1] = position + model[1];
	vertices[2] = position + model[2];
	vertices[3] = position + model[3];

}

Rectangle makeRectangle(vec2 p, float w, float h, float rotation) {
	return Rectangle(p, w, h, rotation);
}

} // namespace wabi
