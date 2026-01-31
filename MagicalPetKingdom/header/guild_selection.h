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
#include "shop.h"
#include "inventory.h"
#include "info.h"
#include "training.h"

extern Music music;
extern Font font;

extern User_info DATABASE;
extern Guild_info GUILD_PRESENT;

class GUILD_AREA
{
private:
    void assigning_role(RenderWindow &window, Player &player)
    {
        Object guildBg("images/bg_guild_2.png", {0.f, 0.f}, {1.f, 1.f});

        // Store original scales
        float originalScales[4][2] = {
            {player.dragon.sprite.getScale().x, player.dragon.sprite.getScale().y},
            {player.griffin.sprite.getScale().x, player.griffin.sprite.getScale().y},
            {player.unicorn.sprite.getScale().x, player.unicorn.sprite.getScale().y},
            {player.phoenix.sprite.getScale().x, player.phoenix.sprite.getScale().y}};

        // Pet display settings
        int current_pet = 0;
        const Vector2f petPosition = {380.f, 250.f};
        const float petScales[4] = {0.8f, 0.72f, 1.11f, 0.9f};
        string petRoles[4] = {"", "", "", ""}; // Stores assigned roles for each pet

        Button buttons[10] = {
            Button(945, 10, 43, 43),   // exit
            Button(24, 534, 45, 43),   // back
            Button(375, 485, 45, 45),  // left
            Button(560, 485, 45, 45),  // right
            Button(620, 130, 140, 40), // attack
            Button(620, 190, 140, 40), // health
            Button(620, 250, 140, 40), // speed
            Button(620, 310, 140, 40), // defence
            Button(620, 370, 140, 40), // revoke
            Button(845, 510, 135, 35)  // leave
        };

        Clock clock;
        while (window.isOpen())
        {
            float d = clock.restart().asSeconds();
            Event event;
            while (window.pollEvent(event))
            {
                if (event.type == Event::Closed)
                {
                    restoreScales(player, originalScales);
                    window.close();
                    savePlayerData(player, player.getName());
                    exit(EXIT_SUCCESS);
                }

                if (event.type == Event::MouseButtonPressed)
                {
                    Vector2i mousePos = Mouse::getPosition(window);

                    cout << mousePos.x << "," << mousePos.y << endl;
                    if (buttons[0].isClicked(mousePos)) // Exit
                    {
                        restoreScales(player, originalScales);
                        window.close();
                        savePlayerData(player, player.getName());
                        exit(EXIT_SUCCESS);
                    }
                    else if (buttons[1].isClicked(mousePos)) // Back
                    {
                        restoreScales(player, originalScales);
                        savePlayerData(player, player.getName());
                        return;
                    }
                    else if (buttons[2].isClicked(mousePos)) // Left
                    {
                        current_pet = (current_pet - 1 + 4) % 4;
                    }
                    else if (buttons[3].isClicked(mousePos)) // Right
                    {
                        current_pet = (current_pet + 1) % 4;
                    }
                    else if (buttons[4].isClicked(mousePos)) // Attack
                    {
                        petRoles[current_pet] = "Attacker";
                        if (current_pet == 0)
                            player.dragon.setRole("Attacker");
                        else if (current_pet == 1)
                            player.griffin.setRole("Attacker");
                        else if (current_pet == 2)
                            player.unicorn.setRole("Attacker");
                        else if (current_pet == 3)
                            player.phoenix.setRole("Attacker");
                    }
                    else if (buttons[5].isClicked(mousePos)) // Health
                    {
                        petRoles[current_pet] = "Healer";
                        if (current_pet == 0)
                            player.dragon.setRole("Healer");
                        else if (current_pet == 1)
                            player.griffin.setRole("Healer");
                        else if (current_pet == 2)
                            player.unicorn.setRole("Healer");
                        else if (current_pet == 3)
                            player.phoenix.setRole("Healer");
                    }
                    else if (buttons[6].isClicked(mousePos)) // Speed
                    {
                        petRoles[current_pet] = "Mage";
                        if (current_pet == 0)
                            player.dragon.setRole("Mage");
                        else if (current_pet == 1)
                            player.griffin.setRole("Mage");
                        else if (current_pet == 2)
                            player.unicorn.setRole("Mage");
                        else if (current_pet == 3)
                            player.phoenix.setRole("Mage");
                    }
                    else if (buttons[7].isClicked(mousePos)) // Defence
                    {
                        petRoles[current_pet] = "Tank";
                        if (current_pet == 0)
                            player.dragon.setRole("Tank");
                        else if (current_pet == 1)
                            player.griffin.setRole("Tank");
                        else if (current_pet == 2)
                            player.unicorn.setRole("Tank");
                        else if (current_pet == 3)
                            player.phoenix.setRole("Tank");
                    }
                    else if (buttons[8].isClicked(mousePos)) // Revoke
                    {
                        petRoles[current_pet] = "---";
                        if (current_pet == 0)
                            player.dragon.setRole("");
                        else if (current_pet == 1)
                            player.griffin.setRole("");
                        else if (current_pet == 2)
                            player.unicorn.setRole("");
                        else if (current_pet == 3)
                            player.phoenix.setRole("");
                    }
                    else if (buttons[9].isClicked(mousePos)) // Leave Guild
                    {
                        restoreScales(player, originalScales);
                        player.setGuildId(0);
                        player.setGuildName("");
                        player.dragon.setRole("");
                        player.unicorn.setRole("");
                        player.phoenix.setRole("");
                        player.griffin.setRole("");
                        cout << "Left the guild" << endl;
                        return;
                    }
                }
            }

            player.dragon.updateAnimation(d);
            player.unicorn.updateAnimation(d);
            player.phoenix.updateAnimation(d);
            player.griffin.updateAnimation(d);

            window.clear();
            guildBg.draw(window);

            // Draw current pet with proper scaling
            drawCurrentPet(window, player, current_pet, petPosition, petScales[current_pet]);

            // Display current pet's role from the object
            string role;
            if (current_pet == 0)
                role = player.dragon.getRole();
            else if (current_pet == 1)
                role = player.griffin.getRole();
            else if (current_pet == 2)
                role = player.unicorn.getRole();
            else if (current_pet == 3)
                role = player.phoenix.getRole();

            if (!role.empty())
            {
                write(window, "Role: " + role, 270, 415, 24, Color::Black);
            }
            else
            {
                write(window, "Role: --- " + role, 270, 415, 24, Color::Black);
            }

            // write(window , "helloworld", 190 , 19, 15) ;
            // write(window , "000" , 195, 43 , 15) ;
            write(window, player.getGuildName(), 190, 19, 15);
            write(window, to_string(GUILD_PRESENT.getScore(player.getGuildName())), 195, 43, 15);
            window.display();
        }
    }

