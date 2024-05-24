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

<p align="center">
<img alt="Beginning" height="75%" src="./images/image1.png" width="75%"/>
</p>
Here in the first picture I first implemented a simple pyramid
<p align="center">

<img alt="Beginning" height="75%" src="./images/image2.png" width="75%"/>
</p>
Here I also implemented some really crude lighting system. This might seem like nothing related to the homework, but in order to create something great sometimes you need to start from the basics.

### Journeys middle...

After implementing some camera controls, shading and better model loading system, I started to implement the skybox since it was the main part of the project.

However, I had some problems with the skybox. I couldn't get the skybox to work properly. I was able to load the skybox texture but I couldn't get it to render properly. The problem was that I was using the wrong shader for the skybox. I was using the object shader instead of the skybox shader. After fixing this problem, I was able to render the skybox properly.

Here is the first picure of the skybox:

<p align="center">
<img alt="Skybox" height="75%" src="./images/image3.png" width="75%"/>
</p>

As you can see in the image above, the skybox is rendered properly. The skybox is a simple cube with a texture on it. The texture is an HDR image that is loaded from the disk. The skybox is rendered in the background of the scene and the object was a perfect mirror. However this mirror was not using the correct exposure values, therefore the mirror was too dim. This was fixed in the next steps.

<p align="center">

<img src="./images/image4.png" alt="Mirror" height="75%" width="75%"/>

</p>

Here as you can see, the exposure system of the skybox is implemented, but still the sphere in the middle was not using correct color for the reflections. This was fixed in the later steps.

<p align="center">

<img src="./images/image5.png" alt="Mirror" height="75%" width="75%"/>

</p>

Here as you can see, the sphere in the middle is using the correct color for the reflections. The mirror is now perfect. The exposure system is also implemented. The mirror is now using the correct exposure values and the mirror is now perfect.

After this I rapidly started working on the median cut algorithm and started experimenting using Python. C++ was not a best place to test and implement the algorithm mention in the paper. Since there is too much boilerplate code is needed in the C++, I prototype anything quickly in Python. After properly creating the base algorithm for the median cut algorithm then implement it in C++.

Here is the first picture of the median cut algorithm:

<p align="center">

<img src="./images/image6.png" alt="Median Cut" height="75%" width="75%"/>

</p>

Here is the first implementation of the algorithm. In this picture algorthim quantize the image into several colors and shows the output colors with quantized image. This was not working properly, later I fixed the algorithm and implemented a base version of it to the main project. However it was still not able to show it in the scene, since I had no idea to how to show it. Therefore first I calculated the regions and position of the centroids and passed that data to a Python visualizer.

Here is the first picture of the visualizer:

<p align="center">

<img src="./images/image7.png" alt="Visualizer" height="75%" width="75%"/>

</p>

In this image we can see the regions calculated by the C++ code that is visualized in python using matplotlib. This was a good way to debug the algorithm and see the output of the algorithm. This was a good way to see the output of the algorithm and see if it is working properly. This was a huge relief for me. After seeing this result I started to think how can I implement the light probe algorithm in the main project.

First I was very confused about how should I place the lights in the world. I was thinking that I should place the lights in the world as a point light. However, after asking the TA, I learned that the lights should be placed as a directional light. This was a game changer for me. After this I was able to implement the light probe algorithm in the main project.

Here is the first picture of the light probe algorithm:

<p align="center">

<img src="./images/image8.png" alt="Light Probe" height="75%" width="75%"/>

</p>

Here as you can see they are extremely bright. In order to tone them down, I had to multiply the light color with a constant. This was not a good way to make the lights look more feasible but in the end it worked.

<p align="center">

<img src="./images/image9.png" alt="Light Probe" height="75%" width="75%"/>

</p>

Here is the final disco mode result.

After implementing the big parts of the project, rest was a piece of cake. I was able to implement the glass and glossy modes in a short amount of time. The glass mode was a simple refraction shader that uses the refract function in the shader to query the skybox texture. The glossy mode was a simple reflection shader that uses the reflect calculation that is mention in the lecture slides in the shader to query the skybox texture.

<p align="center">

<img src="./images/image10.png" alt="Glass" height="75%" width="75%"/>

</p>

Here is also the final version of the light probe mode

<p align="center">

<img src="./images/image11.png" alt="Glossy" height="75%" width="75%"/>

</p>

