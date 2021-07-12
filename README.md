# cel-shader
## Description
A cel-shading effect applied to .obj models written in OpenGL and C++.  
Uses two types of cel-shading: a naive approach using the diffuse light component and an extended toon shader (known as "x-toon" shading).

## How to Build
```
cd part1
python3 build.py
```

## How to Run
This project requires an object file (.obj) and a texture file (.ppm). The texture file is used for the x-toon shading. For example:
``` 
./project.exe ../common/objects/bunny2.obj ../common/textures/back/backlight1.ppm
```

## Interactivity
* Press the R key to rotate the object  
* Press the W and S keys to move the camera up and down  
* Scroll wheel zooms in and out of the object  
* Press X to toggle x-toon shading with the given texture  
* Press O to toggle the outline  
* Press D to toggle the y-coordinate of the texture mapping (this alters the x-toon shading effect)


