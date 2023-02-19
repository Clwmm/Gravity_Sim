#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"
#include "SFML/Graphics.hpp"
#include <memory>
#include <iostream>

#include "Entity.h"

int fromRandom(int a, int b)
{
    return (rand() % (b - a + 1)) + a;
}

int main()
{
    srand(time(NULL));
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "Window Title");
    
    sf::View view(sf::Vector2f(0, 0), sf::Vector2f(1000, 1000));
    window.setView(view);

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
        auto temp = std::make_shared<Entity>((float)fromRandom(1, 8), (float)fromRandom(5, 30), 75.0f, (float)fromRandom(0, 365), 300.0f, 400.0f, whiteColor);
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
            auto temp = std::make_shared<Entity>((float)mass/100, speed, mass, angle, posX, posY, color);
            tempEntities.push_back(temp);
        }
        if (ImGui::Button("Add random object to list"))
        {
            float tempmass = (float)fromRandom(0, 10000);
            float tempcolor[3] =
            {
                (float)(fromRandom(0,255) / 255),
                (float)(fromRandom(0,255) / 255),
                (float)(fromRandom(0,255) / 255)
            };
            auto temp = std::make_shared<Entity>((float)(tempmass/100), (float)fromRandom(1, 50), tempmass, (float)fromRandom(0, 365), posX, posY, whiteColor);
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
                float ax = 0, ay = 0;
                // Gravity math
                for (auto i : entities)
                {
                    if (e != i)
                    {
                        float dx = e->x - i->x;
                        float dy = e->y - i->y;
                        float r3;
                        r3 *= sqrt(r3 = dx * dx + dy * dy);
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