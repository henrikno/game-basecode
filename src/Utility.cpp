#include <stdio.h>
#include <iostream>
#include <string>
#include "Utility.h"
#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#endif

std::string GetResourcePath(std::string filePath) {
#ifdef __APPLE__
    CFBundleRef mainBundle   = CFBundleGetMainBundle();
    CFURLRef    resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
    char path[2048];
    if (!CFURLGetFileSystemRepresentation(resourcesURL, true, (UInt8*)path, 2048)) {
        std::cerr << "Could not find resources directory" << std::endl;
    }
    CFRelease(resourcesURL);
    return std::string(path) +"/" + filePath;
#else
    return std::string("resources/") + filePath;
#endif
}
