#pragma once
// TODO: maybe if we use opengl we need to implement our own clock. sfml's impl is
// reasonable, so we can pretty much copy it when/if the time comes

#include <SFML/System/Clock.hpp> // for now just include sfml

typedef sf::Clock Clock; // and typedef

// Oh yeah I guess we'll need a reasonable time representation. I think just a float with some conversion
// methods is probably ok. Look around the scene and see what other people so. Thank god for the internet and
// Github
