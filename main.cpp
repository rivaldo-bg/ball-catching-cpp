#include <SFML/Graphics.hpp>
#include <algorithm>
#include <string>

int main()
{
    sf::RenderWindow window(sf::VideoMode({400, 600}), "Catch the Ball!");
    window.setFramerateLimit(60);

    // Ball
    sf::CircleShape ball(15.f);
    ball.setFillColor(sf::Color::Green);
    ball.setOrigin({15.f, 15.f});
    sf::Vector2f ballPos(200.f, 50.f);
    sf::Vector2f ballVel(150.f, 200.f);

    // Paddle
    sf::RectangleShape paddle(sf::Vector2f(100.f, 15.f));
    paddle.setFillColor(sf::Color(0, 200, 0));
    paddle.setOrigin({50.f, 7.5f});

    // Score & state
    int score = 0;
    bool gameOver = false;

    sf::Font font;
    (void)font.openFromFile("C:/Windows/Fonts/arial.ttf");

    sf::Text scoreText(font, "Score: 0", 24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition({10.f, 10.f});

    sf::Text gameOverText(font, "Game Over!\nPress R to restart", 30);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setPosition({60.f, 250.f});

    sf::Clock clock;

    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (gameOver && event->is<sf::Event::KeyPressed>())
            {
                auto key = event->getIf<sf::Event::KeyPressed>()->code;
                if (key == sf::Keyboard::Key::R)
                {
                    ballPos = {200.f, 50.f};
                    ballVel = {150.f, 200.f};
                    score = 0;
                    gameOver = false;
                    scoreText.setString("Score: 0");
                }
            }
        }

        if (!gameOver)
        {
            // Move paddle with mouse
            float mouseX = static_cast<float>(sf::Mouse::getPosition(window).x);
            float paddleX = std::clamp(mouseX, 50.f, 350.f);
            paddle.setPosition({paddleX, 550.f});

            // Move ball
            ballPos += ballVel * dt;

            // Wall bounces (left/right)
            if (ballPos.x - 15.f < 0.f)   { ballPos.x = 15.f;  ballVel.x =  std::abs(ballVel.x); }
            if (ballPos.x + 15.f > 400.f)  { ballPos.x = 385.f; ballVel.x = -std::abs(ballVel.x); }

            // Ceiling bounce
            if (ballPos.y - 15.f < 0.f)   { ballPos.y = 15.f;  ballVel.y =  std::abs(ballVel.y); }

            // Paddle collision
            sf::FloatRect ballBounds({ballPos.x - 15.f, ballPos.y - 15.f}, {30.f, 30.f});
            if (ballBounds.findIntersection(paddle.getGlobalBounds()) && ballVel.y > 0)
            {
                ballVel.y = -std::abs(ballVel.y);
                ballVel.x *= 1.05f;
                ballVel.y *= 1.05f;
                score++;
                scoreText.setString("Score: " + std::to_string(score));
            }

            // Fell past paddle
            if (ballPos.y > 620.f)
                gameOver = true;

            ball.setPosition(ballPos);
        }

        window.clear(sf::Color(20, 20, 20));
        window.draw(ball);
        window.draw(paddle);
        window.draw(scoreText);
        if (gameOver)
            window.draw(gameOverText);
        window.display();
    }
}
