#pragma once

extern Font font;

class TRAINING
{
private:
    const int FRUIT_TYPES = 4;
    const int MAX_FRUITS = 100;
    const int GRID_COLS = 10;
    const int GRID_ROWS = 10;
    const float SPAWN_MIN_TIME = 0.1f;
    const float SPAWN_MAX_TIME = 1.0f;
    const float GAME_TIME_LIMIT = 30.0f;
    const float HEALTH_PENALTY = 0.5f;
    float monsterSpawnChance = 0.15f;
    const int MONSTER_CLICKS_TO_LOSE = 1;
    const char *MONSTER_PATH = "images/dead.png";

    const Vector2f BOUNDARY_TOP_LEFT = {361.f, 80.f};
    const Vector2f BOUNDARY_TOP_RIGHT = {985.f, 90.f};
    const Vector2f BOUNDARY_BOTTOM_LEFT = {361.f, 581.f};
    const Vector2f BOUNDARY_BOTTOM_RIGHT = {990.f, 581.f};

    const float CELL_WIDTH = (BOUNDARY_TOP_RIGHT.x - BOUNDARY_TOP_LEFT.x) / GRID_COLS;
    const float CELL_HEIGHT = (BOUNDARY_BOTTOM_LEFT.y - BOUNDARY_TOP_LEFT.y) / GRID_ROWS;

    // Array of fruit paths with corresponding types
    const char *FRUIT_PATHS[4] = {
        "images/cucumber.png",  // type 0
        "images/orange.png",    // type 1
        "images/lemon.png",     // type 2
        "images/watermalen.png" // type 3
    };

    struct FruitData
    {
        Object *obj;
        int type; // This now directly corresponds to FRUIT_PATHS index
        bool isMonster;
        int clickCount;
        int gridRow;
        int gridCol;
    };

    FruitData *activeFruits;
    int activeFruitCount;
    int fruitCounters[4]; // [0]cucumber, [1]orange, [2]lemon, [3]watermelon
    float fakeHealth;
    Clock gameClock;
    Clock spawnClock;
    float nextSpawnTime;
    bool gameOver;
    bool gridOccupied[10][10];
    bool gridBlocked[10][10];
    int monsterCount = 0;
    bool timeOver;
    Vector2f gridToPosition(int row, int col)
    {
        return {
            BOUNDARY_TOP_LEFT.x + col * CELL_WIDTH + CELL_WIDTH / 2,
            BOUNDARY_TOP_LEFT.y + row * CELL_HEIGHT + CELL_HEIGHT / 2};
    }

    void resetGame()
    {

        for (int i = 0; i < activeFruitCount; i++)
        {
            if (activeFruits[i].obj)
                delete activeFruits[i].obj;
        }
        activeFruitCount = 0;

        for (int i = 0; i < FRUIT_TYPES; i++)
            fruitCounters[i] = 0;

        for (int row = 0; row < GRID_ROWS; row++)
            for (int col = 0; col < GRID_COLS; col++)
                gridOccupied[row][col] = gridBlocked[row][col] = false;

        fakeHealth = 100.f;
        gameOver = false;
        monsterCount = 0;
        monsterSpawnChance = 0.05f;
        nextSpawnTime = SPAWN_MIN_TIME;
        timeOver = false;
        gameClock.restart();
        spawnClock.restart();
    }

