#include <iostream>
#include <windows.h>
#include <vector>
using namespace std;

enum class Color { Black, Red, Brown, White };
const int HEIGHT = 7, WIDTH = 9;
HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

int main_field[HEIGHT][WIDTH] =
{
	{1, 1, 0, 1, 0, 1, 1, 1, 1},
	{0, 0, 0, 1, 0, 1, 0, 0, 0},
	{1, 0, 0, 0, 0, 1, 0, 0, 0},
	{0, 0, 1, 0, 0, 0, 0, 0, 1},
	{0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 1, 1, 1},
	{1, 0, 0, 0, 1, 1, 1, 1, 1},
};

struct Ship {
	int deck = 0;
	bool horizontal = true;
	int x = 0, y = 0;

	Ship() {}

	Ship(int deck, int x, int y, bool horizontal) {
		this->deck = deck;
		this->x = x;
		this->y = y;
		this->horizontal = horizontal;
	}
};

void set_color(Color text, Color background) {
	SetConsoleTextAttribute(hStdOut, (WORD)(((int)background << 4) | (int)text));
}


void set_cursor(int x, int y) {
	COORD myCoords = { (SHORT)x,(SHORT)y };
	SetConsoleCursorPosition(hStdOut, myCoords);
}


void draw_field(int mas[HEIGHT][WIDTH], int x) {
	for (int i = 0; i < HEIGHT; i++)
	{
		int y = 1 + 2 * i;
		set_cursor(x, y);

		for (int g = 0; g < WIDTH; g++)
		{
			if (mas[i][g] == 1)
			{
				set_color(Color::Black, Color::Black);
			}
			else if (mas[i][g] >= 4)
			{
				set_color(Color::Red, Color::Red);
			}
			else
			{
				set_color(Color::White, Color::White);
			}
			cout << "++++";
			set_cursor(x + 4 * g, y + 1);
			cout << "++++";
			set_cursor(x + 4 * g + 4, y);
		}
		cout << "\n";
	}
}


vector<Ship> find_best_positions(int deck) {
	vector<Ship> ships;
	int count_empty_cells = 0, min_empty_cells = 14;
	boolean check = true;

	for (int x = 0; x < WIDTH - deck; x++) {
		for (int y = 0; y < HEIGHT; y++) {
			for (int i = 0; i < deck + 2 && check; i++) {
				for (int j = 0; j < 3 && check; j++) {
					if (0 <= i + x - 1 && i + x - 1 < WIDTH && 0 <= j + y - 1 && j + y - 1 < HEIGHT && !(j == 1 && i > 0 && i < deck + 1) &&
						main_field[j + y - 1][i + x - 1] == 0) {
						count_empty_cells++;
					}
					if (count_empty_cells > min_empty_cells || 
						(j == 1 && i > 0 && i < deck + 1 && main_field[j + y - 1][i + x - 1] != 0) ||
						((0 <= i + x - 1 && i + x - 1 < WIDTH && 0 <= j + y - 1 && j + y - 1 < HEIGHT) && main_field[j + y - 1][i + x - 1] >= 4)) {
						check = false;
					}
				}
			}
			if (check) {
				if (count_empty_cells < min_empty_cells) {
					min_empty_cells = count_empty_cells;
					ships.clear();
				}
				ships.push_back(Ship(deck, x, y, true));
			}
			count_empty_cells = 0;
			check = true;
		}
	}
	
	for (int x = 0; x < WIDTH; x++) {
		for (int y = 0; y < HEIGHT - deck; y++) {
			for (int i = 0; i < 3 && check; i++) {
				for (int j = 0; j < deck + 2 && check; j++) {
					if (0 <= i + x - 1 && i + x - 1 < WIDTH && 0 <= j + y - 1 && j + y - 1 < HEIGHT && !(i == 1 && j > 0 && j < deck + 1) &&
						main_field[j + y - 1][i + x - 1] == 0) {
						count_empty_cells++;
					}
					if (count_empty_cells > min_empty_cells || 
						(i == 1 && j > 0 && j < deck + 1 && main_field[j + y - 1][i + x - 1] != 0) ||
						((0 <= i + x - 1 && i + x - 1 < WIDTH && 0 <= j + y - 1 && j + y - 1 < HEIGHT) && main_field[j + y - 1][i + x - 1] >= 4)) {
						check = false;
					}
				}
			}
			if (check) {
				if (count_empty_cells < min_empty_cells) {
					min_empty_cells = count_empty_cells;
					ships.clear();
				}
				ships.push_back(Ship(deck, x, y, false));
			}
			count_empty_cells = 0;
			check = true;
		}
	}
	
	return ships;
}


