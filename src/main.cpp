#include "common.h"
#include "Includes.h"

// Globals
bool isActive = true;
Input input;

struct GameObject {
    sf::Vector2f pos;
    sf::Sprite sprite;
    GameObject() : pos(0,0), sprite() {}
    virtual ~GameObject() {};
    virtual void Update() = 0;
};

struct Player : public GameObject {
    virtual void Update() {
        sf::Vector2f playerSpeed;
        float speedFactor = 2.0;
        if (isActive) {
            if (input.IsKeyDown(sf::Keyboard::Left)) { playerSpeed.x = -speedFactor; };
            if (input.IsKeyDown(sf::Keyboard::Right)) { playerSpeed.x = speedFactor; };
            if (input.IsKeyDown(sf::Keyboard::Up)) { playerSpeed.y = -speedFactor; };
            if (input.IsKeyDown(sf::Keyboard::Down)) { playerSpeed.y = speedFactor; };
        }
        pos += playerSpeed;
    }
};

struct Bullet : public GameObject {
    sf::Vector2f speed;
    Bullet(sf::Vector2f speed)
    : speed(speed) {
    }

    virtual void Update() {
        pos += speed;
    }
};

void PollEvents(Window &window) {
    Event event;
    while (window.PollEvent(event)) {
        input.OnEvent(event);

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
}

#ifdef _WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
#else
int main(int argc, char *argv[]) {
	(void)argc; (void)argv;
#endif

    // Enable depth buffer
    sf::ContextSettings settings;
    settings.DepthBits = 24;

    VideoMode vmode = VideoMode(800, 600);
    RenderWindow window(vmode, "Game", sf::Style::Default, settings);
    window.SetFramerateLimit(60);

    sf::Texture background;
    background.LoadFromFile(GetResourcePath("background.png"));
    sf::Texture playerTex;
    playerTex.LoadFromFile(GetResourcePath("player.png"));
    sf::Texture bulletTex;
    bulletTex.LoadFromFile(GetResourcePath("bullet.png"));

    ShaderProgram shader;
    shader.LoadFromFile(GetResourcePath("shader1.glsl"));

    sf::Sprite bgSprite(background);
    bgSprite.Resize((float)vmode.Width, (float)vmode.Height);

    Player player;
    player.sprite.SetTexture(playerTex);
    player.pos = sf::Vector2f(100, 100);

    //sf::SoundBuffer buffer;
    //buffer.LoadFromFile("sound.wav");

    //sf::Sound sound;
    //sound.SetBuffer(buffer);
    //sound.Play();

    vector<GameObject*> objects;
    objects.push_back(&player);
    sf::Clock timer;
    uint32_t waitTime = 0;

    sf::Vector2f playerSpeed;
    while (window.IsOpened()) {

        PollEvents(window);

        for (size_t i = 0; i < objects.size(); i++) {
            objects[i]->Update();
        }
        if (isActive) {
            if (input.IsKeyDown(sf::Keyboard::Space)) {
            //if (sf::Keyboard::IsKeyPressed(sf::Keyboard::Space)) {
              if (timer.GetElapsedTime() - waitTime > 100) {
                Bullet *bullet = new Bullet(sf::Vector2f(0,-3));
                    bullet->pos = player.pos;
                    bullet->sprite.SetTexture(bulletTex);
                    objects.push_back(bullet);
                    waitTime = timer.GetElapsedTime();
                }
            }
        }

        window.Clear();

        window.Draw(bgSprite);

        //shader.Enable();
        for (size_t i = 0; i < objects.size(); i++) {
            objects[i]->sprite.SetPosition(objects[i]->pos);
            window.Draw(objects[i]->sprite);
        }
        //shader.Disable();

        window.Display();
    }


    return 0;
}
