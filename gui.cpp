#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>

#include "game.h"
#include "gui.h"

#define WIN_WIDTH 800
#define WIN_HEIGHT 800

int main()
{
    sf::RenderWindow window(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "My window");
    window.setFramerateLimit(30);
    window.setKeyRepeatEnabled(false);

    //std::unique_ptr<WindowHandler> handler = std::make_unique<MenuHandler>();
    std::unique_ptr<WindowHandler> handler = std::make_unique<GameHandler>(window);

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
            
            handler->handleEvent(event);
        }


        window.clear(sf::Color::White);
        handler->draw();
        window.display();

        if (handler->switchHandler())
        {
            handler.reset(handler->getNextHandler());
        }

        //std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    return 0;
}

MenuHandler::MenuHandler(sf::RenderWindow& window) : WindowHandler(window)
{
    if (!font.loadFromFile("trim.ttf"))
    {
        throw "Error";
    }
    
}

void MenuHandler::handleEvent(sf::Event& event)
{
    
}

void MenuHandler::draw()
{
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(50);
    text.setFillColor(sf::Color::Black);
    text.setString("Ultimate Tic-Tac-Toe");
    window.draw(text);
}

void GameHandler::handleEvent(sf::Event& event)
{
    switch (event.type)
    {
        case sf::Event::KeyPressed:
            switch (event.key.code)
            {
                case sf::Keyboard::Left:
                    changeCoord(activeCoord.outer.col, activeCoord.inner.col, -1);
                    break;
                case sf::Keyboard::Right:
                    changeCoord(activeCoord.outer.col, activeCoord.inner.col, 1);
                    break;
                case sf::Keyboard::Up:
                    changeCoord(activeCoord.outer.row, activeCoord.inner.row, -1);
                    break;
                case sf::Keyboard::Down:
                    changeCoord(activeCoord.outer.row, activeCoord.inner.row, 1);
                    break;
                case sf::Keyboard::Enter:
                    move();
                    break;

            }
            break;
        default:
            break;
    }
}

void GameHandler::changeCoord(int& outerCoord, int& innerCoord, int change)
{
    int newInnerCoord = innerCoord + change;
    if (0 <= newInnerCoord && newInnerCoord <= 2)
    {
        innerCoord = newInnerCoord;
    }
    else if (game.getNextBoard() == anyBoard)
    {
        int newOuterCoord = outerCoord + change;
        if (0 <= newOuterCoord && newOuterCoord <= 2)
        {
            outerCoord = newOuterCoord;
            innerCoord -= 2 * change;
        }
    }
}

void GameHandler::move()
{
    bool moved = game.move(activeCoord);
    if (moved && game.getNextBoard() != anyBoard)
    {
        activeCoord = {game.getNextBoard(), {1, 1}};
    }
}

#define GAME_SIZE 800
#define LARGE_BOARD_LINE 10
#define SMALL_BOARD_LINE 5
#define CELL_PADDING 10
#define LETTER_WIDTH 10

const sf::Color LINE_COLOR = sf::Color::Black;
const sf::Color X_COLOR = sf::Color::Red;
const sf::Color O_COLOR = sf::Color::Blue;
const sf::Color ACTIVE_COLOR = sf::Color(200, 200, 200);

void GameHandler::draw()
{
    float third = GAME_SIZE / 3;

    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            drawSmallBoard({row, col}, col * third + CELL_PADDING, row * third + CELL_PADDING, third - 2 *  CELL_PADDING);
        }
    }

    drawBoardLines(0, 0, GAME_SIZE, LARGE_BOARD_LINE);
}

void GameHandler::drawSmallBoard(Coord boardCoord, float x, float y, float size)
{
    SmallBoard board = game.getBoard(boardCoord);
    float third = size / 3;

    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            CoordPair curCoord = {boardCoord, {row, col}};
            bool active = activeCoord == curCoord;
            drawCell(board.getCell(row, col), x + col * third, y + row * third, third, active);
        }
    }

    drawBoardLines(x, y, size, SMALL_BOARD_LINE);
}

void GameHandler::drawCell(CellState state, float x, float y, float size, bool active)
{
    if (active)
    {
        sf::RectangleShape rect(sf::Vector2f(size, size));
        rect.setPosition(x, y);
        rect.setFillColor(ACTIVE_COLOR);
        window.draw(rect);
    }

    if (state == X)
    {
        float lowX = x + CELL_PADDING;
        float highX = x + size - CELL_PADDING;
        float lowY = y + CELL_PADDING;
        float highY = y + size - CELL_PADDING;

        drawLine(lowX, lowY, highX, highY, LETTER_WIDTH, X_COLOR);
        drawLine(lowX, highY, highX, lowY, LETTER_WIDTH, X_COLOR);
    }
    else if (state == O)
    {
        sf::CircleShape circle(size / 2 - CELL_PADDING);
        circle.setPosition(x + CELL_PADDING, y + CELL_PADDING);
        circle.setFillColor(sf::Color::Transparent);
        circle.setOutlineThickness(-LETTER_WIDTH);
        circle.setOutlineColor(O_COLOR);

        window.draw(circle);
    }
}

void GameHandler::drawBoardLines(float x, float y, float size, float lineWidth)
{
    float third = size / 3;
    sf::Color color = sf::Color::Black;

    drawLine(x + third, y, x + third, y + size, lineWidth, color);
    drawLine(x + 2 * third, y, x + 2 * third, y + size, lineWidth, color);
    drawLine(x, y + third, x + size, y + third, lineWidth, color);
    drawLine(x, y + 2 * third, x + size, y + 2 * third, lineWidth, color);
}

#define PI 3.14159265

void GameHandler::drawLine(float x1, float y1, float x2, float y2, float lineWidth, const sf::Color& color)
{
    float length = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
    float angle = atan2(y2 - y1, x2 - x1) * 180 / PI;

    sf::RectangleShape rect(sf::Vector2f(length, lineWidth));
    rect.setOrigin(0, lineWidth / 2);
    rect.setPosition(x1, y1);
    rect.setRotation(angle);
    rect.setFillColor(color);
    window.draw(rect);
}