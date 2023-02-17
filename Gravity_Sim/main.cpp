#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"
#include "SFML/Graphics.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 800), "Window Title");

    ImGui::SFML::Init(window);

    bool circleExists = true;
    float circleRadius = 200.0f;

    sf::CircleShape shape(200.f, 100);
    shape.setFillColor(sf::Color(204, 77, 5)); // Color circle
    shape.setPosition(200, 200); // Center circle

    sf::Clock deltaClock;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed)
                window.close();
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::Begin("Circle");
        ImGui::Checkbox("Render", &circleExists);
        ImGui::SliderFloat("Radius", &circleRadius, 10.0f, 400.0f);
        ImGui::End();


        shape.setRadius(circleRadius);

        window.clear(sf::Color(18, 33, 43)); // Color background
        window.draw(shape);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}