vector<Ship> find_all_positions(int deck) {
	vector<Ship> ships;
	boolean check = true;

	for (int x = 0; x < WIDTH - deck + 1; x++) {
		for (int y = 0; y < HEIGHT; y++) {
			for (int i = 0; i < deck + 2 && check; i++) {
				for (int j = 0; j < 3 && check; j++) {
					if ((j == 1 && i > 0 && i < deck + 1 && main_field[j + y - 1][i + x - 1] != 0) ||
						((0 <= i + x - 1 && i + x - 1 < WIDTH && 0 <= j + y - 1 && j + y - 1 < HEIGHT) && main_field[j + y - 1][i + x - 1] >= 4)) {
						check = false;
					}
				}
			}
			if (check) {
				ships.push_back(Ship(deck, x, y, true));
			}
			check = true;
		}
	}

	for (int x = 0; x < WIDTH; x++) {
		for (int y = 0; y < HEIGHT - deck + 1; y++) {
			for (int i = 0; i < 3 && check; i++) {
				for (int j = 0; j < deck + 2 && check; j++) {
					if ((i == 1 && j > 0 && j < deck + 1 && main_field[j + y - 1][i + x - 1] != 0) ||
						((0 <= i + x - 1 && i + x - 1 < WIDTH && 0 <= j + y - 1 && j + y - 1 < HEIGHT) && main_field[j + y - 1][i + x - 1] >= 4)) {
						check = false;
					}
				}
			}
			if (check) {
				ships.push_back(Ship(deck, x, y, false));
			}
			check = true;
		}
	}

	return ships;
}


void draw_ship(Ship ship) {
	if (ship.horizontal == true) {
		for (int i = ship.x; i < ship.x + ship.deck; i++) {
			main_field[ship.y][i] = 4;
		}
	}
	else {
		for (int i = ship.y; i < ship.y + ship.deck; i++) {
			main_field[i][ship.x] = 4;
		}
	}
	//draw_field(main_field, 10);
}


void delete_ship(Ship ship) {
	if (ship.horizontal == true) {
		for (int i = ship.x; i < ship.x + ship.deck; i++) {
			main_field[ship.y][i] = 0;
		}
	}
	else {
		for (int i = ship.y; i < ship.y + ship.deck; i++) {
			main_field[i][ship.x] = 0;
		}
	}
	//draw_field(main_field, 10);
}


int change_indexes(int pos, vector<int> &ship_indexes, vector<vector<Ship>> vectors) {
	static int i;
	i = pos;
	if (ship_indexes[pos] < vectors[pos].size() - 1) ship_indexes[pos]++;
	else {
		ship_indexes[pos] = 0;
		i--;
		if (pos > 0) {
			delete_ship(vectors[pos - 1][ship_indexes[pos - 1]]);
			change_indexes(pos - 1, ship_indexes, vectors);
		}
		else return -1;
	}
	return i;
}


