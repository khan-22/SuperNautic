// SuperNautic
#include <iostream>

#include <GL/glew.h>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <glm/glm.hpp>
// #include <crtdbg.h> // Leak-checking

#include "Core/Game.hpp"
#include "Core/Io/Log.hpp"

#include "Core/Utility/Timer.hpp"
#include "Core/Utility/TimeSummary.hpp"

void foo(size_t depth)
{
  prof::Timer timer("foo");
  std::cout << rand();
  if(depth > 9000)
  {
    return;
  }
  foo(depth + 1);
}

int main() {
  {
    for (size_t i = 0; i < 9000; i++)
    {
      prof::Timer timer("rand");
      std::cout << rand();
    }
    std::cout << std::endl;

    foo(0);



    std::cout << prof::TimeSummary::summarize() << std::endl;
    getchar();
    return 0;
  }
  // _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
  Game game;

  GLenum glewInitResult = glewInit();
  if (glewInitResult != GLEW_OK) {
    LOG_ERROR("Failed to initialize glew!");
  } else {
    game.bInitialize();
    game.run();
  }

  return 0;
}
