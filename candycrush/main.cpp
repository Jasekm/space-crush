#include "SFML/Graphics.hpp"
#include <iostream>
#include "Menu.h"
#include <time.h>
#include <fstream>
#include <iomanip>
#include <windows.h>

using namespace sf;

int ts = 54; 
Vector2i offset(48, 24);

struct piece
{
    int x, y, col, row, kind, match, alpha;
    piece() { match = 0; alpha = 255; }
} grid[10][10];

void swap(piece p1, piece p2)
{
    std::swap(p1.col, p2.col);
    std::swap(p1.row, p2.row);

    grid[p1.row][p1.col] = p1;
    grid[p2.row][p2.col] = p2;
}

void zapis(int czas) {
    using namespace std;
    fstream zapis;
    zapis.open("wyniki.txt", ios::out | ios::app);
    zapis << czas << endl;
    zapis.close();
}

int game()
{
    srand(time(0));

    RenderWindow app(VideoMode(740, 480), "Space crush");
    app.setFramerateLimit(60);

    Texture t1, t2,t3;
    t1.loadFromFile("images/background.png");
    t2.loadFromFile("images/gems.png");
    t3.loadFromFile("images/end.png");

    Clock zegar;
    Time czas;
    Sprite background(t1), gems(t2),end(t3);

    for (int i = 1; i <= 8; i++)
        for (int j = 1; j <= 8; j++)
        {
            grid[i][j].kind = rand() % 3;
            grid[i][j].col = j;
            grid[i][j].row = i;
            grid[i][j].x = j * ts;
            grid[i][j].y = i * ts;
        }

    int x0, y0, x, y; int click = 0; Vector2i pos;
    bool isSwap = false, isMoving = false;
    zegar.restart();
    int punkty = 0;
    int check =0 ;
    
    while (app.isOpen())
    {
        if (check == 0) {
            Event e;
            while (app.pollEvent(e))
            {
                if (e.type == Event::Closed)
                    app.close();

                if (e.type == Event::MouseButtonPressed)
                    if (e.key.code == Mouse::Left)
                    {
                        if (!isSwap && !isMoving) click++;
                        pos = Mouse::getPosition(app) - offset;
                    }
            }

            
            if (click == 1)
            {
                x0 = pos.x / ts + 1; 
                y0 = pos.y / ts + 1;
            }
            if (click == 2)
            {
                x = pos.x / ts + 1;
                y = pos.y / ts + 1;
                if (abs(x - x0) + abs(y - y0) == 1)
                {
                    swap(grid[y0][x0], grid[y][x]); isSwap = 1; click = 0;

                }
                else click = 1;
            }

            
            for (int i = 1; i <= 8; i++)
                for (int j = 1; j <= 8; j++)
                {
                    if (grid[i][j].kind == grid[i + 1][j].kind)
                        if (grid[i][j].kind == grid[i - 1][j].kind) {
                            for (int n = -1; n <= 1; n++) grid[i + n][j].match++;
                        }
                    if (grid[i][j].kind == grid[i][j + 1].kind)
                        if (grid[i][j].kind == grid[i][j - 1].kind)
                            for (int n = -1; n <= 1; n++) grid[i][j + n].match++;

                }

            
            isMoving = false;
            for (int i = 1; i <= 8; i++)
                for (int j = 1; j <= 8; j++)
                {
                    piece& p = grid[i][j];
                    
                    int dx, dy;
                    for (int n = 0; n < 4; n++)  
                    {
                        dx = p.x - p.col * ts;
                        dy = p.y - p.row * ts;
                        if (dx) p.x -= dx / abs(dx);
                        if (dy) p.y -= dy / abs(dy);
                    }
                    if (dx || dy) isMoving = 1;
                }

           
            if (!isMoving)
                for (int i = 1; i <= 8; i++)
                    for (int j = 1; j <= 8; j++)
                        if (grid[i][j].match && grid[i][j].alpha > 10)
                             { grid[i][j].alpha -= 10; isMoving = true; }

            
            int score = 0;
            for (int i = 1; i <= 8; i++)
                for (int j = 1; j <= 8; j++) {
                    score += grid[i][j].match;
                }


            
            if (isSwap && !isMoving)
            {
                if (!score) swap(grid[y0][x0], grid[y][x]); isSwap = 0;
            }

            
            if (!isMoving)
            {
                for (int i = 8; i > 0; i--)
                    for (int j = 1; j <= 8; j++)
                        if (grid[i][j].match)
                            for (int n = i; n > 0; n--)
                                if (!grid[n][j].match)
                                {
                                    swap(grid[n][j], grid[i][j]);

                                    break;
                                };


                for (int j = 1; j <= 8; j++)
                    for (int i = 8, n = 0; i > 0; i--)
                        if (grid[i][j].match)
                        {
                            grid[i][j].kind = rand() % 7;
                            grid[i][j].y = -ts * n++;
                            grid[i][j].match = 0;
                            grid[i][j].alpha = 255;
                            punkty += 10;

                        }
            }


            
            app.draw(background);

            for (int i = 1; i <= 8; i++)
                for (int j = 1; j <= 8; j++)
                {
                    piece p = grid[i][j];
                    gems.setTextureRect(IntRect(p.kind * 49, 0, 49, 49));
                    gems.setColor(Color(255, 255, 255, p.alpha));
                    gems.setPosition(p.x, p.y);
                    gems.move(offset.x - ts, offset.y - ts);
                    app.draw(gems);
                }
            sf::Font font;
            if (!font.loadFromFile("Arial.ttf"))
            {

            }
            czas = zegar.getElapsedTime();
            int i = czas.asSeconds();

            std::string std = std::to_string(i);
            sf::Text pokaz_czas;
            pokaz_czas.setString(std);
            pokaz_czas.setCharacterSize(30);
            pokaz_czas.setPosition(600, 370);
            pokaz_czas.setFillColor(sf::Color::Red);
            pokaz_czas.setFont(font);
            app.draw(pokaz_czas);
            if (i < 5) {
                punkty = 0;
            }

            std::string std1 = std::to_string(punkty);
            sf::Text wynik;
            wynik.setString(std1);
            wynik.setCharacterSize(30);
            wynik.setPosition(600, 213);
            wynik.setFillColor(sf::Color::Red);
            wynik.setFont(font);
            app.draw(wynik);

            app.display();
            if (punkty > 1000) {
                zapis(i);
                check = 1;
                app.clear();
                app.draw(end);
                app.display();
                Sleep(2000);
                return 0;
            }
        }
    }
   
    return 0;
}


int main()
{
	sf::RenderWindow app(sf::VideoMode(740, 480), "Space crush");
    Texture t1;
    t1.loadFromFile("images/menu2.png");

    Menu menu(app.getSize().x, app.getSize().y);
   Sprite background(t1);

	while (app.isOpen())
	{
		sf::Event event;

		while (app.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::KeyReleased:
				switch (event.key.code)
				{
				case sf::Keyboard::Up:
					menu.MoveUp();
					break;

				case sf::Keyboard::Down:
					menu.MoveDown();
					break;

				case sf::Keyboard::Return:
					switch (menu.GetPressedItem())
					{
					case 0:
						game();
						break;
					case 1:
                        
                       
						break;
					case 2:
						app.close();
						break;
					}

					break;
				}

				break;
			case sf::Event::Closed:
				app.close();

				break;

			}
		}

		app.clear();
        app.draw(background);
		menu.draw(app);
		app.display();
	}
}



