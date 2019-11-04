#include "editor.hpp"
#include "editor_action.hpp"

using namespace glm;
using namespace wabi;

// Static shit
SelectAction StaticActions::select = SelectAction();
MoveAction StaticActions::move = MoveAction();
ResizeAction StaticActions::resize = ResizeAction();
PlayAction StaticActions::play = PlayAction();

// Action shit
Action* Action::onMouseButtonPressed(sf::Window&, glm::vec2, Editor& editor) { return editor.getAction(); };
Action* Action::onMouseButtonReleased(sf::Window&, glm::vec2, Editor& editor) {return editor.getAction(); };
Action* Action::onMouseMoved(sf::Window&, glm::vec2, Editor& editor) { return editor.getAction(); };

Action* SelectAction::onMouseButtonPressed(sf::Window& window, glm::vec2 position, Editor& editor) {
	editor.selectedEntity = editor.hotEntity;
	return editor.getAction();
}

Action* MoveAction::onMouseMoved(sf::Window& window, glm::vec2 position, Editor& editor) {
	if(editor.mouseState.down) {
		vec2 delta = position - editor.mouseState.prevPosition;
		editor.selectedEntity->position += delta;
		update(editor.selectedEntity->shape, editor.selectedEntity->position);
		updateAABBS(editor.stage);
		return this;
	} else {
		return editor.getAction();
	}
}

Action* ResizeAction::onMouseMoved(sf::Window& window, glm::vec2 position, Editor& editor) {
	// NOTE: I'm pretty sure this is only gonna work on rectangles
	// so we may need to rethink this if we add other shapes
	if(editor.mouseState.down) {
		vec2 delta = position - editor.mouseState.prevPosition;
		wabi::Polygon& polygon = editor.selectedEntity->shape;
		if(isDiagonal) {
			glm::vec2& prev = edgeIndex == 0 ? polygon.model[polygon.size-1] : polygon.model[edgeIndex-1];
			glm::vec2& next = polygon.model[(edgeIndex+1)%polygon.size];
			polygon.model[edgeIndex] += delta;
			if(edgeIndex % 2) {
				prev.y += delta.y;
				next.x += delta.x;
			} else {
				prev.x += delta.x;
				next.y += delta.y;
			}
		} else {
			// Todo don't allow a resize to cross itself
			// (i.e. so you can't pull edge 1to the left of edge 3 or edge 0 below edge 2 (etc)
			// 				0 (vertical)
			//				 -------
			// 3(horizontal |		| 1 (horizontal)
			// 				 -------
			// 				2 (vertical)
			if(edgeIndex%2) { // horizontal
				polygon.model[edgeIndex].x += delta.x;
				polygon.model[(edgeIndex+1)%polygon.size].x += delta.x;
			} else { // vertical
				polygon.model[edgeIndex].y += delta.y;
				polygon.model[(edgeIndex+1)%polygon.size].y += delta.y;
			}
		}
		update(polygon, editor.selectedEntity->position);
		updateAABBS(editor.stage);
		return this;
	} else {
		return editor.getAction();
	}
}

Action* PlayAction::onMouseButtonPressed(sf::Window& window, glm::vec2 position, Editor& editor) {
	processStartInput(editor.playStage, position);
	return this;
}

Action* PlayAction::onMouseButtonReleased(sf::Window& window, glm::vec2 position, Editor& editor) {
	processEndInput(editor.playStage, position);
	return this;
}

Action* PlayAction::onMouseMoved(sf::Window& window, glm::vec2 position, Editor& editor) {
	processContinuingInput(editor.playStage, position);
	return this;
}

