# KaChow Engine

KaChow Engine is a Video Game Engine created by Arnau González and Pau Argiz. Two students from CITM of Universitat Politècnica de Catalunya (UPC).

The purpose of this project is to create a friendly-user, well optimized Game Engine for the subject “VideoGame Engines” from the 3rd year. The short term plan is to implement the basic features of a Game Engine, such as loading models, working in a 3D environment, etc… Also we implemented the Audio sub-system.

## Team Members:

### Arnau González

https://github.com/arinWald/KaChowEngine/blob/NewMain/docs/media/arnau.jpeg

Arnau González is one of the team members, he worked on configuration screen, console, camera settings, serialization, drag and drop, memory leaks, game objects logic, viewports and audio integration. We mainly worked all together.

* [GitHub Profile](https://github.com/arinWald)
* [Linkedin Profile](https://www.linkedin.com/in/arnau-gonzalez/)


### Pau Argiz

![](https://github.com/arinWald/KaChowEngine/blob/NewMain/docs/media/pau.png)

The other component of the group is Pau Argiz, he took care about display options, bounding boxes, mouse picking and camera movement. We mainly worked all together.

* [GitHub Profile](https://github.com/PauM4)
* [Linkedin Profile](https://www.linkedin.com/in/pau-argiz/)

## Audio sub-system

We had to implement the Audio core subsystem with WWise. The main objective was to be able to play sounds/musics, etc… in our game engine.
Also, another objective from this subsystem was to properly learn how WWise API works.

### Two background music that could blend between each other in a loop:

We did this by basically controlling both the music options in WWise. WWise has many functionalities, including one that lets you create playlist for your sounds, so it’s as easy as creating a sequence and telling Wwise how to play them and in which order. When you do this, as soon as you play the music in the engine, the functions from Wwise have the information on how to play them and in which order.

### Spatial audio

The main goal of this objective was to be able to listen the sound effects/music depending on the listener´s position. This is also done easily by the WWise API, but you have to do some code aswell. In the WWise API you have to go to the “Positioning” tab and check the 3D Spatialization aswell as the Attenuation. With this options, WWise knows how to play the music depending on the listener´s position. But this is not it, you have to go to the source code and implement a position and orientation for the listener, so WWise knows how to calculate the distance. We did this with a simple function that asks for 2 parameters: the UUID from the gameobject and it´s position. Once you call the function correctly, WWise will register the gameobject’s position and will know how to treat it.

### Extra funcionalities

- Sound options: Volume adn activate/desactivate components. This options control the audio so you don’t have to do it via code, volume ranges from 0 to 100.

## Video demo

This video demonstrates how the Audio System works, and it’s utilities.

## License

MIT License

Copyright (c) 2024 Arnau González Acosta and Pau Argiz Gutiérrez

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
