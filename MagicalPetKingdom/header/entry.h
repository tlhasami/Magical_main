#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

#include "start.h"

extern Music music;
extern Font font;

extern User_info DATABASE;

extern Guild_info GUILD_PRESENT;

class entry
{
    string username;

    bool isTypingUsername = false;

    void drawInputField(RenderWindow &window, Font &font, const string &text,
                        float x, float y, bool isActive)
    {
        RectangleShape field(Vector2f(300, 40));
        field.setPosition(x, y);
        field.setFillColor(isActive ? Color(200, 200, 255) : Color(230, 230, 230));
        field.setOutlineThickness(2);
        field.setOutlineColor(isActive ? Color::Blue : Color(150, 150, 150));

        Text displayText(text, font, 24);
        displayText.setPosition(x + 10, y + 5);
        displayText.setFillColor(Color::Black);

        window.draw(field);
        window.draw(displayText);
    }

    int error_handle(RenderWindow &window)
    {
        Object screen("images/error.png", Vector2f(0, 0), Vector2f(1.f, 1.f));

        Button buttons[4] = {Button(945, 10, 43, 43), Button(24, 534, 45, 43),
                             Button(342, 365, 465 - 342, 432 - 365),
                             Button(531, 365, 668 - 542, 433 - 365)};

        while (window.isOpen())
        {
            Event event;
            while (window.pollEvent(event))
            {
                if (event.type == Event::Closed)
                {
                    window.close();
                    exit(EXIT_SUCCESS);
                }

                // Handle mouse clicks
                if (event.type == Event::MouseButtonPressed)
                {
                    Vector2i mousePos = Mouse::getPosition(window);

                    cout << "Right clicked at: (" << mousePos.x << ", " << mousePos.y
                         << ")" << endl;

                    if (buttons[0].isClicked(mousePos))
                    {
                        exit(EXIT_SUCCESS);
                    }
                    else if (buttons[1].isClicked(mousePos))
                    {
                        return -1;
                    }
                    else if (buttons[2].isClicked(mousePos))
                    {
                        // pressed yes
                        cout << "Yes is pressed " << endl;
                        return 1;
                    }
                    else if (buttons[3].isClicked(mousePos))
                    {
                        // pressed no
                        cout << "no is pressed " << endl;
                        return 0;
                    }
                }
            }

            // Draw everything
            window.clear();
            screen.draw(window);
            window.display();
        }

        return -1;
    }

public:
    string signup(RenderWindow &window)
    {
        Object screen("images/signup.png", Vector2f(0, 0), Vector2f(1.f, 1.f));
        Button buttons[3] = {Button(945, 10, 43, 43), Button(24, 534, 45, 43),
                             Button(440, 365, 616 - 440, 438 - 365)};

        // Input field configuration
        const int MAX_CHARS = 18;
        FloatRect inputFieldBounds(380, 290, 300, 40);
        Text inputText("", font, 24);
        inputText.setFillColor(Color::Black);
        string username;

        while (window.isOpen())
        {
            Event event;
            while (window.pollEvent(event))
            {
                if (event.type == Event::Closed)
                {
                    window.close();
                   exit(EXIT_SUCCESS);
                }

                // Handle mouse clicks
                if (event.type == Event::MouseButtonPressed)
                {
                    Vector2i mousePos = Mouse::getPosition(window);

                    cout << "Right clicked at: (" << mousePos.x << ", " << mousePos.y
                         << ")" << endl;

                    if (buttons[0].isClicked(mousePos))
                    {
                        exit(EXIT_SUCCESS);
                    }
                    else if (buttons[1].isClicked(mousePos))
                    {
                        return "";
                    }
                    else if (buttons[2].isClicked(mousePos))
                    {
                        cout << "returning username" << endl;
                        // its signup
                        if (!DATABASE.isPresent(username))
                        {
                            return username;
                        }
                        else
                        {
                            int sol = error_handle(window);

                            if (sol == 1)
                            {
                                return signup(window);
                            }
                            else if (sol == 0)
                            {
                                username = "";
                                return "";
                            }
                        }
                    }
                }

                // Handle text input
                if (event.type == Event::TextEntered)
                {
                    if (event.text.unicode == '\b' && !username.empty())
                    {
                        username.pop_back();
                    }
                    else if (username.length() < MAX_CHARS &&
                             event.text.unicode < 128 && event.text.unicode != '\r')
                    {
                        username += static_cast<char>(event.text.unicode);
                    }

                    // Update displayed text with proper alignment
                    inputText.setString(username);
                    FloatRect textBounds = inputText.getLocalBounds();

                    // Center text within input field
                    if (textBounds.width <= inputFieldBounds.width - 20)
                    {
                        inputText.setPosition(
                            inputFieldBounds.left +
                                (inputFieldBounds.width - textBounds.width) / 2,
                            inputFieldBounds.top + 5);
                    }
                    else
                    {
                        // Right-align if text is longer than field
                        inputText.setPosition(inputFieldBounds.left +
                                                  inputFieldBounds.width -
                                                  textBounds.width - 10,
                                              inputFieldBounds.top + 5);
                    }
                }
            }

            // Draw everything
            window.clear();
            screen.draw(window);

            // Draw input field background
            RectangleShape inputField(
                Vector2f(inputFieldBounds.width, inputFieldBounds.height));
            inputField.setPosition(inputFieldBounds.left, inputFieldBounds.top);
            inputField.setFillColor(Color::White);
            inputField.setOutlineThickness(2);
            inputField.setOutlineColor(Color(150, 150, 150));

            window.draw(inputField);
            window.draw(inputText);
            window.display();
        }
        return username;
    }

