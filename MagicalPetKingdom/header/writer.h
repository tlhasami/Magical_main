#pragma once

#include <fstream>
#include <iostream>
#include "player.h"

using namespace std;

void loadPlayerData(Player &player, const string &filename)
{
        if (filename == "")
                return;
        ifstream inFile("DataBase/" + filename + ".txt");
        if (!inFile.is_open())
        {
                cerr << "Error: Could not open file " << filename << " for reading!" << endl;
                return;
        }

        string name;
        getline(inFile >> ws, name);
        player.setName(name);
        cout << "Loaded Name: " << name << endl;

        int t1, t2, t3, t4, t5, t6, gems, coins;
        inFile >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> gems >> coins;
        player.setItem1(t1);
        player.setItem2(t2);
        player.setItem3(t3);
        player.setItem4(t4);
        player.setItem5(t5);
        player.setItem6(t6);
        player.setGems(gems);
        player.setCoins(coins);
        cout << "Loaded Stats: " << t1 << " " << t2 << " " << t3 << " " << t4 << " " << t5 << " " << t6 << " " << gems << " " << coins << endl;

        int attack, defence, health, maxHealth, level;
        string petName, petRole;

        // Griffin
        inFile >> attack >> defence >> health >> maxHealth >> level;
        inFile.ignore();
        getline(inFile, petName);
        getline(inFile, petRole);
        player.griffin.setAttack(attack);
        player.griffin.setDefence(defence);
        player.griffin.setHealth(health);
        player.griffin.setMaxHealth(maxHealth);
        player.griffin.setLevel(level);
        player.griffin.setPetName(petName);
        player.griffin.setRole(petRole);
        cout << "Loaded Griffin: " << attack << " " << defence << " " << health << " " << maxHealth << " " << level << " | " << petName << " | " << petRole << endl;

        // Phoenix
        inFile >> attack >> defence >> health >> maxHealth >> level;
        inFile.ignore();
        getline(inFile, petName);
        getline(inFile, petRole);
        player.phoenix.setAttack(attack);
        player.phoenix.setDefence(defence);
        player.phoenix.setHealth(health);
        player.phoenix.setMaxHealth(maxHealth);
        player.phoenix.setLevel(level);
        player.phoenix.setPetName(petName);
        player.phoenix.setRole(petRole);
        cout << "Loaded Phoenix: " << attack << " " << defence << " " << health << " " << maxHealth << " " << level << " | " << petName << " | " << petRole << endl;

        // Dragon
        inFile >> attack >> defence >> health >> maxHealth >> level;
        inFile.ignore();
        getline(inFile, petName);
        getline(inFile, petRole);
        player.dragon.setAttack(attack);
        player.dragon.setDefence(defence);
        player.dragon.setHealth(health);
        player.dragon.setMaxHealth(maxHealth);
        player.dragon.setLevel(level);
        player.dragon.setPetName(petName);
        player.dragon.setRole(petRole);
        cout << "Loaded Dragon: " << attack << " " << defence << " " << health << " " << maxHealth << " " << level << " | " << petName << " | " << petRole << endl;

        // Unicorn
        inFile >> attack >> defence >> health >> maxHealth >> level;
        inFile.ignore();
        getline(inFile, petName);
        getline(inFile, petRole);
        player.unicorn.setAttack(attack);
        player.unicorn.setDefence(defence);
        player.unicorn.setHealth(health);
        player.unicorn.setMaxHealth(maxHealth);
        player.unicorn.setLevel(level);
        player.unicorn.setPetName(petName);
        player.unicorn.setRole(petRole);
        cout << "Loaded Unicorn: " << attack << " " << defence << " " << health << " " << maxHealth << " " << level << " | " << petName << " | " << petRole << endl;

        int guildId;
        string guildName;
        inFile >> guildId;
        getline(inFile >> ws, guildName);
        player.setGuildId(guildId);
        player.setGuildName(guildName);
        cout << "Loaded Guild: " << guildId << " | " << guildName << endl;

        inFile.close();
}