And that was it. Project was finished and I stared to test it in some different environments. Here is a collage of the project in different environments:
<table>
  <tr>
    <td align="center" width="50%">
      <img src="./images/image12.png" alt="Collage" width="100%">
    </td>
    <td align="center" width="50%">
      <img src="./images/image13.png" alt="Collage" width="100%">
    </td>
  </tr>
  <tr>
    <td align="center" width="50%">
      <img src="./images/image14.png" alt="Collage" width="100%">
    </td>
    <td align="center" width="50%">
      <img src="./images/image15.png" alt="Collage" width="100%">
    </td>
  </tr>
  <tr>
    <td align="center" width="50%">
      <img src="./images/image16.png" alt="Collage" width="100%">
    </td>
    <td align="center" width="50%">
      <img src="./images/image17.png" alt="Collage" width="100%">
    </td>
  </tr>
  <tr>
    <td align="center" width="50%">
      <img src="./images/image18.png" alt="Collage" width="100%">
    </td>
    <td align="center" width="50%">
      <img src="./images/image19.png" alt="Collage" width="100%">
    </td>
  </tr>
  <tr>
    <td align="center" width="50%">
      <img src="./images/image20.png" alt="Collage" width="100%">
    </td>
    <td align="center" width="50%">
      <img src="./images/image21.png" alt="Collage" width="100%">
    </td>
  </tr>
</table>

### Performance Tests about Skybox

I also did some performance tests about the skybox. I tested the skybox with different resolutions. Here are the results of the performance tests:

First I started adjusting the resolution of the skybox to 512x512, 2048x2048 and 4096x4096. Here are the results of the performance tests:

<table>
<tr>
    <th>Resolution (px * px)</th>
    <th>Time (ms/frame)</th>
  </tr>
  <tr>
    <td>512x512</td>
    <td>8.33</td>
  </tr>

  <tr>
    <td>2048x2048</td>
    <td>8.33</td>
  </tr>

  <tr>
    <td>4096x4096</td>
    <td>8.33</td>
  </tr>
</table>

The resolution of the skybox didn't affect the performance of the project. This was a very interesting result. I was expecting the performance to decrease as the resolution of the skybox increased. However, this was not the case. Then I realized that the OpenGL was capping the framerate by vsync. I need to add `glfwSwapInterval(0);` to set bufferswap interval to immediately to see the real performance. I disabled the vsync and tested the project again. Here are the results of the performance tests:

<table>
<tr>
    <th>Resolution (px * px)</th>
    <th>Time (ms/frame)</th>
  </tr>
  <tr>
    <td>512x512</td>
    <td>0.83</td>
  </tr>

  <tr>
    <td>2048x2048</td>
    <td>1.83</td>
  </tr>

  <tr>
    <td>4096x4096</td>
    <td>4.15</td>
  </tr>
</table>

As you can see as the resolution of the skybox increased, the performance of the projects was declining. However as the resolution going down skybox was being more and more ugly... Therefore I decided to use 2048x2048 resolution for the skybox. Also increasing the skybox resolution was also limited by the resolution of the hdr image we are providing. Anythin bigger than the hdr image was not making any sense.

### Refraction Tests

I also did some refraction tests. I tested the refraction of different materials like water, glass and diamond. Here are the results of the refraction tests:

<table>
  <tr>
    <td align="center" width="33%">
      <img src="./images/image23.png" alt="Collage" width="100%">
    </td>
    <td align="center" width="33%">
      <img src="./images/image22.png" alt="Collage" width="100%">
    </td>
    <td align="center" width="33%">
      <img src="./images/image10.png" alt="Collage" width="100%">
    </td>
  </tr>
</table>


As you can see in the images above (from left to right), the refraction of the diamond, glass and water. As the refractive index is going up, material was almost acting like a invisble object that just flips the image. This was an expected result, since according to the snell's law, the refractive index of the material is directly proportional to the refraction angle.

### Journeys end...

In the end, I was able to implement the project successfully. I was able to implement the skybox, median cut algorithm, light probe algorithm, glass, glossy and mirror modes

This project was a great learning experience for me. I learned a lot about OpenGL, environment mapping, HDR tone mapping, reflectance, and refraction. I also learned a lot about debugging and problem-solving. I faced several challenges during the implementation, but I was able to overcome them through perseverance and creativity. I also learned the importance of asking for help and seeking guidance when stuck.
