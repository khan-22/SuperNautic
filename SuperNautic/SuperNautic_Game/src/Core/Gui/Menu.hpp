#pragma once

#include "MenuElement.hpp"

class Menu
{
    public:
        Menu();

        MenuElement& get_root();

    private:
        std::unique_ptr<MenuElement> _root;
};
