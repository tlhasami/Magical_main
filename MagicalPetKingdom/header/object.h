#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

using namespace sf;
using namespace std;

class Object {
 public:
  Texture texture;
  Sprite sprite;


  Object(string path, Vector2f position, Vector2f scale) {
    if (!texture.loadFromFile(path)) {
      cerr << "Failed to load texture: " << path << endl;
    }
    sprite.setTexture(texture);
    sprite.setPosition(position);
    sprite.setScale(scale);
  }

  void draw(RenderWindow &window) { window.draw(sprite); }

  void changePosition(Vector2f newPosition) { sprite.setPosition(newPosition); }

  void changeScale(Vector2f newScale) { sprite.setScale(newScale); }

  bool checkCollision(Vector2i mousePosition) {
    return sprite.getGlobalBounds().contains(
        static_cast<Vector2f>(mousePosition));
  }

  FloatRect getGlobalBond() const { return sprite.getGlobalBounds(); }

  void moveObject(float deltaTime, Vector2f windowSize) {
    Vector2f position = sprite.getPosition();
    Vector2f scale = sprite.getScale();
    Vector2u textureSize = texture.getSize();

    position.x += 100 * deltaTime;
    position.y += 100 * deltaTime;

    // Wrap around screen edges
    if (position.x > windowSize.x) position.x = -textureSize.x * scale.x;
    if (position.y > windowSize.y) position.y = -textureSize.y * scale.y;
    if (position.x < -textureSize.x * scale.x) position.x = windowSize.x;
    if (position.y < -textureSize.y * scale.y) position.y = windowSize.y;

    sprite.setPosition(position);
  }

  void setIntRect(float left, float top, float width, float height) {
    sprite.setTextureRect(IntRect(static_cast<int>(left), static_cast<int>(top),
                                  static_cast<int>(width),
                                  static_cast<int>(height)));
  }

  float getHeight() const { return sprite.getGlobalBounds().height; }

  float getLeft() const { return sprite.getGlobalBounds().left; }

  float getRight() const {
    FloatRect bounds = sprite.getGlobalBounds();
    return bounds.left + bounds.width;
  }

  float getTop() const { return sprite.getGlobalBounds().top; }

  float getBottom() const {
    FloatRect bounds = sprite.getGlobalBounds();
    return bounds.top + bounds.height;
  }

  float getWidth() const { return sprite.getGlobalBounds().width; }

  void changeImage(string newPath) {
    if (!texture.loadFromFile(newPath)) {
      cerr << "Failed to load new texture: " << newPath << endl;
    } else {
      sprite.setTexture(texture, true);
    }
  }
};