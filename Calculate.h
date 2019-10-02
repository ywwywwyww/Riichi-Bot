#pragma once
#include "Riichi.h"
#include <vector>
#include <utility>
#include <cstring>

static int Used[5][1 << 14]; //匹配了多少张搭子/面子/对子了 不考虑雀头
static int UsedWithPair[5][1 << 14]; //匹配了多少张搭子/面子/对子了 考虑雀头

int CalcUsed(std::vector<Tile> hand) // 给出很多张牌，求有多少组牌能凑成面子和对子什么的 牌数为 3n + 2 或 3n + 1
{
	int n = hand.size();
	int m = n / 3;
	int res = 0; // 有多少组合好的牌了
	//if (m == 4)
	//{ // the Thirteen Orphans
	//	static int b[100];
	//	memset(b, 0, sizeof b);
	//	int flag = 0;
	//	int temp = 0;
	//	for (auto v : hand)
	//		if (v.suit == 'z' || v.number == 1 || v.number == 9)
	//		{
	//			if (!b[TileToNum[v]])
	//				temp++;
	//			else
	//				flag = 1;
	//			b[TileToNum[v]]++;
	//		}
	//	res = std::max(res, temp + flag);
	//}
	//if (m == 4)
	//{ // seven different pairs
	//	static int b[100];
	//	memset(b, 0, sizeof b);
	//	int temp = 0;
	//	for (auto v : hand)
	//		b[TileToNum[v]]++;
	//	for (int i = 1; i <= DifferentTiles; i++)
	//		if (b[i] >= 2)
	//			temp++;
	//	res = std::max(res, 2 * temp + (7 - temp));
	//}
	{ // four groups and a pair
		memset(Used, 0xa0, sizeof Used);
		memset(UsedWithPair, 0xa0, sizeof UsedWithPair);
		Used[0][0] = 0;
		for (int i = 1; i <= m; i++)
		{
			for (int j = 0; j < 1 << n; j++)
			{
				if (Used[i - 1][j] < 0)
					continue;
				for (int k1 = 0; k1 < n; k1++)
				{
					if (!((j >> k1) & 1))
					{
						//Used[i][j ^ (1 << k1)] = std::max(Used[i][j ^ (1 << k1)], Used[i - 1][j] + 1);
						for (int k2 = k1 + 1; k2 < n; k2++)
							if (!((j >> k2) & 1))
							{
								if (IsPair(hand[k1], hand[k2]))
									Used[i][j ^ (1 << k1) ^ (1 << k2)] = std::max(Used[i][j ^ (1 << k1) ^ (1 << k2)], Used[i - 1][j] + 2);
								else if (IsTatsu(hand[k1], hand[k2]))
									Used[i][j ^ (1 << k1) ^ (1 << k2)] = std::max(Used[i][j ^ (1 << k1) ^ (1 << k2)], Used[i - 1][j] + 2);
								else
									continue;
								for (int k3 = k2 + 1; k3 < n; k3++)
									if (!((j >> k3) & 1))
									{
										if (IsGroup(hand[k1], hand[k2], hand[k3]))
											Used[i][j ^ (1 << k1) ^ (1 << k2) ^ (1 << k3)] = std::max(Used[i][j ^ (1 << k1) ^ (1 << k2) ^ (1 << k3)], Used[i - 1][j] + 3);
										else if (IsChow(hand[k1], hand[k2], hand[k3]))
											Used[i][j ^ (1 << k1) ^ (1 << k2) ^ (1 << k3)] = std::max(Used[i][j ^ (1 << k1) ^ (1 << k2) ^ (1 << k3)], Used[i - 1][j] + 3);
									}
							}
					}
				}
			}
		}
		for (int i = 0; i <= m; i++)
			for (int j = 0; j < 1 << n; j++)
				for (int k = 0; k < n; k++)
					if ((j >> k) & 1)
					{
						Used[i][j] = std::max(Used[i][j], Used[i][j ^ (1 << k)]);
						if (i >= 1)
							Used[i][j] = std::max(Used[i][j], Used[i - 1][j ^ (1 << k)] + 1);
						UsedWithPair[i][j] = std::max(UsedWithPair[i][j], UsedWithPair[i][j ^ (1 << k)]);
						if (i >= 1)
							UsedWithPair[i][j] = std::max(UsedWithPair[i][j], UsedWithPair[i - 1][j ^ (1 << k)] + 1);
					}
		for (int i = 0; i <= m; i++)
			for (int j = 0; j < 1 << n; j++)
				for (int k1 = 0; k1 < n; k1++)
					if (!((j >> k1) & 1))
					{
						UsedWithPair[i][j ^ (1 << k1)] = std::max(UsedWithPair[i][j ^ (1 << k1)], Used[i][j] + 1);
						for (int k2 = k1 + 1; k2 < n; k2++)
							if (!((j >> k2) & 1))
								if (IsPair(hand[k1], hand[k2]))
									UsedWithPair[i][j ^ (1 << k1) ^ (1 << k2)] = std::max(UsedWithPair[i][j ^ (1 << k1) ^ (1 << k2)], Used[i][j] + 2);
					}
		res = std::max(res, UsedWithPair[m][(1 << n) - 1]);
	}
	return res;
}

