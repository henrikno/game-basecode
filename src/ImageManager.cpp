#include <iostream>
#include "ImageManager.h"

using namespace std;

ImageManager gImageManager;

sf::Image* ImageManager::load(const std::string &id) {
    sf::Image* image = new sf::Image();
    if (!image->LoadFromFile(id)) {
        std::cout << "[WARN] ImageManager failed to load: " << id << std::endl;
        delete image;
        image = NULL;
    }
    return image;
}
