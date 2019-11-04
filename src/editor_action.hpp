#pragma once

#include "SFML/Window.hpp"

#include "glm/vec2.hpp"

struct Editor;

namespace Cursor {
	enum Style{
	    Arrow = 0,
	    TextInput,
	    ResizeAll,
	    ResizeNS,
	    ResizeEW,
	    ResizeNESW,
	    ResizeNWSE,
	    Hand,
	    COUNT
	};
};

struct Action {
	enum Type {
		SELECT,
		MOVE,
		RESIZE,
		COUNT
	};
	virtual Action* onMouseButtonPressed(sf::Window&, glm::vec2, Editor& editor);
	virtual Action* onMouseButtonReleased(sf::Window&, glm::vec2, Editor& editor);
	virtual Action* onMouseMoved(sf::Window&, glm::vec2, Editor& editor);
	virtual Cursor::Style getCursorStyle() { return Cursor::Arrow; }
};

struct SelectAction : Action {
	Action* operator()() { return this; }
	virtual Action* onMouseButtonPressed(sf::Window&, glm::vec2, Editor& editor) override;
	virtual Cursor::Style getCursorStyle() override { return Cursor::Arrow; }
};

struct MoveAction : Action {
	Action* operator()() { return this; }
	virtual Action* onMouseMoved(sf::Window&, glm::vec2, Editor& editor) override;
	virtual Cursor::Style getCursorStyle() override { return Cursor::ResizeAll; }
};

struct ResizeAction : Action {
	Action* operator()(int edgeIdx, bool isDiag) { edgeIndex = edgeIdx; isDiagonal = isDiag; return this;}
	virtual Action* onMouseMoved(sf::Window&, glm::vec2, Editor& editor) override;
	virtual Cursor::Style getCursorStyle() override {
		Cursor::Style straight[]= {
			Cursor::ResizeNS,
			Cursor::ResizeEW
		};
		Cursor::Style diagonal[]= {
			Cursor::ResizeNWSE,
			Cursor::ResizeNESW
		};
	 	return isDiagonal ? diagonal[edgeIndex%2] : straight[edgeIndex%2];
	}
	int edgeIndex;
	bool isDiagonal;
};

struct PlayAction : Action {
	Action* operator()() { return this; }
	virtual Action* onMouseButtonPressed(sf::Window&, glm::vec2, Editor& editor) override;
	virtual Action* onMouseButtonReleased(sf::Window&, glm::vec2, Editor& editor) override;
	virtual Action* onMouseMoved(sf::Window&, glm::vec2, Editor& editor) override;
	virtual Cursor::Style getCursorStyle() override { return Cursor::Arrow; }
};

struct StaticActions {
	static SelectAction select;
	static MoveAction move;
	static ResizeAction resize;
	static PlayAction play;
};
