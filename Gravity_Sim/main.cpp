#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"
#include "SFML/Graphics.hpp"
#include <memory>
#include <iostream>

#include "Entity.h"

#define G 0.0000000000667

int fromRandom(int a, int b)
{
    int ret = (rand() % (b - a + 1)) + a;
    std::cout << ret << std::endl;
    return ret;
}

int main()
{
    srand(time(NULL));
    sf::Vector2f screenSize = sf::Vector2f(1200, 1000);

    sf::RenderWindow window(sf::VideoMode(screenSize.x, screenSize.y), "Window Title");
    window.setFramerateLimit(60);
    
    sf::View view(sf::Vector2f(0, 0), sf::Vector2f(screenSize.x, screenSize.y));

    bool play = true;

    ImGui::SFML::Init(window);

    std::vector<std::shared_ptr<Entity>> entities;
    std::vector<std::shared_ptr<Entity>> tempEntities;

    sf::CircleShape generatePointer(2, 5);
    generatePointer.setFillColor(sf::Color(181, 38, 131, 150));
    generatePointer.setOrigin(2, 2);

    
    float angle = 0;
    float speed = 10;
    float mass = 500;
    float posX = 0;
    float posY = 0;
    float color[4] =
    {
        255 / 255,
        255 / 255,
        255 / 255,
        255 / 255
    };

    float whiteColor[4] = { 255 / 255, 255 / 255, 255 / 255, 255/255 };
    float blackColor[4] = { 0 / 255, 0 / 255, 0 / 255, 255/255 };

    for (int i = 0; i < 0; i++)
    {
        auto temp = std::make_shared<Entity>((double)fromRandom(1, 8), (double)fromRandom(5, 30), (double)75, (double)fromRandom(0, 365), (double)300, (double)400, whiteColor);
        entities.push_back(temp);
    }

    sf::Clock deltaClock;
    float deltaTime = 0;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);

            /*if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                auto temp = std::make_shared<Entity>((float)fromRandom(1, 8), (float)fromRandom(5, 30), 75.0f, (float)fromRandom(0, 365), mousePos.x, mousePos.y, whiteColor);
                entities.push_back(temp);
            }*/
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
            {
                sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
                sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                posX = worldPos.x;
                posY = worldPos.y;
            }
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;

            case sf::Event::Resized:
                view.setSize(sf::Vector2f(event.size.width, event.size.height));
                break;

            case sf::Event::KeyPressed:
                switch (event.key.code)
                {
                case sf::Keyboard::Escape:
                    window.close();
                    break;

                case sf::Keyboard::C:
                    entities.clear();
                    break;

                default:
                    break;
                }

            default:
                break;
            }
        }
        deltaTime = deltaClock.getElapsedTime().asSeconds();
        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::Begin("Main");
        ImGui::Checkbox("Play", &play);
        ImGui::Text("Entities: %i Fps: %.2f", entities.size(), ImGui::GetIO().Framerate);
        if (ImGui::Button("Clear"))
        {
            entities.clear();
            tempEntities.clear();
        }
        ImGui::End();

        ImGui::Begin("Generate");
        ImGui::SliderFloat("Angle", &angle, 0.0f, 360.0f);
        ImGui::SliderFloat("Speed", &speed, 0.0f, 50.0f);
        ImGui::SliderFloat("Mass", &mass, 0.0f, 10000.0f);
        ImGui::Text("You can set position using Left mouse button on window");
        ImGui::SliderFloat("Position X", &posX, 0.0f, window.getSize().x);
        ImGui::SliderFloat("Position Y", &posY, 0.0f, window.getSize().y);
        ImGui::ColorEdit4("Color", color);
        if (ImGui::Button("Add to generate list"))
        {
            auto temp = std::make_shared<Entity>((double)mass/100, (double)speed, (double)mass, (double)angle, (double)posX, (double)posY, color);
            tempEntities.push_back(temp);
        }
        if (ImGui::Button("Add random object to list"))
        {
            double tempmass = (double)fromRandom(0, 10000);
            auto temp = std::make_shared<Entity>((double)(tempmass/100), (double)fromRandom(1, 50), (double)tempmass, (double)fromRandom(0, 365), (double)posX, (double)posY, color);
            tempEntities.push_back(temp);
        }
        ImGui::Text("Entities to generate: %i", tempEntities.size());
        if (ImGui::Button("Generate"))
        {
            for (auto i : tempEntities)
                entities.push_back(i);
            tempEntities.clear();
        }
        ImGui::End();

        if (play)
        {
            for (auto e : entities)
            {
                double ax = 0, ay = 0;
                // Gravity math
                for (auto i : entities)
                {
                    if (e != i)
                    {
                        double dx = e->x - i->x;
                        double dy = e->y - i->y;
                        double r3;
                        r3 *= sqrt(r3 = (dx * dx) + (dy * dy));
                        ax -= i->mass * 100 * dx / r3;
                        ax -= i->mass * 100 * dy / r3;
                    }
                }
                e->dx += ax * deltaTime;
                e->dy += ay * deltaTime;

                // Update position
                e->update(deltaTime);
            }
        }

        generatePointer.setPosition(posX, posY);

        //view
        int viewSpeed = 1500;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E))
            view.zoom(1 + (2 * deltaTime));
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
            view.zoom(1 - (2 * deltaTime));
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
            view.move(0, -viewSpeed * deltaTime);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
            view.move(0, viewSpeed * deltaTime);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            view.move(-viewSpeed * deltaTime, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
            view.move(viewSpeed * deltaTime, 0);
            

        // Render
        window.setView(view);
        window.clear(sf::Color(18, 33, 43));
        for (auto e : entities)
            e->draw(window);
        window.draw(generatePointer);
        ImGui::SFML::Render(window);
        window.display();
    }

    entities.clear();
    tempEntities.clear();

    ImGui::SFML::Shutdown();
    return 0;
}



/*
    Wielkoœæ zale¿na od masy 1/1
    Po wybraniu miejsca na oknie, pokazanie planety na a = 50%, aby widaæ by³o zmiany
    Play i pause : gdy jest zatrzymana aplikacja mo¿na najechaæ myszk¹ na obiekty i sprawdziæ ich dane i je zmieniæ
    ruszanie widokiem (move and zoom in and zoom out)
    ...

    dodatki:
        wyœwietlanie elipsy poruszania siê cia³
*/