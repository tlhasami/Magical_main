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
#include "fallingrock.h"

extern Music music;
extern Font font;

extern User_info DATABASE;
extern Guild_info GUILD_PRESENT;

// Standardized scales for all pets (width, height)
const Vector2f STANDARD_SCALES[4] = {
    Vector2f(0.3f, 0.3f),   // Dragon
    Vector2f(0.24f, 0.24f), // Griffin
    Vector2f(0.45f, 0.45f), // Phoenix
    Vector2f(0.4f, 0.4f)    // Unicorn
};

// Standardized hitbox sizes (relative to scaled sprite size)
const Vector2f HITBOX_SCALES[4] = {
    Vector2f(0.7f, 0.415f), // Dragon hitbox scale
    Vector2f(0.77f, 0.75f), // Griffin hitbox scale
    Vector2f(0.65f, 0.65f), // Phoenix hitbox scale
    Vector2f(0.63f, 0.28f)  // Unicorn hitbox scale
};

const float RADIUS = 250;

// Hitbox class for the major collison so it feel realistic
class Hitbox
{
public:
    RectangleShape rectHitbox;
    CircleShape originCircle;
    Sprite *petSprite;
    float speed;

    Hitbox(Sprite *sprite, const Vector2f &hitboxScale, float moveSpeed = 5.f)
        : petSprite(sprite), speed(moveSpeed)
    {
        FloatRect bounds = sprite->getGlobalBounds();
        rectHitbox.setSize(Vector2f(
            bounds.width * hitboxScale.x,
            bounds.height * hitboxScale.y));
        rectHitbox.setFillColor(Color(255, 0, 0, 50));
        rectHitbox.setOutlineColor(Color::Red);
        rectHitbox.setOutlineThickness(1.f);

        originCircle.setRadius(10.f);
        originCircle.setFillColor(Color(0, 255, 0, 50));
        originCircle.setOutlineColor(Color::Green);
        originCircle.setOutlineThickness(1.f);

        updatePosition();
    }

    void updatePosition()
    {
        FloatRect spriteBounds = petSprite->getGlobalBounds();
        rectHitbox.setPosition(
            spriteBounds.left + (spriteBounds.width - rectHitbox.getSize().x) / 2,
            spriteBounds.top + (spriteBounds.height - rectHitbox.getSize().y) / 2);
        originCircle.setPosition(
            spriteBounds.left + spriteBounds.width / 2 - originCircle.getRadius(),
            spriteBounds.top + spriteBounds.height / 2 - originCircle.getRadius());
    }

    void move(Vector2f offset)
    {
        petSprite->move(offset);
        updatePosition();
    }

    bool collidesWith(const Hitbox &other) const
    {
        return rectHitbox.getGlobalBounds().intersects(other.rectHitbox.getGlobalBounds());
    }

    Vector2f getCenter() const
    {
        return rectHitbox.getPosition() + Vector2f(rectHitbox.getSize().x / 2, rectHitbox.getSize().y / 2);
    }
};

class BATTLE
{

private:
    Player enemy;

    // Global variable for enemy stun
    bool isEnemyStunned;

    bool alive_pets[4] = {true, true, true, true};

    int score = 0, gems = 0, coins = 0;

    bool GameOver = false;

    // Radar system members
    CircleShape radarCircle;
    Clock radarCooldownClock;
    bool isRadarActive = false;
    const float RADAR_RADIUS = 200.f;
    const float RADAR_COOLDOWN = 30.f;
    const float STUN_DURATION = 7.f;
    Clock stunClock;

    const float FIREBALL_COOLDOWN = 25.f;
    Clock fireballClock;

    // Rock system members
    FallingRock rocks[5];
    Clock rockSpawnClock;
    const float ROCK_SPAWN_INTERVAL = 1.5f;
    const float ROCK_SPAWN_WIDTH = 800.f;
    const float ROCK_START_HEIGHT = -100.f;

    // Cooldown system members
    RectangleShape cooldown_showcase_player[6];
    RectangleShape cooldown_background[6];
    Clock potion_cooldown_player[6];
    bool item_on_cool_player[6] = {false};

    // Enemy cooldown system members
    RectangleShape cooldown_showcase_enemy[6];
    RectangleShape cooldown_background_enemy[6];
    Clock potion_cooldown_enemy[6];
    bool item_on_cool_enemy[6] = {false};

    const float cooldown_duration = 20.0f;
    const float COOLDOWN_X = 13;
    const float COOLDOWN_Y_START = 123;
    const float COOLDOWN_Y_OFFSET = 60.f;
    const float COOLDOWN_WIDTH = 42;
    const float COOLDOWN_HEIGHT = 54;

    // Fireball system members
    Fireball playerFireball;
    Fireball enemyFireball;
    Clock playerFireCooldown;
    Clock enemyFireCooldown;
    const float PLAYER_FIRE_COOLDOWN = 1.5f;
    const float ENEMY_FIRE_COOLDOWN = 2.f;

    // initalization of the radrar according to the selected pet as of two pet ability
    void initRadar(int p)
    {
        radarCircle.setRadius(RADAR_RADIUS);
        radarCircle.setOrigin(RADAR_RADIUS, RADAR_RADIUS);
        radarCircle.setOutlineThickness(2.f);

        if (p == 0)
        {
            radarCircle.setFillColor(Color(0, 0, 0, 30));   // Transparent black fill
            radarCircle.setOutlineColor(Color(30, 30, 30)); // Dark grey/black outline
        }
        else if (p == 1)
        {
            radarCircle.setFillColor(Color(255, 255, 100, 50)); // Transparent yellowish fill
            radarCircle.setOutlineColor(Color(255, 215, 0));    // Golden outline
        }
    }

    // moving it with player
    void updateRadarPosition(const Hitbox &playerHitbox)
    {
        if (isRadarActive)
        {
            Vector2f playerCenter = playerHitbox.getCenter();
            radarCircle.setPosition(playerCenter);
        }
    }

    // radar ahndling whther it can be activatedf or not
    void handleRadarInput(const Hitbox &playerHitbox, Hitbox *enemyHitbox, Pet *enemyPet, int petIdx)
    {
        if (Keyboard::isKeyPressed(Keyboard::E) && radarCooldownClock.getElapsedTime().asSeconds() >= RADAR_COOLDOWN && (petIdx == 0 || petIdx == 1))
        {
            isRadarActive = true;
            radarCooldownClock.restart();
            updateRadarPosition(playerHitbox);
        }

        if (isRadarActive && enemyHitbox && enemyPet)
        {
            Vector2f playerCenter = playerHitbox.getCenter();
            Vector2f enemyCenter = enemyHitbox->getCenter();
            Vector2f diff = enemyCenter - playerCenter;
            float distance = sqrt(diff.x * diff.x + diff.y * diff.y);

            // Check if enemy is within radar radius or hitboxes intersect
            bool withinRadius = distance < RADAR_RADIUS;
            bool hitboxIntersection = radarCircle.getGlobalBounds().intersects(
                enemyHitbox->rectHitbox.getGlobalBounds());

            if ((withinRadius || hitboxIntersection) && !isEnemyStunned)
            {
                cout << "Enmey stunned" << endl;
                isEnemyStunned = true;
                stunClock.restart();
            }
        }
        // Check stun duration
        if (isEnemyStunned && stunClock.getElapsedTime().asSeconds() >= STUN_DURATION)
        {
            cout << "stun removed" << endl;
            isEnemyStunned = false;
            isRadarActive = false;
        }
    }

