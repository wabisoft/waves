# Idea'rs

* Slow time as a mechanic of the game


# Bug
[x] aabb update callback uses entity pointer, the pointers are bad after aabb deletion (needs immediate fixing)
	FIX: Did away with the entity pointer and the call back because moving elements in the array invalidated the pointer. Switched to searching by ids (which are incidentally always inorder)