    void spawnObjects()
    {
        if (spawnClock.getElapsedTime().asSeconds() >= nextSpawnTime && activeFruitCount < MAX_FRUITS)
        {
            spawnClock.restart();
            nextSpawnTime = SPAWN_MIN_TIME + static_cast<float>(rand()) /
                                                 (static_cast<float>(RAND_MAX / (SPAWN_MAX_TIME - SPAWN_MIN_TIME)));

            int spawnCount = 1 + rand() % 4;
            int spawned = 0;

            while (spawned < spawnCount && activeFruitCount < MAX_FRUITS)
            {
                int row = rand() % GRID_ROWS;
                int col = rand() % GRID_COLS;

                if (!gridOccupied[row][col] && !gridBlocked[row][col])
                {
                    bool isMonster = (static_cast<float>(rand()) / RAND_MAX < monsterSpawnChance);

                    Vector2f pos = gridToPosition(row, col);

                    // Determine fruit type first
                    int fruitType = rand() % FRUIT_TYPES;

                    activeFruits[activeFruitCount].obj = new Object(
                        isMonster ? MONSTER_PATH : FRUIT_PATHS[fruitType],
                        pos,
                        {1.f, 1.f});

                    FloatRect bounds = activeFruits[activeFruitCount].obj->sprite.getLocalBounds();
                    float scale = std::min(CELL_WIDTH * 0.9f / bounds.width, CELL_HEIGHT * 0.9f / bounds.height);
                    activeFruits[activeFruitCount].obj->sprite.setScale(scale, scale);
                    activeFruits[activeFruitCount].obj->sprite.setOrigin(bounds.width / 2, bounds.height / 2);

                    // Set the type based on the image path used
                    activeFruits[activeFruitCount].type = fruitType;
                    activeFruits[activeFruitCount].isMonster = isMonster;
                    activeFruits[activeFruitCount].gridRow = row;
                    activeFruits[activeFruitCount].gridCol = col;
                    activeFruits[activeFruitCount].clickCount = 0;

                    if (isMonster)
                    {
                        monsterCount++;
                        monsterSpawnChance += 0.01f;
                    }

                    gridOccupied[row][col] = true;
                    activeFruitCount++;
                    spawned++;
                }
            }
        }
    }

    void handleInput(RenderWindow &window, Event &event)
    {

        if (event.type == Event::Closed)
        {
            exit(EXIT_SUCCESS);
        }

        if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
        {
            Vector2i mousePos = Mouse::getPosition(window);

            for (int i = 0; i < activeFruitCount; i++)
            {
                if (activeFruits[i].obj &&
                    activeFruits[i].obj->sprite.getGlobalBounds().contains(static_cast<Vector2f>(mousePos)))
                {
                    if (activeFruits[i].isMonster)
                    {
                        activeFruits[i].clickCount++;
                        if (activeFruits[i].clickCount >= MONSTER_CLICKS_TO_LOSE)
                        {
                            gridBlocked[activeFruits[i].gridRow][activeFruits[i].gridCol] = true;
                            gridOccupied[activeFruits[i].gridRow][activeFruits[i].gridCol] = false;
                            delete activeFruits[i].obj;
                            monsterCount--;

                            for (int j = i; j < activeFruitCount - 1; j++)
                                activeFruits[j] = activeFruits[j + 1];
                            activeFruitCount--;

                            gameOver = true;
                            return;
                        }
                        else
                        {
                            Color c = activeFruits[i].obj->sprite.getColor();
                            c.a = 255 - (activeFruits[i].clickCount * 85);
                            activeFruits[i].obj->sprite.setColor(c);
                        }
                    }
                    else
                    {
                        // Increment the counter for this specific fruit type
                        fruitCounters[activeFruits[i].type]++;
                        gridOccupied[activeFruits[i].gridRow][activeFruits[i].gridCol] = false;
                        delete activeFruits[i].obj;

                        for (int j = i; j < activeFruitCount - 1; j++)
                            activeFruits[j] = activeFruits[j + 1];
                        activeFruitCount--;
                        break;
                    }
                }
            }
        }
    }

