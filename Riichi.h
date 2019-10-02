#pragma once

#include <algorithm>
#include <vector>

struct Tile
{
	char suit;
	int number; // 0:红宝牌
	Tile()
	{
		suit = -1;
		number = -1;
	}
	Tile(char* str)
	{
		suit = str[1];
		number = str[0] - '0';
	}
	int check() // 检测这张牌是否合法
	{
		return (suit == 'z' && number >= 1 && number <= 7) || ((suit == 'm' || suit == 's' || suit == 'p') && number >= 0 && number <= 9);
	}
	void normal() // 将红宝牌转换为5
	{
		if (number == 0)
			number = 5;
	}
};

std::vector<Tile> AllTiles={; //所有类型的牌（不包括红宝牌）

int operator <(Tile a, Tile b)
{
	const static int number_priority[] = { 6,1,2,3,4,5,7,8,9,10 };
	if (a.suit != b.suit)
		return a.suit < b.suit;
	return number_priority[a.number] < number_priority[b.number];
}

int operator ==(Tile a, Tile b)
{
	return (a.suit == b.suit && a.number == b.number) || ((a.suit == 'm' || a.suit == 's' || a.suit == 'p') && (a.number == 0 || a.number == 5) && (b.number == 0 || b.number == 5));
}

int Chow(Tile a1, Tile a2, Tile a3)
{
	if (!a1.check() || !a2.check() || !a3.check())
		return 0;
	a1.normal();
	a2.normal();
	a3.normal();
	Tile a[4] = { Tile(),a1,a2,a3 };
	std::sort(a + 1, a + 4);
	return a[1].suit != 'z' && a[1].suit == a[2].suit && a[2].suit == a[3].suit && a[1].number + 1 == a[2].number && a[2].number + 1 == a[3].number;
}

int Group(Tile a1, Tile a2, Tile a3) //先不考虑杠
{
	a1.normal();
	a2.normal();
	a3.normal();
	return a1.check() && a1 == a2 && a2 == a3;
}

int Pair(Tile a1, Tile a2)
{
	a1.normal();
	a2.normal();
	return a1.check() && a1 == a2;
}