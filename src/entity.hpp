#pragma once

enum EntityType{		
	NONE		= 1 << 1,
	SEA 		= 1 << 2,
	PLATFORM 	= 1 << 3,
	ROCK 		= 1 << 4,
	SHIP		= 1 << 5,
};


// NOTE(owen): should we have a base entity class? revisit or don't. I can't decide.
