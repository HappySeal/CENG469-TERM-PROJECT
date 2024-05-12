//
// Created by Cafer SELLI on 11.05.2024.
//

#include "../headers/MedianCut.h"

//#ifndef STB_IMAGE_IMPLEMENTATION
//#define STB_IMAGE_IMPLEMENTATION
//#endif

struct Luxel {
    float lum;
    int index;
    Luxel(float _lum, int _index) {
        lum = _lum;
        index = _index;
    }
    Luxel() {
        lum = 0;
        index = 0;
    }
};

struct{
    bool operator()(Luxel a, Luxel b) const
    {
        return a.lum < b.lum;
    }
} customLess;

std::vector<Luxel> splitVector(const std::vector<Luxel>& data, Region& region, int width) {
    std::vector<Luxel> subvector = std::vector<Luxel>();

    for (int i = region.startY; i < region.endY; ++i) {
        // Calculate starting index in the original vector
        int startIndex = i * width + region.startX;
        // Calculate ending index in the original vector
        int endIndex = i * width + region.endX;
        // Append the elements from startIndex to endIndex to the subvector
        subvector.insert(subvector.end(), data.begin() + startIndex, data.begin() + endIndex + 1);
    }
    return subvector;
}

int findMedianIndex(const std::vector<Luxel>& data) {
    std::vector<Luxel> sortedData = std::vector<Luxel>(data);
    std::sort(sortedData.begin(), sortedData.end(), customLess);
    Luxel median = sortedData[sortedData.size() / 2];
    //find index of median in subvector
    return median.index;
}

MedianCut::MedianCut(const std::string& texturePath) {
    iterationCount = 1;

    // Load HDR and create skybox
    stbi_set_flip_vertically_on_load(true);
    int _width, _height, nrComponents;
    float *data = stbi_loadf(texturePath.c_str(), &_width, &_height, &nrComponents, 0);
    std::vector<Luxel> lum_data = std::vector<Luxel>(_width * _height);
    unsigned int hdrTexture;
    if (data)
    {
        glGenTextures(1, &hdrTexture);
        glBindTexture(GL_TEXTURE_2D, hdrTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, _width, _height, 0, GL_RGB, GL_FLOAT, data); // note how we specify the texture's data value to be float

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        std::cout << "Failed to load HDR image." << std::endl;
        std::exit(-1);
    }


//    //calculate time to take median cut
//    auto start = std::chrono::high_resolution_clock::now();
//    auto img = std::vector<float>(data, data + _width * _height * nrComponents);
//    scaleByCosPhi(img, _width, _height);
    // Convert to luminance
    for (int i = 0; i < _height*_width; ++i) {
        lum_data[i] = Luxel(0.2126f * data[i*3] + 0.7152f * data[i*3+1] + 0.0722f * data[i*3+2], i);
    }

    stbi_image_free(data);

    regions.push_back(Region(0, 0, _width, _height, findMedianIndex(lum_data)));

    // Median cut
    while (regions.size() < pow(2, iterationCount)){ // NOLINT(*-narrowing-conversions)
        std::vector<Region> newRegions;
        for (Region region : regions) {
            int width = region.endX - region.startX;
            int height = region.endY - region.startY;

            int medianIndex = region.centroidIndex;

            int medianIndexX = medianIndex % _width;
            int medianIndexY = medianIndex / _width;

            float inclination = abs(((float)(region.startY + region.endY) / 2.0f - (float)_height / 2.0f) / ((float)(region.startX + region.endX) / 2.0f - (float)_width / 2.0f));

            // Split the region
            if ((float)width * inclination > (float)height) {
                Region newRegion1(region.startX, region.startY, medianIndexX, region.endY, 0);
                Region newRegion2(medianIndexX + 1, region.startY, region.endX, region.endY, 0);


                newRegion1.centroidIndex = findMedianIndex(splitVector(lum_data, newRegion1, _width));
                newRegion2.centroidIndex = findMedianIndex(splitVector(lum_data, newRegion2, _width));

                newRegions.push_back(newRegion1);
                newRegions.push_back(newRegion2);
            } else {

                Region newRegion1(region.startX, region.startY, region.endX, medianIndexY, 0);
                Region newRegion2(region.startX, medianIndexY + 1, region.endX, region.endY, 0);

                newRegion1.centroidIndex = findMedianIndex(splitVector(lum_data, newRegion1, _width));
                newRegion2.centroidIndex = findMedianIndex(splitVector(lum_data, newRegion2, _width));

                newRegions.push_back(newRegion1);
                newRegions.push_back(newRegion2);
            }
            std::cout << "Width: " << _width << " Height: " << _height << std::endl;

        }
        regions = newRegions;
    }

    //prints
    for (int i = 0; i < regions.size(); ++i) {
        std::cout << regions[i].startX << " " << regions[i].startY << " " << regions[i].endX << " " << regions[i].endY << std::endl;
        std::cout << regions[i].centroidIndex % _width << " " << regions[i].centroidIndex / _width << std::endl;
    }

//    auto end = std::chrono::high_resolution_clock::now();
//    std::chrono::duration<double> elapsed = end - start;
//    std::cout << "Time to take median cut: " << elapsed.count() << "s\n";
//
}

void MedianCut::OnKeyInput(int key, int action) {
    if (key == GLFW_KEY_E && action == GLFW_PRESS) {
        if(iterationCount > 1){
            iterationCount--;
        }
    }

    if(key == GLFW_KEY_R && action == GLFW_PRESS){
        if(iterationCount < 7){
            iterationCount++;
        }
    }
}