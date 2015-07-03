Asteroids
=========

A little version of Asteroids in C++14 using just GLFW, GLEW, and GLM.

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

Todo
====

* Proper delta-time operation rather than per-frame
* Tweak spaceship physics/controls
* Title screen and game over/leaderboard
* Powerups + bonuses
* Automatically generate asteroids

Maybe
=====

* Thruster animation/visuals
