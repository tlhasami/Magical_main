#pragma once
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

class Guild_info
{
private:
  string *guildNames;
  int *guildScores;
  int capacity;
  int currentGuilds;

public:
  Guild_info()
  {
    guildNames = nullptr;
    guildScores = nullptr;

    ifstream guildFile("guilds.txt");
    if (!guildFile.is_open())
    {
      cerr << "Error: Could not open guilds.txt file!" << endl;
      exit(EXIT_FAILURE);
    }

    // First pass to count entries
    currentGuilds = 0;
    string line;
    while (getline(guildFile, line))
      ++currentGuilds;

    capacity = currentGuilds;
    guildNames = new string[capacity];
    guildScores = new int[capacity];

    // Second pass to read data
    guildFile.clear();
    guildFile.seekg(0, ios::beg);

    for (int i = 0; i < currentGuilds; ++i)
      guildFile >> guildNames[i] >> guildScores[i];

    guildFile.close();

    // Selection Sort: Sort guilds by score descending
    for (int i = 0; i < currentGuilds - 1; ++i)
    {
      int maxIndex = i;
      for (int j = i + 1; j < currentGuilds; ++j)
      {
        if (guildScores[j] > guildScores[maxIndex])
          maxIndex = j;
      }

      if (maxIndex != i)
      {
        // Swap scores
        int tempScore = guildScores[i];
        guildScores[i] = guildScores[maxIndex];
        guildScores[maxIndex] = tempScore;

        // Swap names
        string tempName = guildNames[i];
        guildNames[i] = guildNames[maxIndex];
        guildNames[maxIndex] = tempName;
      }
    }

    // Debug print
    for (int i = 0; i < currentGuilds; ++i)
      cout << guildNames[i] << " " << guildScores[i] << endl;
  }

  ~Guild_info()
  {
    delete[] guildNames;
    delete[] guildScores;
  }

  void appendGuildToFile(const string &guildName, int score)
  {
    // Append to file
    ofstream guildFile("guilds.txt", ios::app);
    if (!guildFile.is_open())
    {
      cerr << "Error: Could not open guilds.txt for appending!" << endl;
      return;
    }
    guildFile << guildName << ' ' << score << '\n';
    guildFile.close();

    // Update in-memory arrays
    string *newNames = new string[currentGuilds + 1];
    int *newScores = new int[currentGuilds + 1];

    for (int i = 0; i < currentGuilds; ++i)
    {
      newNames[i] = guildNames[i];
      newScores[i] = guildScores[i];
    }

    newNames[currentGuilds] = guildName;
    newScores[currentGuilds] = score;

    delete[] guildNames;
    delete[] guildScores;

    guildNames = newNames;
    guildScores = newScores;
    ++currentGuilds;
    capacity = currentGuilds;
  }

  bool isPresent(const string &guildName)
  {
    for (int i = 0; i < currentGuilds; ++i)
    {
      if (guildNames[i] == guildName)
      {
        return true;
      }
    }
    return false;
  }

  int getScore(const string &guildName)
  {
    for (int i = 0; i < currentGuilds; ++i)
    {
      if (guildNames[i] == guildName)
      {
        return guildScores[i];
      }
    }
    return -1; // Not found
  }

  string getGuildName(int idx) const
  {
    if (idx >= currentGuilds)
    {
      return "Empty Guild Slot";
    }
    return guildNames[idx];
  }
  string *getAllGuildsAsStrings() const
  {
    string *guildList = new string[currentGuilds];
    for (int i = 0; i < currentGuilds; ++i)
    {
      guildList[i] = guildNames[i] + " - Score: " + to_string(guildScores[i]);
    }
    return guildList;
  }
  void addScoreAndUpdateFile(const string &guildName, int scoreToAdd)
  {
    bool found = false;
    for (int i = 0; i < currentGuilds; ++i)
    {
      if (guildNames[i] == guildName)
      {
        guildScores[i] += scoreToAdd;
        found = true;
        break;
      }
    }

    if (!found)
    {
      appendGuildToFile(guildName, scoreToAdd);
    }

    // Re-sort after update or insertion
    for (int i = 0; i < currentGuilds - 1; ++i)
    {
      int maxIndex = i;
      for (int j = i + 1; j < currentGuilds; ++j)
      {
        if (guildScores[j] > guildScores[maxIndex])
          maxIndex = j;
      }

      if (maxIndex != i)
      {
        swap(guildScores[i], guildScores[maxIndex]);
        swap(guildNames[i], guildNames[maxIndex]);
      }
    }

    writeAllGuildsToFile();
  }

  int getGuildCount() const { return currentGuilds; }
  void writeAllGuildsToFile() const
  {
    ofstream guildFile("guilds.txt");
    if (!guildFile.is_open())
    {
      cerr << "Error: Could not open guilds.txt for writing!" << endl;
      return;
    }

    for (int i = 0; i < currentGuilds; ++i)
    {
      guildFile << guildNames[i] << ' ' << guildScores[i] << '\n';
    }

    guildFile.close();
  }
};