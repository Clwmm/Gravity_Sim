#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"
#include "SFML/Graphics.hpp"
#include <iostream>

#include "Entity.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 800), "Window Title");

    ImGui::SFML::Init(window);

    bool circleExists = true;
    float circleRadius = 200.0f;

    sf::CircleShape shape(200.f, 100);
    shape.setFillColor(sf::Color(204, 77, 5)); // Color circle
    shape.setPosition(200, 200); // Center circle

    float whiteColor[3] = { 255 / 255, 255 / 255, 255 / 255 };
    float blackColor[3] = { 0 / 255, 0 / 255, 0 / 255 };
    Entity a(5.0f, 20.0f, 100.0f, 45.0f, 400.0f, 400.0f, whiteColor);
    Entity b(3.0f, 25.0f, 75.0f, 90.0f, 300.0f, 400.0f, blackColor);

    sf::Clock deltaClock;
    float deltaTime = 0;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed)
                window.close();
        }
        deltaTime = deltaClock.getElapsedTime().asSeconds();
        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::Begin("Circle");
        ImGui::Checkbox("Render", &circleExists);
        ImGui::SliderFloat("Radius", &circleRadius, 10.0f, 400.0f);
        ImGui::End();

        
        std::cout << "Dx: " << b.dx << "  Dy: " << b.dy << std::endl;
        a.update(deltaTime);
        b.update(deltaTime);

        shape.setRadius(circleRadius);

        window.clear(sf::Color(18, 33, 43)); // Color background
        a.draw(window);
        b.draw(window);
        ImGui::SFML::Render(window);
        window.display();
    }
    ImGui::SFML::Shutdown();
    return 0;
}



/*
    Wielkoœæ zale¿na od masy 1/1
    Po wybraniu miejsca na oknie, pokazanie planety na a = 50%, aby widaæ by³o zmiany
    ...
*/