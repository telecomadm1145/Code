#include "classwiz_bsp.h"
#include "libc.h"
extern "C" void render_copy();

constexpr int GRID_SIZE = 4;
byte grid[GRID_SIZE][GRID_SIZE];
byte new_tile[GRID_SIZE][GRID_SIZE]; // 标记新生成的格子

int get_random(int max)
{
	return Timer0Counter % max;
}
void draw_grid()
{
	memzero_n(GetScreenBuffer(), 0x600 * 2);
	for (int y = 0; y < GRID_SIZE; y++)
	{
		for (int x = 0; x < GRID_SIZE; x++)
		{
			char buf[5];
			auto dat = grid[y][x];
			auto len = itoa_n(1 << grid[y][x], (char __near *)buf);
			if (dat == 0)
				buf[0] = 0;
			if (new_tile[y][x])
				Color = 3;
			else
				Color = 2;
			line_print_n((const char __near *)buf, x * 40 + ((40 - len) >> 1), y * 16);
		}
	}
	render_copy();
}

void add_new_tile()
{
	int empty_spots = 0;
	for (int y = 0; y < GRID_SIZE; y++)
	{
		for (int x = 0; x < GRID_SIZE; x++)
		{
			if (grid[y][x] == 0)
				empty_spots++;
			new_tile[y][x] = 0; // 重置新格子标记
		}
	}
	if (empty_spots == 0)
		return;

	int target = get_random(empty_spots);
	for (int y = 0; y < GRID_SIZE; y++)
	{
		for (int x = 0; x < GRID_SIZE; x++)
		{
			if (grid[y][x] == 0)
			{
				if (target == 0)
				{
					grid[y][x] = get_random(2) + 1; // 2 或 4
					new_tile[y][x] = 1;				// 标记新格子
					return;
				}
				target--;
			}
		}
	}
}

int move(int dx, int dy)
{
	int moved = 0;
	int merged[GRID_SIZE][GRID_SIZE] = {0};

	int x_start = dx > 0 ? GRID_SIZE - 1 : 0;
	int x_end = dx > 0 ? -1 : GRID_SIZE;
	int x_step = dx > 0 ? -1 : 1;

	int y_start = dy > 0 ? GRID_SIZE - 1 : 0;
	int y_end = dy > 0 ? -1 : GRID_SIZE;
	int y_step = dy > 0 ? -1 : 1;

	for (int y = y_start; y != y_end; y += y_step)
	{
		for (int x = x_start; x != x_end; x += x_step)
		{
			if (grid[y][x] == 0)
				continue;

			int nx = x;
			int ny = y;

			while (true)
			{
				int next_nx = nx + dx;
				int next_ny = ny + dy;

				if (next_nx < 0 || next_nx >= GRID_SIZE || next_ny < 0 || next_ny >= GRID_SIZE)
				{
					break;
				}

				if (grid[next_ny][next_nx] == 0)
				{
					grid[next_ny][next_nx] = grid[ny][nx];
					grid[ny][nx] = 0;
					nx = next_nx;
					ny = next_ny;
					moved = 1;
				}
				else if (grid[next_ny][next_nx] == grid[ny][nx] && !merged[next_ny][next_nx])
				{
					grid[next_ny][next_nx] += 1;
					grid[ny][nx] = 0;
					merged[next_ny][next_nx] = 1;
					moved = 1;
					break;
				}
				else
				{
					break;
				}
			}
		}
	}

	return moved;
}

int main()
{
	reset_sfrs();
	add_new_tile();
	add_new_tile();

	while (1)
	{
		draw_grid();

		auto kv = wait_kiko();
		int moved = 0;

		switch (*(ushort *)(&kv))
		{
		case 0x0880:
			moved = move(0, -1);
			break; // 上
		case 0x0820:
			moved = move(0, 1);
			break; // 下
		case 0x0440:
			moved = move(-1, 0);
			break; // 左
		case 0x1040:
			moved = move(1, 0);
			break; // 右
		}

		if (moved)
			add_new_tile();
	}
}