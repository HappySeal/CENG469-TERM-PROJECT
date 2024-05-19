//
// Created by Cafer SELLI on 11.05.2024.
//

#ifndef SAMPLEGL_MEDIANCUT_H
#define SAMPLEGL_MEDIANCUT_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <string>
#include "../headers/stb_image.h"
#include "../interfaces/IInputListener.h"
#include <cassert>
#include <glm/glm.hpp> // GL Math library header
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Summed_Region_Table{
public:
    int width_;
    int height_;
    std::vector<float> srt_;
    float I(int x, int y) const{
        if (x < 0 || y < 0) return 0;
        size_t i = y*width_ + x;
        return srt_[i];
    }
    void create_lum(float* rgb, int width, int height, int nc){
        assert(nc > 2);
        width_ = width;
        height_ = height;
        srt_.clear();
        srt_.resize(width_ * height_);
        for (int y = 0; y < height_; ++y)
        for (int x = 0; x < width_;  ++x)
        {
            size_t i = y*width_ + x;
            float r = rgb[(y*width_ + x)*nc + 0];
            float g = rgb[(y*width_ + x)*nc + 1];
            float b = rgb[(y*width_ + x)*nc + 2];
            float lum = r*0.2125f + g*0.7154f + b*0.0721f;
            srt_[i] = lum + I(x, y-1) + I(x-1, y) - I(x-1, y-1);
        }
    }

    void create_r(float* rgb, int width, int height, int nc){
        assert(nc > 2);
        width_ = width;
        height_ = height;
        srt_.clear();
        srt_.resize(width_ * height_);
        for (int y = 0; y < height_; ++y)
        for (int x = 0; x < width_;  ++x)
        {
            size_t i = y*width_ + x;
            float r = rgb[(y*width_ + x)*nc + 0] * abs(cos(atan2((y - height_), (x - width_))));
            srt_[i] = r + I(x, y-1) + I(x-1, y) - I(x-1, y-1);
        }
    }

    void create_g(float* rgb, int width, int height, int nc){
        assert(nc > 2);
        width_ = width;
        height_ = height;
        srt_.clear();
        srt_.resize(width_ * height_);
        for (int y = 0; y < height_; ++y)
        for (int x = 0; x < width_;  ++x)
        {
            size_t i = y*width_ + x;
            float g = rgb[(y*width_ + x)*nc + 1] * abs(cos(atan2((y - height_), (x - width_))));
            srt_[i] = g + I(x, y-1) + I(x-1, y) - I(x-1, y-1);
        }
    }

    void create_b(float* rgb, int width, int height, int nc){
        assert(nc > 2);
        width_ = width;
        height_ = height;
        srt_.clear();
        srt_.resize(width_ * height_);
        for (int y = 0; y < height_; ++y)
        for (int x = 0; x < width_;  ++x)
        {
            size_t i = y*width_ + x;
            float b = rgb[(y*width_ + x)*nc + 2] * abs(cos(atan2((y - height_), (x - width_))));
            srt_[i] = b + I(x, y-1) + I(x-1, y) - I(x-1, y-1);
        }
    }

    float sum(int x0, int y0, int x1, int y1) const;
};

struct Region {
    int startX;
    int startY;
    int width;
    int height;

    float sum;
    const Summed_Region_Table* srt;

    Region(int _startX, int _startY, int _width, int _height, const Summed_Region_Table* _srt, float initial_sum = -1) {
        startX = _startX;
        startY = _startY;
        width = _width;
        height = _height;
        srt = _srt;
        sum = initial_sum;
        if(sum < 0){
            sum = srt->sum(startX, startY, startX + width - 1, startY + height - 1);
        }
    }

    void split_w(Region& A) const{
        for(size_t w = 1; w <= width; ++w){
            A = Region(startX, startY, w, height, srt);

            if(A.sum*2.0f > sum){
                break;
            }
        }
    }

    void split_w(Region& A, Region& B) const{
        split_w(A);
        B = Region(startX + A.width - 1, startY, width - A.width, height, srt, sum - A.sum);
    }

    void split_h(Region& A) const{
        for(size_t h = 1; h <= height; ++h){
            A = Region(startX, startY, width, h, srt);

            if(A.sum*2.0f > sum){
                break;
            }
        }
    }

    void split_h(Region& A, Region& B) const{
        split_h(A);
        B = Region(startX, startY + A.height - 1, width, height - A.height, srt, sum - A.sum);
    }

    glm::vec2 centroid() const{
        glm::vec2 ret;

        Region A;
        split_w(A);
        ret.x = A.startX + (A.width - 1);

        split_h(A);
        ret.y = A.startY + (A.height - 1);

        return ret;
    }

    Region(){
        startX = 0;
        startY = 0;
        width = 0;
        height = 0;
        sum = 0;
        srt = nullptr;
    }
};

struct LightPoint{
    glm::vec3 pos;
    glm::vec3 color;
};

class MedianCut: public IInputListener {
public:
    int iterationCount;
    int numberOfLights;
    const int maxIterCount = 7;
    std::string texturePath;
    std::vector<Region> regions;
    std::vector<LightPoint> lights;


    MedianCut(const std::string& _texturePath);
    void calculate();
    void OnKeyInput(int key, int action) override;
};

#endif //SAMPLEGL_MEDIANCUT_H
