# CENG469 - HW2 IBL & Shaders

Hey everyone! I'm excited to share my journey with a cool project for my Computer Graphics II course at Middle East Technical University in this blog post. The project was all about diving into the world of environment mapping, HDR tone mapping, reflectance, and refraction using OpenGL. These techniques are super important for making the lighting in computer graphics look realistic and awesome. So, let's jump right in and see what this project was all about!

### Project Overview

The primary objective of this project was to create an OpenGL program that allows user interaction through keyboard and mouse inputs and renders a scene with a central object illuminated by an environment map. The project included several visualization modes, such as LightProbe, Mirror, Glass, Glossy, and SpecularDisco, each offering a unique rendering effect.

#### Environment Mapping and Skybox

The scene setup involved creating a textured HDR skybox that surrounds the scene and serves as the illumination source for the central object at the center. An HDR equirectangular image was read from the disk, and six cubemap face textures were generated mathematically.

#### HDR Image Processing and Tone Mapping

The rendering process was performed in HDR, using a floating-point framebuffer texture. The skybox and central object shaders returned log() HDR colors. The final draw step involved tone-mapping the HDR scene for display on LDR monitors, using an exp() function and a basic tone mapping operator.

#### Light Probe Sampling

The environment's illumination was calculated using the Median Cut Algorithm for Light Probe Sampling. This algorithm generated directional light positions and radiances, which were then used to illuminate the central object in OpenGL's 3D world.

#### Glossy Rendering Technique

The Glossy mode added a reflective component on top of the diffuse lighting calculated in LightProbe mode. By adjusting the mirrorness parameter, the central object exhibited a blend of reflective and diffuse properties, creating a more realistic and visually appealing effect.

#### Glass Rendering Technique

For the Glass mode, the central object was rendered to simulate a glass-like appearance using a refraction technique. This involved using the refract function in the shader to query the skybox texture, creating the effect of light passing through the object. Adjustments to the transmissive parameter (kt) allowed for variations in the glass’s opacity and darkness.

#### Mirror Rendering Technique

In Mirror mode, the central object was rendered as a perfect mirror. The shader used the reflect function to sample the skybox texture, creating a reflection effect. This mode did not involve diffuse or specular lighting, relying solely on the reflection vector to produce the mirror-like appearance.

### Challenges and Solutions

During the implementation, I ran into several issues that required some creative problem-solving. First off, the paper didn’t explain how to calculate the median of a region, so I tried sorting the pixels to find the median, which didn’t work out at all. To make matters worse, I had no easy way to test my outputs from the median cut algorithm. To tackle this, I wrote a Python tester that takes the base HDR photo and draws the regions and their centroids. This really helped me pinpoint what was wrong with my code.

Another tricky part was figuring out the light color of a region. The project guide and the paper were both pretty vague on this. My first idea was to sum all the pixels in a region and multiply each by its pixel inclination. This made the lights way too bright. Since I couldn’t find a better solution, I ended up adjusting their brightness arbitrarily. While this made my results a bit different from the demo video, the overall effect was still there.

The position of the light was also a big question mark for me. Creating a 3D light source from a 2D skymap was confusing, especially figuring out the distance of the light from the origin. Thankfully, after asking on the forum, our awesome TA explained that the light source should be directional, not a point light. This was a game-changer and helped me finish the light probe shader.

### Journeys beginning...

When I started this project, I was very frustrated about HW1. Because I did little to zero progress in that homework I was very hesitant about this one too. Because of this, in order to have a clean start, I decided to start from scratch. I deleted all the files and started to implement the project from the beginning. This was a good decision because I was able to understand the project better and implement it more efficiently. I learned the OpenGL from the ground up and learned the basics again. This was a good refresher for me.

Here there are some beginning screenshots of the project:

<img alt="Beginning" height="75%" src="./images/image1.png" width="75%"/>

Here in the first picture I first implemented a simple pyramid

<img alt="Beginning" height="75%" src="./images/image2.png" width="75%"/>

Here I also implemented some really crude lighting system. This might seem like nothing related to the homework, but in order to create something great sometimes you need to start from the basics.

### Journeys middle...

After implementing camera controls and some more basics shading utilities with better model loading I started to implement the main part of the project. I first implemented the LightProbe mode. This was the most challenging part of the project. I had to implement the median cut algorithm and the light probe sampling. I had to read the HDR image and create the cubemap textures. I had to implement the shader for the light probe. This was a very challenging part of the project. I had to read the paper multiple times and understand the algorithm. I had to implement the algorithm and debug it. I had to implement the shader and debug it. I had to implement the cubemap textures and debug them. This was a very challenging part of the project. But after a lot of debugging and testing, I was able to implement the LightProbe mode.



### Journeys end...

