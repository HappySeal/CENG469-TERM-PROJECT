//
// Created by Cafer SELLI on 11.05.2024.
//

#ifndef SAMPLEGL_MEDIANCUT_H
#define SAMPLEGL_MEDIANCUT_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "../headers/stb_image.h"
#include "../interfaces/IInputListener.h"
#include <cassert>
#include <glm/glm.hpp> // GL Math library header
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Region {
    int startX;
    int startY;
    int endX;
    int endY;
    int centroidIndex;
    Region(int _startX, int _startY, int _endX, int _endY, int _centroidIndex) {
        startX = _startX;
        startY = _startY;
        endX = _endX;
        endY = _endY;
        centroidIndex = _centroidIndex;
    }
};



class MedianCut: public IInputListener {
public:
    int width_, height_;
    int iterationCount;
    std::vector<Region> regions;


    MedianCut(const std::string& texturePath);
    void OnKeyInput(int key, int action) override;
};

#endif //SAMPLEGL_MEDIANCUT_H
