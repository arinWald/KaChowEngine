# KaChowEngine

This project is a 3D Game Engine made by students of CITM for the assigment of Videogame Engine Development of 2023.

It has been made by: Arnau Gonzalez and Pau Argiz

### Our GitHubs:
* [Arnau Gonzalez](https://github.com/arinWald)
* [Pau Argiz](https://github.com/PauM4)

[Link](https://github.com/arinWald/KaChowEngine) to our project

## How to use KaChow Engine?

### Download the engine

1. Download the last release [here](https://github.com/arinWald/KaChowEngine).
2. Extract files from the .zip
3. Execute .exe

### The engine

**Camera**

Right Click - FPS free rotation
Mouse wheel UP/DOWN - Zoom OUT/IN
LALT + Left Click - Orbit around Selected Object.

While mouse right clicking:
(Holding Shift, movement speed boost)
W - Move forward
A - Move Left
S - Move backwards
D - Move Right
Q - Move Up
E - Move Down

F - Focus object selected

**Inspector**

Displays all different components of the selected GameObjects.

**Components**

Component Transform - Rotate, position and scale.
Component Mesh - Show all info about differents meshes vertex of selected object.
Component Materials - Allow viewing textures.

**Drag&Drop**

The engine allows the user to Drag&Drop FBX files from anywhere in the hard drive and later, drag and drop files to put textures for the GameObjects.

### Windows

**Configuration**
FPS histogram
Show FPS

MS Histogram
Show DeltaTime

Window
Window resolution
Display winfdown resolution and two slider to change it
Checkbow with fullscreen option

View
Activate and deactivate windows

Render
Checkbox:
Lightning
Cullface
DepthTest
Color Material
Wireframe
Textures 2D
VSync
Slider brightness

Input:
Display mouse and keyboard inputs

Audio:
Volumen slider


**Objects**
Allows to create all kinds of primitives. Including an empty GameObject.

Hierarchy
Dsiplay all GamObjects and allows to select and move them.

Inspector
Displays the info of the GameObject selected.
Change it's name.
Show mesh info and face normals.
Diplsy texture info and allows to change it.

Console
Prints all important LOGS.

External libraries
Externals libraries used in our engine:

SDL
ImGui
Assimp
Glew
MathGeoLib
DevIL
