#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

#include "entity.hpp"
#include "editor.hpp"
#include "graphics.hpp"
#include "maths.hpp"
#include "serialize.hpp"
#include "win32_file.hpp"
#include "util.hpp"

using namespace glm;
using namespace wabi;


// Editor shit

void Editor::onClosed(sf::Window& aWindow) {
	setMode(EDIT);
	aWindow.close();
}

void Editor::onMouseButtonPressed(sf::Window& window, Event::MouseButtonEvent mouseButton)	{
	// TODO: something state dependent
	vec2 position = screen2GamePos(window, {mouseButton.x, mouseButton.y});
	mouseState.down = true;
	mouseState.downPosition = position;
	mouseState.prevPosition = position;
	pushAction(popAction()->onMouseButtonPressed(window, position, *this));
}

void Editor::onMouseButtonReleased(sf::Window& window, Event::MouseButtonEvent mouseButton) {
	// TODO: Not sure yet...
	vec2 position = screen2GamePos(window, {mouseButton.x, mouseButton.y});
	mouseState.down = false;
	pushAction(popAction()->onMouseButtonReleased(window, position, *this));
}

void Editor::onMouseMoved(sf::Window& window, Event::MouseMoveEvent mouseMove) {
	vec2 position = screen2GamePos(window, {mouseMove.x, mouseMove.y});
	hotEntity = findEntityAtPosition(stage, position);
	pushAction(popAction()->onMouseMoved(window, position, *this));
	mouseState.prevPosition = position;
}

void Editor::onKeyPressed(sf::Window& window, Event::KeyEvent key) {
	using Key = sf::Keyboard;
	if(mode_ == EDIT) {
		switch(key.code) {
			case Key::S:
				if(key.control && key.shift) {
					std::string message;
					if(level.saveAs(window.getSystemHandle(), stage, message)) {
						popups.push_back({"level-save-as-success", message, GREEN});
					} else {
						popups.push_back({"level-save-as-error", message, RED});
					}
				} else if (key.control) {
					std::string message;
					if(level.save(window.getSystemHandle(), stage, message)) {
						popups.push_back({"level-save-success", message, GREEN});
					} else {
						popups.push_back({"level-save-error", message, RED});
					}
				}
				break;
			case Key::O:
				if (key.control) {
					std::string message;
					if(level.open(window.getSystemHandle(), stage, message)) {
						popups.push_back({"level-open-success", message, GREEN});
					} else {
						popups.push_back({"level-open-error", message, RED});
					}
				}
				break;
			case Key::Tilde: // fall through
			case Key::P:
				if(key.control) {
					setMode(PLAY);
					// game.stage = stage;
					// game.run();
				}
			default: break;
		}
	} else if (mode_ == PLAY) {
		switch (key.code) {
			case Key::P:
				if (stage.state.type == StageState::PAUSED) {
					stage.state.type = StageState::RUNNING;
				} else {
					stage.state.type = StageState::PAUSED;
				}
				break;
			case Key::Escape: // fall through
			case Key::Q: setMode(EDIT); break;
			// case Key::Num1: timeScale = 1.f; break;
			// case Key::Num2: timeScale = 2.f; break;
			// case Key::Num3: timeScale = 3.f; break;
			// case Key::Num4: timeScale = 4.f; break;
			// case Key::Num5: timeScale = 100.f; break;
			// case Key::Num6: timeScale = 1000.f; break;
			case Key::R:
				if (key.control) {
					playStage = stage;
					// reloadStage();
				} else {
					stage.rockKind = {Rock::RED};
				}
				break;
			case Key::G: stage.rockKind = {Rock::GREEN}; break;
			case Key::B: stage.rockKind = {Rock::BLUE}; break;
		default: break;
		}
	} else { assert(false); }
}

void Editor::start() {
	stage = {};
	if(!level.filename.empty()) {
		std::string message;
		if(level.open(stage, message)) {
			popups.push_back({"level-open-success", message, GREEN});
		} else {
			popups.push_back({"level-open-error", message, RED});
		}
	}
	subscribe(eventManager);
	imguiListener.subscribe(eventManager);
	ImGui::SFML::Init(*window);
	ImGuiIO& io = ImGui::GetIO();
	ImFont* pFont = io.Fonts->AddFontFromFileTTF((cwd() + "/assets/fonts/IBMPlexMono-Regular.ttf").c_str(), 15.f);
	ImGui::SFML::UpdateFontTexture();
	logging::Logger::setLoggerLevel("EventManager", logging::INFO);
}

