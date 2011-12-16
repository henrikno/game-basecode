#pragma once 

#include <SFML/Graphics.hpp>

#include "ResourceManager.h"

class ImageManager : public ResourceManager<sf::Image> {
private:
protected:
    virtual sf::Image* load( const std::string& strId );
public:
};

extern ImageManager gImageManager;
