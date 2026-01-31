#pragma once
#include <iostream>
#include <string>
#include "guild.h"
#include "pets.h"
#include "user.h"

extern User_info DATABASE;
extern Guild_info GUILD_PRESENT;

class Items
{
protected:
  int item1 = 0;
  int item2 = 0;
  int item3 = 0;
  int item4 = 0;
  int item5 = 0;
  int item6 = 0;
  int gems = 5;
  int coins = 30;

public:
  Items() = default;

  // Setters
  void setItem1(int value) { item1 = value; }
  void setItem2(int value) { item2 = value; }
  void setItem3(int value) { item3 = value; }
  void setItem4(int value) { item4 = value; }
  void setItem5(int value) { item5 = value; }
  void setItem6(int value) { item6 = value; }
  void setGems(int value) { gems = value; }
  void setCoins(int value) { coins = value; }

  // Adders
  void addItem1(int val) { item1 += val; }
  void addItem2(int val) { item2 += val; }
  void addItem3(int val) { item3 += val; }
  void addItem4(int val) { item4 += val; }
  void addItem5(int val) { item5 += val; }
  void addItem6(int val) { item6 += val; }
  void addGems(int val) { gems += val; }
  void addCoins(int val) { coins += val; }

  // Getters
  int getItem1() const { return item1; }
  int getItem2() const { return item2; }
  int getItem3() const { return item3; }
  int getItem4() const { return item4; }
  int getItem5() const { return item5; }
  int getItem6() const { return item6; }
  int getGems() const { return gems; }
  int getCoins() const { return coins; }

  void addItems(int idx)
  {
  }
};

class Player : public Items
{
private:
  std::string name;
  int guildId = 0;
  std::string guildName;

  // Cooldown system
  RectangleShape cooldown_showcase_player[6];
  Clock potion_cooldown[6];
  bool item_on_cool[6] = {false};
  float cooldown_durations[6] = {5.f, 5.f, 5.f, 5.f, 5.f, 5.f}; // Customize as needed

public:
  Griffin griffin;
  Dragon dragon;
  Phoenix phoenix;
  Unicorn unicorn;
  Player(const std::string &playerName = "AIBOT")
      : name(playerName),
        griffin("images/griffin.png",
                rand() % static_cast<int>(0.95 * 1000) + 0.05 * 1000,
                rand() % static_cast<int>(0.95 * 600) + 0.05 * 600,
                0.75f, 0.75f, 9),
        dragon("images/dragon.png",
               rand() % static_cast<int>(0.95 * 1000) + 0.05 * 1000,
               rand() % static_cast<int>(0.95 * 600) + 0.05 * 600,
               1.0f, 1.0f, 9),
        phoenix("images/phoniex.png",
                rand() % static_cast<int>(0.95 * 1000) + 0.05 * 1000,
                rand() % static_cast<int>(0.95 * 600) + 0.05 * 600,
                1.125f, 1.125f, 8),
        unicorn("images/unicorn.png",
                rand() % static_cast<int>(0.95 * 1000) + 0.05 * 1000,
                rand() % static_cast<int>(0.95 * 600) + 0.05 * 600,
                1.255f, 1.255f, 8)
  {
    griffin.sprite.setOrigin(griffin.sprite.getLocalBounds().width / 2,
                             griffin.sprite.getLocalBounds().height / 2);

    phoenix.sprite.setOrigin(phoenix.sprite.getLocalBounds().width / 2,
                             phoenix.sprite.getLocalBounds().height / 2);

    dragon.sprite.setOrigin(dragon.sprite.getLocalBounds().width / 2,
                            dragon.sprite.getLocalBounds().height / 2);

    unicorn.sprite.setOrigin(unicorn.sprite.getLocalBounds().width / 2,
                             unicorn.sprite.getLocalBounds().height / 2);
  }

  // Name methods
  void setName(const std::string &playerName) { name = playerName; }
  std::string getName() const { return name; }

  // Guild methods
  void setGuildId(int id) { guildId = id; }
  int getGuildId() const { return guildId; }
  void setGuildName(const std::string &name) { guildName = name; }
  std::string getGuildName() const { return guildName; }

  void draw(RenderWindow &window, float delta)
  {
    griffin.updateAnimation(delta);
    unicorn.updateAnimation(delta);
    phoenix.updateAnimation(delta);
    dragon.updateAnimation(delta);

    griffin.draw(window);
    unicorn.draw(window);
    phoenix.draw(window);
    dragon.draw(window);
  }

  void reduceSizeMenu()
  {
    // Get current scales and reduce to 60%
    Vector2f griffinScale = griffin.sprite.getScale();
    float size = 0.5;
    griffin.sprite.setScale(griffinScale.x * size + 0.05f, griffinScale.y * size + 0.05f);

    Vector2f unicornScale = unicorn.sprite.getScale();
    unicorn.sprite.setScale(unicornScale.x * size + 0.15f, unicornScale.y * size + 0.15f);

    Vector2f phoenixScale = phoenix.sprite.getScale();
    phoenix.sprite.setScale(phoenixScale.x * size + 0.15, phoenixScale.y * size + 0.15);

    Vector2f dragonScale = dragon.sprite.getScale();
    dragon.sprite.setScale(dragonScale.x * size, dragonScale.y * size);
  }

