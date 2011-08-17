#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

using namespace std;
using namespace sf;

int main(int argc, char *argv[]) {
    VideoMode vmode = VideoMode(800, 600);
    RenderWindow window(vmode, "Game");
    window.SetFramerateLimit(60);

    sf::Texture background;
    background.LoadFromFile("resources/background.png");
    sf::Texture playerTex;
    playerTex.LoadFromFile("resources/player.png");

    sf::Sprite bgSprite(background);
    bgSprite.Resize((float)vmode.Width, (float)vmode.Height);

    sf::Sprite player(playerTex);

    //sf::SoundBuffer buffer;
    //buffer.LoadFromFile("sound.wav");

    //sf::Sound sound;
    //sound.SetBuffer(buffer);
    //sound.Play();

    int isActive = true;
    while (window.IsOpened()) {
        Event event;
        while (window.PollEvent(event)) {
            if (event.Type == Event::Closed)
                window.Close();
            else if (event.Type == sf::Event::GainedFocus)
                isActive = true;
            else if (event.Type == sf::Event::LostFocus)
                isActive = false;
            else if ((event.Type == Event::KeyPressed)) {
                switch (event.Key.Code) {
                    case Keyboard::Escape:
                        window.Close();
                        break;
                    default:
                        break;
                }
            }
        }

        float speed = 1.0;
        if (isActive) {
            if (sf::Keyboard::IsKeyPressed(sf::Keyboard::A)) player.Move(-speed, 0);
            if (sf::Keyboard::IsKeyPressed(sf::Keyboard::D)) player.Move(speed, 0);
            if (sf::Keyboard::IsKeyPressed(sf::Keyboard::W)) player.Move(0, -speed);
            if (sf::Keyboard::IsKeyPressed(sf::Keyboard::S)) player.Move(0, speed);
        }

        window.Clear();

        window.Draw(bgSprite);
        window.Draw(player);


        window.Display();
    }

    (void)argc; (void)argv;

    return 0;
}
