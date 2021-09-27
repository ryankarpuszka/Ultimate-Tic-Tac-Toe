#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

#include "game.h"

class WindowHandler
{
protected:
    sf::RenderWindow& window;
    WindowHandler* nextHandler = nullptr;
public:
    WindowHandler(sf::RenderWindow& window) : window(window) {}
    virtual void handleEvent(sf::Event& event)=0;
    virtual void draw()=0;
    virtual bool switchHandler() {return nextHandler != nullptr;}
    WindowHandler* getNextHandler() {return nextHandler;}
};

class MenuHandler: public WindowHandler
{
    sf::Font font;

public:
    MenuHandler(sf::RenderWindow& window);
    virtual void handleEvent(sf::Event& event);
    virtual void draw();
};

class GameHandler: public WindowHandler
{
    Game game;
    CoordPair activeCoord = {{1, 1}, {1, 1}};

    void changeCoord(int& outerCoord, int& innerCoord, int change);
    void move();

    void drawSmallBoard(Coord boardCoord, float x, float y, float size);
    void drawCell(CellState state, float x, float y, float size, bool active);
    void drawBoardLines(float x, float y, float size, float lineWidth);
    void drawLine(float x1, float y1, float x2, float y2, float lineWidth, const sf::Color& color);
public:
    GameHandler(sf::RenderWindow& window) : WindowHandler(window) {};
    virtual void handleEvent(sf::Event& event);
    virtual void draw();
};