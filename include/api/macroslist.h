#ifndef MACROSLIST_H
#define MACROSLIST_H

#include "macros.h"

class MacrosList
{
public:
    MacrosList();

    map<string, Macros*> macroses;

    void parseExtensions(Macros *macros);
    void parseSettings(string name);
    Macros* getMacros(string name);
    void loadMacroses();
};

#endif // MACROSLIST_H