    // after a certain time it spawns a rock
    void spawnRock()
    {
        if (rockSpawnClock.getElapsedTime().asSeconds() >= ROCK_SPAWN_INTERVAL)
        {
            for (int i = 0; i < 5; i++)
            {
                if (!rocks[i].isActive())
                {
                    float xPos = rand() % static_cast<int>(ROCK_SPAWN_WIDTH) + 100.f;
                    rocks[i].activate(Vector2f(xPos, ROCK_START_HEIGHT));
                    rockSpawnClock.restart();
                    break;
                }
            }
        }
    }

    // update teh positon and animation
    void updateRocks(float deltaTime, const RenderWindow &window)
    {
        for (int i = 0; i < 5; i++)
        {
            if (rocks[i].isActive())
            {
                rocks[i].update(deltaTime, window);
            }
        }
    }

    // drawing all the rocks present on screen
    void drawRocks(RenderWindow &window)
    {
        for (int i = 0; i < 5; i++)
        {
            if (rocks[i].isActive())
            {
                rocks[i].draw(window);
            }
        }
    }

    // checking the collison with rock to deal damaga
    void checkRockCollisions(Hitbox &playerHitbox, Hitbox *enemyHitbox, Pet *playerPet, Pet *enemyPet)
    {
        for (int i = 0; i < 5; i++)
        {
            if (rocks[i].isActive())
            {
                FloatRect rockBounds = rocks[i].getGlobalBounds();

                Vector2f playerCenter = playerHitbox.originCircle.getPosition() +
                                        Vector2f(playerHitbox.originCircle.getRadius(),
                                                 playerHitbox.originCircle.getRadius());

                if (rockBounds.contains(playerCenter))
                {
                    if (playerPet)
                        playerPet->takeDamage(15);
                    rocks[i].deactivate();
                    continue;
                }

                if (enemyHitbox)
                {
                    Vector2f enemyCenter = enemyHitbox->originCircle.getPosition() +
                                           Vector2f(enemyHitbox->originCircle.getRadius(),
                                                    enemyHitbox->originCircle.getRadius());

                    if (rockBounds.contains(enemyCenter))
                    {
                        if (enemyPet)
                            enemyPet->takeDamage(15);
                        rocks[i].deactivate();
                    }
                }
            }
        }
    }

    /////////////////////////////////////////////////////////////////

    ///                    HELPER FUCNTIONS OF BAR
    //////////////////////////////////////
    Color getHealthColor(float ratio)
    {
        if (ratio > 0.66f)
        {
            float t = (ratio - 0.66f) / (1.0f - 0.66f);
            return Color(
                static_cast<Uint8>(200 * (1 - t)),
                static_cast<Uint8>(200 + 55 * t),
                0);
        }
        else if (ratio > 0.33f)
        {
            float t = (ratio - 0.33f) / (0.66f - 0.33f);
            return Color(
                255,
                static_cast<Uint8>(255 * (1 - t)),
                0);
        }
        else
        {
            float t = ratio / 0.33f;
            return Color(
                static_cast<Uint8>(255 * t),
                static_cast<Uint8>(100 * t),
                0);
        }
    }

    void drawGradientBar(RenderWindow &window, Vector2f pos, Vector2f barSize, float ratio)
    {
        if (ratio <= 0.f)
        {
            RectangleShape emptyBar(Vector2f(2, barSize.y));
            emptyBar.setPosition(pos);
            emptyBar.setFillColor(Color(80, 80, 80));
            window.draw(emptyBar);
            return;
        }

        float width = barSize.x * ratio;
        Color startColor = getHealthColor(ratio);
        Color endColor = Color(
            std::min(255, startColor.r + 30),
            std::min(255, startColor.g + 30),
            std::min(255, startColor.b + 30));

        VertexArray gradient(Quads, 4);
        gradient[0].position = pos;
        gradient[1].position = Vector2f(pos.x + width, pos.y);
        gradient[2].position = Vector2f(pos.x + width, pos.y + barSize.y);
        gradient[3].position = Vector2f(pos.x, pos.y + barSize.y);

        gradient[0].color = startColor;
        gradient[1].color = endColor;
        gradient[2].color = endColor;
        gradient[3].color = startColor;

        window.draw(gradient);
    }

    void drawPulseEffect(RenderWindow &window, Vector2f pos, Vector2f barSize, float ratio, float time, Color baseColor)
    {
        if (ratio > 0.f)
        {
            float pulseSize = 4.f + 2.f * sin(time * 4.f);
            RectangleShape pulse(Vector2f(barSize.x * ratio + pulseSize, barSize.y + pulseSize));
            pulse.setPosition(pos - Vector2f(pulseSize / 2, pulseSize / 2));
            baseColor.a = static_cast<Uint8>(70 + 50 * (0.5f + 0.5f * sin(time * 4.f)));
            pulse.setFillColor(baseColor);
            window.draw(pulse);
        }
    }

    void drawHealthText(RenderWindow &window, Font &font, string str, Vector2f pos, Color color, unsigned int size)
    {
        Text text;
        text.setFont(font);
        text.setString(str);
        text.setCharacterSize(size);
        text.setFillColor(color);
        text.setPosition(pos);
        window.draw(text);
    }

    void drawModernHealthBars(RenderWindow &window, Pet *playerPet, Pet *enemyPet, float time)
    {
        if (!playerPet || !enemyPet)
            return;

        Vector2f playerBarPos(80, 550);
        Vector2f enemyBarPos(710, 550);
        Vector2f barSize(250, 25);

        float playerHealth = playerPet->getHealth();
        float playerMaxHealth = playerPet->getMaxHealth();
        float enemyHealth = enemyPet->getHealth();
        float enemyMaxHealth = enemyPet->getMaxHealth();

        float playerHealthRatio = (playerMaxHealth > 0) ? playerHealth / playerMaxHealth : 0.f;
        float enemyHealthRatio = (enemyMaxHealth > 0) ? enemyHealth / enemyMaxHealth : 0.f;

        RectangleShape bgBar(barSize);
        bgBar.setFillColor(Color(30, 30, 30, 220));
        bgBar.setOutlineThickness(2);
        bgBar.setOutlineColor(Color(120, 120, 120, 200));

        bgBar.setPosition(playerBarPos);
        window.draw(bgBar);
        bgBar.setPosition(enemyBarPos);
        window.draw(bgBar);

        drawGradientBar(window, playerBarPos, barSize, playerHealthRatio);
        drawGradientBar(window, enemyBarPos, barSize, enemyHealthRatio);

        drawPulseEffect(window, playerBarPos, barSize, playerHealthRatio, time, getHealthColor(playerHealthRatio));
        drawPulseEffect(window, enemyBarPos, barSize, enemyHealthRatio, time, getHealthColor(enemyHealthRatio));

        drawHealthText(window, font,
                       to_string((int)playerHealth) + "/" + to_string((int)playerMaxHealth),
                       playerBarPos + Vector2f(5, 5), Color::White, 16);

        drawHealthText(window, font,
                       to_string((int)enemyHealth) + "/" + to_string((int)enemyMaxHealth),
                       enemyBarPos + Vector2f(5, 5), Color::White, 16);

        drawHealthText(window, font, "Player", playerBarPos + Vector2f(0, -25), Color::Black, 18);
        drawHealthText(window, font, "Enemy", enemyBarPos + Vector2f(0, -25), Color::Black, 18);
    }
    // it hadnle the fire ball directions when space is pressed wheter it is possible to launch a fire ball or not
    void handlePlayerFireball(const Hitbox &playerHitbox, const Hitbox *enemyHitbox, const Pet *playerPet, int select)
    {
        if (Keyboard::isKeyPressed(Keyboard::Space))
        {
            if (playerFireCooldown.getElapsedTime().asSeconds() >= PLAYER_FIRE_COOLDOWN)
            {
                Vector2f startPos = playerHitbox.getCenter();

                if (select != 3)
                {
                    float direction = (playerPet->sprite.getScale().x > 0) ? -1.f : 1.f;
                    Vector2f targetPos = startPos + Vector2f(direction * 100.f, 0.f);
                    playerFireball.activate(startPos, targetPos);
                    playerFireCooldown.restart();
                    playerFireball.setScalee(0.7f);
                }
                else
                {
                    float direction = (playerPet->sprite.getScale().x > 0) ? 1.f : -1.f;
                    Vector2f targetPos = startPos + Vector2f(direction * 100.f, 0.f);
                    playerFireball.activate(startPos, targetPos);
                    playerFireCooldown.restart();
                    playerFireball.setScalee(0.7f);
                }
            }
        }
        // if q is pressed along side it increase damage and size as it is ability
        if (Keyboard::isKeyPressed(Keyboard::Q) && fireballClock.getElapsedTime().asSeconds() >= FIREBALL_COOLDOWN && (select == 0 || select == 1) && playerFireball.isActive())
        {
            playerFireball.setScalee(1.2f);
            fireballClock.restart();
        }
    }

