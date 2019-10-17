// str.hpp


inline std::string str(EntityType type) {
	const char * names[] = {
		"", 							// 00000000
		"NONE", 						// 00000001
		"SEA", 							// 00000010
		"SEA|NONE", 					// 00000011
		"PLATFORM", 					// 00000100
		"PLATFORM|NONE",				// 00000101
		"PLATFORM|SEA",     			// 00000110
		"PLATFORM|SEA|NONE",			// 00000111
		"ROCK",							// 00001000
		"ROCK|NONE",					// 00001001
		"ROCK|SEA",						// 00001010
		"ROCK|SEA|NONE",				// 00001011
		"ROCK|PLATFORM",				// 00001100
		"ROCK|PLATFORM|NONE",			// 00001101
		"ROCK|PLATFORM|SEA",			// 00001110
		"ROCK|PLATFORM|SEA|NONE",		// 00001111
		"SHIP",							// 00010000
		"SHIP|NONE", 					// 00010001
		"SHIP|SEA", 					// 00010010
		"SHIP|SEA|NONE", 				// 00010011
		"SHIP|PLATFORM", 				// 00010100
		"SHIP|PLATFORM|NONE",			// 00010101
		"SHIP|PLATFORM|SEA",     		// 00010110
		"SHIP|PLATFORM|SEA|NONE",		// 00010111
		"SHIP|ROCK",					// 00011000
		"SHIP|ROCK|NONE",				// 00011001
		"SHIP|ROCK|SEA",				// 00011010
		"SHIP|ROCK|SEA|NONE",			// 00011011
		"SHIP|ROCK|PLATFORM",			// 00011100
		"SHIP|ROCK|PLATFORM|NONE",		// 00011101
		"SHIP|ROCK|PLATFORM|SEA",		// 00011110
		"SHIP|ROCK|PLATFORM|SEA|NONE",	// 00011111
	};
	return std::string(names[type]);
}

inline std::string str(Event::EventType type) {
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
	return std::string(names[type]);
}

inline std::string str(Selection::State s) {
	const char * names[] = {
		"SELECT", "PULL"
	};
	return std::string(names[s]);
}


inline std::string str(glm::vec2 v) {
	std::stringstream ss;
	ss << "[" << v[0] << ", " << v[1] << "]";
	return ss.str();
}