int Shanten(std::vector<Tile> hand) // 给出 3n + 1 张牌，求向听数 0:听牌
{
	int n = hand.size();
	if(n % 3 == 2)
		n--;
	return n - CalcUsed(hand);
}

int IsTenpai(std::vector<Tile> hand) // 给出 3n + 1 张牌，判断是否听牌
{
	return Shanten(hand) == 0;
}

int IsWinningHand(std::vector<Tile> hand)
{
	return CalcUsed(hand) == 14;
}

std::vector<Tile> TenpaiCategory(std::vector<Tile> hand) // 给出若干张牌，求进哪些张可以减少向听数
{
	std::vector<Tile> res;
	int n = hand.size();
	int m = n / 3;
	int UsedNow = CalcUsed(hand);
	for (auto v : AllTiles)
	{
		int temp = UsedNow;
		for (int i = 0; i < n; i++)
		{
			if (IsTatsu(v, hand[i]))
				temp = std::max(temp, UsedWithPair[m - 1][(1 << n) - 1 - (1 << i)] + 2);
			else if (IsPair(v, hand[i]))
				temp = std::max(temp, Used[m][(1 << n) - 1 - (1 << i)] + 2);
			else
				continue;
			for (int j = i + 1; j < n; j++)
			{
				if (IsGroup(v, hand[i], hand[j]))
					temp = std::max(temp, UsedWithPair[m - 1][(1 << n) - 1 - (1 << i) - (1 << j)] + 3);
				else if (IsChow(v, hand[i], hand[j]))
					temp = std::max(temp, UsedWithPair[m - 1][(1 << n) - 1 - (1 << i) - (1 << j)] + 3);
			}
		}
		if (temp > UsedNow)
			res.push_back(v);
	}
	return res;
}

std::vector<std::pair<Tile, std::string> > Discard(std::vector<Tile> hand) // 打哪些牌能在再进一张牌之后减少向听数
{
	std::sort(hand.begin(), hand.end());
	std::vector<std::pair<Tile, std::string> > res;
	int n = hand.size();
	int m = n / 3;
	int UsedNow = CalcUsed(hand);
	for (int l = 0; l < n; l++)
	{
		if (l > 1 && hand[l].normal() == hand[l - 1].normal())
			continue;
		std::string str;
		int UsedMax = 0;
		for (auto v : AllTiles)
		{
			int temp = UsedNow;
			for (int i = 0; i < n; i++)
			{
				if (i == l)
					continue;
				if (IsTatsu(v, hand[i]))
					temp = std::max(temp, UsedWithPair[m - 1][(1 << n) - 1 - (1 << i) - (1 << l)] + 2);
				else if (IsPair(v, hand[i]))
					temp = std::max(temp, Used[m][(1 << n) - 1 - (1 << i) - (1 << l)] + 2);
				else
					continue;
				for (int j = i + 1; j < n; j++)
				{
					if (j == l)
						continue;
					if (IsGroup(v, hand[i], hand[j]))
						temp = std::max(temp, UsedWithPair[m - 1][(1 << n) - 1 - (1 << i) - (1 << j) - (1 << l)] + 3);
					else if (IsChow(v, hand[i], hand[j]))
						temp = std::max(temp, UsedWithPair[m - 1][(1 << n) - 1 - (1 << i) - (1 << j) - (1 << l)] + 3);
				}
			}
			if (temp > UsedNow)
				str += v.ToStr();
			UsedMax = std::max(UsedMax, temp);
		}
		if (UsedMax > UsedNow)
			res.push_back(std::make_pair(hand[l], str));
	}
	return res;
}

int cmp(std::pair<Tile, std::string> a, std::pair<Tile, std::string> b)
{
	if(a.second.size() != b.second.size())
		return a.second.size() > b.second.size();
	return a.first < b.first;
}

std::vector<std::pair<Tile, std::string> > DisplayDiscard(std::vector<Tile> hand) // 打哪些牌能在再进一张牌之后减少向听数
{
	auto s = Discard(hand);
	std::sort(s.begin(), s.end(), cmp);
	printf("-------------------------------\n");
	for(auto v:s)
		printf("%s %s\n",v.first.ToStr().c_str(), v.second.c_str());
	printf("-------------------------------\n");
	return s;
}

std::vector<Tile> DisplayTenpai(std::vector<Tile> hand) // 给出若干张牌，求进哪些张可以减少向听数
{
	auto s = TenpaiCategory(hand);
	for(auto v:s)
		v.display();
	printf("\n"); 
	return s;
}
