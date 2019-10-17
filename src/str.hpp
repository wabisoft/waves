#pragma once

#include <string>
#include <sstream>

#include "maths.hpp"
#include "entity.hpp"
#include "events.hpp"
#include "stage.hpp"

std::string str(EntityType type);
std::string str(Event::EventType type);
std::string str(Selection::State s);
std::string str(glm::vec2 v);
#include "str.inl"
