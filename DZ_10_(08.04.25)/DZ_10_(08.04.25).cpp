#include "Header.h"

int main() {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	srand(time(0));
	rand();
	system("title Bomberman");

	// cursor hide
	CURSOR_HIDE(h);

	const int HEIGHT = 25;
	const int WIDTH = 65;
	int map[HEIGHT][WIDTH] = {};
	// ASCII Table
	//for (int i = 0; i < 256; i++)
	//{
	//	cout << (char)i << " - " << i << "\n";
	//}

	// MODEL (логіка заповнення масиву)

	MODEL(HEIGHT, WIDTH, map);

	///////////////////////////////////////////////////////////

	// VIEW (подання - як буде виглядати ігрова локація)
	VIEW(HEIGHT, WIDTH, map, h);

	////////////////////////////////////////////////////////////
	// показ ГГ

	COORD hero;
	hero.X = 0;
	hero.Y = 2;
	static int coins_collected = 0;
	COORD coins_info;
	coins_info.X = WIDTH + 1;
	coins_info.Y = 0;

	Main_Character(WIDTH, h, hero, coins_info, &coins_collected);

	// ігровий двигунчик
	Game_Engine(map, hero, h, &coins_collected, WIDTH);


	Sleep(INFINITE);
}
