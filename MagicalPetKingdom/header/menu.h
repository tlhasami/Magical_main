#pragma once
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <ctime>
#include <fstream>
#include <string>
#include <cstring>
#include <iostream>
#include <limits>
#include <cstdlib> // for rand()
#include <ctime>   // for time()

#include "pets.h"
#include "button.h"
#include "writer.h"
#include "object.h"
#include "text.h"
#include "player.h"

extern Music music;
extern Font font;

extern User_info DATABASE;
extern Guild_info GUILD_PRESENT;

class MENU
{
private:
    struct PetMovement
    {
        Vector2f currentPos;
        Vector2f targetPos;
        float speed;
    };

    PetMovement petMovements[4]; // Array for 4 pets
    Clock movementClock;
    const float movementChangeInterval = 5.0f; // Change target every 3 seconds

    void initializePetMovements(Player &player)
    {
        // Seed random number generator
        srand(static_cast<unsigned>(time(0)));

        // Initialize positions for each pet
        for (int i = 0; i < 4; i++)
        {
            petMovements[i].currentPos = Vector2f(300 + i * 100, 200 + i * 50);
            petMovements[i].targetPos = Vector2f(
                50 + rand() % 850, // Random x between 100-700
                50 + rand() % 450  // Random y between 150-450
            );
            petMovements[i].speed = 40.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 30.0f)); // Speed between 20-50
        }

        // Assign to actual pets
        player.dragon.sprite.setPosition(petMovements[0].currentPos);
        player.griffin.sprite.setPosition(petMovements[1].currentPos);
        player.phoenix.sprite.setPosition(petMovements[2].currentPos);
        player.unicorn.sprite.setPosition(petMovements[3].currentPos);
    }

    void updatePetMovements(Player &player, float deltaTime)
    {
        // Store original absolute scales
        const float originalScales[4] = {
            std::abs(player.dragon.sprite.getScale().x),
            std::abs(player.griffin.sprite.getScale().x),
            std::abs(player.phoenix.sprite.getScale().x),
            std::abs(player.unicorn.sprite.getScale().x)};

        for (int i = 0; i < 4; i++)
        {
            // Calculate direction and distance to target
            Vector2f direction = petMovements[i].targetPos - petMovements[i].currentPos;
            float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

            if (distance < 5.0f)
            {
                Vector2f newTarget;
                do
                {
                    newTarget = Vector2f(
                        100 + rand() % 800,
                        60 + rand() % 480);
                } while (std::sqrt(std::pow(newTarget.x - petMovements[i].currentPos.x, 2) +
                                   std::pow(newTarget.y - petMovements[i].currentPos.y, 2)) >= 450.0f);

                petMovements[i].targetPos = newTarget;
            }
            else
            {
                direction /= distance; // Normalize
                petMovements[i].currentPos += direction * petMovements[i].speed * deltaTime;
            }

            // Get corresponding sprite
            Sprite *sprite = nullptr;
            switch (i)
            {
            case 0:
                sprite = &player.dragon.sprite;
                break;
            case 1:
                sprite = &player.griffin.sprite;
                break;
            case 2:
                sprite = &player.phoenix.sprite;
                break;
            case 3:
                sprite = &player.unicorn.sprite;
                break;
            }

            if (sprite)
            {
                sprite->setPosition(petMovements[i].currentPos);

                // Handle flipping for Phoenix (only if direction is left, flip)
                if (i == 2) // Phoenix
                {
                    float absScaleX = originalScales[i];
                    if (direction.x < 0)
                        sprite->setScale(-absScaleX, sprite->getScale().y); // face left
                    else
                        sprite->setScale(absScaleX, sprite->getScale().y); // face right
                }
                else // For other pets (flip opposite)
                {
                    float absScaleX = originalScales[i];
                    if (direction.x < 0)
                        sprite->setScale(absScaleX, sprite->getScale().y); // face right
                    else
                        sprite->setScale(-absScaleX, sprite->getScale().y); // face left
                }

                // Adjust origin based on flipping
                FloatRect bounds = sprite->getLocalBounds();
                float originX = (sprite->getScale().x < 0) ? bounds.width : bounds.width / 2;
            }
        }
    }

    void pet_home(RenderWindow &window, Player &player)
    {
        // Define the central 90% area boundaries (50px margin on each side)
        const float minX = 50.f;
        const float maxX = 950.f;
        const float minY = 50.f;
        const float maxY = 550.f;

        // Initialize pet movements for pet home
        struct PetHomeMovement
        {
            Vector2f currentPos;
            Vector2f targetPos;
            float speed;
            Vector2f direction;
        };

        // Store original absolute scales
        const float originalScales[4] = {
            std::abs(player.dragon.sprite.getScale().x),
            std::abs(player.griffin.sprite.getScale().x),
            std::abs(player.phoenix.sprite.getScale().x),
            std::abs(player.unicorn.sprite.getScale().x)};

        PetHomeMovement petMovements[4];
        Clock movementClock;

        // Initialize positions and speeds
        srand(static_cast<unsigned>(time(0)));
        for (int i = 0; i < 4; i++)
        {
            petMovements[i].currentPos = Vector2f(
                minX + rand() % static_cast<int>(maxX - minX),
                minY + rand() % static_cast<int>(maxY - minY));
            petMovements[i].targetPos = Vector2f(
                minX + rand() % static_cast<int>(maxX - minX),
                minY + rand() % static_cast<int>(maxY - minY));
            petMovements[i].speed = 40.f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 30.f));
            petMovements[i].direction = Vector2f(0, 0);
        }

        // Set up background and button
        Object screen("images/pet_home.png", Vector2f(0.f, 0.f), Vector2f(1.f, 1.f));
        Button backButton(24, 534, 45, 43); // Back button at bottom-left

        Clock deltaClock;
        while (window.isOpen())
        {
            float deltaTime = deltaClock.restart().asSeconds();
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
                    if (backButton.isClicked(mousePos))
                    {
                        return; // Return to main menu
                    }
                }
            }

            // Update pet movements and flipping
            for (int i = 0; i < 4; i++)
            {
                // Calculate direction and distance to target
                petMovements[i].direction = petMovements[i].targetPos - petMovements[i].currentPos;
                float distance = std::sqrt(petMovements[i].direction.x * petMovements[i].direction.x +
                                           petMovements[i].direction.y * petMovements[i].direction.y);

                if (distance < 5.0f)
                {
                    // Set new random target within bounds
                    petMovements[i].targetPos = Vector2f(
                        minX + rand() % static_cast<int>(maxX - minX),
                        minY + rand() % static_cast<int>(maxY - minY));
                }
                else
                {
                    petMovements[i].direction /= distance; // Normalize
                    petMovements[i].currentPos += petMovements[i].direction * petMovements[i].speed * deltaTime;

                    // Ensure pets stay within bounds
                    petMovements[i].currentPos.x = std::max(minX, std::min(maxX, petMovements[i].currentPos.x));
                    petMovements[i].currentPos.y = std::max(minY, std::min(maxY, petMovements[i].currentPos.y));
                }

                // Get corresponding sprite
                Sprite *sprite = nullptr;
                switch (i)
                {
                case 0:
                    sprite = &player.dragon.sprite;
                    break;
                case 1:
                    sprite = &player.griffin.sprite;
                    break;
                case 2:
                    sprite = &player.phoenix.sprite;
                    break;
                case 3:
                    sprite = &player.unicorn.sprite;
                    break;
                }

                if (sprite)
                {
                    sprite->setPosition(petMovements[i].currentPos);

                    // Handle flipping for Phoenix (only if direction is left, flip)
                    if (i == 2) // Phoenix
                    {
                        float absScaleX = originalScales[i];
                        if (petMovements[i].direction.x < 0)
                            sprite->setScale(-absScaleX, sprite->getScale().y); // face left
                        else
                            sprite->setScale(absScaleX, sprite->getScale().y); // face right
                    }
                    else // For other pets (flip opposite)
                    {
                        float absScaleX = originalScales[i];
                        if (petMovements[i].direction.x < 0)
                            sprite->setScale(absScaleX, sprite->getScale().y); // face right
                        else
                            sprite->setScale(-absScaleX, sprite->getScale().y); // face left
                    }
                }
            }

            // Draw everything
            window.clear();
            screen.draw(window);

            player.draw(window, deltaTime);

            backButton.draw(window);

            window.display();
        }
    }
    
    void writing_profile(RenderWindow &window, const Player &player)
    {
        write(window, "NAME: " + player.getName(), 100 - 10, 37);
        write(window, to_string(player.getCoins()) + " COINS", 100 - 10, 108, 24, Color::Yellow);
        write(window, to_string(player.getGems()) + " GEMS", 105 - 10, 170, 24, Color::Cyan);
    }