    // enemy fireball just wiht cool down and a little help target shooting
    void handleEnemyFireball(const Hitbox &playerHitbox, const Hitbox *enemyHitbox)
    {
        if (enemyHitbox && enemyFireCooldown.getElapsedTime().asSeconds() >= ENEMY_FIRE_COOLDOWN && !isEnemyStunned)
        {
            Vector2f startPos = enemyHitbox->getCenter();
            Vector2f targetPos = playerHitbox.getCenter();
            enemyFireball.activate(startPos, targetPos);
            enemyFireCooldown.restart();
        }
    }

    void updateFireballs(float deltaTime)
    {
        playerFireball.update(deltaTime);
        enemyFireball.update(deltaTime);
    }

    // it check the fire ball collisons with the player and enemy and deal the damagee accrdingly
    void checkFireballCollisions(Hitbox &playerHitbox, Hitbox *enemyHitbox, Pet *playerPet, Pet *enemyPet)
    {

        // if it is activatee calcualte teh distance and checks it if hte it have a collison woht the
        // center of the fireball shot it
        if (playerFireball.isActive() && enemyHitbox)
        {
            Vector2f fireballCenter = playerFireball.getPosition();
            Vector2f enemyCenter = enemyHitbox->originCircle.getPosition() +
                                   Vector2f(enemyHitbox->originCircle.getRadius(),
                                            enemyHitbox->originCircle.getRadius());

            Vector2f diff = fireballCenter - enemyCenter;
            float distance = sqrt(diff.x * diff.x + diff.y * diff.y);

            if (distance < playerFireball.getRadius() + enemyHitbox->originCircle.getRadius())
            {
                if (playerPet && enemyPet)
                {
                    float attack = playerPet->getAttack();
                    float defense = enemyPet->getDefence();
                    float damage = attack * (1.f - defense / (defense + 100.f));

                    enemyPet->takeDamage(damage + ((playerFireball.getScalee() != 0.7f) ? 40 : 0));

                    score += 10;
                }
                playerFireball.deactivate();
            }
        }

        if (enemyFireball.isActive())
        {
            Vector2f fireballCenter = enemyFireball.getPosition();
            Vector2f playerCenter = playerHitbox.originCircle.getPosition() +
                                    Vector2f(playerHitbox.originCircle.getRadius(),
                                             playerHitbox.originCircle.getRadius());

            Vector2f diff = fireballCenter - playerCenter;
            float distance = sqrt(diff.x * diff.x + diff.y * diff.y);

            if (distance < enemyFireball.getRadius() + playerHitbox.originCircle.getRadius())
            {
                if (playerPet && enemyPet)
                {
                    float attack = enemyPet->getAttack();
                    float defense = playerPet->getDefence();
                    float damage = attack * (1.f - defense / (defense + 100.f));
                    playerPet->takeDamage(damage);
                    score -= 4;
                }
                enemyFireball.deactivate();
            }
        }
    }

    void drawFireballs(RenderWindow &window)
    {
        playerFireball.draw(window);
        enemyFireball.draw(window);
    }

    // it follow backs the player
    void track_back(Pet *enemyPet, Hitbox *enemyHitbox, const Pet *playerPet, const Hitbox &playerHitbox,
                    float deltaTime, float trackRadius = RADIUS, float fleeRadius = 150.f)
    {
        if (!enemyPet || !enemyHitbox || isEnemyStunned)
            return;

        Vector2f playerPos = playerHitbox.getCenter();
        Vector2f enemyPos = enemyHitbox->getCenter();
        Vector2f direction = playerPos - enemyPos;
        float distance = sqrt(direction.x * direction.x + direction.y * direction.y);

        if (distance > 0)
        {
            direction /= distance;
        }

        if (distance < trackRadius)
        {
            if (distance < fleeRadius)
            {
                Vector2f fleeMovement = (-direction * enemyHitbox->speed) * (deltaTime * 60);
                enemyHitbox->move(fleeMovement);
            }
            else
            {
                Vector2f chaseMovement = (direction * enemyHitbox->speed) * (deltaTime * 60);
                enemyHitbox->move(chaseMovement);
            }

            enemyPet->getSprite().setPosition(enemyHitbox->rectHitbox.getPosition());
        }
    }

    bool isPetHealthy(int petIndex, Player &player)
    {
        switch (petIndex)
        {
        case 0:
            return player.dragon.getHealth() > 0;
        case 1:
            return player.griffin.getHealth() > 0;
        case 2:
            return player.unicorn.getHealth() > 0;
        case 3:
            return player.phoenix.getHealth() > 0;
        default:
            return false;
        }
    }

    // it is just a selection screen
    int battle_mode(RenderWindow &window, const Player &player)
    {
        Object mode("images/mode.png", {0.f, 0.f}, {1.f, 1.f});

        Button button[5] = {
            Button(945, 10, 43, 43),
            Button(24, 534, 45, 43),
            Button(407, 211, 195, 60),
            Button(407, 300, 195, 60),
            Button(407, 391, 195, 60),
        };

        while (window.isOpen())
        {
            Event event;
            while (window.pollEvent(event))
            {
                if (event.type == Event::Closed)
                {
                    exit(EXIT_SUCCESS);
                    window.close();
                }

                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
                {
                    Vector2i mousePos = Mouse::getPosition(window);
                    cout << mousePos.x << "," << mousePos.y << endl;

                    if (button[0].isClicked(mousePos))
                    {
                        cout << "Exit is pressed" << endl;
                        exit(EXIT_SUCCESS);
                        window.close();
                    }
                    else if (button[1].isClicked(mousePos))
                    {
                        cout << "Back is pressed" << endl;
                        return -1;
                    }
                    else if (button[2].isClicked(mousePos))
                    {
                        cout << " 1 VS 1 is pressed " << endl;
                        return 1;
                    }
                    else if (button[3].isClicked(mousePos))
                    {
                        cout << " 2 VS 2 is pressed " << endl;
                        return 2;
                    }
                    else if (button[4].isClicked(mousePos))
                    {
                        cout << " GUILD WAR is pressed " << endl;
                        return 4;
                    }
                }
            }

            window.clear(Color::Black);
            mode.draw(window);
            window.display();
        }

        return -1;
    }

