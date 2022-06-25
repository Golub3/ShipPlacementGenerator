#include "gtest/gtest.h"
#include "task2.h"

vector<vector<int>> test_field =
{
	{1, 1, 0, 1, 0, 1, 1, 1, 1},
	{0, 0, 0, 1, 0, 1, 0, 0, 0},
	{1, 0, 0, 0, 0, 1, 0, 0, 0},
	{0, 0, 1, 0, 0, 0, 0, 0, 1},
	{0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 1, 1, 1},
	{1, 0, 0, 0, 1, 1, 1, 1, 1},
};

TEST(Test, TestThreadGenerating) {
	main_field =
	{
		{1, 1, 0, 1, 0, 1, 1, 1, 1},
		{0, 0, 0, 1, 0, 1, 0, 0, 0},
		{1, 0, 0, 0, 0, 1, 0, 0, 0},
		{0, 0, 1, 0, 0, 0, 0, 0, 1},
		{0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 1, 1, 1},
		{1, 0, 0, 0, 1, 1, 1, 1, 1},
	};


	vector<Ship> vect4 = find_best_positions(main_field, 4);
	vector<Ship> vect3 = find_all_positions(main_field, 3, HEIGHT, WIDTH);
	vector<Ship> vect2 = find_all_positions(main_field, 2, HEIGHT, WIDTH);
	vector<int> ship_sizes = { 4, 3, 3, 2, 2, 2 };;
	vector<int> ship_indexes = { 0, 0, 1, 0, 1, 2 };;
	vector<vector<Ship>> vectors = { vect4, vect3, vect3, vect2, vect2, vect2 };

	EXPECT_TRUE(thread_generate(1, vect4[0], vect3, vect2, ship_sizes, ship_indexes));
	EXPECT_FALSE(thread_generate(2, vect4[1], vect3, vect2, ship_sizes, ship_indexes));
}


TEST(Test, TestFindingBestPositions) {
	vector<Ship> ships1;
	ships1.push_back(Ship(4, 4, 0, false));
	ships1.push_back(Ship(4, 7, 1, false));
	vector<Ship> ships2 = find_best_positions(test_field, 4);
	EXPECT_EQ(ships1[0].x, ships2[0].x);
	EXPECT_EQ(ships1[0].y, ships2[0].y);
	EXPECT_EQ(ships1[1].x, ships2[1].x);
	EXPECT_EQ(ships1[1].y, ships2[1].y);
}


TEST(Test, TestFindingAllPositions) {
	vector<vector<int>> small_test_field =
	{
		{1, 1 },
		{0, 0 },
		{1, 0 },
	};

	vector<Ship> ships3 = find_all_positions(small_test_field, 3, 3, 2);
	EXPECT_EQ((int)ships3.size(), 0);

	vector<Ship> ships2_1;
	ships2_1.push_back(Ship(2, 0, 1, true));
	ships2_1.push_back(Ship(2, 1, 1, false));
	vector<Ship> ships2_2 = find_all_positions(small_test_field, 2, 3, 2);
	EXPECT_EQ(ships2_1[0].x, ships2_2[0].x);
	EXPECT_EQ(ships2_1[0].y, ships2_2[0].y);
	EXPECT_EQ(ships2_1[1].x, ships2_2[1].x);
	EXPECT_EQ(ships2_1[1].y, ships2_2[1].y);
}


TEST(Test, TestDrawingShip) {
	draw_ship(test_field, Ship(3, 0, 1, true));
	EXPECT_EQ(4, test_field[1][0]);
	EXPECT_EQ(4, test_field[1][1]);
	EXPECT_EQ(4, test_field[1][2]);
	EXPECT_NE(4, test_field[1][3]);
	delete_ship(test_field, Ship(3, 0, 1, true));
}


TEST(Test, TestDeletingShip) {
	draw_ship(test_field, Ship(3, 0, 1, true));
	delete_ship(test_field, Ship(3, 0, 1, true));
	EXPECT_EQ(0, test_field[1][0]);
	EXPECT_EQ(0, test_field[1][1]);
	EXPECT_EQ(0, test_field[1][2]);
}


TEST(Test, TestChangingIndexes) {
	vector<Ship> vect4 = find_best_positions(test_field, 4);
	vector<Ship> vect3 = find_all_positions(test_field, 3, 7, 9);
	vector<Ship> vect2 = find_all_positions(test_field, 2, 7, 9);
	vector<int> ship_indexes = { 0, 0, 1, 0, 1, 2 };;
	vector<vector<Ship>> vectors = { vect4, vect3, vect3, vect2, vect2, vect2 };;

	EXPECT_EQ(0, change_indexes(test_field, 0, ship_indexes, vectors));
	EXPECT_EQ(-1, change_indexes(test_field, 0, ship_indexes, vectors));
}


TEST(Test, TestShipCanBePlaced) {
	EXPECT_TRUE(ship_can_be_placed(test_field, Ship(1, 0, 1, true)));
	EXPECT_TRUE(ship_can_be_placed(test_field, Ship(2, 0, 1, true)));
	EXPECT_TRUE(ship_can_be_placed(test_field, Ship(3, 0, 1, true)));
	EXPECT_FALSE(ship_can_be_placed(test_field, Ship(4, 0, 1, true)));

	EXPECT_FALSE(ship_can_be_placed(test_field, Ship(3, 0, 0, false)));
	EXPECT_FALSE(ship_can_be_placed(test_field, Ship(4, 0, 5, false)));
}


TEST(Test, TestIsPossibleToPlaceOneDeckShips) {
	vector<vector<int>> test_field1 =
	{
		{1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 0, 0, 1, 1, 1, 0, 0, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1},
	};

	EXPECT_FALSE(is_possible_to_place_one_deck_ships(test_field1));

	vector<vector<int>> test_field2 =
	{
		{0, 1, 1, 1, 1, 1, 1, 1, 0},
		{1, 1, 1, 1, 1, 1, 1, 0, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1},
		{0, 1, 1, 1, 1, 1, 1, 1, 0},
	};
	EXPECT_TRUE(is_possible_to_place_one_deck_ships(test_field2));
}


TEST(Test, TestGenerating) {
	main_field =
	{
		{0, 1, 1, 1, 1, 1, 1, 1, 0},
		{1, 1, 1, 1, 1, 1, 1, 0, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1},
		{0, 1, 1, 1, 1, 1, 1, 1, 0},
	};
	EXPECT_FALSE(generate());

	main_field = {
		{1, 1, 0, 1, 0, 1, 1, 1, 1},
		{0, 0, 0, 1, 0, 1, 0, 0, 0},
		{1, 0, 0, 0, 0, 1, 0, 0, 0},
		{0, 0, 1, 0, 0, 0, 0, 0, 1},
		{0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 1, 1, 1},
		{1, 0, 0, 0, 1, 1, 1, 1, 1},
	};
	EXPECT_TRUE(generate());
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}