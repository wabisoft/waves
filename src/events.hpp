#pragma once

#include <vector>

#include <SFML/Window.hpp>

struct Event : sf::Event {
	bool handled = false;
};

struct EventListener {
	virtual void onClosed(sf::Window&) { }
	virtual void onResized(sf::Window&, Event::SizeEvent sizeEvent) { }
	virtual void onLostFocus(sf::Window&) { }
	virtual void onGainedFocus(sf::Window&) { }
	virtual void onTextEntered(sf::Window&, Event::TextEvent) { }
	virtual void onKeyPressed(sf::Window&, Event::KeyEvent) { }
	virtual void onKeyReleased(sf::Window&, Event::KeyEvent) { }
	virtual void onMouseWheelMoved(sf::Window&, Event::MouseWheelEvent) { }
	virtual void onMouseWheelScrolled(sf::Window&, Event::MouseWheelScrollEvent) { }
	virtual void onMouseButtonPressed(sf::Window&, Event::MouseButtonEvent) { }
	virtual void onMouseButtonReleased(sf::Window&, Event::MouseButtonEvent) { }
	virtual void onMouseMoved(sf::Window&, Event::MouseMoveEvent) { }
	virtual void onMouseEntered(sf::Window& window) { }
	virtual void onMouseLeft(sf::Window& window) { }
	virtual void onJoystickButtonPressed(sf::Window&, Event::JoystickButtonEvent) { }
	virtual void onJoystickButtonReleased(sf::Window&, Event::JoystickButtonEvent) { }
	virtual void onJoystickMoved(sf::Window&, Event::JoystickMoveEvent) { }
	virtual void onJoystickConnected(sf::Window&, Event::JoystickConnectEvent) { }
	virtual void onJoystickDisconnected(sf::Window&, Event::JoystickConnectEvent) { }
	virtual void onTouchBegan(sf::Window&, Event::TouchEvent) { }
	virtual void onTouchMoved(sf::Window&, Event::TouchEvent) { }
	virtual void onTouchEnded(sf::Window&, Event::TouchEvent) { }
	virtual void onSensorChanged(sf::Window&, Event::SensorEvent) { }
	virtual void onAll(sf::Window&, Event&) { }

	const char * name = "anonymous";
};

// one event manager per sfml window
struct EventManager {
	std::vector<EventListener*> _listeners[sf::Event::Count+1]; // an array of vectors of listeners
	void subscribe(EventListener& listener, sf::Event::EventType eventType);
	void dispatchEvents(sf::Window& window);
};



