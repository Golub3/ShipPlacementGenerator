#include "pch.h"
#include "CppUnitTest.h"
#include "../task2/task2.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
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

	TEST_CLASS(UnitTests)
	{
	public:
		
		TEST_METHOD(TestThreadGenerating)
		{
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

			Assert::IsTrue(thread_generate(1, vect4[0], vect3, vect2, ship_sizes, ship_indexes));

			Assert::IsFalse(thread_generate(2, vect4[1], vect3, vect2, ship_sizes, ship_indexes));
		}

		TEST_METHOD(TestFindingBestPositions)
        {
			vector<Ship> ships1;
			ships1.push_back(Ship(4, 4, 0, false));
			ships1.push_back(Ship(4, 7, 1, false));

			vector<Ship> ships2 = find_best_positions(test_field, 4);
			Assert::AreEqual(ships1[0].x, ships2[0].x);
			Assert::AreEqual(ships1[0].y, ships2[0].y);
			Assert::AreEqual(ships1[1].x, ships2[1].x);
			Assert::AreEqual(ships1[1].y, ships2[1].y);
        }

		TEST_METHOD(TestFindingAllPositions)
		{
			vector<vector<int>> small_test_field =
			{
				{1, 1 },
				{0, 0 },
				{1, 0 },
			};

			vector<Ship> ships3 = find_all_positions(small_test_field, 3, 3, 2);
			Assert::AreEqual((int)ships3.size(), 0);

			vector<Ship> ships2_1;
			ships2_1.push_back(Ship(2, 0, 1, true));
			ships2_1.push_back(Ship(2, 1, 1, false));
			vector<Ship> ships2_2 = find_all_positions(small_test_field, 2, 3, 2);
			Assert::AreEqual(ships2_1[0].x, ships2_2[0].x);
			Assert::AreEqual(ships2_1[0].y, ships2_2[0].y);
			Assert::AreEqual(ships2_1[1].x, ships2_2[1].x);
			Assert::AreEqual(ships2_1[1].y, ships2_2[1].y);
		}

		TEST_METHOD(TestDrawingShip)
		{
			draw_ship(test_field, Ship(3, 0, 1, true));
			Assert::AreEqual(4, test_field[1][0]);
			Assert::AreEqual(4, test_field[1][1]);
			Assert::AreEqual(4, test_field[1][2]);
			Assert::AreNotEqual(4, test_field[1][3]);
			delete_ship(test_field, Ship(3, 0, 1, true));
		}

		TEST_METHOD(TestDeletingShip)
		{
			draw_ship(test_field, Ship(3, 0, 1, true));
			delete_ship(test_field, Ship(3, 0, 1, true));
			Assert::AreEqual(0, test_field[1][0]);
			Assert::AreEqual(0, test_field[1][1]);
			Assert::AreEqual(0, test_field[1][2]);
		}

		TEST_METHOD(TestChangingIndexes)
		{
			vector<Ship> vect4 = find_best_positions(test_field, 4);
			vector<Ship> vect3 = find_all_positions(test_field, 3, 7, 9);
			vector<Ship> vect2 = find_all_positions(test_field, 2, 7, 9);
			vector<int> ship_indexes = { 0, 0, 1, 0, 1, 2 };;
			vector<vector<Ship>> vectors = { vect4, vect3, vect3, vect2, vect2, vect2 };;

			Assert::AreEqual(0, change_indexes(test_field, 0, ship_indexes, vectors));
			Assert::AreEqual(-1, change_indexes(test_field, 0, ship_indexes, vectors));
		}

		TEST_METHOD(TestShipCanBePlaced)
		{
			Assert::IsTrue(ship_can_be_placed(test_field, Ship(1, 0, 1, true)));
			Assert::IsTrue(ship_can_be_placed(test_field, Ship(2, 0, 1, true)));
			Assert::IsTrue(ship_can_be_placed(test_field, Ship(3, 0, 1, true)));
			Assert::IsFalse(ship_can_be_placed(test_field, Ship(4, 0, 1, true)));

			Assert::IsFalse(ship_can_be_placed(test_field, Ship(3, 0, 0, false)));
			Assert::IsFalse(ship_can_be_placed(test_field, Ship(4, 0, 5, false)));
		}

		TEST_METHOD(TestIsPossibleToPlaceOneDeckShips)
		{
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

			Assert::IsFalse(is_possible_to_place_one_deck_ships(test_field1));

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
			Assert::IsTrue(is_possible_to_place_one_deck_ships(test_field2));
		}

		TEST_METHOD(TestGenerating)
		{
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
			Assert::IsFalse(generate());

			main_field = {
				{1, 1, 0, 1, 0, 1, 1, 1, 1},
				{0, 0, 0, 1, 0, 1, 0, 0, 0},
				{1, 0, 0, 0, 0, 1, 0, 0, 0},
				{0, 0, 1, 0, 0, 0, 0, 0, 1},
				{0, 0, 0, 0, 0, 0, 0, 0, 1},
				{1, 0, 0, 0, 0, 0, 1, 1, 1},
				{1, 0, 0, 0, 1, 1, 1, 1, 1},
			};
			Assert::IsTrue(generate());
		}
	};
}
