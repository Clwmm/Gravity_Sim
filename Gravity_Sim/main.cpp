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

    bool circleExists = true;
    float circleRadius = 200.0f;

    sf::CircleShape shape(200.f, 100);
    shape.setFillColor(sf::Color(204, 77, 5)); // Color circle
    shape.setPosition(200, 200); // Center circle

    std::vector<std::shared_ptr<Entity>> entities;
    std::vector<std::shared_ptr<Entity>> tempEntities;


    float whiteColor[3] = { 255 / 255, 255 / 255, 255 / 255 };
    float blackColor[3] = { 0 / 255, 0 / 255, 0 / 255 };
    auto a = std::make_shared<Entity>(5.0f, 20.0f, 100.0f, 45.0f, 400.0f, 400.0f, whiteColor);
    auto b = std::make_shared<Entity>(3.0f, 25.0f, 75.0f, 90.0f, 300.0f, 400.0f, blackColor);

    entities.push_back(a);
    entities.push_back(b);

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
            /*if (event.type == sf::Event::Closed)
                window.close();*/
        }
        deltaTime = deltaClock.getElapsedTime().asSeconds();
        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::Begin("");
        ImGui::Checkbox("Play", &play);
        ImGui::Text("Entities: %i Fps: %.2f", entities.size(), ImGui::GetIO().Framerate);
        ImGui::End();

        ImGui::Begin("Generate");
        ImGui::SliderFloat("Angle", &circleRadius, 0.0f, 360.0f);
        ImGui::SliderFloat("Speed", &circleRadius, 0.0f, 50.0f);
        ImGui::SliderFloat("Mass", &circleRadius, 0.0f, 10000.0f);
        if (ImGui::Button("Add to generate list"))
        {
            auto temp = std::make_shared<Entity>((float)fromRandom(1, 8), (float)fromRandom(5, 30), 75.0f, (float)fromRandom(0, 365), 300.0f, 400.0f, whiteColor);
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
            for (auto e : entities)
                e->update(deltaTime);
        

        shape.setRadius(circleRadius);

        window.clear(sf::Color(18, 33, 43)); // Color background
        for (auto e : entities)
            e->draw(window);
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
*/