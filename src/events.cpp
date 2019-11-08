#include <iostream>
#include "events.hpp"
#include "str.hpp"


void EventListener::subscribe(EventManager& manager, std::vector<Event::EventType> eventTypes) {
	for(Event::EventType eventType : eventTypes) {
		auto search = std::lower_bound(_subscribedEvents.begin(), _subscribedEvents.end(), eventType);
		if(search == _subscribedEvents.end() || *search != eventType) {
			sorted_insert(_subscribedEvents, eventType);
		}
		manager._subscribe(*this, eventType);
	}
}

void EventListener::subscribe(EventManager& manager) {
	for(Event::EventType eventType : _subscribedEvents) {
		manager._subscribe(*this, eventType);
	}
}

void EventListener::unsubscribe(EventManager& manager, std::vector<Event::EventType> eventTypes) {
	for(Event::EventType eventType : eventTypes) {
		manager._unsubscribe(*this, eventType);
	}
}

void EventListener::unsubscribe(EventManager& manager) {
	for(Event::EventType eventType : _subscribedEvents) {
		manager._unsubscribe(*this, eventType);
	}
}


void EventManager::_subscribe(EventListener& listener, Event::EventType eventType) {
	std::vector<EventListener*>& listeners = _listeners[eventType];
	auto search = std::find(listeners.begin(), listeners.end(), &listener);
	if(search != listeners.end()) {
		logger.warning("%s subscribed to %s, but was already subscribed\n", listener.name, str(eventType).c_str());
		return;
	} else {
		_listeners[eventType].push_back(&listener);
		logger.info("%s subscribed to %s.\n", listener.name, str(eventType).c_str());
	}
}

void EventManager::_unsubscribe(EventListener& listener, Event::EventType eventType) {
	std::vector<EventListener*>& listeners = _listeners[eventType];
	auto search = std::find(listeners.begin(), listeners.end(), &listener);
	if(search != listeners.end()) {
		listeners.erase(search);
		logger.info("%s unsubscribed from %s.\n", listener.name, str(eventType).c_str());
	} else {
		logger.warning("%s unsubscribed from %s, but was never subscribed\n", listener.name, str(eventType).c_str());
	}
}

void EventManager::dispatchEvents(sf::Window& window){
	Event event;
	while (window.pollEvent(event)) {
		EventListener * listener;
		std::vector<EventListener*>& _power_listeners = _listeners[Event::Count];
		std::vector<EventListener*>& listeners  = _listeners[event.type];
		for(auto it = _power_listeners.begin(); it != _power_listeners.end(); ++it) {
			listener = *it;
			listener->onAll(window, event);
			if (event.handled) {
				if (event.type == Event::MouseButtonPressed) {
					logger.debug("blah");
				}
				logger.debug("Event %s marked handled by: %s!\n", str(event.type).c_str(), listener->name);
				goto OUTER;
			}

		}
		if (listeners.empty()) {
			logger.debug("Recieved Event %s with no listeners\n", str(event.type).c_str());
			continue;
		}
		for(auto it = listeners.begin(); it != listeners.end(); ++it) {
			listener = *it;
			switch (event.type) {
 				case Event::Closed:
					listener->onClosed(window);
					break;
        		case Event::Resized:
					listener->onResized(window, event.size);
					break;
        		case Event::LostFocus:
					listener->onLostFocus(window);
					break;
        		case Event::GainedFocus:
					listener->onGainedFocus(window);
					break;
        		case Event::TextEntered:
					listener->onTextEntered(window, event.text);
					break;
        		case Event::KeyPressed:
					listener->onKeyPressed(window, event.key);
					break;
        		case Event::KeyReleased:
					listener->onKeyReleased(window, event.key);
					break;
        		case Event::MouseWheelMoved: // depricated
					listener->onMouseWheelMoved(window, event.mouseWheel);
					break;
        		case Event::MouseWheelScrolled:
					listener->onMouseWheelScrolled(window, event.mouseWheelScroll);
					break;
        		case Event::MouseButtonPressed:
					listener->onMouseButtonPressed(window, event.mouseButton);
					break;
        		case Event::MouseButtonReleased:
					listener->onMouseButtonReleased(window, event.mouseButton);
					break;
        		case Event::MouseMoved:
					listener->onMouseMoved(window, event.mouseMove);
					break;
        		case Event::MouseEntered:
					listener->onMouseEntered(window);
					break;
        		case Event::MouseLeft:
					listener->onMouseLeft(window);
					break;
        		case Event::JoystickButtonPressed:
					listener->onJoystickButtonPressed(window, event.joystickButton);
					break;
        		case Event::JoystickButtonReleased:
					listener->onJoystickButtonReleased(window, event.joystickButton);
					break;
        		case Event::JoystickMoved:
					listener->onJoystickMoved(window, event.joystickMove);
					break;
        		case Event::JoystickConnected:
					listener->onJoystickConnected(window, event.joystickConnect);
					break;
        		case Event::JoystickDisconnected:
					listener->onJoystickDisconnected(window, event.joystickConnect);
					break;
        		case Event::TouchBegan:
					listener->onTouchBegan(window, event.touch);
					break;
        		case Event::TouchMoved:
					listener->onTouchMoved(window, event.touch);
					break;
        		case Event::TouchEnded:
					listener->onTouchEnded(window, event.touch);
					break;
        		case Event::SensorChanged:
					listener->onSensorChanged(window, event.sensor);
					break;
				default: break;
			}
			if (event.handled) {
				logger.debug("Event %s marked handled by: %s!\n", str(event.type).c_str(), listener->name);
				goto OUTER;
			}
		}
OUTER:
		;
	}
}

