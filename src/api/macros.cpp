#include "../../include/api/macros.h"
#include "../../include/api/utils.h"

#include <QFile>
#include <fstream>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

Macros::Macros(string name, string text)
{
    this->name = name;
    this->text = text;
}

void Macros::addExtension(string extension) {
    this->extensions.insert(extensions.end(), extension);
}

bool Macros::hasExtension(string extension) {
    if (extensions.empty()) {
        return true;
    }

    if ( std::find(extensions.begin(), extensions.end(), extension) != extensions.end() ) {
        return true;
    }

    return false;
}