    // pet selction screen wiht navigation
    int pet_selection(RenderWindow &window, Player &player)
    {
        Object mode("images/selection.png", {0.f, 0.f}, {1.f, 1.f});

        float originalScales[4][2];
        originalScales[0][0] = player.dragon.sprite.getScale().x;
        originalScales[0][1] = player.dragon.sprite.getScale().y;
        originalScales[1][0] = player.griffin.sprite.getScale().x;
        originalScales[1][1] = player.griffin.sprite.getScale().y;
        originalScales[2][0] = player.unicorn.sprite.getScale().x;
        originalScales[2][1] = player.unicorn.sprite.getScale().y;
        originalScales[3][0] = player.phoenix.sprite.getScale().x;
        originalScales[3][1] = player.phoenix.sprite.getScale().y;

        Button button[5] = {
            Button(945, 10, 43, 43),
            Button(24, 534, 45, 43),
            Button(432, 425, 140, 40),
            Button(432, 520, 50, 50),
            Button(524, 520, 50, 50)};

        int current = 0;
        int originalCurrent = current;
        do
        {
            current = (current - 1 + 4) % 4;
            if (current == originalCurrent)
                break;
        } while (!isPetHealthy(current, player));

        const Vector2f petPosition = {505.f, 270.f};
        const float petScales[4] = {0.9f, 0.8f, 1.25f, 1.f};
        Clock animationClock;

        while (window.isOpen())
        {
            float deltaTime = animationClock.restart().asSeconds();
            Event event;
            while (window.pollEvent(event))
            {
                if (event.type == Event::Closed)
                {
                    player.dragon.sprite.setScale(originalScales[0][0], originalScales[0][1]);
                    player.griffin.sprite.setScale(originalScales[1][0], originalScales[1][1]);
                    player.unicorn.sprite.setScale(originalScales[2][0], originalScales[2][1]);
                    player.phoenix.sprite.setScale(originalScales[3][0], originalScales[3][1]);
                    exit(EXIT_SUCCESS);
                    window.close();
                }

                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
                {
                    Vector2i mousePos = Mouse::getPosition(window);
                    cout << mousePos.x << "," << mousePos.y << endl;

                    if (button[0].isClicked(mousePos))
                    {
                        player.dragon.sprite.setScale(originalScales[0][0], originalScales[0][1]);
                        player.griffin.sprite.setScale(originalScales[1][0], originalScales[1][1]);
                        player.unicorn.sprite.setScale(originalScales[2][0], originalScales[2][1]);
                        player.phoenix.sprite.setScale(originalScales[3][0], originalScales[3][1]);
                        cout << "Exit is pressed " << endl;
                        exit(EXIT_SUCCESS);
                        window.close();
                    }
                    else if (button[1].isClicked(mousePos))
                    {
                        player.dragon.sprite.setScale(originalScales[0][0], originalScales[0][1]);
                        player.griffin.sprite.setScale(originalScales[1][0], originalScales[1][1]);
                        player.unicorn.sprite.setScale(originalScales[2][0], originalScales[2][1]);
                        player.phoenix.sprite.setScale(originalScales[3][0], originalScales[3][1]);
                        cout << "Back is pressed " << endl;
                        return -1;
                    }
                    else if (button[2].isClicked(mousePos))
                    {
                        for (int i = 0; i < 4; i++)
                        {
                            if (i != current)
                            {
                                switch (i)
                                {
                                case 0:
                                    player.dragon.sprite.setScale(originalScales[0][0], originalScales[0][1]);
                                    break;
                                case 1:
                                    player.griffin.sprite.setScale(originalScales[1][0], originalScales[1][1]);
                                    break;
                                case 2:
                                    player.unicorn.sprite.setScale(originalScales[2][0], originalScales[2][1]);
                                    break;
                                case 3:
                                    player.phoenix.sprite.setScale(originalScales[3][0], originalScales[3][1]);
                                    break;
                                }
                            }
                        }
                        player.dragon.sprite.setScale(originalScales[0][0], originalScales[0][1]);
                        player.griffin.sprite.setScale(originalScales[1][0], originalScales[1][1]);
                        player.unicorn.sprite.setScale(originalScales[2][0], originalScales[2][1]);
                        player.phoenix.sprite.setScale(originalScales[3][0], originalScales[3][1]);
                        cout << "select is pressed " << endl;
                        return current;
                    }
                    else if (button[3].isClicked(mousePos))
                    {
                        cout << "left is pressed " << endl;
                        int originalCurrent = current;
                        do
                        {
                            current = (current - 1 + 4) % 4;
                            if (current == originalCurrent)
                                break;
                        } while (!isPetHealthy(current, player));
                    }
                    else if (button[4].isClicked(mousePos))
                    {
                        cout << "right is pressed " << endl;
                        int originalCurrent = current;
                        do
                        {
                            current = (current + 1) % 4;
                            if (current == originalCurrent)
                                break;
                        } while (!isPetHealthy(current, player));
                    }
                }
            }

            player.dragon.updateAnimation(deltaTime);
            player.griffin.updateAnimation(deltaTime);
            player.unicorn.updateAnimation(deltaTime);
            player.phoenix.updateAnimation(deltaTime);

            window.clear(Color::Black);
            mode.draw(window);

            float currentScale = petScales[current];
            switch (current)
            {
            case 0:
                player.dragon.sprite.setScale(currentScale, currentScale);
                player.dragon.sprite.setPosition(petPosition);
                player.dragon.draw(window);
                break;
            case 1:
                player.griffin.sprite.setScale(currentScale, currentScale);
                player.griffin.sprite.setPosition(petPosition);
                player.griffin.draw(window);
                break;
            case 2:
                player.unicorn.sprite.setScale(currentScale, currentScale);
                player.unicorn.sprite.setPosition(petPosition);
                player.unicorn.draw(window);
                break;
            case 3:
                player.phoenix.sprite.setScale(currentScale, currentScale);
                player.phoenix.sprite.setPosition(petPosition);
                player.phoenix.draw(window);
                break;
            }

            window.display();
        }
        return -1;
    }

    // reset the shole battle to use it again
    void reset(Player &player)
    {
        for (int i = 0; i < 4; i++)
        {
            alive_pets[i] = true;
        }
        gems = coins = score = 0;
        enemy = player;
        GameOver = false;
        isEnemyStunned = false;

        // Pure random text to trigger diff: x9k2p7q

        int attackVar = (rand() % 11) - 5;
        int defenseVar = (rand() % 11) - 5;
        int healthVar = (rand() % 11) - 5;

        player.griffin.setHealth(player.griffin.getMaxHealth());
        player.dragon.setHealth(player.dragon.getMaxHealth());
        player.unicorn.setHealth(player.unicorn.getMaxHealth());
        player.phoenix.setHealth(player.phoenix.getMaxHealth());

        enemy.griffin.addAttack(attackVar);
        enemy.griffin.addDefence(defenseVar);
        enemy.griffin.addMaxHealth(healthVar);
        enemy.griffin.setHealth(enemy.griffin.getMaxHealth());
        enemy.griffin.addSpeed(-0.25f);

        enemy.phoenix.addAttack(attackVar);
        enemy.phoenix.addDefence(defenseVar);
        enemy.phoenix.addMaxHealth(healthVar);
        enemy.phoenix.setHealth(enemy.phoenix.getMaxHealth());
        enemy.phoenix.addSpeed(-2.0f);

        enemy.dragon.addAttack(attackVar);
        enemy.dragon.addDefence(defenseVar);
        enemy.dragon.addMaxHealth(healthVar);
        enemy.dragon.setHealth(enemy.dragon.getMaxHealth());
        enemy.dragon.addSpeed(-0.43f);

        enemy.unicorn.addAttack(attackVar);
        enemy.unicorn.addDefence(defenseVar);
        enemy.unicorn.addMaxHealth(healthVar);
        enemy.unicorn.setHealth(enemy.unicorn.getMaxHealth());
        enemy.unicorn.addSpeed(-1.0f);
    }

    int get_enemyPet()
    {
        static int lastIndex = -1; // Keeps track of the last returned index

        for (int i = 1; i <= 4; i++) // Try all 4 options
        {
            int nextIndex = (lastIndex + i) % 4; // Move to the next index in order
            if (alive_pets[nextIndex])
            {
                lastIndex = nextIndex;
                return nextIndex;
            }
        }

        return -1; // No alive pets
    }