    string login(RenderWindow &window)
    {
        Object screen("images/login.png", Vector2f(0, 0), Vector2f(1.f, 1.f));
        Button buttons[3] = {Button(945, 10, 43, 43), Button(24, 534, 45, 43),
                             Button(376, 376, 639 - 376, 443 - 376)};

        // Input field configuration
        const int MAX_CHARS = 18;
        FloatRect inputFieldBounds(350, 250, 300, 40);
        Text inputText("", font, 24);
        inputText.setFillColor(Color::Black);
        string username;

        while (window.isOpen())
        {
            Event event;
            while (window.pollEvent(event))
            {
                if (event.type == Event::Closed)
                {
                    window.close();
                    exit(EXIT_SUCCESS);
                }

                // Handle mouse clicks
                if (event.type == Event::MouseButtonPressed)
                {
                    Vector2i mousePos = Mouse::getPosition(window);

                    cout << "Right clicked at: (" << mousePos.x << ", " << mousePos.y
                         << ")" << endl;

                    if (buttons[0].isClicked(mousePos))
                    {
                        exit(EXIT_SUCCESS);
                    }
                    else if (buttons[1].isClicked(mousePos))
                    {
                        return "";
                    }
                    else if (buttons[2].isClicked(mousePos))
                    {
                        if (DATABASE.isPresent(username))
                        {
                            return username;
                        }
                        else
                        {
                            int sol = error_handle(window);
                            if (sol == 1)
                            {
                                return login(window);
                            }
                            else if (sol == 0)
                            {
                                username = "";
                                return "";
                            }
                        }
                    }
                }

                // Handle text input
                if (event.type == Event::TextEntered)
                {
                    if (event.text.unicode == '\b' && !username.empty())
                    {
                        username.pop_back();
                    }
                    else if (username.length() < MAX_CHARS &&
                             event.text.unicode < 128 && event.text.unicode != '\r')
                    {
                        username += static_cast<char>(event.text.unicode);
                    }

                    // Update displayed text with proper alignment
                    inputText.setString(username);
                    FloatRect textBounds = inputText.getLocalBounds();

                    // Center text within input field
                    if (textBounds.width <= inputFieldBounds.width - 20)
                    {
                        inputText.setPosition(
                            inputFieldBounds.left +
                                (inputFieldBounds.width - textBounds.width) / 2,
                            inputFieldBounds.top + 5);
                    }
                    else
                    {
                        // Right-align if text is longer than field
                        inputText.setPosition(inputFieldBounds.left +
                                                  inputFieldBounds.width -
                                                  textBounds.width - 10,
                                              inputFieldBounds.top + 5);
                    }
                }
            }

            // Draw everything
            window.clear();
            screen.draw(window);

            // Draw input field background
            RectangleShape inputField(
                Vector2f(inputFieldBounds.width, inputFieldBounds.height));
            inputField.setPosition(inputFieldBounds.left, inputFieldBounds.top);
            inputField.setFillColor(Color::White);
            inputField.setOutlineThickness(2);
            inputField.setOutlineColor(Color(150, 150, 150));

            window.draw(inputField);
            window.draw(inputText);
            window.display();
        }
        return username;
    }

    string login_or_signup(RenderWindow &window)
    {
        Object screen("images/sign_log.png", Vector2f(0, 0), Vector2f(1.f, 1.f));

        Button button[4] = {Button(945, 10, 43, 43), Button(24, 534, 45, 43),
                            Button(360, 220, 641 - 358, 285 - 220),
                            Button(360, 318, 630 - 360, 382 - 315)};
        while (window.isOpen())
        {
            Event event;
            while (window.pollEvent(event))
            {
                if (event.type == Event::Closed)
                {
                    window.close();
                    exit(EXIT_SUCCESS);
                }

                // Handle mouse clicks
                if (event.type == Event::MouseButtonPressed &&
                    event.mouseButton.button == Mouse::Left)
                {
                    Vector2i mousePos = Mouse::getPosition(window);
                    cout << "Right clicked at: (" << mousePos.x << ", " << mousePos.y
                         << ")" << endl;

                    if (button[0].isClicked(mousePos))
                    {
                        cout << "Exit" << endl;
                        exit(EXIT_SUCCESS);
                    }
                    else if (button[1].isClicked(mousePos))
                    {
                        cout << "Return";
                        exit(EXIT_SUCCESS);
                    }
                    else if (button[2].isClicked(mousePos))
                    {
                        cout << "Login " << endl;
                        return login(window);
                    }
                    else if (button[3].isClicked(mousePos))
                    {
                        cout << "Signup" << endl;
                        return signup(window);
                    }
                }
            }

            // Draw everything
            window.clear();
            screen.draw(window);
            window.display();
        }
        return "unknown";
    }
};
