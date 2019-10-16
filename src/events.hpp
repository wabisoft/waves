#pragma once

#include <vector>

#include <SFML/Window.hpp>

using Event = sf::Event;

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
};

// one event manager per sfml window
struct EventManager {
	std::vector<EventListener*> _listeners[sf::Event::Count]; // an array of vectors of listeners

	void subscribe(EventListener& listener, sf::Event::EventType eventType);
	void dispatchEvents(sf::Window& window);
};

inline const char * str(Event::EventType type) {
	const char * names[] = {
		"Closed",
        "Resized",
        "LostFocus",
        "GainedFocus",
        "TextEntered",
        "KeyPressed",
        "KeyReleased",
        "MouseWheelMoved",
        "MouseWheelScrolled",
        "MouseButtonPressed",
        "MouseButtonReleased",
        "MouseMoved",
        "MouseEntered",
        "MouseLeft",
        "JoystickButtonPressed",
        "JoystickButtonReleased",
        "JoystickMoved",
        "JoystickConnected",
        "JoystickDisconnected",
        "TouchBegan",
        "TouchMoved",
        "TouchEnded",
        "SensorChanged"
	};
	return names[type];
}