boolean ship_can_be_placed(Ship ship) {
	if (ship.horizontal == true) {
		for (int i = 0; i < ship.deck + 2; i++) {
			for (int j = 0; j < 3; j++) {
				if ((j == 1 && i > 0 && i < ship.deck + 1 && main_field[j + ship.y - 1][i + ship.x - 1] != 0) ||
					((0 <= i + ship.x - 1 && i + ship.x - 1 < WIDTH && 0 <= j + ship.y - 1 && j + ship.y - 1 < HEIGHT) && main_field[j + ship.y - 1][i + ship.x - 1] >= 4)) {
					return false;
				}
			}
		}
	}
	else {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < ship.deck + 2; j++) {
				if ((i == 1 && j > 0 && j < ship.deck + 1 && main_field[j + ship.y - 1][i + ship.x - 1] != 0) ||
					((0 <= i + ship.x - 1 && i + ship.x - 1 < WIDTH && 0 <= j + ship.y - 1 && j + ship.y - 1 < HEIGHT) && main_field[j + ship.y - 1][i + ship.x - 1] >= 4)) {
					return false;
				}
			}
		}
	}
	return true;
}


boolean is_possible_to_place_one_deck_ships() {
	vector<Ship> arr;
	int count = 0, num = 0;
	boolean check = true;
	stop: while (num < 4) {
		for (int x = 0; x < WIDTH; x++) {
			for (int y = 0; y < HEIGHT; y++) {
				for (int i = 0; i < 3 && check; i++) {
					for (int j = 0; j < 3 && check; j++) {
						if ((j == 1 && i == 1 && main_field[y][x] != 0) ||
							((0 <= i + x - 1 && i + x - 1 < WIDTH && 0 <= j + y - 1 && j + y - 1 < HEIGHT) && main_field[j + y - 1][i + x - 1] >= 4)) {
							check = false;
						}
					}
				}
				if (check) {
					main_field[y][x] = 4;
					//draw_field(main_field, 10);
					num++;
					count++;
					arr.push_back(Ship(1, x, y, false));
					goto stop;
				}
				check = true;
			}
		}
		num++;
	}
	if (count == 4) return true;
	else {
		for (int i = 0; i < count; i++) {
			main_field[arr[i].y][arr[i].x] = 0;
			//draw_field(main_field, 10);
		}
		arr.clear();
		return false;
	}
}


boolean generate() {
	vector<Ship> vect4 = find_best_positions(4);
	if (vect4.size() == 0) return false;
	vector<Ship> vect3 = find_all_positions(3);
	vector<Ship> vect2 = find_all_positions(2);
	vector<vector<Ship>> vectors = { vect4, vect3, vect3, vect2, vect2, vect2 };;
	vector<int> ship_sizes = { 4, 3, 3, 2, 2, 2 };;
	vector<int> ship_indexes = { 0, 0, 1, 0, 1, 2 };;
	boolean ship_is_not_placed = true, all_ships_are_not_placed = true;
	int i = 0;

	do {
		for (; i < ship_indexes.size(); i++) {
			if (i != 0 && ship_sizes[i] == ship_sizes[i - 1] && ship_indexes[i] < ship_indexes[i - 1]) {
				ship_indexes[i] = ship_indexes[i - 1];
				i = change_indexes(i, ship_indexes, vectors);
			}

			while (ship_is_not_placed) {
				if (ship_can_be_placed(vectors[i][ship_indexes[i]])) {
					draw_ship(vectors[i][ship_indexes[i]]);
					ship_is_not_placed = false;
				}
				else {
					i = change_indexes(i, ship_indexes, vectors);
					if (i == -1) return false;
				}
			}
			ship_is_not_placed = true;
		}

		if (is_possible_to_place_one_deck_ships()) {
			all_ships_are_not_placed = false;
		} 
		else {
			delete_ship(vectors[i - 1][ship_indexes[i - 1]]);
			i = change_indexes(ship_indexes.size() - 1, ship_indexes, vectors);
		}
	} while (all_ships_are_not_placed);

	vect4.clear();
	vect3.clear();
	vect2.clear();
	vectors.clear();
	ship_sizes.clear();
	ship_indexes.clear();
	return true;
}


int main() {
	draw_field(main_field, 10);
	generate();
	draw_field(main_field, 60);
}