void savePlayerData(const Player &player, const string &filename)
{
        if (filename == "")
                return;
        ofstream outFile("DataBase/" + filename + ".txt");
        if (!outFile)
        {
                cerr << "Error opening file for writing!" << endl;
                return;
        }

        outFile << player.getName() << endl;
        cout << "Saved Name: " << player.getName() << endl;

        outFile << player.getItem1() << " " << player.getItem2() << " " << player.getItem3() << " "
                << player.getItem4() << " " << player.getItem5() << " " << player.getItem6() << " "
                << player.getGems() << " " << player.getCoins() << endl;
        cout << "Saved Stats: " << player.getItem1() << " " << player.getItem2() << " " << player.getItem3()
             << " " << player.getItem4() << " " << player.getItem5() << " " << player.getItem6() << " "
             << player.getGems() << " " << player.getCoins() << endl;

        // Griffin
        outFile << player.griffin.getAttack() << " " << player.griffin.getDefence() << " "
                << player.griffin.getHealth() << " " << player.griffin.getMaxHealth() << " "
                << player.griffin.getLevel() << endl;
        outFile << player.griffin.getPetName() << endl;
        outFile << player.griffin.getRole() << endl;
        cout << "Saved Griffin: " << player.griffin.getAttack() << " " << player.griffin.getDefence()
             << " " << player.griffin.getHealth() << " " << player.griffin.getMaxHealth()
             << " " << player.griffin.getLevel() << " | " << player.griffin.getPetName()
             << " | " << player.griffin.getRole() << endl;

        // Phoenix
        outFile << player.phoenix.getAttack() << " " << player.phoenix.getDefence() << " "
                << player.phoenix.getHealth() << " " << player.phoenix.getMaxHealth() << " "
                << player.phoenix.getLevel() << endl;
        outFile << player.phoenix.getPetName() << endl;
        outFile << player.phoenix.getRole() << endl;
        cout << "Saved Phoenix: " << player.phoenix.getAttack() << " " << player.phoenix.getDefence()
             << " " << player.phoenix.getHealth() << " " << player.phoenix.getMaxHealth()
             << " " << player.phoenix.getLevel() << " | " << player.phoenix.getPetName()
             << " | " << player.phoenix.getRole() << endl;

        // Dragon
        outFile << player.dragon.getAttack() << " " << player.dragon.getDefence() << " "
                << player.dragon.getHealth() << " " << player.dragon.getMaxHealth() << " "
                << player.dragon.getLevel() << endl;
        outFile << player.dragon.getPetName() << endl;
        outFile << player.dragon.getRole() << endl;
        cout << "Saved Dragon: " << player.dragon.getAttack() << " " << player.dragon.getDefence()
             << " " << player.dragon.getHealth() << " " << player.dragon.getMaxHealth()
             << " " << player.dragon.getLevel() << " | " << player.dragon.getPetName()
             << " | " << player.dragon.getRole() << endl;

        // Unicorn
        outFile << player.unicorn.getAttack() << " " << player.unicorn.getDefence() << " "
                << player.unicorn.getHealth() << " " << player.unicorn.getMaxHealth() << " "
                << player.unicorn.getLevel() << endl;
        outFile << player.unicorn.getPetName() << endl;
        outFile << player.unicorn.getRole() << endl;
        cout << "Saved Unicorn: " << player.unicorn.getAttack() << " " << player.unicorn.getDefence()
             << " " << player.unicorn.getHealth() << " " << player.unicorn.getMaxHealth()
             << " " << player.unicorn.getLevel() << " | " << player.unicorn.getPetName()
             << " | " << player.unicorn.getRole() << endl;

        outFile << player.getGuildId() << endl;
        outFile << player.getGuildName() << endl;
        cout << "Saved Guild: " << player.getGuildId() << " | " << player.getGuildName() << endl;

        outFile.close();
}