    // wining screen
    void wining(RenderWindow &window, Player &player)
    {
        Object game_screen("images/win.png", {0.f, 0.f}, {1.f, 1.f});

        coins += (score / 2);
        gems += (score / 3);
        if (score > 0)
        {
            player.addCoins(coins);
            player.addGems(gems);
        }
        if (GUILD_PRESENT.isPresent(player.getGuildName()))
            GUILD_PRESENT.addScoreAndUpdateFile(player.getGuildName(), score);

        Button button[2] = {
            Button(945, 10, 43, 43),
            Button(24, 534, 45, 43),
        };

        while (window.isOpen())
        {
            Event event;
            while (window.pollEvent(event))
            {
                if (event.type == Event::Closed)
                {
                    exit(EXIT_SUCCESS);
                    window.close();
                }

                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
                {
                    Vector2i mousePos = Mouse::getPosition(window);
                    cout << mousePos.x << "," << mousePos.y << endl;

                    if (button[0].isClicked(mousePos))
                    {
                        cout << "Exit is pressed" << endl;
                        exit(EXIT_SUCCESS);
                        window.close();
                    }
                    else if (button[1].isClicked(mousePos))
                    {
                        cout << "Back is pressed" << endl;
                        return;
                    }
                }
            }

            window.clear(Color::Black);
            game_screen.draw(window);
            write(window, to_string(score), 550, 302);
            write(window, to_string(gems), 515, 375);
            write(window, to_string(coins), 505, 455);
            window.display();
        }
    }

    // loosing screen
    void game_over(RenderWindow &window, Player &player)
    {
        Object game_screen("images/game_over.png", {0.f, 0.f}, {1.f, 1.f});

        coins += (score / 2);
        gems += (score / 3);
        player.addCoins(coins);
        player.addGems(gems);
        if (GUILD_PRESENT.isPresent(player.getGuildName()))
            GUILD_PRESENT.addScoreAndUpdateFile(player.getGuildName(), score);

        Button button[2] = {
            Button(945, 10, 43, 43),
            Button(24, 534, 45, 43),
        };

        while (window.isOpen())
        {
            Event event;
            while (window.pollEvent(event))
            {
                if (event.type == Event::Closed)
                {
                    exit(EXIT_SUCCESS);
                    window.close();
                }

                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
                {
                    Vector2i mousePos = Mouse::getPosition(window);
                    cout << mousePos.x << "," << mousePos.y << endl;

                    if (button[0].isClicked(mousePos))
                    {
                        cout << "Exit is pressed" << endl;
                        exit(EXIT_SUCCESS);
                        window.close();
                    }
                    else if (button[1].isClicked(mousePos))
                    {
                        cout << "Back is pressed" << endl;
                        return;
                    }
                }
            }

            window.clear(Color::Black);
            game_screen.draw(window);
            write(window, to_string(score), 550, 302);
            write(window, to_string(gems), 515, 375);
            write(window, to_string(coins), 505, 455);
            window.display();
        }
    }

    // if game is paused in the middle

    bool paused(RenderWindow &window, Player &player)
    {
        Object game_screen("images/pause.png", {0.f, 0.f}, {1.f, 1.f});

        Button button[3] = {
            Button(945, 10, 43, 43),
            Button(425, 160, 150, 73),
            Button(407.5f, 340.f, 190.f, 65.f)};

        while (window.isOpen())
        {
            Event event;
            while (window.pollEvent(event))
            {
                if (event.type == Event::Closed)
                {
                    exit(EXIT_SUCCESS);
                    window.close();
                }

                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
                {
                    Vector2i mousePos = Mouse::getPosition(window);
                    cout << mousePos.x << "," << mousePos.y << endl;

                    if (button[0].isClicked(mousePos))
                    {
                        cout << "Exit is pressed" << endl;
                        exit(EXIT_SUCCESS);
                        window.close();
                    }
                    else if (button[1].isClicked(mousePos))
                    {
                        cout << " Play Game is pressed" << endl;
                        return true;
                    }
                    else if (button[2].isClicked(mousePos))
                    {
                        cout << " Giveup is pressed" << endl;
                        return false;
                    }
                }
            }

            window.clear(Color::Black);
            game_screen.draw(window);
            window.display();
        }

        return false;
    }

    // sprite setup accroding to selected index
    void setupPetSprite(Pet *&pet, int petIdx, bool isPlayer, const Vector2f &playerPos, const Vector2f &enemyPos)
    {
        if (!pet)
            return;

        pet->getSprite().setScale(STANDARD_SCALES[petIdx]);

        if (isPlayer)
        {
            if (petIdx != 3)
            {
                pet->getSprite().setScale(-STANDARD_SCALES[petIdx].x, STANDARD_SCALES[petIdx].y);
            }
            pet->getSprite().setPosition(playerPos);
        }
        else
        {
            if (petIdx == 3)
            {
                pet->getSprite().setScale(-STANDARD_SCALES[petIdx].x, STANDARD_SCALES[petIdx].y);
            }
            pet->getSprite().setPosition(enemyPos);
        }
    }

    Vector2f getPlayerMovementInput(float speed, float deltaTime)
    {
        Vector2f movement(0.f, 0.f);

        if (Keyboard::isKeyPressed(Keyboard::W))
            movement.y -= speed * deltaTime * 50;
        if (Keyboard::isKeyPressed(Keyboard::S))
            movement.y += speed * deltaTime * 50;
        if (Keyboard::isKeyPressed(Keyboard::A))
            movement.x -= speed * deltaTime * 50;
        if (Keyboard::isKeyPressed(Keyboard::D))
            movement.x += speed * deltaTime * 50;

        return movement;
    }

    void updatePetSpriteDirection(Vector2f proposedMovement, int petIdx, float playerOriginalScale, Pet *playerPet)
    {
        if (proposedMovement.x == 0)
            return;

        float scaleX = playerOriginalScale;
        float scaleY = STANDARD_SCALES[petIdx].y;

        if (petIdx == 3)
        {
            if (proposedMovement.x < 0)
                scaleX = -playerOriginalScale;
        }
        else
        {
            if (proposedMovement.x >= 0)
                scaleX = -playerOriginalScale;
        }

        playerPet->getSprite().setScale(scaleX, scaleY);
    }

    void updateEnemyChaseAndDirection(
        Hitbox &playerHitbox,
        Hitbox *enemyHitbox,
        Pet *enemyPet,
        int enemyIdx,
        float enemyOriginalScale,
        float deltaTime,
        float chaseRadius)
    {
        if (!enemyPet || !enemyHitbox || isEnemyStunned)
            return;

        Vector2f playerPos = playerHitbox.getCenter();
        Vector2f enemyPos = enemyHitbox->getCenter();
        Vector2f toPlayer = playerPos - enemyPos;
        float distance = sqrt(toPlayer.x * toPlayer.x + toPlayer.y * toPlayer.y);

        if (distance > chaseRadius || abs(playerPos.y - enemyPos.y) > 5.0f)
        {
            Vector2f direction = toPlayer / distance;

            if (distance <= chaseRadius + 10.0f && abs(playerPos.y - enemyPos.y) > 5.0f)
            {
                direction.x = 0;
            }

            Vector2f movement = direction * enemyHitbox->speed * deltaTime * 60.f;

            if (distance >= chaseRadius - 10.0f && distance <= chaseRadius + 10.0f)
            {
                movement.x = 0;
            }

            enemyHitbox->move(movement);
        }

        bool playerIsLeft = playerPos.x < enemyPos.x;
        float scaleX = enemyOriginalScale;
        float scaleY = STANDARD_SCALES[enemyIdx].y;

        if (enemyIdx == 3)
            scaleX = playerIsLeft ? -enemyOriginalScale : enemyOriginalScale;
        else
            scaleX = playerIsLeft ? enemyOriginalScale : -enemyOriginalScale;

        enemyPet->getSprite().setScale(scaleX, scaleY);
    }

