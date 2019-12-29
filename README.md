# 3D Game Engine: NekoRainbow-Engine

## Description 
This is a 3D Game Engine created by Laia Martinez Motis and Alex Morales Garcia,  two Students at CITM (UPC) university for the 3D engines subject during 3rd year of Game Design and Game Development degree.

Link to  Github Repository: [**NekoRainbow Engine**](https://github.com/AlexMG99/RainbowNeko-Engine).
Link to  Github Web:[**Link to NekoRainbow Web**](https://alexmg99.github.io/RainbowNeko-Engine/)

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
	* _**Game**_.
	* _**Hierarchy**_.
	* _**Inspector**_.
   * _**Panel Shape**_.
   * _**Assets**_.
   * _**Play**_.



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
* **Camera Setting**: Camera parameters:
	* _**Near Plane**_.
	* _**Far Plane**_.
	* _**FOV**_.
                            
	 

### Hierarchy
The panel Hierarchy shows all the loaded GameObjects. You can select them by clicking in the name.
Also you can select the scene camera.


### Game
Shows game scene 

* **Check Boxes**: 
	* _**Activate/Deactivate Grid**_. 
	* _**Activate/Deactivate Fill**_.
	* _**Activate/Deactivate Points**_:.
	* _**Activate/Deactivate Camera Culling**_. 
	* _**Activate/Deactivate Gizmos**_.
	
### Play
Shows scene through camera 

* **Buttons**: 
	* _**Play/Stop button: Starts/Stops/Resets Game Timer**_.  
	* _**Continue/Pause button: Continue or Pauses Game Timer**_.
	* _**One Frame button: Adds one frame to the paused Game Timer**_:.
* **Timer**: 
	* _**Real Timer: Starts when we start debugging**_.
	* _**Game Timer: Starts when Start Button is clicked**_:.
       

### Inspector
The panel Inspector shows information about that GameObject selected in the panel Hierarchy.
* **Check Box**: Name of the GameObject selected.

* **Check Box**: Static or non-static object. Creates Quadtree.

* **Transform**: Shows info about:
	* _**Position of the object**_. 
	* _**Scale of the object**_.
	* _**Rotation of the object**_:.
	
* **Mesh**: Clicking in the check boxes you can:
	* _**Activate or deactivate the mesh**_. 
  * _**Activate or deactivate the normal vertex**_.
  * _**Activate or deactivate the normal face**_:.

* **Mesh**: It shows information about:
	* _**Resource Mesh ID**_. 
	* _**ID Vertex**_.
	* _**Number of Vertex**_:.
	* _**ID Index**_.
	* _**Number of index**_:.
	* _**ID Uv**_:.
        
	
* **Mesh**: Bounding Boxes check box:
	* _**Show AABB**_.
        * _**Show OBB**_. 
      	
* **Mesh**: Mode:
       
	* _**Change wireframe color**_.
        * _**Change vertex color**_. 
      	
	

* **Texture**: Clicking in the check box you can:
	* _**Activate or deactivate the texture**_. 
	
* **Texture**: Clicking in the check box you can:
        * _**Resource Texture ID**_.
	* _**Path of the texture**_.
	* _**Id of the texture**_:. 
	* _**Width and Height of the texture**_. 
	* _**Image of the texture**_.
		
### Shape Creator
The panel Shape Creator allows you to create 5 types of shapes:
* **Saphes**:
	* _**Cube**_.
	* _**Sphere**_. 
	* _**Cylinder**_. 
	* _**Cone**_.
	* _**Plane**_.
        * _**Torus**_.
	* _**Octahedron**_.


### Console
The panel Console prints: 
* **Prints**:
	* _**Assimp Logs**_.
	* _**Normal Logs**_. 
	

### Assets
The panel loads the floders and resources that are inside the Library folder.
If you left-click on a floder, it will show the resources inside it. To use one of the shown resorces on the Panel Scene, you must left-click on the resource, then it will appear in the scene without the texture.
To put the texture on it you must have the texture folder opened, then select the object you want to punt the texture on and then left-click on the texture.  

### UI
The UI is activated when clicked PLAY button. For this Assignment we have done a Demo. The Demo consists of a Main Menu, whith a label for the menu title, and a butoon start which leads the user to a new scene. In this new scene, the BakerHouse.fbx is loaded and appears a Crosshair in the middle of the screen. When pressed F1, a options menu is opened, where the player can activate/deactivate Vysnc and dragable UI. When F1 or the close button is clicked the menu will close. When the STOP button is clicked, the scene will go to his original state.



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
* **Right Mouse Click on scene object**:
	* _**Selects GameObject**_.
* **F1**:
	* _**Activates/Deactivates UI**_.

## Guizmo Controls
When a GameObject in the scene is selected:
* **W key**:
	* _**Translate GameObject**_.
* **E key**:
	* _**Rotate GameObject**_.
* **R key**:
	* _**Scale GameObject**_.
* **X key**:
	* _**Changes to World**_.
* **D key**:
	* _**Changes to Local**_.


## Innovations
Innovations implemented in our Engine: 
* **Code**:
	* _**Render screen as a game panel**_.
	* _**Created a DockingSpace which allows us to change the panels position**_.
* **Style**:
	* _**Created two types of cursor icons (eye and hand)**_.
	* _**Created our Engine style and applied to it**_.
* **Panel Assets**:
	* _**Panel that loads the resources inside the Library folder**_.
	* _**Left_Click on the resources to place them in the panel scene to use them**_.
        * _**Load the resources texture, selecting the object in the scene and then left_clicking on the texture**_.


## Tasks by Member Assigment 2

* Laia Martinez:

	* Mouse Picking
	* Panel Game
	* Scene Guizmos
	* Engine Icons
	* Timer
	* Start, pause and tick 
	
	

* Alex Morales:

        * Quadtree
	* AABB and OBB
	* Camera as component
	* Frustum & Frustum Culling
	* Resource Manager
        * Resizing
        * Own Format
	* Serialize scene
	* Library created again if it doesn't exists
        * Panel Assets
 

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
* **ImGuizmo**


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