    void drawGrid(RenderWindow &window)
    {
        // Draw grid lines
        for (int col = 0; col <= GRID_COLS; col++)
        {
            float x = BOUNDARY_TOP_LEFT.x + col * CELL_WIDTH;
            Vertex line[] = {
                Vertex({x, BOUNDARY_TOP_LEFT.y}, Color(255, 255, 255, 50)),
                Vertex({x, BOUNDARY_BOTTOM_LEFT.y}, Color(255, 255, 255, 50))};
            window.draw(line, 2, Lines);
        }

        for (int row = 0; row <= GRID_ROWS; row++)
        {
            float y = BOUNDARY_TOP_LEFT.y + row * CELL_HEIGHT;
            Vertex line[] = {
                Vertex({BOUNDARY_TOP_LEFT.x, y}, Color(255, 255, 255, 50)),
                Vertex({BOUNDARY_TOP_RIGHT.x, y}, Color(255, 255, 255, 50))};
            window.draw(line, 2, Lines);
        }

        // Draw blocked cells
        for (int row = 0; row < GRID_ROWS; row++)
        {
            for (int col = 0; col < GRID_COLS; col++)
            {
                if (gridBlocked[row][col])
                {
                    RectangleShape blockedCell(Vector2f(CELL_WIDTH * 0.9f, CELL_HEIGHT * 0.9f));
                    blockedCell.setFillColor(Color(100, 100, 100, 200));
                    blockedCell.setOrigin(CELL_WIDTH * 0.45f, CELL_HEIGHT * 0.45f);
                    blockedCell.setPosition(gridToPosition(row, col));
                    window.draw(blockedCell);
                }
            }
        }

        // Draw counters - positions adjusted to match your layout
        write(window, to_string(fruitCounters[0]), 110, 370 - 8, 30, Color::White); // cucumber (type 0)
        write(window, to_string(fruitCounters[3]), 230, 370 - 8, 30, Color::White); // watermelon (type 3)
        write(window, to_string(fruitCounters[1]), 110, 450 - 8, 30, Color::White); // lemon (type 2)
        write(window, to_string(fruitCounters[2]), 230, 450 - 8, 30, Color::White); // orange (type 1)
    }

