#pragma one
#include "Riichi.h"
#include <vector>
#include <utility>

int Used(std::vector<Tile> hand) // 给出很多张牌，求有多少组牌能凑成面子和对子什么的 牌数为 3n + 2 或 3n + 1
{
	int n = hand.size();
	int m = n / 3;
	int res = 0; // 有多少组合好的牌了
	if (m == 4)
	{ // the Thirteen Orphans
		static int b[100];
		memset(b, 0, sizeof b);
		int flag = 0;
		int temp = 0;
		for (auto v : hand)
			if (v.suit == 'z' || v.number == 1 || v.number == 9)
			{
				if (!b[TileToNum[v]])
					temp++;
				else
					flag = 1;
				b[TileToNum[v]]++;
			}
		res = std::max(res, temp + flag);
	}
	if (m == 4)
	{ // seven different pairs
		static int b[100];
		memset(b, 0, sizeof b);
		int temp = 0;
		for (auto v : hand)
			b[TileToNum[v]]++;
		for (int i = 1; i <= DifferentTiles; i++)
			if (b[i] >= 2)
				temp++;
		res = std::max(res, 2 * temp + (7 - temp));
	}
	{ // four groups and a pair
		static int f[5][1 << 14]; //匹配了多少张搭子/面子/对子了
		memset(f, 0xa0, sizeof f);
		f[0][0] = 0;
		for (int i = 0; i < n; i++)
			f[0][1 << i] = 1;
		for (int i = 0; i < n; i++)
			for (int j = i + 1; j < n; j++)
				if (hand[i] == hand[j])
					f[0][(1 << i) | (1 << j)] = 2;
		for (int i = 1; i <= m; i++)
		{
			for (int j = 0; j < 1 << n; j++)
			{
				if (f[i - 1][j] < 0)
					continue;
				for (int k1 = 0; k1 < n; k1++)
				{
					if (!((j >> k1) & 1))
					{
						f[i][j ^ (1 << k1)] = std::max(f[i][j ^ (1 << k1)], f[i - 1][j] + 1);
						for (int k2 = k1 + 1; k2 < n; k2++)
							if (!((j >> k2) & 1))
							{
								if (IsPair(hand[k1], hand[k2]))
									f[i][j ^ (1 << k1) ^ (1 << k2)] = std::max(f[i][j ^ (1 << k1) ^ (1 << k2)], f[i - 1][j] + 2);
								else if (IsTatsu(hand[k1], hand[k2]))
									f[i][j ^ (1 << k1) ^ (1 << k2)] = std::max(f[i][j ^ (1 << k1) ^ (1 << k2)], f[i - 1][j] + 2);
								else
									continue;
								for (int k3 = k2 + 1; k3 < n; k3++)
									if (!((j >> k3) & 1))
									{
										if (IsGroup(hand[k1], hand[k2], hand[k3]))
											f[i][j ^ (1 << k1) ^ (1 << k2) ^ (1 << k3)] = std::max(f[i][j ^ (1 << k1) ^ (1 << k2) ^ (1 << k3)], f[i - 1][j] + 3);
										else if (IsChow(hand[k1], hand[k2], hand[k3]))
											f[i][j ^ (1 << k1) ^ (1 << k2) ^ (1 << k3)] = std::max(f[i][j ^ (1 << k1) ^ (1 << k2) ^ (1 << k3)], f[i - 1][j] + 3);
									}
							}
					}
				}
			}
		}
		int temp = 0;
		for (int i = 0; i < 1 << n; i++)
			temp = std::max(temp, f[m][i]);
		res = std::max(res, temp);
	}
	return res;
}

int Shanten(std::vector<Tile> hand) // 给出 3n + 1 张牌，求向听数 0:听牌
{
	int n = hand.size();
	return n - Used(hand);
}

int IsTenpai(std::vector<Tile> hand) // 给出 3n + 1 张牌，判断是否听牌
{
	return Shanten(hand) == 0;
}

int IsWinningHand(std::vector<Tile> hand)
{
	return Used(hand) == 14;
}

std::vector<Tile> TenpaiCategory(std::vector<Tile> hand) // 给出若干张牌，求进哪些张可以减少向听数
{
	std::vector<Tile> res;
	int UsedNow = Used(hand);
	for (auto v : AllTiles)
	{
		auto next = hand;
		next.push_back(v);
		if (Used(next) > UsedNow)
			res.push_back(v);
	}
	return res;
}

std::vector<std::pair<Tile, std::string> > Discard(std::vector<Tile> hand) // 打每张牌
{

}