    // Helper functions
    void restoreScales(Player &player, float originalScales[4][2])
    {
        player.dragon.sprite.setScale(originalScales[0][0], originalScales[0][1]);
        player.griffin.sprite.setScale(originalScales[1][0], originalScales[1][1]);
        player.unicorn.sprite.setScale(originalScales[2][0], originalScales[2][1]);
        player.phoenix.sprite.setScale(originalScales[3][0], originalScales[3][1]);
    }

    void drawCurrentPet(RenderWindow &window, Player &player, int current_pet, Vector2f position, float scale)
    {
        switch (current_pet)
        {
        case 0:
            player.dragon.sprite.setScale(scale, scale);
            player.dragon.sprite.setPosition(position);
            player.dragon.draw(window);
            break;
        case 1:
            player.griffin.sprite.setScale(scale, scale);
            player.griffin.sprite.setPosition(position);
            player.griffin.draw(window);
            break;
        case 2:
            player.unicorn.sprite.setScale(scale, scale);
            player.unicorn.sprite.setPosition(position);
            player.unicorn.draw(window);
            break;
        case 3:
            player.phoenix.sprite.setScale(scale, scale);
            player.phoenix.sprite.setPosition(position);
            player.phoenix.draw(window);
            break;
        }
    }

    void handleCreateResponse(RenderWindow &window, Player &player)
    {
        Object guildBg("images/create_guild.png", {0.f, 0.f}, {1.f, 1.f});
        Button button[3] = {
            Button(945, 10, 43, 43),   // exit
            Button(24, 534, 45, 43),   // back
            Button(415, 453, 170, 46), // Create
        };

        string input = "\0";
        Text displayText;

        displayText.setFont(font);
        displayText.setCharacterSize(30);
        displayText.setFillColor(Color::White);

        while (window.isOpen())
        {
            bool possible = !input.empty() && !GUILD_PRESENT.isPresent(input);

            Event event;
            while (window.pollEvent(event))
            {
                if (event.type == Event::Closed)
                    window.close();

                if (event.type == Event::TextEntered)
                {
                    char ch = static_cast<char>(event.text.unicode);
                    if ((isalpha(ch) || ch == '_') && input.length() < 16)
                    {
                        input += ch;
                    }
                    else if (ch == 8 && !input.empty()) // Backspace
                    {
                        input.pop_back();
                    }
                }

                if (event.type == Event::MouseButtonPressed)
                {
                    Vector2i mousePos = Mouse::getPosition(window);

                    cout << mousePos.x << "," << mousePos.y << endl;

                    if (button[0].isClicked(mousePos))
                    {
                        cout << "Exit" << endl;
                        return;
                    }
                    else if (button[1].isClicked(mousePos))
                    {
                        cout << "Back pressed" << endl;
                        return;
                    }
                    else if (button[2].isClicked(mousePos))
                    {
                        cout << "Create pressed" << endl;
                        if (!input.empty() && possible)
                        {
                            cout << "Create Guild: " << input << endl;
                            GUILD_PRESENT.appendGuildToFile(input , 0);
                            player.setGuildName(input);
                            player.setGuildId(1);
                            
                            return;
                            // Call your guild creation logic here
                        }
                    }
                }
            }

            // Centering logic
            displayText.setString(input);
            FloatRect bounds = displayText.getLocalBounds();
            displayText.setOrigin(bounds.width / 2, bounds.height / 2);
            displayText.setPosition(500, 295); // Center of 1000x600 screen

            window.clear();
            guildBg.draw(window);
            window.draw(displayText);

            if (possible)
            {
                write(window, "Available", 410, 350 , 24, Color::Black);
            }
            else
            {
                write(window, "Not Available", 410, 350 , 24, Color::Black);
            }

            window.display();
        }
    }

