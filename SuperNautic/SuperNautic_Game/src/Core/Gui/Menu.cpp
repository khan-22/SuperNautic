#include "Menu.hpp"

Menu::Menu()
: _root(new MenuElement())
{
}


MenuElement& Menu::get_root()
{
    return *_root;
}