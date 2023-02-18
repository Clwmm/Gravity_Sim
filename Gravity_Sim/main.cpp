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
    sf::RenderWindow window(sf::VideoMode(800, 800), "Window Title");
    bool play = true;

    ImGui::SFML::Init(window);

    std::vector<std::shared_ptr<Entity>> entities;
    std::vector<std::shared_ptr<Entity>> tempEntities;

    sf::CircleShape generatePointer(2, 5);
    generatePointer.setFillColor(sf::Color(181, 38, 131, 150));

    float angle = 0;
    float speed = 10;
    float mass = 500;
    float posX = 400;
    float posY = 400;
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

            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                auto temp = std::make_shared<Entity>((float)fromRandom(1, 8), (float)fromRandom(5, 30), 75.0f, (float)fromRandom(0, 365), mousePos.x, mousePos.y, whiteColor);
                entities.push_back(temp);
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                posX = mousePos.x;
                posY = mousePos.y;
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
            auto temp = std::make_shared<Entity>(mass/100, speed, mass, angle, posX, posY, color);
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
                // Gravity math


                // Update position
                e->update(deltaTime);
            }
        }

        generatePointer.setPosition(posX, posY);

        // Render
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