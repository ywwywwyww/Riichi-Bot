#pragma once

#ifndef Riichi_h
#define Riichi_h
#include <algorithm>
#include <vector>
#include <map>
#include <string>

struct Tile
{
	char suit;
	int number; // 0:红宝牌
	Tile()
	{
		suit = -1;
		number = -1;
	}
	Tile(int _number, char _suit)
	{
		number = _number;
		suit = _suit;
	}
	Tile(const char* str)
	{
		suit = str[1];
		number = str[0] - '0';
	}
	int valid() // 检测这张牌是否合法
	{
		return (suit == 'z' && number >= 1 && number <= 7) || ((suit == 'm' || suit == 's' || suit == 'p') && number >= 0 && number <= 9);
	}
	Tile normal() // 将红宝牌转换为5
	{
		if (number == 0)
			number = 5;
		return *this;
	}
	void display()
	{
		printf("%d%c", number, suit);
	}
	std::string ToStr()
	{
		std::string res;
		res += char(number + '0');
		res += char(suit);
		return res;
	}
};

//std::vector<Tile> AllTiles = { "1m","2m","3m","4m","5m","6m","7m","8m","9m","1p","2p","3p","4p","5p","6p","7p","8p","9p","1s","2s","3s","4s","5s","6s","7s","8s","9s","1z","2z","3z","4z","5z","6z","7z" };//所有类型的牌（不包括红宝牌）
std::vector<Tile> AllTiles;
std::map<Tile, int> TileToNum;

const int DifferentTiles = 34;

void TileInit()
{
	static int Initiated = 0;
	//if (Initiated == 1)
	//	return;
	Initiated = 1;
	for (int i = 1; i <= 9; i++)
		AllTiles.push_back(Tile(i, 'm'));
	for (int i = 1; i <= 9; i++)
		AllTiles.push_back(Tile(i, 'p'));
	for (int i = 1; i <= 9; i++)
		AllTiles.push_back(Tile(i, 's'));
	for (int i = 1; i <= 7; i++)
		AllTiles.push_back(Tile(i, 'z'));
	int cnt = 0;
	for (auto v : AllTiles)
		TileToNum[v] = ++cnt;
}

struct TileInitiator
{
	TileInitiator()
	{
		TileInit();
	}
} TileInitiator;

int operator <(Tile a, Tile b)
{
	const static int number_priority[] = { 6,1,2,3,4,5,7,8,9,10 };
	if (a.suit != b.suit)
		return a.suit < b.suit;
	return number_priority[a.number] < number_priority[b.number];
}

int operator ==(Tile a, Tile b)
{
	a.normal();
	b.normal();
	return a.suit == b.suit && a.number == b.number;
}

int IsChow(Tile a1, Tile a2, Tile a3)
{
	if (!a1.valid() || !a2.valid() || !a3.valid())
		return 0;
	a1.normal();
	a2.normal();
	a3.normal();
	Tile a[4] = { Tile(),a1,a2,a3 };
	std::sort(a + 1, a + 4);
	return a[1].suit != 'z' && a[1].suit == a[2].suit && a[2].suit == a[3].suit && a[1].number + 1 == a[2].number && a[2].number + 1 == a[3].number;
}

int IsGroup(Tile a1, Tile a2, Tile a3) //先不考虑杠
{
	a1.normal();
	a2.normal();
	a3.normal();
	return a1.valid() && a1 == a2 && a2 == a3;
}

int IsPair(Tile a1, Tile a2)
{
	a1.normal();
	a2.normal();
	return a1.valid() && a1 == a2;
}

int IsTatsu(Tile a1, Tile a2)
{
	//if (IsPair(a1, a2))
	//	return 1;
	a1.normal();
	a2.normal();
	Tile a[3] = { Tile(),a1,a2 };
	std::sort(a + 1, a + 3);
	return a[1].suit != 'z' && a[1].suit == a[2].suit && (a[1].number + 1 == a[2].number || a[1].number + 2 == a[2].number);
}

std::vector<Tile> StrToTiles(std::string str)
{
	std::vector<Tile> res;
//	for (int i = 0; i < (int)str.size(); i += 2)
//		res.push_back(Tile(str[i] - '0', str[i + 1]));
	std::vector<int> number;
	for(auto v:str)
		if(v>='0'&&v<='9')
			number.push_back(v);
		else
		{
			for(auto v2:number)
				res.push_back(Tile(v2-'0', v));
			number.clear();
		}
	return res;
}

void Display(std::vector<Tile> tiles)
{
	for (auto v : tiles)
		v.display();
	putchar('\n');
}

#endif
