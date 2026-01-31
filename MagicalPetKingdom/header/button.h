
#include <SFML/Graphics.hpp>
#pragma once

using namespace sf;

class Button {
 private:
  RectangleShape m_rect;

 public:
  // Constructor - creates transparent button by default
  Button(float x = 0, float y = 0, float width = 100, float height = 50,
         Color c = Color::Transparent) {
    m_rect.setPosition(x, y);
    m_rect.setSize(Vector2f(width, height));
    m_rect.setFillColor(c);
    m_rect.setOutlineThickness(0);
  }

  // Check if button is clicked
  bool isClicked(const Vector2i &mousePos) const {
    return m_rect.getGlobalBounds().contains(static_cast<Vector2f>(mousePos));
  }

  // Draw the button (will be invisible if transparent)
  void draw(RenderWindow &window) const { window.draw(m_rect); }

  // Set visual properties
  void setFillColor(const Color &color) { m_rect.setFillColor(color); }

  void setOutline(float thickness, const Color &color) {
    m_rect.setOutlineThickness(thickness);
    m_rect.setOutlineColor(color);
  }

  void setPosition(float x, float y) { m_rect.setPosition(x, y); }

  void setSize(float width, float height) {
    m_rect.setSize(Vector2f(width, height));
  }

  // Get button bounds
  FloatRect getGlobalBounds() const { return m_rect.getGlobalBounds(); }
};