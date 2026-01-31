#pragma once
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <limits>
#include "entry.h"
#include "start.h"
#include "user.h"
#include "writer.h"
#include "menu.h"

class INVENTORY
{
private:
    void writing_profile(RenderWindow &window, const Player &player)
    {
        Color brown(139, 69, 19); // Define once before these calls

        write(window, to_string(player.getCoins()) + " COINS", 90, 33, 24, Color::Yellow);
        write(window, to_string(player.getGems()) + " GEMS", 100, 90, 24, Color::Cyan);
        // Display item quantities above buttons (right side)
        write(window, "x" + to_string(player.getItem1()), 231, 299, 30, brown);
        write(window, "x" + to_string(player.getItem2()), 499, 307, 30, brown);
        write(window, "x" + to_string(player.getItem3()), 738, 301, 30, brown);
        write(window, "x" + to_string(player.getItem4()), 234, 512, 30, brown);
        write(window, "x" + to_string(player.getItem5()), 502, 520, 30, brown);
        write(window, "x" + to_string(player.getItem6()), 733, 519, 30, brown);
    }

public:
    void checking(RenderWindow &window, Player &player)
    {
        Object screen("images/inventory.png", Vector2f(0.f, 0.f), Vector2f(1.f, 1.f));

        // Button positions: exit, back, item1, item2, item3, item4, item5, item6
        Button buttons[2] = {
            Button(945, 10, 43, 43), // exit
            Button(24, 534, 45, 43), // back
        };

        while (window.isOpen())
        {
            Event event;
            while (window.pollEvent(event))
            {
                if (event.type == Event::Closed)
                {
                    window.close();
                    exit(EXIT_SUCCESS);
                }

                if (event.type == Event::MouseButtonPressed &&
                    event.mouseButton.button == Mouse::Left)
                {

                    Vector2i mousePos = Mouse::getPosition(window);
                    cout << mousePos.x << ", " << mousePos.y << endl;
                    if (buttons[0].isClicked(mousePos))
                    {
                        savePlayerData(player, player.getName());
                        exit(EXIT_SUCCESS);
                    }
                    else if (buttons[1].isClicked(mousePos))
                    {
                        return; // BACK INVENTORY
                    }
                }
            }

            window.clear();
            screen.draw(window);
            writing_profile(window, player);

            window.display();
        }
    }
};