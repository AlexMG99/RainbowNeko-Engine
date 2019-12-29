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

### Assets Panel

Inside the Engine we have included a Panel where you can see all the assets that we have in our library folder:
* Models
* Meshes
* Scenes
* Textures

If you left-click on a floder, it will show the resources inside it. To use one of the shown resorces on the Panel Scene, you must left-click on the resource, then it will appear in the scene without the texture.
To put the texture on it you must have the texture folder opened, then select the object you want to punt the texture on and then left-click on the texture.  


![assetsmenu](https://github.com/AlexMG99/RainbowNeko-Engine/blob/master/docs/assetsmenu.gif?raw=true)

### Frustum

## User Interface Sub-system
We have implemented a main menu that contains a fullscreen background image. 
As a child of the canvas we have an **Input Box** which request the testers name and a **Start Button** which can be pressed with the mouse or the keyboard.

As soon as the **Start Button** is pressed the fullscreen background and all that it contain fades away and show an 3D scene.
When the new scene is loaded pressing the button **F1** the teaster can open an options window. In it the teaster will find a **Check Box** which allow to activate or deactivate vsync.

![openpanel](https://github.com/AlexMG99/RainbowNeko-Engine/blob/master/docs/openpanel.gif?raw=true)
## Links

*  Link to [**Code Repository** ](https://github.com/AlexMG99/RainbowNeko-Engine) 
*  Link to [**Last Release** ](https://github.com/AlexMG99/RainbowNeko-Engine/releases) 

