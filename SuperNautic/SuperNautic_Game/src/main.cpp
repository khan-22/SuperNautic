// SuperNautic
#include <iostream>

#include <GL/glew.h>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <glm/glm.hpp>
// #include <crtdbg.h> // Leak-checking

#include "Core/Game.hpp"
#include "Core/Io/Log.hpp"

int main() {
  // _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
  {
    Game game;
    GLenum glewInitResult = glewInit();
    if (glewInitResult != GLEW_OK) {
      LOG_ERROR("Failed to initialize glew!");
    } else {
      game.bInitialize();
      game.run();
    }
  }
  CLOSE_LOG();
  return 0;
}
