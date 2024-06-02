#include <SFML/Graphics.hpp>
#include <deque>
#include <cstdlib>
#include <ctime>

const int gridSize = 20;
const int gridWidth = 30;
const int gridHeight = 20;

enum Direction { Up, Down, Left, Right };

struct SnakeSegment
{
    int x, y;
    SnakeSegment(int x, int y) : x(x), y(y) {}
};

class Snake
{
public:
    Snake()
    {
        segments.push_back(SnakeSegment(gridWidth / 2, gridHeight / 2));
        segments.push_back(SnakeSegment(gridWidth / 2 - 1, gridHeight / 2));
        segments.push_back(SnakeSegment(gridWidth / 2 - 2, gridHeight / 2));
        dir = Right;
    }

    void move()
    {
        for (int i = segments.size() - 1; i > 0; --i) {
            segments[i] = segments[i - 1];
        }

        switch (dir)
        {
            case Up:    segments[0].y -= 1; break;
            case Down:  segments[0].y += 1; break;
            case Left:  segments[0].x -= 1; break;
            case Right: segments[0].x += 1; break;
        }
    }

    void grow()
    {
        segments.push_back(segments.back());
    }

    bool checkCollision()
    {
        if (segments[0].x < 0 || segments[0].x >= gridWidth || segments[0].y < 0 || segments[0].y >= gridHeight)
            return true;
        for (int i = 1; i < segments.size(); ++i) {
            if (segments[0].x == segments[i].x && segments[0].y == segments[i].y)
                return true;
        }
        return false;
    }

    void setDirection(Direction newDir)
    {
        if ((dir == Up && newDir != Down) || (dir == Down && newDir != Up) ||
            (dir == Left && newDir != Right) || (dir == Right && newDir != Left)) {
            dir = newDir;
        }
    }

    std::deque<SnakeSegment>& getSegments()
    {
        return segments;
    }

private:
    std::deque<SnakeSegment> segments;
    Direction dir;
};

class Game
{
public:
    Game() : window(sf::VideoMode(gridWidth * gridSize, gridHeight * gridSize), "Snake Game"), snake(), foodX(0), foodY(0) {
        window.setFramerateLimit(10);
        std::srand(std::time(nullptr));
        placeFood();
    }

    void run()
    {
        while (window.isOpen()) {
            processEvents();
            update();
            render();
        }
    }

private:
    sf::RenderWindow window;
    Snake snake;
    int foodX, foodY;

    void processEvents()
    {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                    case sf::Keyboard::Z: snake.setDirection(Up); break;
                    case sf::Keyboard::S: snake.setDirection(Down); break;
                    case sf::Keyboard::Q: snake.setDirection(Left); break;
                    case sf::Keyboard::D: snake.setDirection(Right); break;
                    default: break;
                }
            }
        }
    }

    void update()
    {
        snake.move();
        if (snake.getSegments().front().x == foodX && snake.getSegments().front().y == foodY) {
            snake.grow();
            placeFood();
        }
        if (snake.checkCollision()) {
            window.close();
        }
    }

    void render()
    {
        window.clear();
        for (auto& segment : snake.getSegments()) {
            sf::RectangleShape rectangle(sf::Vector2f(gridSize, gridSize));
            rectangle.setFillColor(sf::Color::Green);
            rectangle.setPosition(segment.x * gridSize, segment.y * gridSize);
            window.draw(rectangle);
        }
        sf::RectangleShape rectangle(sf::Vector2f(gridSize, gridSize));
        rectangle.setFillColor(sf::Color::Red);
        rectangle.setPosition(foodX * gridSize, foodY * gridSize);
        window.draw(rectangle);

        window.display();
    }

    void placeFood()
    {
        foodX = std::rand() % gridWidth;
        foodY = std::rand() % gridHeight;
    }
};

int main()
{
    Game game;
    game.run();
    return 0;
}
