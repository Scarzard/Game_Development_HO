# Warped by GroundZero

## Game Info

Warped is a platformer game developed by two students at Universitat Politècnica de Catalunya for the Game Development subject. As of collisions with the environment, the player can walk left or right, jump, and if it falls on a pit, it's sent to the start of the level. The game reads the map and most of the gameplay values from .xml files, and .tmx files generated via Tiled.

DISCLAIMER: The tileset used in this game is copyright free (credit to ansimuz, tileset URL: https://ansimuz.itch.io/warped-caves). 
The tileset was edited in Adobe Photoshopc so the original work wont have as many elements in the tileset as ours.
The sprites used for the main character are taken from the 2007 game "Mega Man ZX Advent", developed 
by Inti Creates and published by Capcom. We do not own the rights for these sprites, nor we 		
claim to have made them. All rights belong to Capcom.

##	Innovation

 - Assignment 1: For innovation (doing something that the assignment didn't require), we developed a system that can read all the animations of the player from a .tmx file, so that 		
animations can later be easily edited from Tiled. The number of animations the player has, and their names, still has to be specified in the code, 
but all pushbacks (frame data) are read from the .tmx, and a designer can modify the .png spritesheet, and then set the animations from Tiled.

- Assignment 2: We created a method to load every collider automatically from an Object Layer in the TMX files. It differentiates between each type of possible colliders and sets them
depending on the value given in the TMX file. Pathfinding debug mode.

##	Controls

- A and D: 	Move the character left or right.

- J:		Jump. By inputting again in midair, the player can double-jump before touching ground again.

### Debug Features

- F1:		Start again from the first level of the game.
- F2:		Start again from the beginning of the current level.
- F2:		Start from the beginning of the second level.
- F5:		Save current state of the game. (save file: "save_game.xml")
- F6:		Load state from earlier save.
- F9:		See colliders/hitboxes.
- F10:		Godmode (can walk over pits, can double jump limitless, character changes visually).
- F11:		Enable/disable FPS cap (Capped at 30FPS)
- While in GodMode, you can click on the map twice. If the tiles are walkable, it will create a path from the first click (origin) to its destination, second click.

## Github

As stated on the license, anyone can look at or modify the code of this project. 
It can be found here: https://github.com/Scarzard/Game_Development

## Authors
   
Joel Cabaco Pérez
   - [Joel's GitHub Link](https://github.com/dynamiczero99)
   
Victor Chen Chen
   - [Victor's GitHub Link](https://github.com/Scarzard)
   
## Tasklist:
	
- Victor Chen: 
	- Timer and PerfTimer modules added.
	- Pathfinding module.
	- Pathfinding base code. Pathfinding debug mode added.
	- Overhaul of both maps.
	- Brofiler added for basic profiling features.
	- Title with relevant information 
	- Framerate cap to 30FPS added.
	
- Joel Cabaco: 
	- UML file creation.



