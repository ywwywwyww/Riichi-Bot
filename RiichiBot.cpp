#include "Riichi.h"
#include "Calculate.h"

#include <algorithm>
#include <iostream>


int main()
{
//	//printf("%d\n", IsTatsu(Tile("2m"), Tile("2m")));
//	for (auto v : AllTiles)
//		printf("%d%c", v.number, v.suit);
//	printf("\n");
//	//auto tiles1 = StrToTiles("1m1m1m2m3m4m5m6m7m8m9m9m9m");
//	//Display(tiles1);
//	//printf("%d\n", IsTenpai(tiles1));
//	//auto tenpai = TenpaiCategory(tiles1);
//	//Display(tenpai);
//	auto tiles2 = StrToTiles("2m3m5m6m7m8m9m");
//	auto tenpai = TenpaiCategory(tiles2);
//	Display(tenpai);
//	auto tiles3 = StrToTiles("2m3m4m5m6m7m8m9m");
//	auto s = Discard(tiles3);
//	std::sort(s.begin(), s.end(), cmp);
//	for(auto v:s)
//		printf("%s %s\n",v.first.ToStr().c_str(), v.second.c_str());
	std::vector<Tile> CurrentHand;
	std::string op;
	while(1)
	{
		std::sort(CurrentHand.begin(), CurrentHand.end());
		printf("现在的手牌："); 
		Display(CurrentHand);
		std::cin >> op;
		if(op == "clear")
			CurrentHand.clear();
		else if(op == "shanten")
			printf("%d\n",Shanten(CurrentHand));
		else if(op == "display")
		{
			printf("现在的手牌："); 
			Display(CurrentHand);
			DisplayDiscard(CurrentHand);
		}
		else if(op[0] == '-')
		{
			std::string str=op.substr(1);
			std::vector<Tile> tiles1 = StrToTiles(str);
			for(auto v:tiles1)
			{
				for(auto i = CurrentHand.begin(); i != CurrentHand.end(); i++) 
					if(*i == v)
					{
						CurrentHand.erase(i);
						break;
					}
			}
		}
		else if(op[0] == '+')
		{
			std::string str=op.substr(1);
			std::vector<Tile> tiles1 = StrToTiles(str);
			for(auto v:tiles1)
			{
				if(!v.valid())
					continue;
				CurrentHand.push_back(v);
			}
		}
		else
		{
			std::string str=op;
			std::vector<Tile> tiles1 = StrToTiles(str);
			for(auto v:tiles1)
			{
				if(!v.valid())
					continue;
				CurrentHand.push_back(v);
			}
		}
		if(op != "display")
		{
			printf("%d向听\n",Shanten(CurrentHand)); 
			DisplayDiscard(CurrentHand);
		}
		{
			printf("听牌：");
			DisplayTenpai(CurrentHand);
		}
	}
	return 0;
}