    void result_Screen(RenderWindow &window, Player &player)
    {
        int points = 0;

        points += fruitCounters[0] * 10;
        points += fruitCounters[1] * 11;
        points += fruitCounters[2] * 15;
        points += fruitCounters[3] * 12;

        if (points >= 100)
        {
            points /= 50;
        }
        else
        {
            points = 0;
        }
        // Store original pet scales
        float originalScales[4][2]; // [pet_index][x,y]

        // Dragon
        originalScales[0][0] = player.dragon.sprite.getScale().x;
        originalScales[0][1] = player.dragon.sprite.getScale().y;
        // Griffin
        originalScales[1][0] = player.griffin.sprite.getScale().x;
        originalScales[1][1] = player.griffin.sprite.getScale().y;
        // Unicorn
        originalScales[2][0] = player.unicorn.sprite.getScale().x;
        originalScales[2][1] = player.unicorn.sprite.getScale().y;
        // Phoenix
        originalScales[3][0] = player.phoenix.sprite.getScale().x;
        originalScales[3][1] = player.phoenix.sprite.getScale().y;

        Object resultBg("images/result_screen.png", {0.f, 0.f}, {1.f, 1.f});
        Button buttons[8] = {
            Button(945, 10, 43, 43),   // exit
            Button(24, 534, 45, 43),   // back
            Button(380, 502, 75, 65),  // Left pet selector
            Button(530, 502, 75, 65),  // Right pet selector
            Button(695, 230, 135, 50), // Add Attack
            Button(695, 295, 135, 50), // Add Health
            Button(695, 362, 135, 50), // Add Speed
            Button(695, 423, 135, 50)  // Add Defence
        };

        int current_pet = 0; // 0:dragon, 1:griffin, 2:unicorn, 3:phoenix
        const Vector2f petPosition = {440.f, 290.f};
        const float petScales[4] = {0.9f, 0.8f, 1.25f, 1.f}; // Scales for each pet

        Clock animationClock;
        while (window.isOpen())
        {
            float deltaTime = animationClock.restart().asSeconds();
            Event event;
            while (window.pollEvent(event))
            {
                if (event.type == Event::Closed)
                {
                    window.close();
                    exit(EXIT_SUCCESS);
                }

                if (event.type == Event::MouseButtonPressed &&
                    event.mouseButton.button == Mouse::Left)
                {
                    Vector2i mousePos = Mouse::getPosition(window);

                    cout << mousePos.x << "," << mousePos.y << endl;
                    if (buttons[0].isClicked(mousePos)) // Exit
                    {
                        // Restore original scales before exiting
                        player.dragon.sprite.setScale(originalScales[0][0], originalScales[0][1]);
                        player.griffin.sprite.setScale(originalScales[1][0], originalScales[1][1]);
                        player.unicorn.sprite.setScale(originalScales[2][0], originalScales[2][1]);
                        player.phoenix.sprite.setScale(originalScales[3][0], originalScales[3][1]);

                        savePlayerData(player, player.getName());
                        exit(EXIT_SUCCESS);
                    }
                    else if (buttons[1].isClicked(mousePos)) // Back
                    {
                        // Restore original scales
                        player.dragon.sprite.setScale(originalScales[0][0], originalScales[0][1]);
                        player.griffin.sprite.setScale(originalScales[1][0], originalScales[1][1]);
                        player.unicorn.sprite.setScale(originalScales[2][0], originalScales[2][1]);
                        player.phoenix.sprite.setScale(originalScales[3][0], originalScales[3][1]);
                        return;
                    }
                    else if (buttons[2].isClicked(mousePos) && points > 0) // Left pet selector
                    {
                        current_pet = (current_pet - 1 + 4) % 4;
                    }
                    else if (buttons[3].isClicked(mousePos) && points > 0) // Right pet selector
                    {
                        current_pet = (current_pet + 1) % 4;
                    }
                    else if (buttons[4].isClicked(mousePos) && points > 0) // Add Attack
                    {
                        points--;
                        switch (current_pet)
                        {
                        case 0:
                            player.dragon.addAttack(1 + 1);
                            player.dragon.increaseLevel();
                            break;
                        case 1:
                            player.griffin.addAttack(1 + 1);
                            player.griffin.increaseLevel();
                            break;
                        case 2:
                            player.unicorn.addAttack(1 + 1);
                            player.unicorn.increaseLevel();
                            break;
                        case 3:
                            player.phoenix.addAttack(1 + 1);
                            player.phoenix.increaseLevel();
                            break;
                        }
                    }
                    else if (buttons[5].isClicked(mousePos) && points > 0) // Add Health
                    {
                        points--;
                        switch (current_pet)
                        {
                        case 0:
                            player.dragon.addMaxHealth(15);
                            player.dragon.increaseLevel();
                            break;
                        case 1:
                            player.griffin.addMaxHealth(15);
                            player.griffin.increaseLevel();
                            break;
                        case 2:
                            player.unicorn.addMaxHealth(15);
                            player.unicorn.increaseLevel();
                            break;
                        case 3:
                            player.phoenix.addMaxHealth(15);
                            player.phoenix.increaseLevel();
                            break;
                        }
                    }
                    else if (buttons[6].isClicked(mousePos) && points > 0) // Add Speed
                    {
                        points--;
                        switch (current_pet)
                        {
                        case 0:
                            player.dragon.addSpeed(1);
                            player.dragon.increaseLevel();
                            break;
                        case 1:
                            player.griffin.addSpeed(1);
                            player.griffin.increaseLevel();
                            break;
                        case 2:
                            player.unicorn.addSpeed(1);
                            player.unicorn.increaseLevel();
                            break;
                        case 3:
                            player.phoenix.addSpeed(1);
                            player.phoenix.increaseLevel();
                            break;
                        }
                    }
                    else if (buttons[7].isClicked(mousePos) && points > 0) // Add Defense
                    {
                        points--;
                        switch (current_pet)
                        {
                        case 0:
                            player.dragon.addDefence(1 + 1);
                            player.dragon.increaseLevel();
                            break;
                        case 1:
                            player.griffin.addDefence(1 + 1);
                            player.griffin.increaseLevel();
                            break;
                        case 2:
                            player.unicorn.addDefence(1 + 1);
                            player.unicorn.increaseLevel();
                            break;
                        case 3:
                            player.phoenix.addDefence(1 + 1);
                            player.phoenix.increaseLevel();
                            break;
                        }
                    }
                }
            }

            // Update animations
            player.dragon.updateAnimation(deltaTime);
            player.griffin.updateAnimation(deltaTime);
            player.unicorn.updateAnimation(deltaTime);
            player.phoenix.updateAnimation(deltaTime);

            window.clear();
            resultBg.draw(window);

            // Draw current pet
            float currentScale = petScales[current_pet];
            switch (current_pet)
            {
            case 0: // Dragon
                player.dragon.sprite.setScale(currentScale, currentScale);
                player.dragon.sprite.setPosition(petPosition);
                player.dragon.draw(window);
                break;
            case 1: // Griffin
                player.griffin.sprite.setScale(currentScale, currentScale);
                player.griffin.sprite.setPosition(petPosition);
                player.griffin.draw(window);
                break;
            case 2: // Unicorn
                player.unicorn.sprite.setScale(currentScale, currentScale);
                player.unicorn.sprite.setPosition(petPosition);
                player.unicorn.draw(window);
                break;
            case 3: // Phoenix
                player.phoenix.sprite.setScale(currentScale, currentScale);
                player.phoenix.sprite.setPosition(petPosition);
                player.phoenix.draw(window);
                break;
            }

            write(window, to_string(points), 830, 520, 22);
            window.display();
        }
    }

public:
    TRAINING()
        : activeFruits(new FruitData[MAX_FRUITS]),
          activeFruitCount(0),
          fakeHealth(100.f),
          nextSpawnTime(SPAWN_MIN_TIME),
          gameOver(false),
          monsterCount(0)
    {
        for (int i = 0; i < MAX_FRUITS; i++)
            activeFruits[i].obj = nullptr;
        for (int i = 0; i < FRUIT_TYPES; i++)
            fruitCounters[i] = 0;

        for (int row = 0; row < GRID_ROWS; row++)
            for (int col = 0; col < GRID_COLS; col++)
                gridOccupied[row][col] = gridBlocked[row][col] = false;
    }

