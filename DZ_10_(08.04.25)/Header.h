#include <iostream>
#include <windows.h> // COORD HANDLE SetConsoleTextAttribute SetConsoleCursorPosition
#include <conio.h> // _getch
using namespace std;

enum GameObject : short { HALL, WALL, COIN, ENEMY };
enum Color : short {
	BLACK, DARKBLUE, DARKGREEN, TURQUOISE, DARKRED,
	PURPLE, DARKYELLOW, GREY, DARKGREY, BLUE, GREEN,
	CYAN, RED, PINK, YELLOW, WHITE
};
enum Key : short {
	LEFT = 75, RIGHT = 77, UP = 72, DOWN = 80,
	ENTER = 13, SPACE = 32, ESCAPE = 27, BACKSPACE = 8
};


void CURSOR_HIDE(HANDLE h);
void MODEL(const int HEIGHT, const int WIDTH, int map[25][65]);
void VIEW(const int HEIGHT, const int WIDTH, int map[25][65], HANDLE h);
void Main_Character(const int WIDTH, HANDLE h, COORD hero, COORD coins_info, int* coins_collected);
void Game_Engine(int map[25][65], COORD hero, HANDLE h, int* coins_collected, const int WIDTH);



void CURSOR_HIDE(HANDLE h) {
	CONSOLE_CURSOR_INFO cursor;
	cursor.bVisible = false;
	cursor.dwSize = 100; // 1-100
	SetConsoleCursorInfo(h, &cursor);
}

void MODEL(const int HEIGHT, const int WIDTH, int map[25][65]) {
	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			map[y][x] = rand() % 4; // 0 1 2 3

			// рамка по краях
			if (x == 0 || y == 0 || x == WIDTH - 1 || y == HEIGHT - 1)
				map[y][x] = GameObject::WALL;

			// отвори для входу і виходу
			if (x == 0 && y == 2 ||
				x == 1 && y == 2 ||
				x == 2 && y == 2 ||

				x == WIDTH - 1 && y == HEIGHT - 3 ||
				x == WIDTH - 2 && y == HEIGHT - 3 ||
				x == WIDTH - 3 && y == HEIGHT - 3)
				map[y][x] = GameObject::HALL;

			if (map[y][x] == GameObject::ENEMY) {
				int r = rand() % 10; // 0 1 2 3 4 5 6 7 8 9
				if (r != 0) {
					map[y][x] = GameObject::HALL;
				}
			}
		}
	}
}


void VIEW(const int HEIGHT, const int WIDTH, int map[25][65], HANDLE h) {
	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			switch (map[y][x]) {
			case GameObject::HALL: // 0
				// як показати коридор
				SetConsoleTextAttribute(h, Color::BLACK);
				cout << " ";
				break;
			case GameObject::WALL: // 1
				SetConsoleTextAttribute(h, Color::DARKGREEN);
				// cout << "#";
				cout << (char)178;
				break;
			case GameObject::COIN:
				SetConsoleTextAttribute(h, Color::YELLOW);
				cout << ".";
				break;
			case GameObject::ENEMY:
				SetConsoleTextAttribute(h, Color::RED);
				cout << "O";
				break;
				// case GameObject::SMTH_ELSE:
					// як показати щось інше
				//	break;
			}

		}
		cout << "\n";
	}
}


void Main_Character(const int WIDTH, HANDLE h, COORD hero, COORD coins_info, int* coins_collected) {
	SetConsoleCursorPosition(h, hero);
	SetConsoleTextAttribute(h, Color::BLUE);
	cout << "O";

	SetConsoleCursorPosition(h, coins_info);
	SetConsoleTextAttribute(h, Color::DARKYELLOW);
	cout << "COINS: ";
	SetConsoleTextAttribute(h, Color::YELLOW);
	cout << *coins_collected;
}


void Game_Engine(int map[25][65], COORD hero, HANDLE h, int* coins_collected, const int WIDTH) {
	while (true) {
		int code = _getch(); // get character
		if (code == 224) {
			code = _getch();
		}

		COORD old_position = hero;
		bool has_been_moved = false; // false - пересування не було

		// cout << code << "\n";
		switch (code) {
		case Key::LEFT:
			if (hero.X > 0 && map[hero.Y][hero.X - 1] != GameObject::WALL) {
				has_been_moved = true;
				hero.X--;
			}
			break;
		case Key::RIGHT:
			if (map[hero.Y][hero.X + 1] != GameObject::WALL) {
				has_been_moved = true;
				hero.X++;
			}
			break;
		case Key::UP:
			if (map[hero.Y - 1][hero.X] != GameObject::WALL) {
				has_been_moved = true;
				hero.Y--;
			}
			break;
		case Key::DOWN:
			if (map[hero.Y + 1][hero.X] != GameObject::WALL) {
				has_been_moved = true;
				hero.Y++;
			}
			break;
		}

		if (has_been_moved) {
			// стирання ГГ в "старих" координатах
			SetConsoleCursorPosition(h, old_position);
			SetConsoleTextAttribute(h, Color::BLACK);
			cout << " ";
			// перемальовка ГГ в нових координатах
			SetConsoleCursorPosition(h, hero);
			SetConsoleTextAttribute(h, Color::BLUE);
			cout << "O";
		}

		// приклад перевірки на перетинання з якимось об'єктом в лабірінті
		if (map[hero.Y][hero.X] == GameObject::COIN) {
			// cout << "COIN";
			(*coins_collected)++;
			map[hero.Y][hero.X] = GameObject::HALL;
			COORD coins_info;
			coins_info.X = WIDTH + 1;
			coins_info.Y = 0;
			SetConsoleCursorPosition(h, coins_info);
			SetConsoleTextAttribute(h, Color::DARKYELLOW);
			cout << "COINS: ";
			SetConsoleTextAttribute(h, Color::YELLOW);
			cout << *coins_collected;
		}

		if (map[hero.Y][hero.X] == GameObject::ENEMY) {
			cout << "ENEMY";
		}

	}
}