    void handleJoinResponse(RenderWindow &window, Player &player)
    {
        Object guildBg("images/join_guild.png", {0.f, 0.f}, {1.f, 1.f});
        Button button[10] = {
            Button(945, 10, 43, 43),   // exit
            Button(24, 534, 45, 43),   // back
            Button(767, 250, 55, 55),  // up
            Button(767, 330, 55, 55),  // down
            Button(565, 170, 145, 45), // join1
            Button(565, 233, 145, 45), // join2
            Button(565, 296, 145, 45), // join3
            Button(565, 361, 145, 45), // join4
            Button(565, 426, 145, 45), // join5
            Button(565, 490, 145, 45)  // join6
        };

        int current = 0;
        const int visibleGuilds = 6;
        int totalGuilds = GUILD_PRESENT.getGuildCount();

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

                if (event.type == Event::MouseButtonPressed)
                {
                    Vector2i mousePos = Mouse::getPosition(window);
                    cout << mousePos.x << "," << mousePos.y << endl;

                    if (button[0].isClicked(mousePos)) // Exit
                    {
                        cout << "Exit pressed" << endl;
                        exit(EXIT_SUCCESS);
                    }
                    else if (button[1].isClicked(mousePos)) // Back
                    {
                        cout << "Back pressed" << endl;
                        return;
                    }
                    else if (button[2].isClicked(mousePos)) // Up
                    {
                        if (current > 0)
                        {
                            current--;
                            cout << "Scrolled Up. Current: " << current << endl;
                        }
                    }
                    else if (button[3].isClicked(mousePos)) // Down
                    {
                        if (current + visibleGuilds < totalGuilds)
                        {
                            current++;
                            cout << "Scrolled Down. Current: " << current << endl;
                        }
                    }

                    else
                    {
                        for (int i = 0; i < visibleGuilds; ++i)
                        {
                            if (button[4 + i].isClicked(mousePos))
                            {
                                int guildIndex = current + i;
                                string guildName = GUILD_PRESENT.getGuildName(guildIndex);
                                if (guildName != "Empty Guild Slot")
                                {
                                    cout << "Joining guild: " << guildName << endl;
                                    player.setGuildName(guildName); // Assuming Player has this
                                    return;
                                }
                                else
                                {
                                    cout << "Invalid guild slot." << endl;
                                }
                            }
                        }
                    }
                }
            }

            window.clear();
            guildBg.draw(window);

            for (int i = 0; i < visibleGuilds; ++i)
            {
                string guildName = GUILD_PRESENT.getGuildName(current + i);
                write(window, guildName, 270, 173 + 64 * i, 24, Color::Black); // Adjusted Y position spacing
            }

            window.display();
        }
    }

    bool checking_status(const Player &player)
    {

        string playerGuild = player.getGuildName();

        return GUILD_PRESENT.isPresent(playerGuild);
    }

public:
    // Public function that checks guild requirements and manages interaction
    void checkGuildAccess(RenderWindow &window, Player &player)
    {
        Object guildBg("images/bg_guild_1.png", {0.f, 0.f}, {1.f, 1.f});

        Button buttons[4] = {
            Button(945, 10, 43, 43),   // exit
            Button(24, 534, 45, 43),   // back
            Button(328, 316, 140, 40), // create
            Button(527, 314, 140, 40), // Join
        };

        while (window.isOpen())
        {
            if (checking_status(player))
            {
                cout << "Player is already in a guild" << endl;
                assigning_role(window, player);
                return;
            }

            Event event;
            while (window.pollEvent(event))
            {
                if (event.type == Event::Closed)
                {
                    window.close();
                    exit(EXIT_SUCCESS);
                }

                if (event.type == Event::MouseButtonPressed)
                {
                    Vector2i mousePos = Mouse::getPosition(window);
                    cout << mousePos.x << "," << mousePos.y << endl;

                    if (buttons[0].isClicked(mousePos))
                    {
                        window.close();
                        exit(EXIT_SUCCESS);
                        cout << " exit is pressed" << endl;
                    }
                    else if (buttons[1].isClicked(mousePos))
                    {
                        cout << " back is pressed" << endl;
                        return;
                    }
                    else if (buttons[2].isClicked(mousePos))
                    {
                        cout << " join is pressed" << endl;
                        handleJoinResponse(window, player);
                        return;
                    }
                    else if (buttons[3].isClicked(mousePos))
                    {
                        cout << " create is pressed" << endl;
                        handleCreateResponse(window, player);
                    }
                }
            }

            window.clear();
            guildBg.draw(window);

            write(window, "unknown", 202, 15, 21);
            write(window, "00", 202, 42, 21);

            window.display();
        }
    }
};