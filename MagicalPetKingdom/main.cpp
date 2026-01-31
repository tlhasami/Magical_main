#include "header/game.h"
#include "header/guild.h"
#include "header/user.h"

using namespace sf;
using namespace std;

// it contains all teh info of whether the user is present
User_info DATABASE;

// it contains all teh info of the guilds present
Guild_info GUILD_PRESENT;

// Global music for all game
Music music;

// gloabl font for all game
Font font;

int main()
{
  if (!font.loadFromFile("fonts/font.ttf"))
  {
    cerr << "Failed to load font!" << endl;
    exit(EXIT_FAILURE);
  }
  // random seed here
  srand(static_cast<unsigned int>(time(0)));

  if (!music.openFromFile("sounds/bg1.ogg"))
  {
    std::cerr << "Error loading background music" << std::endl;
    exit(EXIT_FAILURE);
  }
  music.setLoop(true);
  music.setVolume(100);
  music.play();

  RenderWindow window(VideoMode(screenResolution.x, screenResolution.y),
                      "MAGICAL PET KINGDOM  ( BY TALHA SAMI )");
  window.setFramerateLimit(60); // Limit to 60 frames per second

  // in this class our whole game runs
  
  Game game;
  game.game(window);

  return EXIT_SUCCESS;
}