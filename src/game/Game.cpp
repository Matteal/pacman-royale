#include <iostream>
#include <ctime>

#include "Game.h"

Game::Game() : _t(19, 21), _score(0), _lives(3)
{
}

void Game::init()
{
    _t = Terrain(30, 30);
    _t.hardcodeTerrain();

    start_time = new time_t();
    std::time(start_time);
    std::cout << "Bonjour, c'est le jeu";
}

void Game::update()
{
    std::cout << *start_time << std::endl;
}

void Game::render()
{
    _t.drawToTerminal();
}

void Game::end()
{
    std::cout << "C'est la fin du jeu." << std::endl;
    _t.~Terrain();
}

void Game::set_map(Terrain t) { _t = t; }

int Game::get_lives() { return _lives; }
int Game::get_score() { return _score; }
float Game::get_speed() { return _speed; }

void Game::set_lives(int lives) { _lives = lives; }
void Game::set_score(int score) { _score = score; }
void Game::set_speed(float speed) { _speed = speed; }
