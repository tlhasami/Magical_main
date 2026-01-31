#pragma once
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <limits>

class INFO
{
private:
    int current_index = 0;
    Vector2f position = Vector2f(321.f, 245.f);
    float originalScales[4][2]; // Array to store original scales [pet_index][x,y]

    void storeOriginalScales(Player& player) {
        // Dragon
        originalScales[0][0] = player.dragon.sprite.getScale().x;
        originalScales[0][1] = player.dragon.sprite.getScale().y;
        // Griffin
        originalScales[1][0] = player.griffin.sprite.getScale().x;
        originalScales[1][1] = player.griffin.sprite.getScale().y;
        // Unicorn
        originalScales[2][0] = player.unicorn.sprite.getScale().x;
        originalScales[2][1] = player.unicorn.sprite.getScale().y;
        // Phoenix
        originalScales[3][0] = player.phoenix.sprite.getScale().x;
        originalScales[3][1] = player.phoenix.sprite.getScale().y;
    }

    void restoreOriginalScales(Player& player) {
        // Dragon
        player.dragon.sprite.setScale(originalScales[0][0], originalScales[0][1]);
        // Griffin
        player.griffin.sprite.setScale(originalScales[1][0], originalScales[1][1]);
        // Unicorn
        player.unicorn.sprite.setScale(originalScales[2][0], originalScales[2][1]);
        // Phoenix
        player.phoenix.sprite.setScale(originalScales[3][0], originalScales[3][1]);
    }

    void writing_profile(sf::RenderWindow &window, Player &player)
    {
        int level = 0, attack = 0, defence = 0, speed = 0, health = 0;
        
        // Scale array for each pet in order: dragon, griffin, unicorn, phoenix
        const float scales[4] = {0.9f, 0.8f, 1.25f, 1.f};
    
        // Apply scale based on current_index
        float currentScale = scales[current_index];
    
        if (current_index == 0)
        {
            player.dragon.sprite.setScale(currentScale, currentScale);
            player.dragon.sprite.setPosition(position);
            player.dragon.draw(window);
            level = player.dragon.getLevel();
            attack = player.dragon.getAttack();
            defence = player.dragon.getDefence();
            speed = player.dragon.getSpeed();
            health = player.dragon.getHealth();
        }
        else if (current_index == 1)
        {
            player.griffin.sprite.setScale(currentScale, currentScale);
            player.griffin.sprite.setPosition(position);
            player.griffin.draw(window);
            level = player.griffin.getLevel();
            attack = player.griffin.getAttack();
            defence = player.griffin.getDefence();
            speed = player.griffin.getSpeed();
            health = player.griffin.getHealth();
        }
        else if (current_index == 2)
        {
            player.unicorn.sprite.setScale(currentScale, currentScale);
            player.unicorn.sprite.setPosition(position);
            player.unicorn.draw(window);
            level = player.unicorn.getLevel();
            attack = player.unicorn.getAttack();
            defence = player.unicorn.getDefence();
            speed = player.unicorn.getSpeed();
            health = player.unicorn.getHealth();
        }
        else if (current_index == 3)
        {
            player.phoenix.sprite.setScale(currentScale, currentScale);
            player.phoenix.sprite.setPosition(position);
            player.phoenix.draw(window);
            level = player.phoenix.getLevel();
            attack = player.phoenix.getAttack();
            defence = player.phoenix.getDefence();
            speed = player.phoenix.getSpeed();
            health = player.phoenix.getHealth();
        }
    
        // Draw attribute values using write()
        int yStart = 190;
        int gap = 32;
    
        write(window, std::to_string(level), 690, yStart + 0 * gap, 24, sf::Color(139, 69, 19));
        write(window, std::to_string(attack), 690, yStart + 1 * gap, 24, sf::Color::Red);
        write(window, std::to_string(defence), 690, yStart + 2 * gap, 24, sf::Color::Yellow);
        write(window, std::to_string(speed), 690, yStart + 3 * gap, 24, sf::Color::Cyan);
        write(window, std::to_string(health), 690, yStart + 4 * gap, 24, sf::Color::Green);
    }
    
public:
    void information(RenderWindow &window, Player &player)
    {
        storeOriginalScales(player); // Store original scales before modifying
        
        Object screen("images/playerView.png", Vector2f(0.f, 0.f), Vector2f(1.f, 1.f));
        Button buttons[4] = {
            Button(945, 10, 43, 43),  // exit
            Button(24, 534, 45, 43),  // back
            Button(380, 502, 75, 65), // Left
            Button(530, 502, 75, 65), // Right
        };
        
        Clock c;
        while (window.isOpen())
        {
            float d = c.restart().asSeconds();
            Event event;
            while (window.pollEvent(event))
            {
                if (event.type == Event::Closed){

                    window.close();
                    exit(EXIT_SUCCESS);
                }

                if (event.type == Event::MouseButtonPressed &&
                    event.mouseButton.button == Mouse::Left)
                {
                    Vector2i mousePos = Mouse::getPosition(window);

                    if (buttons[0].isClicked(mousePos))
                    {
                        restoreOriginalScales(player);
                        savePlayerData(player, player.getName());
                        exit(EXIT_SUCCESS);
                    }
                    else if (buttons[1].isClicked(mousePos))
                    {
                        restoreOriginalScales(player);
                        return; // BACK INVENTORY
                    }
                    else if (buttons[2].isClicked(mousePos)) // Left
                    {
                        current_index = (current_index - 1 + 4) % 4;
                    }
                    else if (buttons[3].isClicked(mousePos)) // Right
                    {
                        current_index = (current_index + 1) % 4;
                    }
                }
            }

            player.dragon.updateAnimation(d);
            player.unicorn.updateAnimation(d);
            player.griffin.updateAnimation(d);
            player.phoenix.updateAnimation(d);

            window.clear();
            screen.draw(window);
            writing_profile(window, player);
            window.display();
        }
    }
};