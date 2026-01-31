#pragma once
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

class User_info
{
private:
  string *name;
  int capacity;
  int currentUsers;

public:
  User_info()
  {
    name = nullptr;

    ifstream userFile("users.txt");
    if (!userFile.is_open())
    {
      cerr << "Error: Could not open users.txt file!" << endl;
      exit(EXIT_FAILURE);
    }

    // Calculate number of users
    currentUsers = 0;
    string line;
    while (getline(userFile, line))
    {
      ++currentUsers;
    }

    // Allocate memory for user names
    capacity = currentUsers;
    name = new string[capacity];

    // Move the file reading pointer to the start
    userFile.clear();
    userFile.seekg(0, ios::beg);

    // Read all user names
    for (int i = 0; i < currentUsers; ++i)
    {
      getline(userFile, name[i]);
      cout << name[i] << endl;
    }

    userFile.close();
  }

  ~User_info() { delete[] name; }

  void appendUserToFile(const string &newUser)
  {
    // Append to file
    if (newUser == "")
      return;
    ofstream userFile("users.txt", ios::app);
    if (!userFile.is_open())
    {
      cerr << "Error: Could not open users.txt for appending!" << endl;
      return;
    }
    userFile << newUser << '\n';
    userFile.close();

    // Update in-memory array
    string *newArray = new string[currentUsers + 1];
    for (int i = 0; i < currentUsers; ++i)
    {
      newArray[i] = name[i];
    }
    newArray[currentUsers] = newUser;

    delete[] name;
    name = newArray;
    ++currentUsers;
    capacity = currentUsers;
  }

  bool isPresent(const string &username)
  {
    for (int i = 0; i < currentUsers; ++i)
    {
      if (name[i] == username)
      {
        return true;
      }
    }
    return false;
  }
};