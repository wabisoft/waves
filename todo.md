# TODO

## General
[x] Get platforms
[ ] Figure out how to get collision normals
[ ] Get Rocks bouncing off platforms
[ ] Get Rocks sitting calmly on plarforms
[ ] Get rocks launching off platforms
[ ] Get a ships riding waves

## Bug
[x] aabb update callback uses entity pointer, the pointers are bad after aabb deletion (needs immediate fixing)
	FIX: Did away with the entity pointer and the call back because moving elements in the array invalidated the pointer. Switched to searching by ids (which are incidentally always inorder)


## Extra Curricular
[ ] Start playing with getting an android build. We'll probably need to get an iOS device
	and get enrolled in the dev program (or jailbreak it) so we can run some builds for that

## Nice to haves

## Idea'rs

* Slow time as a mechanic of the game


