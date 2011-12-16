#include "Input.h"
#include <iostream>

Input::Input() {
    ResetStates();
}

bool Input::IsKeyDown(sf::Keyboard::Key KeyCode) const {
    return myKeys[KeyCode];
}

void Input::OnEvent(const sf::Event &event) {
    switch (event.Type) {
        // Key events
        case sf::Event::KeyPressed:  myKeys[event.Key.Code] = true;  break;
        case sf::Event::KeyReleased: myKeys[event.Key.Code] = false; break;
        case sf::Event::LostFocus: ResetStates(); break;
        default: break;
    }
}

void Input::ResetStates() {
    for (int i = 0; i < sf::Keyboard::KeyCount; ++i) {
        myKeys[i] = false;
    }
}