    // a boundry check so it should not go out of bound
    void restrictPlayerMovement(Hitbox &playerHitbox, const RenderWindow &window)
    {
        FloatRect playerBounds = playerHitbox.rectHitbox.getGlobalBounds();

        if (playerBounds.left < 0)
            playerHitbox.move(Vector2f(-playerBounds.left, 0));
        if (playerBounds.top < 0)
            playerHitbox.move(Vector2f(0, -playerBounds.top));
        if (playerBounds.left + playerBounds.width > window.getSize().x)
            playerHitbox.move(Vector2f(window.getSize().x - (playerBounds.left + playerBounds.width), 0));
        if (playerBounds.top + playerBounds.height > window.getSize().y)
            playerHitbox.move(Vector2f(0, window.getSize().y - (playerBounds.top + playerBounds.height)));
    }

    // all things shown in order on screen
    void renderBattleScreen(RenderWindow &window, Player &player, Object &background,
                            Pet *playerPet, Pet *enemyPet,
                            Hitbox &playerHitbox, Hitbox *enemyHitbox,
                            Button *button, bool showHitboxes,
                            int battle, float battle_elapsed_time)
    {
        background.draw(window);

        window.draw(playerPet->getSprite());
        if (enemyPet)
            window.draw(enemyPet->getSprite());

        if (showHitboxes)
        {
            window.draw(playerHitbox.rectHitbox);
            window.draw(playerHitbox.originCircle);
            if (enemyHitbox)
            {
                window.draw(enemyHitbox->rectHitbox);
                window.draw(enemyHitbox->originCircle);
            }
        }

        for (int i = 0; i < 3; ++i)
            button[i].draw(window);

        if (battle == 1)
            write(window, "      1 VS 1 ", 160, 25);
        else if (battle == 2)
            write(window, "      2 VS 2 ", 160, 25);
        else
            write(window, "GUILD WAR", 160, 25);

        int minutes = static_cast<int>(battle_elapsed_time) / 60;
        int seconds = static_cast<int>(battle_elapsed_time) % 60;
        write(window, "TIME : " + to_string(minutes) + ":" + (seconds < 10 ? "0" : "") + to_string(seconds), 700, 23);

        drawCooldowns(window, player);

        // Draw radar if active
        if (isRadarActive)
            window.draw(radarCircle);
    }

    // player abilities countdown
    void updateCooldowns(float deltaTime)
    {
        for (int i = 0; i < 6; i++)
        {
            if (item_on_cool_player[i])
            {
                float elapsed = potion_cooldown_player[i].getElapsedTime().asSeconds();
                if (elapsed >= cooldown_duration)
                {
                    item_on_cool_player[i] = false;
                    cooldown_showcase_player[i].setFillColor(Color::Transparent);
                }
            }
        }

        for (int i = 0; i < 6; i++)
        {
            if (item_on_cool_enemy[i])
            {
                float elapsed = potion_cooldown_enemy[i].getElapsedTime().asSeconds();
                if (elapsed >= cooldown_duration)
                {
                    item_on_cool_enemy[i] = false;
                    cooldown_showcase_enemy[i].setFillColor(Color::Transparent);
                }
            }
        }
    }

    // count down setter
    void initCooldownIndicators()
    {
        for (int i = 0; i < 6; i++)
        {
            float yPos = COOLDOWN_Y_START + i * COOLDOWN_Y_OFFSET;

            cooldown_background[i].setSize(Vector2f(COOLDOWN_WIDTH, COOLDOWN_HEIGHT));
            cooldown_background[i].setFillColor(Color(100, 100, 100, 150));
            cooldown_background[i].setPosition(COOLDOWN_X, yPos);

            cooldown_showcase_player[i].setSize(Vector2f(COOLDOWN_WIDTH, COOLDOWN_HEIGHT));
            cooldown_showcase_player[i].setFillColor(Color::Transparent);
            cooldown_showcase_player[i].setPosition(COOLDOWN_X, yPos);
        }

        const float ENEMY_COOLDOWN_X = 945;
        for (int i = 0; i < 6; i++)
        {
            float yPos = COOLDOWN_Y_START + i * (COOLDOWN_Y_OFFSET - 3);

            cooldown_background_enemy[i].setSize(Vector2f(COOLDOWN_WIDTH, COOLDOWN_HEIGHT));
            cooldown_background_enemy[i].setFillColor(Color(100, 100, 100, 150));
            cooldown_background_enemy[i].setPosition(ENEMY_COOLDOWN_X, yPos);

            cooldown_showcase_enemy[i].setSize(Vector2f(COOLDOWN_WIDTH, COOLDOWN_HEIGHT));
            cooldown_showcase_enemy[i].setFillColor(Color::Transparent);
            cooldown_showcase_enemy[i].setPosition(ENEMY_COOLDOWN_X, yPos);
        }
    }

    void drawCooldowns(RenderWindow &window, Player &player)
    {
        for (int i = 0; i < 6; i++)
        {
            bool hasItem = false;
            switch (i + 1)
            {
            case 1:
                hasItem = player.getItem1() > 0;
                break;
            case 2:
                hasItem = player.getItem2() > 0;
                break;
            case 3:
                hasItem = player.getItem3() > 0;
                break;
            case 4:
                hasItem = player.getItem4() > 0;
                break;
            case 5:
                hasItem = player.getItem5() > 0;
                break;
            case 6:
                hasItem = player.getItem6() > 0;
                break;
            }

            if (!hasItem)
            {
                cooldown_background[i].setFillColor(Color(50, 50, 50, 150));
                window.draw(cooldown_background[i]);
            }

            if (hasItem && item_on_cool_player[i])
            {
                float ratio = potion_cooldown_player[i].getElapsedTime().asSeconds() / cooldown_duration;
                ratio = std::min(ratio, 1.0f);

                float height = COOLDOWN_HEIGHT * ratio;

                if (ratio > 0.66f)
                    cooldown_showcase_player[i].setFillColor(Color(0, 255, 0, 100));
                else if (ratio > 0.33f)
                    cooldown_showcase_player[i].setFillColor(Color(255, 165, 0, 100));
                else
                    cooldown_showcase_player[i].setFillColor(Color(255, 0, 0, 100));

                cooldown_showcase_player[i].setSize(Vector2f(COOLDOWN_WIDTH, height));
                cooldown_showcase_player[i].setPosition(
                    COOLDOWN_X,
                    COOLDOWN_Y_START + i * (COOLDOWN_Y_OFFSET) + (COOLDOWN_HEIGHT - height));

                window.draw(cooldown_showcase_player[i]);
            }
        }

        for (int i = 0; i < 6; i++)
        {
            bool hasItem = false;
            switch (i + 1)
            {
            case 1:
                hasItem = enemy.getItem1() > 0;
                break;
            case 2:
                hasItem = enemy.getItem2() > 0;
                break;
            case 3:
                hasItem = enemy.getItem3() > 0;
                break;
            case 4:
                hasItem = enemy.getItem4() > 0;
                break;
            case 5:
                hasItem = enemy.getItem5() > 0;
                break;
            case 6:
                hasItem = enemy.getItem6() > 0;
                break;
            }

            if (!hasItem)
            {
                cooldown_background_enemy[i].setFillColor(Color(50, 50, 50, 150));
                window.draw(cooldown_background_enemy[i]);
            }

            if (hasItem && item_on_cool_enemy[i])
            {
                float ratio = potion_cooldown_enemy[i].getElapsedTime().asSeconds() / cooldown_duration;
                ratio = std::min(ratio, 1.0f);

                float height = COOLDOWN_HEIGHT * ratio;

                if (ratio > 0.66f)
                    cooldown_showcase_enemy[i].setFillColor(Color(0, 255, 0, 100));
                else if (ratio > 0.33f)
                    cooldown_showcase_enemy[i].setFillColor(Color(255, 165, 0, 100));
                else
                    cooldown_showcase_enemy[i].setFillColor(Color(255, 0, 0, 100));

                cooldown_showcase_enemy[i].setSize(Vector2f(COOLDOWN_WIDTH, height));
                cooldown_showcase_enemy[i].setPosition(
                    945,
                    COOLDOWN_Y_START + i * (COOLDOWN_Y_OFFSET) + (COOLDOWN_HEIGHT - height));

                window.draw(cooldown_showcase_enemy[i]);
            }
        }
    }

