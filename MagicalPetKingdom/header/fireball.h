#pragma once

#pragma once

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

class Fireball
{
private:
    Texture texture;
    Sprite sprite;
    Vector2f direction;
    float speed;
    bool active;
    Clock lifetimeClock;
    float maxLifetime;

    // Animation
    int currentFrame;
    const int totalFrames = 6;
    const float frameWidth = 91.5f;
    const float frameHeight = 92.f;
    float frameTime = 0.05f; // Time between frames
    Clock animationClock;

public:
    Fireball(float spd = 350.f, float lifetime = 2.f)
        : speed(spd), maxLifetime(lifetime), active(false), currentFrame(0)
    {
        if (!texture.loadFromFile("images/fireball.png"))
        {
            cerr << "Failed to load fireball texture\n";
        }
        sprite.setTexture(texture);
        sprite.setTextureRect(IntRect(0, 0, static_cast<int>(frameWidth), static_cast<int>(frameHeight)));
        sprite.setScale(0.7, 0.7);

        sprite.setOrigin(frameWidth / 2.f, frameHeight / 2.f);
    }

    void color(int col)
    {
        if (col == 0)
        {
            cout << "color changes " << endl;
            sprite.setColor(Color::Blue);
        }
        else if (col == 1)
        {
            cout << "color changes " << endl;
            sprite.setColor(Color::White);
        }
        else if (col == 2)
        {
            cout << "color changes " << endl;
            sprite.setColor(Color::Red); // Light Red (custom)
        }
        else if (col == 3)
        {
            cout << "color changes " << endl;
            sprite.setColor(Color::Cyan); // Sky Blue (custom light blue)
        }
    }

    void activate(const Vector2f &startPos, const Vector2f &targetPos)
    {
        sprite.setPosition(startPos);
        direction = targetPos - startPos;
        float length = sqrt(direction.x * direction.x + direction.y * direction.y);
        if (length > 0)
            direction /= length;

        active = true;
        lifetimeClock.restart();
        animationClock.restart();
        currentFrame = 0;
        updateFrame(); // Set first frame
    }

    void update(float deltaTime)
    {
        if (!active)
            return;

        sprite.move(direction * speed * deltaTime);

        // Lifetime check
        if (lifetimeClock.getElapsedTime().asSeconds() > maxLifetime)
        {
            active = false;
            return;
        }

        // Animation frame update
        if (animationClock.getElapsedTime().asSeconds() >= frameTime)
        {
            currentFrame = (currentFrame + 1) % totalFrames;
            updateFrame();
            animationClock.restart();
        }
    }

    void draw(RenderWindow &window) const
    {
        if (active)
            window.draw(sprite);
    }

    bool isActive() const { return active; }
    FloatRect getGlobalBounds() const { return sprite.getGlobalBounds(); }
    void deactivate() { active = false; }

    Vector2f getPosition()
    {
        return sprite.getPosition();
    }

    void setScalee(float x)
    {
        sprite.setScale({x,x});
    }

    float getScalee() const {
        return sprite.getScale().x;
    }
    float getRadius() const
    {
        return sprite.getGlobalBounds().width / 2.f;
    }

private:
    void updateFrame()
    {
        sprite.setTextureRect(IntRect(static_cast<int>(currentFrame * frameWidth), 0, static_cast<int>(frameWidth), static_cast<int>(frameHeight)));
    }
};
