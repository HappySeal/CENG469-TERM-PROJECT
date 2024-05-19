//
// Created by Cafer SELLI on 11.05.2024.
//

#include "../headers/MedianCut.h"

float Summed_Region_Table::sum(int x0, int y0, int x1, int y1) const {
    return I(x1, y1) - I(x0, y1) - I(x1, y0) + I(x0, y0);
}

glm::vec3 UnSampleSphericalMap(glm::vec2 uv)
{
    uv -= 0.5;
    uv /= glm::vec2(0.1591, 0.3183);
    return glm::vec3(cos(uv.y) * cos(uv.x), sin(uv.y), cos(uv.y) * sin(uv.x));
}

void split_region(const Region& region, size_t n, std::vector<Region>& regions){
    if(region.width < 2 || region.height < 2 || n == 0){
        regions.push_back(region);
        return;
    }

    Region A, B;

    if(region.width > region.height) {
        region.split_w(A, B);
    }else{
        region.split_h(A, B);
    }

    split_region(A, n-1, regions);
    split_region(B, n-1, regions);
}

void get_light_color(const Summed_Region_Table& r_srt, const Summed_Region_Table& g_srt, const Summed_Region_Table& b_srt, const Region& region, glm::vec3& color){
    float r = r_srt.sum(region.startX, region.startY, region.startX + region.width, region.startY + region.height);
    float g = g_srt.sum(region.startX, region.startY, region.startX + region.width, region.startY + region.height);
    float b = b_srt.sum(region.startX, region.startY, region.startX + region.width, region.startY + region.height);

    float area = region.width * region.height;
    color = glm::vec3(r, g, b);
    std::cout << "Color: " << color.x << " " << color.y << " " << color.z << "|| Area: " << area << std::endl;
}


void create_lights(const std::vector<Region>& regions, std::vector<LightPoint>& lights, const Summed_Region_Table& r_srt, const Summed_Region_Table& g_srt, const Summed_Region_Table& b_srt, int width, int height){
    for(size_t i = 0; i < regions.size(); ++i){
        glm::vec2 centroid = regions[i].centroid();
        centroid.x /= (float)width;
        centroid.y /= (float)height;
        glm::vec3 worldPos = UnSampleSphericalMap(centroid);
        glm::vec3 color;

        get_light_color(r_srt, g_srt, b_srt, regions[i], color);

        LightPoint lp;
        lp.pos = worldPos;
        lp.color = color;
        lights.push_back(lp);
    }
}

void median_cut(const Summed_Region_Table& img, size_t n, std::vector<Region>& regions){
    regions.clear();

    Region r(0, 0, img.width_, img.height_, &img);

    split_region(r, n, regions);
}

MedianCut::MedianCut(const std::string& _texturePath) {
    iterationCount = 5;
    texturePath = _texturePath;

    // Load HDR and create skybox
}

void MedianCut::calculate() {
    numberOfLights = pow(2, iterationCount);
    regions.clear();
    lights.clear();

    stbi_set_flip_vertically_on_load(true);
    int _width, _height, nrComponents;
    float *data = stbi_loadf(texturePath.c_str(), &_width, &_height, &nrComponents, 0);

    Summed_Region_Table lum_srt;

    Summed_Region_Table r_srt;
    Summed_Region_Table g_srt;
    Summed_Region_Table b_srt;

    lum_srt.create_lum(data, _width, _height, nrComponents);
    r_srt.create_r(data, _width, _height, nrComponents);
    g_srt.create_g(data, _width, _height, nrComponents);
    b_srt.create_b(data, _width, _height, nrComponents);


    median_cut(lum_srt, iterationCount, regions);

    create_lights(regions, lights, r_srt, g_srt, b_srt, _width, _height);
}

void MedianCut::OnKeyInput(int key, int action) {
    if (key == GLFW_KEY_E && action == GLFW_PRESS) {
        if(iterationCount > 1){
            iterationCount--;
            calculate();
        }
    }

    if(key == GLFW_KEY_R && action == GLFW_PRESS){
        if(iterationCount < 7){
            iterationCount++;
            calculate();
        }
    }

//    std::cout << "iter:" << iterationCount << std::endl;
}
