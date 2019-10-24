// str.hpp


inline std::string str(Entity::Type type) {
	const char * names[] = {
		"", 								// 00000000
		"NONE", 							// 00000001
		"SEA", 								// 00000010
		"SEA|NONE", 						// 00000011
		"PLATFORM", 						// 00000100
		"PLATFORM|NONE",					// 00000101
		"PLATFORM|SEA",     				// 00000110
		"PLATFORM|SEA|NONE",				// 00000111
		"ROCK",								// 00001000
		"ROCK|NONE",						// 00001001
		"ROCK|SEA",							// 00001010
		"ROCK|SEA|NONE",					// 00001011
		"ROCK|PLATFORM",					// 00001100
		"ROCK|PLATFORM|NONE",				// 00001101
		"ROCK|PLATFORM|SEA",				// 00001110
		"ROCK|PLATFORM|SEA|NONE",			// 00001111
		"SHIP",								// 00010000
		"SHIP|NONE", 						// 00010001
		"SHIP|SEA", 						// 00010010
		"SHIP|SEA|NONE", 					// 00010011
		"SHIP|PLATFORM", 					// 00010100
		"SHIP|PLATFORM|NONE",				// 00010101
		"SHIP|PLATFORM|SEA",     			// 00010110
		"SHIP|PLATFORM|SEA|NONE",			// 00010111
		"SHIP|ROCK",						// 00011000
		"SHIP|ROCK|NONE",					// 00011001
		"SHIP|ROCK|SEA",					// 00011010
		"SHIP|ROCK|SEA|NONE",				// 00011011
		"SHIP|ROCK|PLATFORM",				// 00011100
		"SHIP|ROCK|PLATFORM|NONE",			// 00011101
		"SHIP|ROCK|PLATFORM|SEA",			// 00011110
		"SHIP|ROCK|PLATFORM|SEA|NONE",		// 00011111
		"WIN",								// 00100000
		"WIN|NONE", 						// 00100001
		"WIN|SEA", 							// 00100010
		"WIN|SEA|NONE", 					// 00100011
		"WIN|PLATFORM", 					// 00100100
		"WIN|PLATFORM|NONE",				// 00100101
		"WIN|PLATFORM|SEA",     			// 00100110
		"WIN|PLATFORM|SEA|NONE",			// 00100111
		"WIN|ROCK",							// 00101000
		"WIN|ROCK|NONE",					// 00101001
		"WIN|ROCK|SEA",						// 00101010
		"WIN|ROCK|SEA|NONE",				// 00101011
		"WIN|ROCK|PLATFORM",				// 00101100
		"WIN|ROCK|PLATFORM|NONE",			// 00101101
		"WIN|ROCK|PLATFORM|SEA",			// 00101110
		"WIN|ROCK|PLATFORM|SEA|NONE",		// 00101111
		"WIN|SHIP",							// 00110000
		"WIN|SHIP|NONE", 					// 00110001
		"WIN|SHIP|SEA", 					// 00110010
		"WIN|SHIP|SEA|NONE", 				// 00110011
		"WIN|SHIP|PLATFORM", 				// 00110100
		"WIN|SHIP|PLATFORM|NONE",			// 00110101
		"WIN|SHIP|PLATFORM|SEA",     		// 00110110
		"WIN|SHIP|PLATFORM|SEA|NONE",		// 00110111
		"WIN|SHIP|ROCK",					// 00111000
		"WIN|SHIP|ROCK|NONE",				// 00111001
		"WIN|SHIP|ROCK|SEA",				// 00111010
		"WIN|SHIP|ROCK|SEA|NONE",			// 00111011
		"WIN|SHIP|ROCK|PLATFORM",			// 00111100
		"WIN|SHIP|ROCK|PLATFORM|NONE",		// 00111101
		"WIN|SHIP|ROCK|PLATFORM|SEA",		// 00111110
		"WIN|SHIP|ROCK|PLATFORM|SEA|NONE",	// 00111111
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
        "SensorChanged",
		"Count(All Events)"
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

