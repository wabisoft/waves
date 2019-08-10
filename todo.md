# Idea'rs

* Slow time as a mechanic of the game


# Bug
[ ] AABB life time is not right yet, Probably a mistake in the clearing
	in `fixedUpdateRocks`. If you create rocks in rapid succession you can
	the rock to not collide(maybe there's no aabb?) and then you can get a 
	bad read on a rockid in graphics. Needs looking at 

