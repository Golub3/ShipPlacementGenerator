#include <iostream>
#include <vector>
#include <thread>
#include <stdlib.h>
using namespace std;

enum class Color { Black, Red, Brown, White };
vector<vector<int>> main_field =
{
	{1, 1, 0, 1, 0, 1, 1, 1, 1},
	{0, 0, 0, 1, 0, 1, 0, 0, 0},
	{1, 0, 0, 0, 0, 1, 0, 0, 0},
	{0, 0, 1, 0, 0, 0, 0, 0, 1},
	{0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 1, 1, 1},
	{1, 0, 0, 0, 1, 1, 1, 1, 1},
};
const int HEIGHT = 7, WIDTH = 9;

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


void draw_field(vector<vector<int>> mas) {
	for (int i = 0; i < HEIGHT; i++)
	{
		cout << (char)255;
		for (int g = 0; g < WIDTH; g++)
		{
			if (mas[i][g] == 1) {
				cout << (char)176;
				cout << (char)176;
			}
			else if (mas[i][g] == 4) {
				cout << (char)178;
				cout << (char)178;
			}
			else {
				cout << (char)255;
				cout << (char)255;
			}
		}
		cout << "\n";
	}
	cout << "\n";
	cout << "\n";
}


vector<Ship> find_best_positions(vector<vector<int>> field, int deck) {
	vector<Ship> ships;
	int count_empty_cells = 0, min_empty_cells = 14;
	bool check = true;

	for (int x = 0; x < WIDTH - deck; x++) {
		for (int y = 0; y < HEIGHT; y++) {
			for (int i = 0; i < deck + 2 && check; i++) {
				for (int j = 0; j < 3 && check; j++) {
					if (0 <= i + x - 1 && i + x - 1 < WIDTH && 0 <= j + y - 1 && j + y - 1 < HEIGHT && !(j == 1 && i > 0 && i < deck + 1) &&
						field[j + y - 1][i + x - 1] == 0) {
						count_empty_cells++;
					}
					if (count_empty_cells > min_empty_cells || 
						(j == 1 && i > 0 && i < deck + 1 && field[j + y - 1][i + x - 1] != 0) ||
						((0 <= i + x - 1 && i + x - 1 < WIDTH && 0 <= j + y - 1 && j + y - 1 < HEIGHT) && field[j + y - 1][i + x - 1] >= 4)) {
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
						field[j + y - 1][i + x - 1] == 0) {
						count_empty_cells++;
					}
					if (count_empty_cells > min_empty_cells || 
						(i == 1 && j > 0 && j < deck + 1 && field[j + y - 1][i + x - 1] != 0) ||
						((0 <= i + x - 1 && i + x - 1 < WIDTH && 0 <= j + y - 1 && j + y - 1 < HEIGHT) && field[j + y - 1][i + x - 1] >= 4)) {
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


vector<Ship> find_all_positions(vector<vector<int>> field, int deck, int h, int w) {
	vector<Ship> ships;
	bool check = true;

	for (int x = 0; x < w - deck + 1; x++) {
		for (int y = 0; y < h; y++) {
			for (int i = 0; i < deck + 2 && check; i++) {
				for (int j = 0; j < 3 && check; j++) {
					if ((j == 1 && i > 0 && i < deck + 1 && field[j + y - 1][i + x - 1] != 0) ||
						((0 <= i + x - 1 && i + x - 1 < w && 0 <= j + y - 1 && j + y - 1 < h) && field[j + y - 1][i + x - 1] >= 4)) {
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

	for (int x = 0; x < w; x++) {
		for (int y = 0; y < h - deck + 1; y++) {
			for (int i = 0; i < 3 && check; i++) {
				for (int j = 0; j < deck + 2 && check; j++) {
					if ((i == 1 && j > 0 && j < deck + 1 && field[j + y - 1][i + x - 1] != 0) ||
						((0 <= i + x - 1 && i + x - 1 < w && 0 <= j + y - 1 && j + y - 1 < h) && field[j + y - 1][i + x - 1] >= 4)) {
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


void draw_ship(vector<vector<int>> &copy_field, Ship ship) {
	if (ship.horizontal == true) {
		for (int i = ship.x; i < ship.x + ship.deck; i++) {
			copy_field[ship.y][i] = 4;
		}
	}
	else {
		for (int i = ship.y; i < ship.y + ship.deck; i++) {
			copy_field[i][ship.x] = 4;
		}
	}
}


void delete_ship(vector<vector<int>>& copy_field, Ship ship) {
	if (ship.horizontal == true) {
		for (int i = ship.x; i < ship.x + ship.deck; i++) {
			copy_field[ship.y][i] = 0;
		}
	}
	else {
		for (int i = ship.y; i < ship.y + ship.deck; i++) {
			copy_field[i][ship.x] = 0;
		}
	}
}


int change_indexes(vector<vector<int>>& copy_field, int pos, vector<int> &ship_indexes, vector<vector<Ship>> vectors) {
	int i = pos;
	while (true) {
		if (ship_indexes[i] < (int)vectors[i].size() - 1) {
			ship_indexes[i]++;
			break;
		}
		else {
			ship_indexes[i] = 0;
			if (i > 0) {
				delete_ship(copy_field, vectors[i - 1][ship_indexes[i - 1]]);
				i--;
			}
			else return -1;
		}
	}
	return i;
}


bool ship_can_be_placed(vector<vector<int>> copy_field, Ship ship) {
	if (ship.horizontal == true) {
		for (int i = 0; i < ship.deck + 2; i++) {
			for (int j = 0; j < 3; j++) {
				if ((j == 1 && i > 0 && i < ship.deck + 1 && copy_field[j + ship.y - 1][i + ship.x - 1] != 0) ||
					((0 <= i + ship.x - 1 && i + ship.x - 1 < WIDTH && 0 <= j + ship.y - 1 && j + ship.y - 1 < HEIGHT) && copy_field[j + ship.y - 1][i + ship.x - 1] >= 4)) {
					return false;
				}
			}
		}
	}
	else {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < ship.deck + 2; j++) {
				if ((i == 1 && j > 0 && j < ship.deck + 1 && copy_field[j + ship.y - 1][i + ship.x - 1] != 0) ||
					((0 <= i + ship.x - 1 && i + ship.x - 1 < WIDTH && 0 <= j + ship.y - 1 && j + ship.y - 1 < HEIGHT) && copy_field[j + ship.y - 1][i + ship.x - 1] >= 4)) {
					return false;
				}
			}
		}
	}
	return true;
}


bool is_possible_to_place_one_deck_ships(vector<vector<int>>& copy_field) {
	vector<Ship> arr;
	int count = 0, num = 0;
	bool check = true;
	stop: while (num < 4) {
		for (int x = 0; x < WIDTH; x++) {
			for (int y = 0; y < HEIGHT; y++) {
				for (int i = 0; i < 3 && check; i++) {
					for (int j = 0; j < 3 && check; j++) {
						if ((j == 1 && i == 1 && copy_field[y][x] != 0) ||
							((0 <= i + x - 1 && i + x - 1 < WIDTH && 0 <= j + y - 1 && j + y - 1 < HEIGHT) && copy_field[j + y - 1][i + x - 1] >= 4)) {
							check = false;
						}
					}
				}
				if (check) {
					copy_field[y][x] = 4;
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
			copy_field[arr[i].y][arr[i].x] = 0;
		}
		arr.clear();
		return false;
	}
}


bool thread_generate(int thread_num, Ship ship, vector<Ship> vect3, vector<Ship> vect2,
	vector<int> ship_sizes, vector<int> ship_indexes) {
	bool ship_is_not_placed = true, all_ships_are_not_placed = true;
	vector<Ship> vect4 = { ship };
	vector<vector<Ship>> vectors = { vect4, vect3, vect3, vect2, vect2, vect2 };;
	int i = 0;
	vector<vector<int>> copy_field(HEIGHT, vector <int>(WIDTH));

	for (int x = 0; x < HEIGHT; x++)
	{
		for (int y = 0; y < WIDTH; y++)
		{
			copy_field[x][y] = main_field[x][y];
		}
	}

	do {
		for (; i < (int)ship_indexes.size(); i++) {
			if (i != 0 && ship_sizes[i] == ship_sizes[i - 1] && ship_indexes[i] < ship_indexes[i - 1]) {
				ship_indexes[i] = ship_indexes[i - 1];
				i = change_indexes(copy_field, i, ship_indexes, vectors);
			}

			while (ship_is_not_placed) {
				if (ship_can_be_placed(copy_field, vectors[i][ship_indexes[i]])) {
					draw_ship(copy_field, vectors[i][ship_indexes[i]]);
					ship_is_not_placed = false;
				}
				else {
					i = change_indexes(copy_field, i, ship_indexes, vectors);
					if (i == -1) return false;
				}
			}
			ship_is_not_placed = true;
		}

		if (is_possible_to_place_one_deck_ships(copy_field)) {
			all_ships_are_not_placed = false;
		}
		else {
			delete_ship(copy_field, vectors[i - 1][ship_indexes[i - 1]]);
			i = change_indexes(copy_field, ship_indexes.size() - 1, ship_indexes, vectors);
		}
	} while (all_ships_are_not_placed);
	
	for (int x = 0; x < HEIGHT; x++)
	{
		for (int y = 0; y < WIDTH; y++)
		{
			main_field[x][y] = copy_field[x][y];
		}
	}
	return true;
}


void generate_start_field() {
	srand(time(NULL));
	vector<vector<int>> generated_field(HEIGHT, vector <int>(WIDTH));
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			if (rand() % 4 == 0) generated_field[i][j] = 1;
			else generated_field[i][j] = 0;
		}
	}
	main_field = generated_field;
}


bool generate() {
	vector<Ship> vect4 = find_best_positions(main_field, 4);
	if (vect4.size() == 0) return false;
	vector<Ship> vect3 = find_all_positions(main_field, 3, HEIGHT, WIDTH);
	vector<Ship> vect2 = find_all_positions(main_field, 2, HEIGHT, WIDTH);
	vector<int> ship_sizes = { 4, 3, 3, 2, 2, 2 };;
	vector<int> ship_indexes = { 0, 0, 1, 0, 1, 2 };;
	vector<thread> threads;

	for (int thread_num = 1; thread_num <= (int)vect4.size(); thread_num++) {
		threads.push_back(thread(thread_generate, thread_num, vect4[thread_num - 1], vect3, vect2, ship_sizes, ship_indexes));
	}

	for (int i = 0; i < (int)threads.size(); i++) {
		threads[i].join();
	}

	return true;
}