  void increaseSize()
  {
    griffin.sprite.setScale(1.f, 1.f);
    dragon.sprite.setScale(1.05f, 1.05f);
    unicorn.sprite.setScale(1.325f, 1.325f);
    phoenix.sprite.setScale(1.f, 1.f);
  }

  Pet *getPet(int idx)
  {
    if (idx == 0)
    {
      return &dragon;
    }
    else if (idx == 1)
    {
      return &griffin;
    }
    else if (idx == 2)
    {
      return &unicorn;
    }
    else if (idx == 3)
    {
      return &phoenix;
    }

    return nullptr;
  }

  Player &operator=(const Player &other)
  {
    if (this != &other)
    { // Check for self-assignment
      // Copy Items base class members
      static_cast<Items &>(*this) = other;

      // Copy Player-specific members
      name = other.name;
      guildId = other.guildId;
      guildName = other.guildName;

      // Manually copy dragon attributes
      dragon.sprite = other.dragon.sprite;
      dragon.setAttack(other.dragon.getAttack());
      dragon.setDefence(other.dragon.getDefence());
      dragon.setHealth(other.dragon.getHealth());
      dragon.setMaxHealth(other.dragon.getMaxHealth());
      dragon.setSpeed(other.dragon.getSpeed());
      dragon.setLevel(other.dragon.getLevel());
      dragon.setRole(other.dragon.getRole());

      // Manually copy griffin attributes
      griffin.sprite = other.griffin.sprite;
      griffin.setAttack(other.griffin.getAttack());
      griffin.setDefence(other.griffin.getDefence());
      griffin.setHealth(other.griffin.getHealth());
      griffin.setMaxHealth(other.griffin.getMaxHealth());
      griffin.setSpeed(other.griffin.getSpeed());
      griffin.setLevel(other.griffin.getLevel());
      griffin.setRole(other.griffin.getRole());

      // Manually copy phoenix attributes
      phoenix.sprite = other.phoenix.sprite;
      phoenix.setAttack(other.phoenix.getAttack());
      phoenix.setDefence(other.phoenix.getDefence());
      phoenix.setHealth(other.phoenix.getHealth());
      phoenix.setMaxHealth(other.phoenix.getMaxHealth());
      phoenix.setSpeed(other.phoenix.getSpeed());
      phoenix.setLevel(other.phoenix.getLevel());
      phoenix.setRole(other.phoenix.getRole());

      // Manually copy unicorn attributes
      unicorn.sprite = other.unicorn.sprite;
      unicorn.setAttack(other.unicorn.getAttack());
      unicorn.setDefence(other.unicorn.getDefence());
      unicorn.setHealth(other.unicorn.getHealth());
      unicorn.setMaxHealth(other.unicorn.getMaxHealth());
      unicorn.setSpeed(other.unicorn.getSpeed());
      unicorn.setLevel(other.unicorn.getLevel());

      unicorn.setRole(other.unicorn.getRole());
    }
    return *this;
  }

  // Initialize cooldown indicators (call this after window is created)
  void initCooldownIndicators()
  {
    for (int i = 0; i < 6; i++)
    {
      cooldown_showcase_player[i].setSize(Vector2f(40, 5));
      cooldown_showcase_player[i].setFillColor(Color::Red);
      cooldown_showcase_player[i].setPosition(100 + i * 50, 550); // Adjust position as needed
    }
  }

  // Use an item with cooldown
  bool useItem(int itemIndex)
  {
    if (itemIndex < 0 || itemIndex >= 6)
      return false;

    if (!item_on_cool[itemIndex])
    {
      // Perform item effect here
      // ...

      // Start cooldown
      item_on_cool[itemIndex] = true;
      potion_cooldown[itemIndex].restart();
      return true;
    }
    return false;
  }

  // Update cooldowns (call this every frame)
  void updateCooldowns(float deltaTime)
  {
    for (int i = 0; i < 6; i++)
    {
      if (item_on_cool[i])
      {
        float elapsed = potion_cooldown[i].getElapsedTime().asSeconds();
        if (elapsed >= cooldown_durations[i])
        {
          item_on_cool[i] = false;
        }
      }
    }
  }

  // Draw cooldown indicators
  void drawCooldowns(RenderWindow &window)
  {
    for (int i = 0; i < 6; i++)
    {
      if (item_on_cool[i])
      {
        float elapsed = potion_cooldown[i].getElapsedTime().asSeconds();
        float ratio = elapsed / cooldown_durations[i];

        // Update cooldown bar
        float width = 40 * (1 - ratio);
        cooldown_showcase_player[i].setSize(Vector2f(width, 5));

        // Color transition from red to green
        if (ratio > 0.66f)
        {
          cooldown_showcase_player[i].setFillColor(Color::Red);
        }
        else if (ratio > 0.33f)
        {
          cooldown_showcase_player[i].setFillColor(Color(255, 165, 0)); // Orange
        }
        else
        {
          cooldown_showcase_player[i].setFillColor(Color::Green);
        }

        window.draw(cooldown_showcase_player[i]);
      }
    }
  }

  // Check if item is on cooldown
  bool isItemOnCooldown(int index) const
  {
    if (index >= 0 && index < 6)
    {
      return item_on_cool[index];
    }
    return false;
  }
};