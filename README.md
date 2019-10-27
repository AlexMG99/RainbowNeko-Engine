# 3D Game Engine: NekoRainbow-Engine

## Description 
This is a 3D Game Engine created by Laia Martinez Motis and Alex Morales Garcia,  two Students at CITM (UPC) university for the 3D engines subject during 3rd year of Game Design and Game Development degree.

Link to  Github Repository: [**NekoRainbow Engine**](https://github.com/AlexMG99/RainbowNeko-Engine).

## Authors Githubs
* Link to [**Laia Martinez Motis**  ](https://github.com/LaiaMartinezMotis) GithubPage.
*  Link to [**Alex Morales Garcia** ](https://github.com/AlexMG99) GithubPage.

## Game Engine Intructions
### Main Menu Bar
* **File**
	* _**Quit (Escape)**_: Exits the application
* **Help**: Links to the folllowing pages:
	* _**Github Repository**_. 
	* _**Documentation**_: Github README. 
	* _**Download latest**_: Latest engine version.
	* _**Report a bug**_.
	* _**About**_. 
* **Window**: Show/Hide Panels.
	* _**Console**_.
	* _**Configuration**_.
	*	_**Game**_.
	*	_**Hierarchy**_.
	*	_**Inspector**_.
        * _**Panel Shape**_.
### Configuration
* **Application**: Shows info about:
	* _**Project Name**_. 
	* _**Organization**_.
	* _**Max FPS**_: Cap FPS.
	* _**Framerate Plot**_.
	* _**Milliseconds Plot**_.
* **Window**: Window parameters:
	* _**Brightness**_.
	* _**Width and height**_.
	* _**Fullscreen/Fullscreen Desktop/Resizable/Borderless**_.    
	* _**Mouse position**_.
	* _**Mouse movement**_.
	* _**Mouse wheel**_.
	* _**Game panel mouse**_.
* **Hardware**: It shows the capabilities of the hardware.
* **Renderer**: Change backgroundcolor and activate/deactivate glRender properties and change view between POLYGON, WIREFRAME and VERTEX.

### Hierarchy
The panel Hierarchy shows al the loaded GameObjects. You can select them by clicking in the name.

### Inspector
The panel Inspector shows information about that GameObject selected in the panel Hierarchy.
* **Name**: Name of the object selected.
* **Transform**: Shows info about:
	* _**Position of the object**_. 
	* _**Scale of the object**_.
	* _**Rotation of the object**_:.
	
* **Mesh**: Clicking in the check boxes you can:
	* _**Activate or deactivate the mesh**_. 
	* _**Activate or deactivate the normal vertex**_.
	* _**Activate or deactivate the normal face**_:.
	    
	    
	
* **Mesh**: Shows info about:
	* _**Id Vertices**_. 
	* _**Number of Vertices**_.
	* _**Id Indices**_:. 
	* _**Id Indices**_. 
	* _**Number of Indices**_.
	* _**Id Uv**_.

* **Texture**: Clicking in the check box you can:
	* _**Activate or deactivate the texture**_. 
	
* **Texture**: Clicking in the check box you can:
	* _**Path of the texture**_.
	* _**Id of the texture**_:. 
	* _**Width and Height of the texture**_. 
	* _**Number of Indices**_.
	* _**Image of the texture**_.
		
### Shape Creator
The panel Shape Creator allows you to create 5 types of shapes:
* **Saphes**:
	* _**Cube**_.
	* _**Sphere**_. 
	* _**Cylinder**_. 
	* _**Cone**_.
	* _**Plane**_.
	

### Console
The panel Console prints: 
* **Prints**:
	* _**Assimp Logs**_.
	* _**Normal Logs**_. 
	

## Game Engine Controls
* **Camera Movement**:
	* _**While right clicking, and using "WASD" movement and free look around is enabled**_.
* **Zoom In/ Zoom Out**:
	* _**Mouse Wheel**_.
* **F key**:
	* _**Focus the camera around the object selected**_.
* **Alt+Left**:
	* _**Orbit the object**_.
* **Supr**:
	* _**Delete the selected GameObject and childrens**_.
* **C key**:
	* _**Centers the camera**_.


## Innovations
Innovations implemented in our Engine: 
* **Code**:
	* _**Render screen as a game panel**_.
	* _**Created a DockingSpace which allows us to change the panels position**_.
* **Style**:
	* _**Created two types of cursor icons (eye and hand)**_.
	* _**Created our Engine style and applied to it**_.



## Libraries Used
Libraries used in our Engine:
* **SDL 2.0**
* **ImGui 1.73**
* **Glew 2.1.0**
* **OpenGL**
* **MatGeoLib 1.5**
* **PCG 0.98.1**
* **Parson**
* **Devil**
* **Assimp**
* **Par Shapes**
* **Deviceid**
* **mmgr**



## License

MIT License

Copyright (c) 2018 @LaiaMartinezMotis (Laia Martinez Motis) and @AlexMG99 (Alex Morales Garcia)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

