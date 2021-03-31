#pragma once
#include <vector>
#include <stack>
#include <queue>
#include <iostream>
#include <iomanip>
#include "gamecore.h"
#include "constants.h"

using namespace std;

class Bot
{
private:
	int step = 0;
	bool generated = false;
	int intmap[MAP_SIZE_X][MAP_SIZE_Y];
	struct GLoop
	{
		bool init = false;
		int step = 0;
		int matrix[MAP_SIZE_X][MAP_SIZE_Y];
	};
	GLoop g_loop;
public:

	bool g_check()
	{
		for (int i = 0; i < MAP_SIZE_X; i++)
			for (int j = 0; j < MAP_SIZE_Y; j++)
				if (g_loop.matrix[i][j] == 0) return false;
		return true;
	}

	void ghamelton(pair <int, int> player, int map[MAP_SIZE_X][MAP_SIZE_Y])
	{
		if (!g_loop.init)
		{
			for (int i = 0; i < MAP_SIZE_X; i++)
				for (int j = 0; j < MAP_SIZE_Y; j++)
					g_loop.matrix[i][j] = (map[i][j] == -1) ? -1 : 0;
			g_loop.step++;
			g_loop.matrix[player.first][player.second] = g_loop.step;
			g_loop.init = true;
		}
		if (g_loop.matrix[player.first][player.second] == 0)
		{
			if (
				g_loop.matrix[player.first + 1][player.second] == g_loop.step ||
				g_loop.matrix[player.first - 1][player.second] == g_loop.step ||
				g_loop.matrix[player.first][player.second + 1] == g_loop.step ||
				g_loop.matrix[player.first][player.second - 1] == g_loop.step
				)
			{
				g_loop.step++;
				g_loop.matrix[player.first][player.second] = g_loop.step;
			}
		}
	}
	void generate_intmap(pair<int, int> player, pair<int, int> apple, int map_temp[MAP_SIZE_X][MAP_SIZE_Y])
	{
		for (int i = 0; i < MAP_SIZE_X; i++)
			for (int j = 0; j < MAP_SIZE_Y; j++)
				intmap[i][j] = map_temp[i][j];
		intmap[player.first][player.second] = 0;
		queue <pair<int, int>> nodes;
		int x0 = apple.first, y0 = apple.second, xe = player.first, ye = player.second;
		intmap[xe][ye] = 0;
		intmap[x0][y0] = 1;
		nodes.push(pair<int, int>{x0, y0});
		int step = 1;
		do
		{
			if (intmap[xe][ye] != 0) break;
			if (nodes.empty()) break;
			pair <int, int> node = nodes.front();
			nodes.pop();
			int x = node.first, y = node.second;
			step = intmap[x][y] + 1;
			if (intmap[x + 1][y] == 0 || intmap[x + 1][y] > step)
			{
				intmap[x + 1][y] = step;
				nodes.push(pair<int, int> {x + 1, y});
			}
			if (intmap[x - 1][y] == 0 || intmap[x - 1][y] > step)
			{
				intmap[x - 1][y] = step;
				nodes.push(pair<int, int> {x - 1, y});
			}
			if (intmap[x][y - 1] == 0 || intmap[x][y - 1] > step)
			{
				intmap[x][y - 1] = step;
				nodes.push(pair<int, int> {x, y - 1});
			}
			if (intmap[x][y + 1] == 0 || intmap[x][y + 1] > step)
			{
				intmap[x][y + 1] = step;
				nodes.push(pair<int, int> {x, y + 1});
			}
		} while (!nodes.empty());
	}
	int wave_generator(pair<int, int> player, pair<int, int> apple, int map_temp[MAP_SIZE_X][MAP_SIZE_Y], int direction)
	{
		generate_intmap(player, apple, map_temp);
		if (intmap[player.first][player.second] == 0)
		{
			if (intmap[player.first - 1][player.second] != -1)
				return 1;
			if (intmap[player.first + 1][player.second] != -1)
				return 2;
			if (intmap[player.first][player.second + 1] != -1)
				return 3;
			if (intmap[player.first][player.second - 1] != -1)
				return 4;
		}
		step = intmap[player.first][player.second] - 1;
		if (intmap[player.first - 1][player.second] == step && intmap[player.first - 1][player.second] != 0 && direction != 2)
			return 1;
		if (intmap[player.first + 1][player.second] == step && intmap[player.first + 1][player.second] != 0 && direction != 1)
			return 2;
		if (intmap[player.first][player.second + 1] == step && intmap[player.first][player.second + 1] != 0 && direction != 4)
			return 3;
		if (intmap[player.first][player.second - 1] == step && intmap[player.first][player.second - 1] != 0 && direction != 3)
			return 4;
		if (intmap[player.first - 1][player.second] != -1 && intmap[player.first - 1][player.second] != 0)
			return 1;
		if (intmap[player.first + 1][player.second] != -1 && intmap[player.first + 1][player.second] != 0)
			return 2;
		if (intmap[player.first][player.second + 1] != -1 && intmap[player.first][player.second + 1] != 0)
			return 3;
		if (intmap[player.first][player.second - 1] != -1 && intmap[player.first][player.second - 1] != 0)
			return 4;
		if (intmap[player.first - 1][player.second] != -1)
			return 1;
		if (intmap[player.first + 1][player.second] != -1)
			return 2;
		if (intmap[player.first][player.second + 1] != -1)
			return 3;
		if (intmap[player.first][player.second - 1] != -1)
			return 4;
	}

	void bot_logic(pair<int, int> player, pair<int, int> apple, int map[MAP_SIZE_X][MAP_SIZE_Y], int* direction)
	{
		ghamelton(player, map);
		if (!g_check())
		{
			*direction = wave_generator({ player.first, player.second }, { apple.first, apple.second }, map, *direction);
			if (intmap[player.first][player.second] == 0 &&
				(*direction == 1 && map[player.first - 2][player.second] == -1) ||
				(*direction == 2 && map[player.first + 2][player.second] == -1) ||
				(*direction == 3 && map[player.first][player.second + 2] == -1) ||
				(*direction == 4 && map[player.first][player.second - 2] == -1)
				)
			{
				if (intmap[player.first - 1][player.second] != -1)
					*direction = 1;
				if (intmap[player.first + 1][player.second] != -1)
					*direction = 2;
				if (intmap[player.first][player.second + 1] != -1)
					*direction = 3;
				if (intmap[player.first][player.second - 1] != -1)
					*direction = 4;
			}
			if (
				(*direction == 1 && map[player.first - 1][player.second] == -1) ||
				(*direction == 2 && map[player.first + 1][player.second] == -1) ||
				(*direction == 3 && map[player.first][player.second + 1] == -1) ||
				(*direction == 4 && map[player.first][player.second - 1] == -1)
				)
			{
				*direction = wave_generator({ player.first, player.second }, { apple.first, apple.second }, map, *direction);
			}
		}
		else
		{
			int n = g_loop.matrix[player.first][player.second] + 1;
			if (n == g_loop.step) n = 1;
			if(g_loop.matrix[player.first + 1][player.second] == n) *direction = 2;
			if(g_loop.matrix[player.first - 1][player.second] == n) *direction = 1;
			if(g_loop.matrix[player.first][player.second + 1] == n) *direction = 3;
			if(g_loop.matrix[player.first][player.second - 1] == n) *direction = 4;
		}
	}
};