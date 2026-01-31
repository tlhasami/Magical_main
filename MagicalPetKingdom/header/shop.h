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

class SHOP
{
private:
    // Item prices [coins, gems]
    const int item_prices[6][2] = {
        {3, 0}, // item 1
        {1, 0}, // item 2
        {2, 0}, // item 3
        {0, 1}, // item 4
        {1, 1}, // item 5
        {1, 2}  // item 6
    };

    void writing_profile(RenderWindow &window, const Player &player)
    {
        write(window, to_string(player.getCoins()) + " COINS", 90, 37, 24, Color::Yellow);
        write(window, to_string(player.getGems()) + " GEMS", 100, 95, 24, Color::Cyan);
        // Display item quantities above buttons (right side)
        write(window, "x" + to_string(player.getItem1()), 204 + 90 , 315 - 65, 30, Color::White);
        write(window, "x" + to_string(player.getItem2()), 465 + 90 , 315 - 65, 30, Color::White);
        write(window, "x" + to_string(player.getItem3()), 704 + 90 , 315 - 65, 30, Color::White);
        write(window, "x" + to_string(player.getItem4()), 204 + 90 , 520 - 65, 30, Color::White);
        write(window, "x" + to_string(player.getItem5()), 465 + 90 , 520 - 65, 30, Color::White);
        write(window, "x" + to_string(player.getItem6()), 704 + 90 , 520 - 65, 30, Color::White);
    }

    bool canAfford(const Player &player, int itemIndex)
    {
        return (player.getCoins() >= item_prices[itemIndex][0]) &&
               (player.getGems() >= item_prices[itemIndex][1]);
    }

    void purchaseItem(Player &player, int itemIndex)
    {
        if (!canAfford(player, itemIndex))
        {
            cout << "Not enough currency for item " << itemIndex + 1 << endl;
            return;
        }

        // Deduct currency
        player.addCoins(-item_prices[itemIndex][0]);
        player.addGems(-item_prices[itemIndex][1]);

        // Add item to inventory
        switch (itemIndex)
        {
        case 0:
            player.addItem1(1);
            break;
        case 1:
            player.addItem2(1);
            break;
        case 2:
            player.addItem3(1);
            break;
        case 3:
            player.addItem4(1);
            break;
        case 4:
            player.addItem5(1);
            break;
        case 5:
            player.addItem6(1);
            break;
        }

        cout << "Purchased item " << itemIndex + 1 << endl;
    }

public:
    void shopping(RenderWindow &window, Player &player)
    {
        Object screen("images/shop.png", Vector2f(0.f, 0.f), Vector2f(1.f, 1.f));

        // Button positions: exit, back, item1, item2, item3, item4, item5, item6
        Button buttons[8] = {
            Button(945, 10, 43, 43),   // exit
            Button(24, 534, 45, 43),   // back
            Button(204, 315, 104, 30), // item 1
            Button(465, 315, 104, 30), // item 2
            Button(704, 315, 104, 30), // item 3
            Button(204, 520, 104, 30), // item 4
            Button(465, 520, 104, 30), // item 5
            Button(704, 520, 104, 30)  // item 6
        };

        while (window.isOpen())
        {
            Event event;
            while (window.pollEvent(event))
            {
                if (event.type == Event::Closed)
                {
                    window.close();
                }

                if (event.type == Event::MouseButtonPressed &&
                    event.mouseButton.button == Mouse::Left)
                {

                    Vector2i mousePos = Mouse::getPosition(window);

                    if (buttons[0].isClicked(mousePos))
                    {
                        savePlayerData(player,player.getName());
                        exit(EXIT_SUCCESS);
                    }
                    else if (buttons[1].isClicked(mousePos))
                    {
                        savePlayerData(player,player.getName());
                        return; // Exit shop
                    }
                    // Handle item purchases
                    else if (buttons[2].isClicked(mousePos))
                    {
                        purchaseItem(player, 0); // Item 1
                    }
                    else if (buttons[3].isClicked(mousePos))
                    {
                        purchaseItem(player, 1); // Item 2
                    }
                    else if (buttons[4].isClicked(mousePos))
                    {
                        purchaseItem(player, 2); // Item 3
                    }
                    else if (buttons[5].isClicked(mousePos))
                    {
                        purchaseItem(player, 3); // Item 4
                    }
                    else if (buttons[6].isClicked(mousePos))
                    {
                        purchaseItem(player, 4); // Item 5
                    }
                    else if (buttons[7].isClicked(mousePos))
                    {
                        purchaseItem(player, 5); // Item 6
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