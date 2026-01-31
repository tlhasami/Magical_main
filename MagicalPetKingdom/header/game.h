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
#include "guild_selection.h"
#include "battle.h"

/*
Global variable for the screen resolution and other stuff extern use for 
not reintializing it
*/
Vector2f screenResolution(1000, 600);

extern Music music;
extern Font font;

extern User_info DATABASE;
extern Guild_info GUILD_PRESENT;

class Game
{
private:

// it handles all the class inside it and keep calling the funciton needed
    MENU menu;
    INVENTORY inventory;
    SHOP shop;
    TRAINING training;
    INFO info;
    GUILD_AREA guild;
    BATTLE battle;
    // it is create for the game login system 
    entry going_in;

public:
    Game() {};

    // this is game starter
    void game(RenderWindow &window)
    {

        // string for the name
        string username;

   

        // Player player("tlhasami");
// a dummy laoding screen 
        starting_screen::hello(window);
        // An info scren after pressing the enter the game starts
        if (starting_screen::loading(window))
        {
            do
            {
                cout << "Enter the main game " << endl;
                username = going_in.login_or_signup(window);
            } while (username == "" && window.isOpen());
        }
        Player player(username);


        player.reduceSizeMenu();

        // read or wirte ir create the file according to the current situation
        if (DATABASE.isPresent(username))
        {
            loadPlayerData(player, username);
        }
        else
        {
            savePlayerData(player, username);
            DATABASE.appendUserToFile(username);
        }

        int choice = 0;

        do
        {
            // here the menu starts

            choice = menu.selection(window, player);

            switch (choice)
            {
            case -1:
            {
                // leaving game
                savePlayerData(player, username);
                exit(EXIT_SUCCESS);
                break;
            }
            case 11:
            {
                // again reloading the game from the start
                savePlayerData(player, username);
                game(window);
                exit(EXIT_SUCCESS);
                break;
            }
            case 1:
            {
                // going to guild
                guild.checkGuildAccess(window, player);
                break;
            }
            case 2:
            {
                player.increaseSize();
                // going to info checking
                info.information(window, player);
                player.reduceSizeMenu();
                break;
            }
            case 3:
            {
                // going to shop
                shop.shopping(window, player);
                break;
            }
            case 4:
            {
                // going to ITEMS

                inventory.checking(window, player);

                break;
            }
            case 5:
            {
                cout << "going into training" << endl;
                // Going for a training
                training.training_center(window, player);
                break;
            }
            case 6:
            {
                cout << "going for battle" << endl;
                // Going for a training
                battle.start_battle(window, player);
                break;
            }
            }
            savePlayerData(player, player.getName());
        } while (true);

        window.clear();
        return;
    }
};