//
// Created by miki on 22.12.2020.
//

#include "GameProcess.h"

#include <SFML/Graphics.hpp>

[[noreturn]] void GameProcess::run()
{
    // create empty messages
    GameMes mes_in{};
    LogMes mes_out{};

    // setup the renderer and open the window
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Pointer");
    sf::CircleShape shape(5, 16);
    shape.setFillColor(sf::Color::Green);

    while (true)
    {
        sf::Event event{};
        while (window.pollEvent(event));

        // get a message
        auto* temp = &mes_in;
        game_q.receiveMes(temp);

        // store the time when the message has arrived
        auto end = std::chrono::system_clock::now();

        if (temp)
        {
#ifndef NDEBUG
            std::cout << "GameProcess (receiving): " << mes_in->id << ": " << mes_in->x << ' ' << mes_in->y << '\n';
#endif
            // create a message to send to the logger
            mes_out.id = mes_in.id;
            mes_out.begin = mes_in.timestamp;
            mes_out.end = end;

            // send the message
            log_q.sendMes(&mes_out);

            //update the display
            shape.setPosition(mes_in.x, mes_in.y);
            window.clear();
            window.draw(shape);
            window.display();
        }
    }
}