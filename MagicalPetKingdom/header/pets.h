#pragma once
#include <SFML/Graphics.hpp>
#include <string>

#include "object.h"

using namespace sf;

class Pet : public Object
{
protected:
  int MaxHealth;

  int Health;
  int Defence;
  int Attack;

  int TotalFrames;
  int currentFrame;

  float speed;

  float animationTimer;

  std::string petName;

  int level;

  string role;

public:
  bool defenceActivation = false;

  // Constructor for Pet
  Pet(std::string path, float startX, float startY, float scaleX, float scaleY,
      int totalFrames, const std::string &name)
      : Object(path, {startX, startY}, {scaleX, scaleY}),
        TotalFrames(totalFrames),
        currentFrame(0),
        animationTimer(0.0f),
        petName(name),
        level(1),
        role("") {}
  Sprite &getSprite() { return sprite; } // assuming `sprite` is a member of Pet

  int getMaxHealth() const { return MaxHealth; }

  void setPetName(const std::string &name) { petName = name; }

  std::string getPetName() const { return petName; }

  void addMaxHealth(int val)
  {
    if (MaxHealth + val > 180)
    {
      val = 180 - MaxHealth; // Adjust the value to not exceed the limit
    }
    MaxHealth += val;
    Health += val; // Increase current health as well
    if (Health > MaxHealth)
      Health = MaxHealth;
  }

  void takeDamage(int amount)
  {
    int damageAfterDefence = amount;
    if (damageAfterDefence < 0)
      damageAfterDefence = 0;

    Health -= damageAfterDefence;
    if (Health < 0)
      Health = 0;
  }

  void setRole(const string &r) { role = r; }

  string getRole() const { return role; }
  void addHealth(int val)
  {
    if (Health + val > MaxHealth)
    {
      val = MaxHealth - Health; // Adjust the value to not exceed MaxHealth
    }
    Health += val;
  }
  void addDefence(int val) { Defence += val; }
  void addAttack(int val) { Attack += val; }

  void increaseAttack(int amount) { Attack += amount; }

  void increaseDefence(int amount) { Defence += amount; }

  void increaseHealth(int amount)
  {
    Health += amount;
    if (Health > MaxHealth)
      Health = MaxHealth;
  }

  bool isAlive() const { return Health > 0; }

  void moveObject(Vector2f velocity, float deltaTime, Vector2f windowSize)
  {
    // Move the sprite first
    sprite.move(velocity.x * deltaTime, velocity.y * deltaTime);

    // Get the sprite's global bounds (accounts for scale and rotation)
    FloatRect bounds = sprite.getGlobalBounds();

    // Calculate half dimensions for centered origin
    float halfWidth = bounds.width / 2;
    float halfHeight = bounds.height / 2;

    // Boundary checking with centered origin
    if (sprite.getPosition().x - halfWidth < 0) // Left edge
    {
      sprite.setPosition(halfWidth, sprite.getPosition().y);
    }
    else if (sprite.getPosition().x + halfWidth > windowSize.x) // Right edge
    {
      sprite.setPosition(windowSize.x - halfWidth, sprite.getPosition().y);
    }

    if (sprite.getPosition().y - halfHeight < 0) // Top edge
    {
      sprite.setPosition(sprite.getPosition().x, halfHeight);
    }
    else if (sprite.getPosition().y + halfHeight >
             windowSize.y) // Bottom edge
    {
      sprite.setPosition(sprite.getPosition().x, windowSize.y - halfHeight);
    }
  }

  int getLevel() const { return level; }

  void increaseLevel(int up = 1) { level += up; }

  void setSpeed(float s) { speed = s; }
  void addSpeed(float s) { speed += s; }

  float getSpeed() const { return speed; }

  void draw(RenderWindow &window, bool health = false)
  {
    window.draw(sprite);
    if (!health)
    {
      return;
    }
  }
  void setAttack(int value) { Attack = value; }
  void setDefence(int value) { Defence = value; }
  void setHealth(int value) { 
    if(value > MaxHealth){
      Health = MaxHealth;
      return;
    }
    Health = value;
  }
  void setLevel(int value) { level = value; }

  void decreaseHealth(int amount)
  {
    Health -= amount;
    if (Health < 0)
    {
      Health = 0;
    }
  }

  int getAttack() const { return Attack; }
  int getDefence() const { return Defence; }
  int getHealth() const { return Health; }
  void setMaxHealth(int value)
  {
    MaxHealth = value;
    Health = value;
  }
  virtual void updateAnimation(float deltaTime) = 0;
};

class Dragon : public Pet
{
private:
  // Animation constants matching your sprite sheet
  const int frameWidth = 384;
  const int frameHeight = 528;
  const int totalFrames = 9;
  const float frameDuration = 0.1f; // seconds per frame

public:
  Dragon(std::string path, float startX, float startY, float scaleX,
         float scaleY, int totalFrames)
      : Pet(path, startX, startY, scaleX, scaleY, totalFrames, "Dragon")
  {
    // Dragon stats
    Health = 150;
    MaxHealth = 200;
    Attack = 10;
    Defence = 25;
    speed = 3.f;

    // Set initial frame
    setIntRect(0, 0, frameWidth, frameHeight);
  }

  void updateAnimation(float deltaTime) override
  {
    animationTimer += deltaTime;

    if (animationTimer >= frameDuration)
    {
      currentFrame = (currentFrame + 1) % totalFrames;

      // Calculate position in sprite sheet
      int row = 0; // Assuming single row animation
      int col = currentFrame;

      // Update texture rectangle
      setIntRect(col * frameWidth, row * frameHeight, frameWidth, frameHeight);

      animationTimer = 0.0f;
    }
  }