    ~TRAINING()
    {
        for (int i = 0; i < MAX_FRUITS; i++)
            if (activeFruits[i].obj)
                delete activeFruits[i].obj;
        delete[] activeFruits;
    }

    void training_center(RenderWindow &window, Player &player)
    {
        Object bg("images/training.png", {0.f, 0.f}, {1.f, 1.f});
        gameClock.restart();
        spawnClock.restart();
        Button b(24, 534, 45, 43);

        while (window.isOpen() && !gameOver)
        {

            float elapsedTime = gameClock.getElapsedTime().asSeconds();

            if (elapsedTime >= GAME_TIME_LIMIT)
            {
                timeOver = true;

                break;
            }
            Event event;
            while (window.pollEvent(event))
            {
                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
                {
                    Vector2i mousePos = Mouse::getPosition(window);

                    if (b.isClicked(mousePos))
                    {
                        resetGame();
                        return;
                    }
                }
                handleInput(window, event);
            }
            spawnObjects();

            window.clear();
            bg.draw(window);
            drawGrid(window);

            for (int i = 0; i < activeFruitCount; i++)
                if (activeFruits[i].obj)
                    activeFruits[i].obj->draw(window);

            write(window, to_string(static_cast<int>(GAME_TIME_LIMIT - elapsedTime)) + " sec", 706, 35, 25, Color::Yellow);

            window.display();
        }

        if (gameOver)
        {
            write(window, "GAME OVER!", 430, 270, 60, Color::Magenta);
            window.display();
            sleep(seconds(4));
            result_Screen(window, player);
            resetGame();
        }
        else if (timeOver)
        {
            write(window, "TIME UP!", 430, 270, 60, Color::Magenta);
            window.display();
            sleep(seconds(4));
            result_Screen(window, player);
            resetGame();
        }
    }

    const int *getFruitCounters() const { return fruitCounters; }
    bool isGameOver() const { return gameOver; }
    float getFakeHealth() const { return fakeHealth; }
};