    bool usePlayerItem(Player &player, int itemIndex, Pet *pet)
    {
        if (itemIndex < 1 || itemIndex > 6)
            return false;
        int idx = itemIndex - 1;

        bool hasItem = false;
        switch (itemIndex)
        {
        case 1:
            hasItem = player.getItem1() > 0;
            break;
        case 2:
            hasItem = player.getItem2() > 0;
            break;
        case 3:
            hasItem = player.getItem3() > 0;
            break;
        case 4:
            hasItem = player.getItem4() > 0;
            break;
        case 5:
            hasItem = player.getItem5() > 0;
            break;
        case 6:
            hasItem = player.getItem6() > 0;
            break;
        }

        if (!hasItem)
            return false;

        if (!item_on_cool_player[idx])
        {
            item_on_cool_player[idx] = true;
            potion_cooldown_player[idx].restart();

            switch (itemIndex)
            {
            case 1:
                player.addItem1(-1);
                pet->addDefence(2);
                break;
            case 2:
                player.addItem2(-1);
                pet->addSpeed(0.02);
                break;
            case 3:
                player.addItem3(-1);
                pet->addDefence(2);
                break;
            case 4:
                player.addItem4(-1);
                pet->addAttack(5);
                break;
            case 5:
                player.addItem5(-1);
                pet->addHealth(10);
                break;
            case 6:
                player.addItem6(-1);
                pet->addHealth(5);
                pet->addDefence(1);
                break;
            }

            return true;
        }
        return false;
    }

    void handleItemInput(Player &player, Pet *p)
    {
        if (Keyboard::isKeyPressed(Keyboard::Num1))
            usePlayerItem(player, 1, p);
        if (Keyboard::isKeyPressed(Keyboard::Num2))
            usePlayerItem(player, 2, p);
        if (Keyboard::isKeyPressed(Keyboard::Num3))
            usePlayerItem(player, 3, p);
        if (Keyboard::isKeyPressed(Keyboard::Num4))
            usePlayerItem(player, 4, p);
        if (Keyboard::isKeyPressed(Keyboard::Num5))
            usePlayerItem(player, 5, p);
        if (Keyboard::isKeyPressed(Keyboard::Num6))
            usePlayerItem(player, 6, p);
    }

