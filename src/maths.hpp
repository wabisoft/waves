#pragma once

#include <cassert>
#include <cmath>
#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/common.hpp>
#include <glm/ext/matrix_common.hpp>
#include <glm/geometric.hpp>
#include <glm/mat3x3.hpp>

const glm::vec2 VEC2_UP {0.f, 1.f};
const glm::vec2 VEC2_DOWN {0.f, -1.f};
const glm::vec2 VEC2_LEFT {-1.f, 0.f};
const glm::vec2 VEC2_RIGHT {1.f, 0.f};
const glm::vec2 VEC2_ZERO {0.f, 0.f};
const glm::mat3 MAT3_IDENTITY(1);

glm::vec2 operator*(const glm::mat3& m, const glm::vec2& v);
float cross(glm::vec2 a, glm::vec2 b);
float sideProduct(glm::vec2 a, glm::vec2 b, glm::vec2 point);
float sideSign(glm::vec2 a, glm::vec2 b, glm::vec2 point);
bool bounded(glm::vec2 a, glm::vec2 b, glm::vec2 point);
bool lineSegmentIntersection(glm::vec2 a, glm::vec2 b, glm::vec2 c, glm::vec2 d, glm::vec2 & intersection);
glm::vec2 findNormal(glm::vec2 a, glm::vec2 b, glm::vec2 c);
glm::vec2& clamp(glm::vec2& v, float s); // clamp this vector to a scalar magnitude

glm::vec2 centroid(const std::vector<glm::vec2>& points);
bool isClockwise(glm::vec2 center, glm::vec2 a, glm::vec2 b);

