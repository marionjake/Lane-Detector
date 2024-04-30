#ifndef _UTILS_HPP
#define _UTILS_HPP

#include <iostream>
#include <string>

enum resource
{
    IMAGE = 0,
    VIDEO = 1
};

std::string getResourcePath(std::string path, resource type)
{
    switch (type)
    {
        case IMAGE:
            return std::string{"../resources/images/"} + path;
            break;
        
        case VIDEO:
            return std::string{"../resources/videos/"} + path;
            break;

        default:
            std::cerr << "Invalid resource type...\n";
            return " ";
    }
}




#endif // _UTILS_HPP