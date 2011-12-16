#pragma once
#include <SFML/Window.hpp>

class Input {
public:
    Input();
    bool IsKeyDown(sf::Keyboard::Key KeyCode) const;

    void OnEvent(const sf::Event &event);
    void ResetStates();

private:
    bool myKeys[sf::Keyboard::KeyCount];
};

