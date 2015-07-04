Asteroids
=========

A little version of Asteroids in C++14 using just GLFW, GLEW, and GLM.

Screenshots
===========

![Playing](/screenshots/playing.png?raw=true "Playing")
![Paused](/screenshots/paused.png?raw=true "Paused")

Done
====

* Hit detection
* Hook up asteroids so that they split when hit by a bullet
* Have ship + asteroid classes reference 'static' drawable classes
* Precompute asteroid shapes and then reuse them
* Have asteroids get generated offscreen and drift on
* Particle effects when an asteroid splits
* Score counter
* Find a way of bundling the score counter font into the app
* Proper delta-time operation rather than per-frame
* Title screen
* Automatically generate asteroids
* Tweak spaceship physics/controls

Todo
====

* Pause mode
* Leaderboard
* Powerups + bonuses

Maybe
=====

* Thruster animation/visuals
