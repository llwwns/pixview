#pragma once

#include <vector>
#include <string>


struct LibraryPath {
    std::vector<std::string> paths;
    std::string name;
};

struct Setting {
    bool fullscreen;
    int interval;
    bool autoRotate;
    std::string defaultLibrary;
    std::vector<LibraryPath> libraries;

    static Setting load();
    static Setting _default();
};
