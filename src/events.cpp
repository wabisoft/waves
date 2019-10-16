#include <iostream>
#include "events.hpp"

void EventManager::subscribe(EventListener& listener, Event::EventType eventType) {
	_listeners[eventType].push_back(&listener);
}

void EventManager::dispatchEvents(sf::Window& window){
	Event event;
	while (window.pollEvent(event)) {
		std::vector<EventListener*>& listeners  = _listeners[event.type];
		if (listeners.empty()) {
			printf_s("Warning: Recieved Event %s with no listeners\n", str(event.type));
		}
		for(EventListener* listener : listeners) {
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
		}
	}
}

