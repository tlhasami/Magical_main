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
#include "fireball.h"


extern Music music;
extern Font font;

extern User_info DATABASE;
extern Guild_info GUILD_PRESENT;


class FallingRock {
    private:
        Sprite sprite;
        Texture texture;
        Vector2f position;
        Vector2f velocity;
        bool active;
        Clock animationClock;
        Clock fallClock;
        float animationSpeed;
        int currentFrame;
        int frameWidth;
        int frameHeight;
        int totalFrames;
        float speed;
    
    public:
        FallingRock() : active(false), animationSpeed(0.025f), currentFrame(0), 
                       frameWidth(64), frameHeight(64), totalFrames(16), speed(270.f) {
            if (!texture.loadFromFile("images/rock.png")) {
                cerr << "Failed to load rock texture!" << endl;
            }
            sprite.setTexture(texture);
            sprite.setTextureRect(IntRect(0, 0, frameWidth, frameHeight));
            sprite.setOrigin(frameWidth/2, frameHeight/2);
            sprite.setScale(1.3f,1.3f);
        }
    
        void activate(Vector2f startPos) {
            position = startPos;
            velocity = Vector2f(0, speed);
            active = true;
            currentFrame = 0;
            animationClock.restart();
            fallClock.restart();
            sprite.setPosition(position);
            sprite.setTextureRect(IntRect(0, 0, frameWidth, frameHeight));
        }
    
        void update(float deltaTime, const RenderWindow& window) {
            if (!active) return;
    
            // Update animation
            if (animationClock.getElapsedTime().asSeconds() >= animationSpeed) {
                currentFrame = (currentFrame + 1) % totalFrames;
                sprite.setTextureRect(IntRect(currentFrame * frameWidth, 0, frameWidth, frameHeight));
                animationClock.restart();
            }
    
            // Update position
            position += velocity * deltaTime;
            sprite.setPosition(position);
    
            // Deactivate if out of screen
            if (position.y > window.getSize().y + frameHeight) {
                active = false;
            }
        }
    
        void draw(RenderWindow& window) {
            if (active) {
                window.draw(sprite);
            }
        }
    
        bool isActive() const { return active; }
        FloatRect getGlobalBounds() const { return sprite.getGlobalBounds(); }
        void deactivate() { active = false; }
        float getFallTime() const { return fallClock.getElapsedTime().asSeconds(); }
        Vector2f getPosition() const { return position; }
    };
