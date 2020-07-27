/*
	1.数据设计
	2.模块化设计
	3.实现过程

*/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>	//光标设置api
#include <time.h>		//食物随机
#include <conio.h>		//按键监控
//辅助宏定义
#define MAPHEIGHT 25	//地图大小
#define MAPWIDTH  60
#define SNAKESIZE 50	//蛇最大节数

// 数据设计
////食物结构体
struct
{
	//坐标定位食物
	int x;
	int y;
}food;
////蛇结构体
struct
{
	//记录每一节蛇的坐标
	int x[SNAKESIZE];
	int y[SNAKESIZE];
	int len;	//蛇的长度
	int speed;	//蛇的速度
}snake;
////全局变量
int key = 'w';		//初始化运动方向
int last_key = 0;	//记录上一次按键
int changeFlag = 0;	//变化标记


// 模块化设计-->功能的划分-->抽象到函数（根据业务逻辑）
////1.画地图
void drawMap();
////2.产生食物
void createFood();
////3.按键操作
void keyDown();
////4.蛇的状态：判断是否结束游戏
int snakeStatus();
////5.辅助函数：光标移动
void gotoxy(int x, int y);


int main()
{
	drawMap();
	while (1) 
	{
		createFood();
		Sleep(snake.speed);
		keyDown();
		if (!snakeStatus())
		{
			break;
		}
	}
	gotoxy(MAPWIDTH / 2, MAPHEIGHT / 2);
	printf("GG");

	system("pause");
	return 0;
}

void drawMap()
{
	//● ■
	srand((unsigned int)time(NULL));	//随机函数种子
	//1.圈地
		//左右边框
	for (int i = 0; i <= MAPHEIGHT; i++)
	{
		gotoxy(0, i);
		printf("x");
		gotoxy(MAPWIDTH, i);
		printf("x");
	}
	//上下边框
	for (int i = 0; i <= MAPWIDTH; i++)
	{
		gotoxy(i, 0);
		printf("x");
		gotoxy(i, MAPHEIGHT);
		printf("x");
	}
	//2.画蛇
		//确定蛇的属性
	snake.len = 3;
	snake.speed = 200;
	snake.x[0] = MAPWIDTH / 2;	//蛇头在屏幕中间
	snake.y[0] = MAPHEIGHT / 2;
	//画蛇头
	gotoxy(snake.x[0], snake.y[0]);
	printf("■");
	//画剩下的身体
	for (int k = 1; k < snake.len; k++)
	{
		snake.x[k] = snake.x[k - 1] + 2;
		snake.y[k] = snake.y[k - 1];
		gotoxy(snake.x[k], snake.y[k]);
		printf("■");
	}


	//3.画食物
		//确定坐标
	while (1)
	{
		food.x = rand() % (MAPWIDTH - 4);
		food.y = rand() % (MAPHEIGHT - 2);
		if (food.x % 2 == 0)
		{
			break;
		}
	}
	//画食物
	gotoxy(food.x, food.y);
	printf("●");
}

void createFood()
{
	//蛇把食物吃了
	if (snake.x[0] == food.x && snake.y[0] == food.y)
	{
		srand((unsigned int)time(NULL));
		//产生新的食物不能在蛇的身上，并且坐标是偶数
		while (1)
		{
			int flag = 1;
			food.x = rand() % (MAPWIDTH - 4) + 2;
			food.y = rand() % (MAPHEIGHT - 2) + 1;
			//判断新食物是否在蛇的身上，是的话重置
			for (int k = 0; k < snake.len; k++)
			{
				if (snake.x[k] == food.x && snake.y[k] == food.y)
				{
					flag = 0;	//需要重置食物
					break;
				}
			}
			if (flag && food.x % 2 == 0)
			{
				break;
			}
		}
		gotoxy(food.x, food.y);
		printf("●");
		snake.len++;
		changeFlag = 1;	//蛇的标记
	}
}

void keyDown()
{
	//无按键的处理
	if (_kbhit())
	{	//有按键就接受
		fflush(stdin);
		key = _getch();
	}
	//擦除最后的位置
	if (!changeFlag)
	{
		gotoxy(snake.x[snake.len-1], snake.y[snake.len-1]);
		printf("  ");
	}
	//后面的蛇身往前挪
	for (int i = snake.len - 1; i > 0; i--)
	{
		snake.x[i] = snake.x[i - 1];
		snake.y[i] = snake.y[i - 1];
	}
	//移动方向的处理
	switch (key)
	{
	//上:y-
	case'w':
	case'W':
		snake.y[0]--;
		break;
	//下：y+
	case's':
	case'S':
		snake.y[0]++;
		break;
	//左：x-
	case'a':
	case'A':
		snake.x[0]-=2;
		break;
	case'd':
	case'D':
		snake.x[0]+=2;
		break;
	}
	gotoxy(snake.x[0], snake.y[0]);
	printf("■");
	changeFlag = 0;
	gotoxy(MAPHEIGHT + 2, 0);
}


int snakeStatus()
{
	//判断是否撞墙
	if (snake.x[0] == 0 || snake.x[0] == MAPHEIGHT - 2 || snake.y[0] == 0 || snake.y[0] == MAPWIDTH - 1)
	{
		return 0;
	}
	//判断是否撞自己
	for (int k = 1; k < snake.len; k++)
	{
		if (snake.x[0] == snake.x[k] && snake.y[k] == snake.y[0])
		{
			return 0;
		}
	}
	return 1;
}

void gotoxy(int x, int y)
{
	//调用win32API设置控制台光标位置
	//1.找到控制台的窗口(获取句柄）
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	//2.光标的结构体
	COORD coord;
	//3.设置坐标
	coord.X = x;
	coord.Y = y;
	//4.同步到控制台
	SetConsoleCursorPosition(handle, coord);
}