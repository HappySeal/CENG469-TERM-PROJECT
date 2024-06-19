# CENG 469 - TERM Project
This project is done for the course CENG 469 - Introduction to Computer Graphics at METU. The project is a simple 3D scene with a rotating cube and a rotating sphere. The scene is rendered using OpenGL and GLSL.

Full blogpost can be found [here](./pages/blogpost.md)

## How to Run
The project is developed using CMake. To run the project, you need to have CMake installed on your system. You can install CMake using the following command:
```bash
make all
./app
```
To run the project with Makefile, you need to use the following commands:
```bash
make
./main
```

## Controls
- `A`, `D`: Move the camera backward, and forward, respectively. Change camL, the distance of the camera from the center of the wormhole.
- `W`, `S`: Increase and decrease wormhole length, respectively.
- `Q`, `E`:  Decrease and increase wormhole smoothness, respectively.
- `Z`, `X`:  Decrease and increase iteration of differentiation, respectively.
- `ESC`: Close the window
