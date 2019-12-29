## RainbowNeko
This is a 3D Game Engine created by Laia Martinez Motis and Alex Morales Garcia,  two Students at CITM (UPC) university for the 3D engines subject during 3rd year of Game Design and Game Development degree.

 <p align="center">
<img src= "https://github.com/AlexMG99/RainbowNeko-Engine/blob/master/docs/LogoB.png?raw=true" width="360" height="350">   
</p>

The code is written in C++.

Link to  Github Repository: [**NekoRainbow Engine**](https://github.com/AlexMG99/RainbowNeko-Engine).

![teampic](https://github.com/AlexMG99/RainbowNeko-Engine/blob/master/docs/Menu.png?raw=true)

## Team

 <p align="center">
<img src= "https://github.com/AlexMG99/RainbowNeko-Engine/blob/master/docs/teampic.jpeg?raw=true" width="360" height="430">   
</p>

### Laia Martínez Motis

* Link to [**Laia Martinez Motis**  ](https://github.com/LaiaMartinezMotis) 

      
	* Mouse Picking
	* Panel Game
	* Scene Guizmos
	* Engine Icons
	* Timer
	* Start, pause and tick 
	* ImGui panels


### Alex Morales Garcia

*  Link to [**Alex Morales Garcia** ](https://github.com/AlexMG99) 


        
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
	* ImGui panels

## Main Core Sub-Systems 

### GUI (Library ImGui)
In order to make our Engine easy to understand, we decided to use the ImGui library, which helped us through the Engine creation process. With it we created an easy-to-use UI, divided in different panels, each of them with their own functions.

 * Panel Configurations: Engine configurations.
 * Panel Hierarchy: Shows all the loaded GameObjects. You can select them by clicking in the name and also select the scene camera.
 * Panel Game: Shows game scene 
 * Panel Play: Shows scene through camera 
 * Panel Inspector: Shows information about that GameObject selected in the panel Hierarchy.
 * Panel Console: Prints Assimp Logs and Normal Logs
 * Panel Assets: Loads the floders and resources that are inside the Library folder.
 
 To give the user more interaction with the Engine, all the panels composition is made using the docking sistem which let the user change the panels organizationa, as he she wishes.

![docking](https://github.com/AlexMG99/RainbowNeko-Engine/blob/master/docs/docking.gif?raw=true)


### Assets Panel

Inside the Engine we have included a Panel where you can see all the assets that we have in our library folder:
* Models
* Meshes
* Scenes
* Textures

If you left-click on a floder, it will show the resources inside it. To use one of the shown resorces on the Panel Scene, you must left-click on the resource, then it will appear in the scene without the texture.
To put the texture on it you must have the texture folder opened, then select the object you want to punt the texture on and then left-click on the texture.  


![assetsmenu](https://github.com/AlexMG99/RainbowNeko-Engine/blob/master/docs/assetsmenu.gif?raw=true)

## User Interface Sub-system

For our last assigment we have done a Demo. 

When the **PLAY Button** is clicked, the UI is activated.
It consists of a Main Menu which includes a **Label** for the menu title, a **Button**  which lead the user to a new scene and an **Image** that contains the background image.

When the user presses the **Start Button** a new scene is loaded. In it, it appears the BakerHouse.fbx and the Crosshair in the middle of the screen.

![openpanel](https://github.com/AlexMG99/RainbowNeko-Engine/blob/master/docs/Engine.gif?raw=true)

When the user is in the loaded scene he/she can press **F1 Button**, and an optiions menu will show up.

In it the user will  find two  **Check Boxes**:

* The first one allows to activate or deactivate vsync.
* The second one allows to make the panel draggable.

To close the menu the user has to press the **F1 Button** again.

When the **STOP Button** is clicked, the scene will go to its original state.

![engine2](https://github.com/AlexMG99/RainbowNeko-Engine/blob/master/docs/Engine2.gif?raw=true)

## Engine Teaser

[![IMAGE ALT TEXT HERE](https://github.com/AlexMG99/RainbowNeko-Engine/blob/master/docs/LogoB.png?raw=true)](https://www.youtube.com/watch?v=UVvoXTUFyeU)

## Links

*  Link to [**Code Repository** ](https://github.com/AlexMG99/RainbowNeko-Engine) 
*  Link to [**Last Release** ](https://github.com/AlexMG99/RainbowNeko-Engine/releases) 

