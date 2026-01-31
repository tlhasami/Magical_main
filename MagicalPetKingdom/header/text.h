#pragma once
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <limits>

#include "player.h"
#include "pets.h"
#include "button.h"
#include "writer.h"
#include "object.h"

extern Music music;
extern Font font;

void write(RenderWindow &window, const string &text, float x, float y, unsigned int size = 24, Color color = Color::White)
{
    Text message;
    message.setFont(font);   // use global font
    message.setString(text); // the text to display
    message.setCharacterSize(size);
    message.setFillColor(color);
    message.setPosition(x, y);

    window.draw(message);
}