public:
    int selection(RenderWindow &window, Player &player)
    {

        Object screen("images/menu.png", Vector2f(0.f, 0.f), Vector2f(1.f, 1.f));
        Button button[9] = {
            Button(945, 10, 43, 43),                // exit
            Button(24, 534, 45, 43),                // back
            Button(465, 91, 616 - 471, 133 - 91),   // battle
            Button(465, 175, 614 - 470, 222 - 175), // info
            Button(465, 280, 610 - 470, 320 - 279), // shop
            Button(465, 370, 610 - 470, 415 - 370), // items
            Button(465, 460, 608 - 470, 43),        // train
            Button(720, 241, 140, 43),              // Pet
            Button(720, 500, 140, 43)               // Battle
        };

        Clock deltaClock;

        while (window.isOpen())
        {
            float deltaTime = deltaClock.restart().asSeconds();
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
                    cout << "Mouse clicked at: (" << mousePos.x << ", " << mousePos.y << ")\n";

                    if (button[0].isClicked(mousePos))
                        return -1;
                    else if (button[1].isClicked(mousePos))
                        return 11;
                    else if (button[2].isClicked(mousePos))
                        return 1;
                    else if (button[3].isClicked(mousePos))
                        return 2;
                    else if (button[4].isClicked(mousePos))
                        return 3;
                    else if (button[5].isClicked(mousePos))
                    {

                        return 4;
                    }
                    else if (button[6].isClicked(mousePos))
                    {
                        cout << "Training is pressed" << endl;
                        return 5;
                    }
                    else if (button[7].isClicked(mousePos))
                    {
                        cout << "Pet is pressed" << endl;
                        pet_home(window, player);
                        
                    }
                    else if (button[8].isClicked(mousePos))
                    {
                        cout << "Battle is pressed" << endl;
                        return 6;
                    }
                }
            }

            window.clear();
            screen.draw(window);

            writing_profile(window, player);
            // Draw top 5 guilds on left side
            float pos = 310.f; // You can adjust spacing manually
            for (int i = 0; i < min(6, GUILD_PRESENT.getGuildCount()); ++i)
            {
                string displayText = to_string(i + 1) + ". " + GUILD_PRESENT.getGuildName(i) + " (" + to_string(GUILD_PRESENT.getScore(GUILD_PRESENT.getGuildName(i))) + ")";
                write(window, displayText, 50, pos, 15, Color::Cyan); // x = 30 (left side), y = pos
                pos += 30.f;                                          // Adjust this value based on your spacing preference
            }

            window.display();
        }
        return 0;
    }
};