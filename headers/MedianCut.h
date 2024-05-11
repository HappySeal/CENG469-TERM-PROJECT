//
// Created by Cafer SELLI on 11.05.2024.
//

#ifndef SAMPLEGL_MEDIANCUT_H
#define SAMPLEGL_MEDIANCUT_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

class MedianCut {
public:
    unsigned int captureFBO, captureRBO;
    int width, height;

    void medianCut();
};

#endif //SAMPLEGL_MEDIANCUT_H
