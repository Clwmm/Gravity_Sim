#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"
#include "SFML/Graphics.hpp"
#include <memory>
#include <iostream>
#include <cmath>

#include "Entity.h"

const double G = 6.67e-11;
const double PI = 3.14159265359;

int fromRandom(int a, int b)
{
    int ret = (rand() % (b - a + 1)) + a;
    return ret;
}

bool isInsideCircle(Entity& e, sf::Vector2f mousePos)
{
    double dis = pow((mousePos.x - e.x), 2) + pow(mousePos.y - e.y, 2);
    if (dis <= pow(e.r, 2))
        return true;
    else
        return false;
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

    /*sf::Cursor handCoursor;
    if (!handCoursor.loadFromSystem(sf::Cursor::Hand))
        std::cout << "Error loading Hand Cursor" << std::endl;
    sf::Cursor arrowCoursor;
    if (!handCoursor.loadFromSystem(sf::Cursor::Arrow))
        std::cout << "Error loading Arrow Cursor" << std::endl;*/
    
    
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

            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
                sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
                sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
                auto temp = std::make_shared<Entity>((float)fromRandom(1, 8), (float)fromRandom(5, 30), 75.0f, (float)fromRandom(0, 365), worldPos.x, worldPos.y, whiteColor);
                entities.push_back(temp);
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
            {
                sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
                sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
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
        ImGui::SliderFloat("Speed", &speed, 0.0f, 10000.0f);
        ImGui::SliderFloat("Mass", &mass, 0.0f, 1000000.0f);
        ImGui::Text("You can set position using Left mouse button on window");
        ImGui::SliderFloat("Position X", &posX, 0.0f, window.getSize().x);
        ImGui::SliderFloat("Position Y", &posY, 0.0f, window.getSize().y);
        ImGui::ColorEdit4("Color", color);
        if (ImGui::Button("Add to generate list"))
        {
            auto temp = std::make_shared<Entity>((double)mass/1000, (double)speed, (double)mass, (double)angle, (double)posX, (double)posY, color);
            tempEntities.push_back(temp);
        }
        if (ImGui::Button("Add random object to list"))
        {
            double tempmass = (double)fromRandom(0, 10000);
            auto temp = std::make_shared<Entity>((double)(tempmass/1000), (double)fromRandom(1, 50), (double)tempmass, (double)fromRandom(0, 365), (double)posX, (double)posY, color);
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
                        double diffx = e->x - i->x;
                        double diffy = e->y - i->y;
                        double dis = sqrt((diffx * diffx) + (diffy * diffy));
                        double F = (G * e->mass*1000 * i->mass*1000) / (dis * dis);
                        double angle = atan2(diffy, diffx);
                        double accel = F / e->mass*1000;
                        ax += -(accel * cos(angle));
                        ay += -(accel * sin(angle));

                        /*double angle = atan2(i->y - e->y, i->x - e->x) * 180 / PI;
                        std::cout << angle << std::endl;
                        double dis = sqrt((i->x - e->x)*(i->x - e->x) + (i->y - e->y)*(i->y - e->y));
                        double forceG = G * e->mass*100 * i->mass*100 / pow(dis,2);
                        ax = cos(angle) * 5;
                        ay = sin(angle) * 5;*/
                    }
                }
                e->dx += ax;
                e->dy += ay;

                // Update position
                e->update(deltaTime);
            }
        }
        else
        {
            sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
            sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
            for (auto e : entities)
            {
                if (isInsideCircle(*e, worldPos))
                {
                    std::cout << "Inside" << std::endl;
                }
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
        usuniêciê obiektu mniejszego gdy wszed³ w CA£OŒÆI do innego wiêkszego obiektu
        wyœwietlanie elipsy poruszania siê cia³
*/