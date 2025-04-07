#include "Menu.h"


Menu::Menu(float width, float height)
{
	if (!font.loadFromFile("arial.ttf"))
	{
		
	}

	menu[0].setFont(font);
	menu[0].setFillColor(sf::Color::Red);
	menu[0].setString("Play");
	menu[0].setCharacterSize(25);
	menu[0].setPosition(sf::Vector2f(360, 169));

	menu[1].setFont(font);
	menu[1].setFillColor(sf::Color::Black);
	menu[1].setString("Scoreboard");
	menu[1].setCharacterSize(25);
	menu[1].setPosition(sf::Vector2f(325,  233));

	menu[2].setFont(font);
	menu[2].setFillColor(sf::Color::Black);
	menu[2].setString("Exit");
	menu[2].setCharacterSize(25);
	menu[2].setPosition(sf::Vector2f(360,  297));

	selectedItemIndex = 0;
}


Menu::~Menu()
{
}

void Menu::draw(sf::RenderWindow &window)
{

	for (int i = 0; i < MAX_NUMBER_OF_ITEMS; i++)
	{
		
		window.draw(menu[i]);
	}
}

void Menu::MoveUp()
{
	if (selectedItemIndex - 1 >= 0)
	{
		menu[selectedItemIndex].setFillColor(sf::Color::Black);
		selectedItemIndex--;
		menu[selectedItemIndex].setFillColor(sf::Color::Red);
	}
}

void Menu::MoveDown()
{
	if (selectedItemIndex + 1 < MAX_NUMBER_OF_ITEMS)
	{
		menu[selectedItemIndex].setFillColor(sf::Color::Black);
		selectedItemIndex++;
		menu[selectedItemIndex].setFillColor(sf::Color::Red);
	}
}