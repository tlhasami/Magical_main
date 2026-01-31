#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <ctime>
#include <fstream>
#include <iostream>
#include <limits>

#include "button.h"
#include "guild.h"
#include "object.h"
#include "player.h"
#include "user.h"
extern Font font;

extern User_info DATABASE;

extern Guild_info GUILD_PRESENT;

class starting_screen
{
public:
  // Draws a welcome/hello screen with unskippable loading bar
  static void hello(RenderWindow &window)
  {
    // Create screen object with proper scaling
    Object first_screen("images/entry.png", Vector2f(0.f, 0.f),
                        Vector2f(0.775f, 0.85f));

    first_screen.setIntRect(30, 35, 1366, 772);

    // Create progress bar
    RectangleShape progressBar(Vector2f(0, 15));
    progressBar.setFillColor(Color(100, 255, 100)); // Light green
    progressBar.setOutlineThickness(2);
    progressBar.setOutlineColor(Color::White);

    // Center progress bar horizontally
    float barWidth = window.getSize().x * 0.6f;
    progressBar.setPosition(window.getSize().x / 2 - barWidth / 2,
                            window.getSize().y * 0.85f);

    // Main loop for hello screen
    Clock loadingClock;
    bool loadingComplete = false;

    while (window.isOpen() && !loadingComplete)
    {
      Event event;
      while (window.pollEvent(event))
      {
        if (event.type == Event::Closed)
        {
          window.close();
          exit(EXIT_SUCCESS);
        }
      }

      // Update loading progress (0-100)
      float progress =
          min(100.f, loadingClock.getElapsedTime().asSeconds() * 25.f);
      loadingComplete = (progress >= 100.f);

      // Update progress bar
      progressBar.setSize(Vector2f(barWidth * (progress / 100.f), 15));

      // Clear and draw
      window.clear();
      first_screen.draw(window);
      window.draw(progressBar);

      // Draw loading percentage text
      Font font;
      if (font.loadFromFile("fonts/font.ttf"))
      {
        Text progressText(to_string((int)progress) + "%", font, 20);
        progressText.setFillColor(Color::Black);
        progressText.setPosition(
            window.getSize().x / 2 - progressText.getLocalBounds().width / 2,
            window.getSize().y * 0.82f - 5.f);
        window.draw(progressText);
      }

      window.display();
    }

    // Optional: Add a small delay at 100% before proceeding
    sleep(milliseconds(800));
  }
  // Draws a loading screen
  static bool loading(RenderWindow &window)
  {
    // Calculate scaling factors to fit the window while maintaining aspect
    // ratio
    Vector2f scale(window.getSize().x / 1366.f, window.getSize().y / 772.f);

    // Create loading screen object with proper scaling
    Object loading_screen("images/info.png", Vector2f(0.f, 0.f), scale);

    // creating a Button for click
    Button enter(391.f, 466, 634 - 391, 540 - 466);

    // Main loop for loading screen
    Clock loadingClock;
    while (window.isOpen())
    {
      Event event;
      while (window.pollEvent(event))
      {
        if (event.type == Event::Closed)
        {
          exit(EXIT_SUCCESS);
          return false;
        }
        // Check for right mouse button click
        if (event.type == Event::MouseButtonPressed &&
            event.mouseButton.button == Mouse::Left)
        {
          Vector2i mousePos = Mouse::getPosition(window);
          cout << "Right clicked at: (" << mousePos.x << ", " << mousePos.y
               << ")" << endl;
          if (enter.isClicked(mousePos))
          {
            cout << "Enter is pressed" << endl;
            return true;
          }
        }
      }

      // Clear and draw
      window.clear();
      loading_screen.draw(window);
      window.display();

      // Optional: Add a small delay to prevent spamming
      sleep(milliseconds(50));
    }

    return false;
  }
};