void Editor::run() {
	Clock updateClock;
	Clock renderClock;
	while (window->isOpen()) {
		eventManager.dispatchEvents(*window);
		auto style = actions.top()->getCursorStyle();
		ImGui::SetMouseCursor(style);
		updateDelta = updateClock.getElapsedTime().asSeconds();
		if(updateDelta >= FIXED_TIMESTEP) {
			update(updateClock.restart());
			loopsPerUpdate = 0;
		} else {loopsPerUpdate++;}
		renderDelta = renderClock.getElapsedTime().asSeconds();
		if (renderDelta >= FRAME_RATE) {
			render(renderClock.restart());
		}
	}
}

void Editor::stop() {
 	if(!level.filename.empty()) {
		std::string message;
		if(level.save(window->getSystemHandle(), stage, message)) {
			popups.push_back({"level-save-success", message, GREEN});
		} else {
			popups.push_back({"level-save-error", message, RED});
		}
	}
	unsubscribe(eventManager);
	imguiListener.unsubscribe(eventManager);
	ImGui::SFML::Shutdown();
	// selectedEntity = hotEntity = nullptr;
}

void Editor::update(sf::Time deltaTime) {
	if(mode_ == EDIT) {
		WABI_NOT_USED(deltaTime);
	} else if (mode_ == PLAY ) {
		Stage::update(playStage, std::min(deltaTime.asSeconds(), FIXED_TIMESTEP));
		if(playStage.state.type == StageState::FINISHED && !playStage.state.finished.win) {
			playStage = stage; // reload the stage
		}
	} else { assert(false); }
}

void Editor::render(sf::Time deltaTime) {
	if(mode_ == EDIT) {
		window->clear();
		ImGui::SFML::Update(*window, deltaTime);
		graphics.drawStage(*window, stage, true);
		if(selectedEntity) {
			graphics.drawPolygon(*window, selectedEntity->shape, sf::Color(0, 255, 204), sf::Color::Red);
		}
		auto intersections = pointsOfIntersection(stage.ship.shape, stage.seas[0].shape);
		if(intersections.size() > 1) {
			auto vertices = intersections;
			// auto cent = centroid(vertices);
			// std::sort(vertices.begin(), vertices.end(), [&cent](const vec2& v1, const vec2& v2) {
			// 	bool res = isClockwise(cent, v1, v2);
			// 	return res;
			// });
			wabi::Polygon p = wabi::Polygon(vertices, 0, true);
			graphics.drawPolygon(*window, p, sf::Color(255, 153, 153), sf::Color::Green);
		}
		drawImGui();
		ImGui::SFML::Render(*window);
		window->display();
	} else if (mode_ == PLAY) {
		graphics.drawStage(*window, playStage, false);
		// graphics.drawInfoText(*window, playStage, renderDelta, updateDelta, loopsPerUpdate);
		ImGui::SFML::Update(*window, deltaTime);
		ImGui::Begin("PlayMode");
		ImGui::Text("Draw delta: %f", 1/renderDelta);
		ImGui::Text("Update delta: %f", 1/updateDelta);
		ImGui::Text("loops/update: %f", loopsPerUpdate);
		ImGui::End();
		wabi::Polygon c = clip(playStage.ship.shape, playStage.seas[0].shape);
		graphics.drawPolygon(*window, c, sf::Color(255, 153, 153), sf::Color::Green);
		ImGui::SFML::Render(*window);
		window->display();
	} else { assert(false); }
}

void Editor::enterMode(int mode) {
	if(mode == EDIT) {
		imguiListener.subscribe(eventManager);
	} else if (mode == PLAY) {
		playStage = stage;
		pushAction(StaticActions::play());
	} else { assert(false); }
	mode_ = mode;
}

void Editor::exitMode(int mode) {
	if(mode == EDIT) {
		WABI_NOT_USED(mode);
	} else if (mode == PLAY) {
		popAction();
	} else { assert(false); }
}

void Editor::setMode(int mode) {
	exitMode(mode_);
	enterMode(mode);
}

Action* Editor::popAction() {
	return actions.rpop();
}

void Editor::pushAction(Action* action) {
	actions.push(action);
}

Action* Editor::getAction() {
	if(selectedEntity == nullptr || hotEntity == nullptr || hotEntity->id != selectedEntity->id) {
		return StaticActions::select();
	} else {
		// check closeness to edge and do either translate or resize pointers
		int edgeIndex = 0;
		bool onVert = false;
		float distFromSelectedEntityEdge = minDistFromEdge(mouseState.prevPosition, selectedEntity->shape, edgeIndex, onVert);
		if(distFromSelectedEntityEdge <= 1) {
			// RESIZE
			return StaticActions::resize(edgeIndex, onVert);
		} else {
			// MOVE
			return StaticActions::move();
		}
	}
}

