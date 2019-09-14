## TODO
- [ ] Make a stage editor
- [ ] Remove the notion of a "sea" and hold all the waves in the stage struct

## DONE
- [x] Get platforms
- [x] Figure out how to get collision normals
- [x] Get Rocks bouncing off platforms
- [x] Resize rocks with clicks and touches and shit
- [x] Update polygon's vertices every frame with position and rotation;
- [x] Get Rocks sitting calmly on plarforms
	- [x] Lock rocks to a platform if they are in a continuous collision with it
		- [x] Not in love with the way this currently works. I think that the rock should check if it's velocity would move it from the platform and set it's state depend of that.
	- [ ] roll rocks moving on platforms // Not sure if I actually care about this
- [x] Get rocks launching off platforms
- [x] Get ship riding waves (most of the hard work was done in v1 (should mostly just need porting ...knock on wood...)
- [x] Be able to save/load a stage to/from JSON (code in serialize.hpp/.cpp)
- [x] Probably rework the way graphics is done (don't like that it's it's own object, should just be a set of function that act on a RenderTarget?)


## Bug
- [x] aabb update callback uses entity pointer, the pointers are bad after aabb deletion (needs immediate fixing)
	FIX: Did away with the entity pointer and the call back because moving elements in the array invalidated the pointer. Switched to searching by ids (which are incidentally always inorder)


## Extra Curricular
- [ ] Start playing with getting an android build. We'll probably need to get an iOS device
	and get enrolled in the dev program (or jailbreak it) so we can run some builds for that

## Nice to haves

## Idea'rs

* Slow time as a mechanic of the game
* Bouncy platforms
* Speed independence for waves determined by ball "type" (more orthoganality)



