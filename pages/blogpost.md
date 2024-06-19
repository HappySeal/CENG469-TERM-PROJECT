# CENG469 - TERM PROJECT
Contributors: Cafer Selli, Elif Ecem Ümütlü

Hi everyone, we are excited to share our term project for Computer Graphics II course at Middle East Technical University in this blog post. The project is about wormhole implementation based on the paper named "Visualizing Interstellar's Wormhole". (link can be found [here](https://www.researchgate.net/publication/272195706_Visualizing_Interstellar's_Wormhole)). The purpose of the project is to replicate the behaviour of the wormhole to create a wormhole-like effect on the surfaces, spaces.

## Project Overview
There are several aspects of the project:
- Use equilateral image to replicate a never-ending space
- Shader implementation of the wormhole
- Allow user interaction through mouse and keyboard input as well as through wormhole settings UI


## Equilateral Image to Cubemap
In this section, we focused on converting an equilateral image into a cubemap to create a seamless environment that gives the illusion of an endless space. The cubemap allows us to map a 360-degree image onto a cube, which can then be used to create reflections and background scenery in our wormhole effect. By using an equilateral image, we ensure that the transitions between the faces of the cube are smooth and continuous, enhancing the immersive experience.

Here rather than caculating each 6 faces of the cube one by one, we do the calculations on the shader. In our implementation, camera never moves. The rays from the camera follows a path calculated by the wormhole function. Ray tracing is utilized to obtain the pixel value from the other universe. Moreover, in each draw call, a recalculation with ray tracing is necessary since the camera moves between the two universes. Hence, we obtain the corresponding cubemap value using ray tracing and wormhole function in the shaders.

## Shader Implementation of the Wormhole
The core of our project lies in the shader implementation. The fragment shader is responsible for rendering the wormhole effect by manipulating texture coordinates and applying complex mathematical transformations.

<img src="images/wormhole1.png">

In the above image, there are the calculations of the approximation of the wormhole. Think of wormhole as a cyclinder whose two ends are two different universes, or spaces of shape sphere with a radius of ρ. Additionally, we have a simple cylinder of length 2a in the middle as the throat of the wormhole. "r" represents the radial distance from the center of the cylinder to end of the sphere. 

To obtain a smooth transition view of the other side, we need to smooth the intersection of cylinder with the spheres. Such an effect can be achieved by expanding the ends of the cylinder towards the spheres. Through utilizing the function below, such an effect is achieved and applied to the ray direction calculations.

<img src="images/wormhole2.png"> 

As indicated in the paper, to catch a realistic wormhole effect, there should be gravitational lensing (distortions) close to the intersections. Consequently, the smooth transitions are necessary. The below image depicts the wormhole with and without distortions. The desired look is the second one.

<img src="images/wormhole3.png"> 


## User Interaction and Camera Movements