    void handleEnemyItemUsage(Pet *pet)
    {
        if (!pet)
            return;

        if (rand() % 300 < 3)
        {
            int itemToUse = rand() % 6;

            if (!item_on_cool_enemy[itemToUse])
            {
                item_on_cool_enemy[itemToUse] = true;
                potion_cooldown_enemy[itemToUse].restart();

                switch (itemToUse)
                {
                case 1:
                    pet->addDefence(2);
                    break;
                case 2:
                    pet->addSpeed(0.02);
                    break;
                case 3:
                    pet->addDefence(2);
                    break;
                case 4:
                    pet->addAttack(5);
                    break;
                case 5:
                    pet->addHealth(10);
                    break;
                case 6:
                    pet->addHealth(5);
                    pet->addDefence(1);
                    break;
                }
            }
        }
    }
    void handleAbilities(Pet *playerPet, bool &speedActive, bool &defenceActive,
                         bool &speedUsed, bool &defenceUsed,
                         Clock &speedClock, Clock &defenceClock,
                         const float abilityDuration, const float abilityCooldown,
                         float battle_elapsed_time, bool &GameOver)
    {
        // Reset speed after duration
        if (speedActive && speedClock.getElapsedTime().asSeconds() >= abilityDuration)
        {
            playerPet->setSpeed(playerPet->getSpeed() - 0.55f);
            speedActive = false;
            speedClock.restart(); // start cooldown
        }

        // Reset defence after duration
        if (defenceActive && defenceClock.getElapsedTime().asSeconds() >= abilityDuration)
        {
            playerPet->setDefence(playerPet->getDefence() - 20);
            defenceActive = false;
            defenceClock.restart(); // start cooldown
        }

        // Cooldown over for speed
        if (!speedActive && speedUsed && speedClock.getElapsedTime().asSeconds() >= abilityCooldown)
        {
            speedUsed = false;
        }

        // Cooldown over for defence
        if (!defenceActive && defenceUsed && defenceClock.getElapsedTime().asSeconds() >= abilityCooldown)
        {
            defenceUsed = false;
        }
    }

public:
    void start_battle(RenderWindow &window, Player &player)
    {
        reset(player);

        int totalEnemiesToDefeat = battle_mode(window, player);
        if (totalEnemiesToDefeat <= 0)
            return;

        int swapsCan = totalEnemiesToDefeat - 1;
        int petIdx = pet_selection(window, player);
        if (petIdx == -1)
            return;

        int enemyIdx = get_enemyPet();

        Object background("images/battle_" + to_string(rand() % 4 + 1) + ".png", {0.f, 0.f}, {1.f, 1.f});

        Button button[3] = {
            Button(945, 10, 43, 43),
            Button(24, 534, 45, 43),
            Button(6, 13, 47, 47),
        };

        Pet *playerPet = player.getPet(petIdx);
        setupPetSprite(playerPet, petIdx, true, {150, 300}, {750, 300});

        Pet *enemyPet = (enemyIdx != -1) ? enemy.getPet(enemyIdx) : nullptr;
        if (enemyPet)
            setupPetSprite(enemyPet, enemyIdx, false, {150, 300}, {750, 300});

        Hitbox playerHitbox(&playerPet->getSprite(), HITBOX_SCALES[petIdx], playerPet->getSpeed());
        Hitbox *enemyHitbox = (enemyPet) ? new Hitbox(&enemyPet->getSprite(), HITBOX_SCALES[enemyIdx], enemyPet->getSpeed()) : nullptr;

        bool showHitboxes = false;

        Clock clock, collisionClock, battle_clock;
        float timeSinceLastCollision = 0.f;
        const float collisionCooldown = 0.5f;
        float playerOriginalScale = abs(playerPet->getSprite().getScale().x);
        float enemyOriginalScale = (enemyPet) ? abs(enemyPet->getSprite().getScale().x) : 0.f;

        enemyFireball.color(enemyIdx);
        playerFireball.color(petIdx);

        initCooldownIndicators();
        initRadar(petIdx);

        Clock bar;

        int enemyKilledCount = 0;
        int playerDeadCount = 0;
        const int maxPlayerPets = 4;

        Clock speedClock, defenceClock;
        bool speedActive = false, defenceActive = false;
        bool speedUsed = false, defenceUsed = false;
        const float abilityDuration = 5.0f;
        const float abilityCooldown = 20.0f;

        Clock reviveClock, healClock;
        bool hasRevived = false;
        bool healUsed = false;
        const float healCooldown = 30.0f;

        while (window.isOpen())
        {
            float deltaTime = clock.restart().asSeconds();
            float battle_elapsed_time = battle_clock.getElapsedTime().asSeconds();
            timeSinceLastCollision += deltaTime;

            handleAbilities(playerPet, speedActive, defenceActive, speedUsed, defenceUsed,
                            speedClock, defenceClock, abilityDuration, abilityCooldown,
                            battle_elapsed_time, GameOver);

            if (battle_elapsed_time >= 150)
            {
                GameOver = true;
            }

            if (GameOver)
            {
                cout << "Game over Time" << endl;
                game_over(window, player);
                if (enemyHitbox)
                    delete enemyHitbox;
                return;
            }

            handlePlayerFireball(playerHitbox, enemyHitbox, playerPet, petIdx);
            handleEnemyFireball(playerHitbox, enemyHitbox);
            updateFireballs(deltaTime);
            checkFireballCollisions(playerHitbox, enemyHitbox, playerPet, enemyPet);

            updateCooldowns(deltaTime);
            handleItemInput(player, playerPet);
            handleEnemyItemUsage(enemyPet);

            handleRadarInput(playerHitbox, enemyHitbox, enemyPet, petIdx);
            updateRadarPosition(playerHitbox);

            spawnRock();
            updateRocks(deltaTime, window);
            checkRockCollisions(playerHitbox, enemyHitbox, playerPet, enemyPet);

            // Heal cooldown reset for Pet 2
            if (petIdx == 2 && healUsed && healClock.getElapsedTime().asSeconds() >= healCooldown)
            {
                healUsed = false;
            }

            Event event;
            while (window.pollEvent(event))
            {
                if (event.type == Event::Closed)
                {
                    window.close();
                    if (enemyHitbox)
                        delete enemyHitbox;
                    exit(EXIT_SUCCESS);
                }

                if (event.type == Event::KeyPressed && event.key.code == Keyboard::H)
                    showHitboxes = !showHitboxes;

                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
                {
                    Vector2i mousePos = Mouse::getPosition(window);
                    if (button[0].isClicked(static_cast<Vector2i>(mousePos)))
                    {
                        savePlayerData(player, player.getName());
                        window.close();
                        exit(EXIT_SUCCESS);
                    }
                    else if (button[1].isClicked(static_cast<Vector2i>(mousePos)))
                    {
                        savePlayerData(player, player.getName());
                        game_over(window, player);
                        GameOver = true;
                        return;
                    }
                    else if (button[2].isClicked(static_cast<Vector2i>(mousePos)))
                    {
                        if (!paused(window, player))
                        {
                            savePlayerData(player, player.getName());
                            game_over(window, player);
                            GameOver = true;
                            return;
                        }
                    }
                }
            }

            // Speed ability with Q
            if (Keyboard::isKeyPressed(Keyboard::Q) && petIdx == 3 && !speedUsed)
            {
                cout << "speed started" << endl;
                playerPet->setSpeed(playerPet->getSpeed() + 0.55f);
                speedClock.restart();
                speedActive = true;
                speedUsed = true;
            }

            // Defence ability with E
            if (Keyboard::isKeyPressed(Keyboard::E) && petIdx == 3 && !defenceUsed)
            {
                cout << "defence started" << endl;
                playerPet->setDefence(playerPet->getDefence() + 20);
                defenceClock.restart();
                defenceActive = true;
                defenceUsed = true;
            }

            // Healing ability for pet index 2 with Q key
            if (Keyboard::isKeyPressed(Keyboard::Q) && petIdx == 2 && !healUsed)
            {

                cout << "Healing Pet 2" << endl;
                playerPet->setHealth(playerPet->getHealth() + 50); // Cap at 50
                healUsed = true;
                healClock.restart();
            }

            Vector2f proposedMovement = getPlayerMovementInput(playerHitbox.speed, deltaTime);
            updatePetSpriteDirection(proposedMovement, petIdx, playerOriginalScale, playerPet);
            updateEnemyChaseAndDirection(playerHitbox, enemyHitbox, enemyPet, enemyIdx, enemyOriginalScale, deltaTime, RADIUS);

            playerHitbox.move(proposedMovement);
            restrictPlayerMovement(playerHitbox, window);

            playerPet->updateAnimation(deltaTime);
            if (enemyPet)
                enemyPet->updateAnimation(deltaTime);

            if (Keyboard::isKeyPressed(Keyboard::P) && swapsCan)
            {
                swapsCan--;
                petIdx = pet_selection(window, player);
                if (petIdx == -1)
                {
                    if (enemyHitbox)
                        delete enemyHitbox;

                    return;
                }

                playerPet = player.getPet(petIdx);
                setupPetSprite(playerPet, petIdx, true, {150, 300}, {750, 300});
                enemyPet->getSprite().setPosition({750, 300}); // Reset player position

                playerHitbox = Hitbox(&playerPet->getSprite(), HITBOX_SCALES[petIdx], playerPet->getSpeed());
                playerOriginalScale = abs(playerPet->getSprite().getScale().x);
                playerFireball.color(petIdx);
                initRadar(petIdx);
            }

            // Enemy death and replacement
            if (enemyPet && enemyPet->getHealth() <= 0)
            {
                enemyKilledCount++;
                cout << "Enemy Killed: " << enemyKilledCount << endl;
                if (enemyKilledCount >= totalEnemiesToDefeat)
                {

                    wining(window, player);
                    if (enemyHitbox)
                        delete enemyHitbox;
                    return; // Player wins
                }

                enemyIdx = get_enemyPet();
                if (enemyIdx != -1)
                {
                    if (enemyHitbox)
                    {
                        delete enemyHitbox;
                        enemyHitbox = nullptr;
                    }

                    enemyPet = enemy.getPet(enemyIdx);
                    setupPetSprite(enemyPet, enemyIdx, false, {150, 300}, {750, 300});
                    playerPet->getSprite().setPosition({150, 300}); // Reset enemy position

                    enemyHitbox = new Hitbox(&enemyPet->getSprite(), HITBOX_SCALES[enemyIdx], enemyPet->getSpeed());
                    enemyOriginalScale = abs(enemyPet->getSprite().getScale().x);
                    enemyFireball.color(enemyIdx);
                }
            }

            // Pet death and swap
            if (playerPet->getHealth() <= 0)
            {
                // Revival logic for petIdx 2
                if (petIdx == 2 && !hasRevived)
                {
                    playerPet->setHealth(playerPet->getMaxHealth()); // revive with 50 HP or your desired amount
                    hasRevived = true;
                    reviveClock.restart();
                    cout << "Pet 2 revived once!" << endl;
                }
                else
                {
                    playerDeadCount++;
                    if (playerDeadCount >= totalEnemiesToDefeat)
                    {
                        GameOver = true;
                        game_over(window, player);
                        if (enemyHitbox)
                            delete enemyHitbox;
                        return;
                    }

                    petIdx = pet_selection(window, player);
                    if (petIdx == -1)
                    {
                        if (enemyHitbox)
                            delete enemyHitbox;
                        return;
                    }

                    playerPet = player.getPet(petIdx);
                    setupPetSprite(playerPet, petIdx, true, {150, 300}, {750, 300});
                    enemyPet->getSprite().setPosition({750, 300});

                    playerHitbox = Hitbox(&playerPet->getSprite(), HITBOX_SCALES[petIdx], playerPet->getSpeed());
                    playerOriginalScale = abs(playerPet->getSprite().getScale().x);
                    playerFireball.color(petIdx);
                    initRadar(petIdx);
                }
            }
            // Rendering
            window.clear(Color::Black);
            renderBattleScreen(window, player, background,
                               playerPet, enemyPet,
                               playerHitbox, enemyHitbox,
                               button, showHitboxes,
                               totalEnemiesToDefeat, battle_elapsed_time);

            drawFireballs(window);
            if (playerPet && enemyPet)
            {
                float elapsedTime = bar.getElapsedTime().asSeconds();
                drawModernHealthBars(window, playerPet, enemyPet, elapsedTime);
            }
            drawRocks(window);
            window.display();
        }

        if (enemyHitbox)
            delete enemyHitbox;
    }
};