  // Optional: Add specialized dragon methods
  void fireballAttack()
  {
    // Implement fireball attack logic
  }

  void dragonsRoar()
  {
    // Implement dragon's roar ability logic
  }
};
class Griffin : public Pet
{
private:
  // Animation constants matching your sprite sheet
  const int frameWidth = 500;
  const int frameHeight = 375;
  const float frameDuration = 0.1f; // seconds per frame
  const int framesInFirstRow = 5;   // First row has 5 frames, second has 4

public:
  Griffin(std::string path, float startX, float startY, float scaleX,
          float scaleY, int totalFrames)
      : Pet(path, startX, startY, scaleX, scaleY, totalFrames, "Griffin")
  {
    // Griffin stats
    Health = 120;
    MaxHealth = 200;
    Attack = 8;
    Defence = 15;
    speed = 2.5f;

    // Set initial frame
    setIntRect(0, 0, frameWidth, frameHeight);
  }

  void updateAnimation(float deltaTime) override
  {
    animationTimer += deltaTime;

    if (animationTimer >= frameDuration)
    {
      currentFrame = (currentFrame + 1) % TotalFrames;

      // Calculate position in sprite sheet (5 frames in first row, 4 in second)
      int row = (currentFrame < framesInFirstRow) ? 0 : 1;
      int col = (currentFrame < framesInFirstRow) ? currentFrame : currentFrame - framesInFirstRow;

      // Update texture rectangle
      setIntRect(col * frameWidth, row * frameHeight, frameWidth, frameHeight);

      animationTimer = 0.0f;
    }
  }

  // Ability methods
  void useClawStrike()
  {

    // Implement claw strike logic
    Attack *= 1.5f; // Temporary attack boost
  }

  void useDiveBomb()
  {

    // Implement dive bomb logic
    Defence *= 0.8f; // Temporary defense reduction for powerful attack
  }
};
class Phoenix : public Pet
{
private:
  // Animation constants matching your sprite sheet
  const int frameWidth = 344;
  const int frameHeight = 616;
  const int totalFrames = 9;
  const float frameDuration = 0.1f; // seconds per frame
  const int framesInFirstRow = 5;   // First row has 5 frames, second has 4

public:
  bool isAbilityUsed = false;

  Phoenix(std::string path, float startX, float startY, float scaleX,
          float scaleY, int totalFrames)
      : Pet(path, startX, startY, scaleX, scaleY, totalFrames, "Phoenix")
  {
    // Phoenix stats
    Health = 110;
    MaxHealth = 200;
    Attack = 5;
    Defence = 10;
    speed = 4.5f;

    // Set initial frame
    setIntRect(0, 0, frameWidth, frameHeight);
  }

  void updateAnimation(float deltaTime) override
  {
    animationTimer += deltaTime;

    if (animationTimer >= frameDuration)
    {
      currentFrame = (currentFrame + 1) % totalFrames;

      // Calculate position in sprite sheet (5 frames in first row, 4 in second)
      int row = (currentFrame < framesInFirstRow) ? 0 : 1;
      int col = (currentFrame < framesInFirstRow) ? currentFrame : currentFrame - framesInFirstRow;

      // Update texture rectangle
      setIntRect(col * frameWidth, row * frameHeight, frameWidth, frameHeight);

      animationTimer = 0.0f;
    }
  }

  // Special abilities
  void flameHealing()
  {
    if (!isAbilityUsed)
    {
      // Implement healing logic
      Health = std::min(Health + 50, MaxHealth);
      isAbilityUsed = true;
    }
  }

  void rebirth()
  {
    if (!isAbilityUsed)
    {
      // Implement rebirth logic
      Health = MaxHealth;
      isAbilityUsed = true;
    }
  }
};

class Unicorn : public Pet
{
private:
  // Animation constants matching your sprite sheet
  const int frameWidth = 256;
  const int frameHeight = 256;
  const float frameDuration = 0.1f; // seconds per frame

  // Frame coordinates in order: (col, row)
  const std::vector<sf::Vector2i> frameOrder = {
      {0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, // First row
      {0, 1},
      {1, 1}, // Second row
      {0, 2}  // Third row
  };

public:
  int howMuchUsed;
  bool absorberActivated;

  Unicorn(std::string path, float startX, float startY, float scaleX,
          float scaleY, int totalFrames)
      : Pet(path, startX, startY, scaleX, scaleY, totalFrames, "Unicorn"),
        howMuchUsed(3), absorberActivated(false)
  {
    // Unicorn stats
    Health = 130;
    MaxHealth = 130;
    Attack = 7;
    Defence = 18;
    speed = 5.f;

    // Set initial frame
    setIntRect(frameOrder[0].x * frameWidth,
               frameOrder[0].y * frameHeight,
               frameWidth,
               frameHeight - 15); // Height adjustment
  }

  void updateAnimation(float deltaTime) override
  {
    animationTimer += deltaTime;

    if (animationTimer >= frameDuration)
    {
      currentFrame = (currentFrame + 1) % frameOrder.size();

      // Update texture rectangle
      const auto &frame = frameOrder[currentFrame];
      setIntRect(frame.x * frameWidth,
                 frame.y * frameHeight,
                 frameWidth,
                 frameHeight - 15); // Height adjustment

      animationTimer = 0.0f;
    }
  }

  // Special abilities
  void speedBurst()
  {
    if (howMuchUsed > 0)
    {
      speed *= 1.5f; // 50% speed boost
      howMuchUsed--;
    }
  }

  void magicShield()
  {
    if (!absorberActivated && howMuchUsed > 0)
    {
      Defence *= 2; // Double defence
      absorberActivated = true;
      howMuchUsed--;
    }
  }
};