# Super Mario Breakout

This project is essentially a fully featured Breakout, or Brick Breaker, clone (you know the Atari 2600 game), with a total pallet/audio swap with Super Mario textures and sounds. The player's goal is to use their paddle (pipe) to hit the ball (chain chomp) to break all of the bricks in the level. Once all bricks are cleared the level is over and the user advances to the next dynamically loaded level, play continues until they run out of lives.

I built this project using modern opengl (shaders) as my final project for my graduate graphics course in the fall of 2015 at NCSU.

## Video Walkthrough

[![walkthrough video](http://img.youtube.com/vi/https://youtu.be/sngWY6lbA60/0.jpg)](http://www.youtube.com/watch?v=https://youtu.be/sngWY6lbA60)

## Features

Aside from the standard gameplay loop and features you would expect in a basic Breakout clone this project also provides:
- Multiple lives: user is given 3 lives at the start of the game
- Sound: background and collision sound in the form of .wav and .mp3
- Level transitions: when a level is completed a screen is presented which
congratulates the user for beating the level, and prompts to continue
- Scoring: score is incremented when breaking bricks, and is displayed at top
- Powerups: powerups can spawn from bricks randomly

## Controls
- 'Left/A'      = Move paddle left
- 'RIGHT/D'     = Move paddle right
- 'SPACE/ENTER' = Proceed to next level, or launch ball from stuck paddle
- 'ESC'         = Quit game

## Powerups

- Speed up          = increases the ball's speed by 3x (can stack)
- Sticky ball       = makes the ball stick to the paddle on collision
- Pass through ball = ball can pass through blocks
- Paddle increase   = increases the length of the paddle (can stack)

## Levels

Levels are dynamically loaded from text files in the /levels directory,
they can be an arbitrary number of rows and columns as long as they are
uniform and are formatted as such "X Y Z W ...", spaces between the #s
are critical. Here is the breakdown of what numbers correspond to what:

- 0 = no block
- 1 = unbreakable block '/models/solidblock'
- 2 = brick block '/models/brickblock'
- 3 = question block '/models/questionblock'
- 4 = gold block '/models/goldblock'

## Directories

- '/audio'   = contains all of the .wav and .mp3 files used in the project
- '/inc'     = standard include directory
- '/levels'  = contains all of the levels in the form of .txt files
- '/lib'     = standard library directory w/.libs and .dlls
- '/models'  = contains all model information, backgrounds, etc.
- '/shaders' = contains fragment and vertex shaders
- '/source'  = all of the .c/.h's for this project

## Third Party Libraries

Significant parts of this project rely on these third party libraries:
- [freetype 2.61](http://www.freetype.org)
- [irrKlang](http://www.ambiera.com/irrklang/downloads.html)
- [SOIL](http://www.lonesock.net/soil.html)
- [tinyobj](https://syoyo.github.io/tinyobjloader/)

## Necessary DLL's

- 'glew32.dll'
- 'ikpMP3.dll'      (irrklang)
- 'irrKlang.dll'    (irrklang)
- 'freetype261.dll' (freetype)
