#include<iostream>
#include<ctime>
#include<string>
#include<stdio.h>
#include<fstream>
#include<cmath>
#include<cstdlib>
#include<conio.h>
#include<graphics.h>
#include<windows.h>
#include<Windows.h>
#include<mmsystem.h>//包含多媒体设备接口
#pragma comment(lib,"winmm.lib")//加载静态库
using namespace std;

int APLCNT = 0;
int TheBestScore;//目前只能读取前一次的分数，而是最高分
bool isd = false;
//----------------------------------------------------------------------------------
class Apple
{
public:
	int g_x;
	int g_y;
	bool is_large = false;
};
class Snake
{
public:
	int m_x;
	int m_y;
	bool is_exsist = false;
};
class Snakes
{
public:
	Snake* snake = new Snake[10000];//数组过大 放栈区可能导致溢出
	int m_savecnt = 0;  //记录Snake中有效位置的个数
	bool m_isDeath = false;
};


//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
void LoadMusic();
void InitialSnakes(Snakes& snakes, Apple& apl);
void PrintSnakes(Snakes& snakes, Apple& apl);
void SnakeGetApl(Snakes& snakes, Apple& apl);
void MoveSnakes(Snakes& snakes, Apple& apl);
void KeyOperator(Snakes& snakes, Apple& apl);
bool isDeath(Snakes& snakes);
void HadDied();
bool GameMenu();
void GameBeging();


//----------------------------------------------------------------------------------
void LoadMusic()
{
	mciSendString(TEXT("open SnakeBGM.mp3 alias BGM"), 0, 0, 0);
}
void HadDied()
{
	mciSendString(TEXT("open Clock.mp3"), 0, 0, 0);
	mciSendString(TEXT("play Clock.mp3"), 0, 0, 0);
	cleardevice();
	Sleep(1000);
	cout << "GAMEOVER" << endl;
}
bool isDeath(Snakes& snakes)
{
	//碰到自己，游戏结束
	bool ddd = false;
	for (int i = 1; i < snakes.m_savecnt; ++i)
	{
		if (
			snakes.snake[0].m_x == snakes.snake[i].m_x &&
			snakes.snake[0].m_y == snakes.snake[i].m_y
			)
		{
			ddd = true;
			break;
		}
	}
	//碰到墙壁，游戏结束
	if (snakes.snake[0].m_x < -10 || snakes.snake[0].m_y < -10
		|| snakes.snake[0].m_x > 490 || snakes.snake[0].m_y > 490
		|| ddd == true)
	{
		ddd = true;
		return true;
	}
	else return false;
}
void SnakeGetApl(Snakes& snakes, Apple& apl)
{
	//如果Snake碰到了APL，SnakeLength+=10
	if (!apl.is_large)//小果子
	{
		if (snakes.snake[0].m_x >= apl.g_x - 2 && snakes.snake[0].m_x <= apl.g_x + 12
			&& snakes.snake[0].m_y >= apl.g_y - 2 && snakes.snake[0].m_y <= apl.g_y + 12)
		{
			int temp_cnt = snakes.m_savecnt;
			snakes.m_savecnt += 10;
			for (int i = temp_cnt + 1; i < snakes.m_savecnt; ++i)
			{
				snakes.snake[i].is_exsist = true;
				snakes.snake[i].m_y = snakes.snake[temp_cnt].m_y;
				int _k = 1;
				snakes.snake[i].m_x = (snakes.snake[temp_cnt].m_x -= _k);
				++_k;
			}
			MoveSnakes(snakes, apl);
			bool is_Same = true;
			while (is_Same)
			{
				is_Same = false;
				apl.g_x = rand() % 476 + 2;
				apl.g_y = rand() % 476 + 2;
				for (int i = 0; i < snakes.m_savecnt; ++i)
				{
					if (apl.g_x == snakes.snake[i].m_x && apl.g_y == snakes.snake[i].m_y)
					{
						is_Same = true;
						break;
					}
				}
			}
			APLCNT++;//感觉外面可能也会用到这个int型参数，采用全局变量，初值为0
			const int aplcnt = 5;//吃aplcnt个变大
			if (APLCNT == aplcnt) apl.is_large = true;
		}
	}
	else//大果子
	{
		if (snakes.snake[0].m_x >= apl.g_x - 2 && snakes.snake[0].m_x <= apl.g_x + 22
			&& snakes.snake[0].m_y >= apl.g_y - 2 && snakes.snake[0].m_y <= apl.g_y + 22)
		{
			int temp_cnt = snakes.m_savecnt;
			snakes.m_savecnt += 50;
			for (int i = temp_cnt + 1; i < snakes.m_savecnt; ++i)
			{
				snakes.snake[i].is_exsist = true;
				snakes.snake[i].m_y = snakes.snake[temp_cnt].m_y;
				int _k = 1;
				snakes.snake[i].m_x = (snakes.snake[temp_cnt].m_x -= _k);
				++_k;
			}
			MoveSnakes(snakes, apl);
			bool is_Same = true;
			while (is_Same)
			{
				is_Same = false;
				apl.g_x = rand() % 476 + 2;
				apl.g_y = rand() % 476 + 2;
				for (int i = 0; i < snakes.m_savecnt; ++i)
				{
					if (apl.g_x == snakes.snake[i].m_x && apl.g_y == snakes.snake[i].m_y)
					{
						is_Same = true;
						break;
					}
				}
			}
			APLCNT = 0;
			apl.is_large = false;
		}
	}
}

//----------------------------------------------------------------------------------


//###################################   UI界面   #########################################
//----------------------------------------------------------------------------------------
void LoadandPut//dx为图片长度， x为坐标
(
	LPCTSTR name1, double dx1, double dy1, double x1, double y1,
	LPCTSTR name2, double dx2, double dy2, double x2, double y2,
	LPCTSTR name3, double dx3, double dy3, double x3, double y3,
	double k
)
{
	mciSendString(TEXT("open ButtonBKM.mp3"), 0, 0, 0);
	mciSendString(TEXT("play ButtonBKM.mp3 repeat"), 0, 0, 0);

flag://查看最高分后回到初始界面而不是直接退出

	//加载总背景图片
	IMAGE img0;
	loadimage(&img0, TEXT("ButtonBackGround1.png"), 900, 600, true);
	putimage(0, 0, &img0);

	//双下标为放大图，单下表为原大小图
	IMAGE img1;
	loadimage(&img1, name1, dx1, dy1, true);
	putimage(x1, y1, &img1);
	IMAGE img11;
	//double k = 1.5;//放大倍数
	loadimage(&img11, name1, k * dx1, k * dy1, true);//放大k倍

	IMAGE img2;
	loadimage(&img2, name2, dx2, dy2, true);
	putimage(x2, y2, &img2);
	IMAGE img22;
	loadimage(&img22, name2, k * dx2, k * dy2, true);

	IMAGE img3;
	loadimage(&img3, name3, dx3, dy3, true);
	putimage(x3, y3, &img3);
	IMAGE img33;
	loadimage(&img33, name3, k * dx3, k * dy3, true);

	BeginBatchDraw();
	while (1)
	{
		MOUSEMSG msg = GetMouseMsg();

		if (msg.x >= x1 && msg.x <= x1 + dx1 && msg.y >= y1 && msg.y <= y1 + dy1)
		{
			cleardevice();
			putimage(0, 0, &img0);
			putimage(x1 - ((k - 1) / 2) * dx1, y1 - ((k - 1) / 2) * dy1, &img11);//调整位置(中心不变)
			putimage(x2, y2, &img2);
			putimage(x3, y3, &img3);
			FlushBatchDraw();

			if (msg.uMsg == WM_LBUTTONDOWN)
			{
				mciSendString(TEXT("open ButtonEFM.mp3"), 0, 0, 0);
				mciSendString(TEXT("play ButtonEFM.mp3"), 0, 0, 0);
				//真正使用时这段代码可以替换为游戏入口代码
				//这个位置插入进入下一步的代码
				//----------------------------------------
				if (GameMenu())//加此代码点击开始游戏后还需点击小A头像才能进游戏
				{
					GameBeging();
					//存档（最高分）
					ofstream fout;
					fout.open("TheBestScore.txt", ios::out);
					fout << TheBestScore << endl;
				}
				return;
			}
		}
		else if (msg.x >= x2 && msg.x <= x2 + dx2 && msg.y >= y2 && msg.y <= y2 + dy2)
		{
			cleardevice();
			putimage(0, 0, &img0);
			putimage(x2 - ((k - 1) / 2) * dx2, y2 - ((k - 1) / 2) * dy2, &img22);//调整位置(中心不变)
			putimage(x1, y1, &img1);
			putimage(x3, y3, &img3);
			FlushBatchDraw();

			if (msg.uMsg == WM_LBUTTONDOWN)
			{
				mciSendString(TEXT("open ButtonEFM.mp3"), 0, 0, 0);
				mciSendString(TEXT("play ButtonEFM.mp3"), 0, 0, 0);
				//真正使用时这段代码可以替换为游戏入口代码
				//这个位置插入进入下一步的代码
				//----------------------------------------
				//读档（最高分）
				ifstream ifs;
				ifs.open("TheBestScore.txt", ios::in);
				if (!ifs.is_open())
				{
					cout << "文件打开失败" << endl;
					return;
				}
				char buf[1024] = { 0 };
				while (ifs.getline(buf, sizeof(buf)))
				{
					cout << "当前的最高分为：" << buf << endl;			
				}
				goto flag;
			}
		}
		else if (msg.x >= x3 && msg.x <= x3 + dx3 && msg.y >= y3 && msg.y <= y3 + dy3)
		{
			cleardevice();
			putimage(0, 0, &img0);
			putimage(x3 - ((k - 1) / 2) * dx3, y3 - ((k - 1) / 2) * dy3, &img33);//调整位置(中心不变)
			putimage(x1, y1, &img1);
			putimage(x2, y2, &img2);
			FlushBatchDraw();

			if (msg.uMsg == WM_LBUTTONDOWN)
			{
				mciSendString(TEXT("open ButtonEFM.mp3"), 0, 0, 0);
				mciSendString(TEXT("play ButtonEFM.mp3"), 0, 0, 0);
				//真正使用时这段代码可以替换为游戏入口代码
				//这个位置插入进入下一步的代码
				//----------------------------------------
				cleardevice();
				IMAGE exitimg;
				loadimage(&exitimg, TEXT("fireground.jpg"), 0, 0, true);
				putimage(0, 0, &exitimg);

				//文本输入，用于提示游戏成功退出
				setbkmode(TRANSPARENT); // 透明显示文字
				settextcolor(BLACK);  // 设置字体颜色
				settextstyle(50, 0, _T("黑体")); // 设置文字大小、字体
				outtextxy(80, 100, TEXT("已退出游戏进程\t"));
				outtextxy(80, 350, TEXT("请按任意键退出\t"));

				FlushBatchDraw();
				_getch();
				return;
			}
		}
		//鼠标不在指定位置时摆上按钮原图（不放大）
		else
		{
			cleardevice();
			putimage(0, 0, &img0);
			putimage(x1, y1, &img1);
			putimage(x2, y2, &img2);
			putimage(x3, y3, &img3);
			FlushBatchDraw();
		}
	}
	EndBatchDraw();
}
//简单地调用界面
void Menu()
{
	LoadandPut(TEXT("Start.jpg"), 100, 50, 190, 70,
		TEXT("Load.jpg"), 100, 50, 190, 220,
		TEXT("Exit.jpg"), 100, 50, 190, 370, 1.2);
}
//小A头像
bool GameMenu()
{
	cleardevice();
	IMAGE img;
	loadimage(&img, TEXT("Start.png"), 100, 200, true);
	putimage(200, 150, &img);
	FlushBatchDraw();
	while (1)
	{
		MOUSEMSG msg = GetMouseMsg();
		if (msg.x >= 220 && msg.x <= 266 && msg.y >= 200 && msg.y <= 286)
		{
			if (msg.uMsg == WM_LBUTTONDOWN)
			{
				cleardevice();
				return true;
			}
		}
	}
}
//########################################################################################
void GameBeging()
{
	LoadMusic();
	mciSendString(TEXT("close ButtonBKM.mp3"), 0, 0, 0);
	mciSendString(TEXT("play BGM repeat"), 0, 0, 0);
	IMAGE img;  //载入背景图片
	loadimage(&img, TEXT("ButtonBackGround1.png"), 480, 480, false);
	putimage(0, 0, &img);
	//游戏开始
	//随机生成果子给蛇蛇吃
	Apple apl;
	Snakes snakes;
	InitialSnakes(snakes, apl);//初始化蛇和果子
	PrintSnakes(snakes, apl);
	//游戏代码调用核心
	BeginBatchDraw();
	while (1)
	{
		KeyOperator(snakes, apl);
		if (isd)
		{
			TheBestScore = snakes.m_savecnt;//记录最高分
			HadDied();
			system("pause");
			delete[]snakes.snake;
			EndBatchDraw();
			_getch();
			closegraph();
			return;
		}
		FlushBatchDraw();//此代码仅在可斜着走模式中有意义
		//Sleep(1);//控制蛇速,此代码仅在可斜着走模式中有意义
		SnakeGetApl(snakes, apl);
	}
	EndBatchDraw();
}
int main()
{
	initgraph(480, 480, SHOWCONSOLE);//初始化界面
	srand((unsigned int)time(NULL));//播撒随机数种子
	BeginBatchDraw();

	Menu();

	EndBatchDraw();
	return 0;
}
//#######################################以下函数不可修改############################################
void InitialSnakes(Snakes& snakes, Apple& apl)
{
	for (int i = 99; i >= 0; --i)//初始化蛇蛇的长度为50,改成150更好玩不然太短了
	{
		snakes.snake[snakes.m_savecnt].m_x = i;
		snakes.snake[snakes.m_savecnt].m_y = 1;
		snakes.snake[snakes.m_savecnt].is_exsist = true;
		snakes.m_savecnt++;
	}
	apl.g_x = rand() % 479 + 1;
	apl.g_y = rand() % 479 + 1;
	setfillcolor(BROWN);
	fillrectangle(apl.g_x, apl.g_y, apl.g_x + 10, apl.g_y + 10);//画矩形区域的果果
}
void MoveSnakes(Snakes& snakes, Apple& apl)
{
	for (int i = snakes.m_savecnt - 1; i >= 1; --i)
	{
		snakes.snake[i].m_x = snakes.snake[i - 1].m_x;
		snakes.snake[i].m_y = snakes.snake[i - 1].m_y;
	}
	PrintSnakes(snakes, apl);
}
//第二个位置在调用瞬间与第一个位置重合，注意调用时需先调用死亡判别函数
int N = 2;//N可以 控制蛇速 和 初始长度 (因为每个蛇点的半径大于点的半径，所以直观上感觉蛇变长了，当N较大时出现断点)
int slp = 10;//Sleep的时间
char temp_key;
void KeyOperator(Snakes& snakes, Apple& apl)//移动蛇头
{
	//这个代码不能斜着走
	if (_kbhit())
	{
		char key = _getch();
		switch (key)
		{
		case 'w':
		case 'W':
			temp_key = key;
		flagW:
			while (!_kbhit() && !isd)
			{
				//防误触，注意此处不与紧跟着的蛇位比较（观察蛇移动函数）
				if (snakes.snake[0].m_y > snakes.snake[2].m_y)
				{
					snakes.snake[0].m_y += N;
				}
				else snakes.snake[0].m_y -= N;
				Sleep(slp);//控制蛇速,此项若过高会导致卡顿
				isd = isDeath(snakes);
				MoveSnakes(snakes, apl);
				SnakeGetApl(snakes, apl);
			}
			break;
		case's':
		case'S':
			temp_key = key;
		flagS:
			while (!_kbhit() && !isd)
			{
				if (snakes.snake[0].m_y < snakes.snake[2].m_y)
				{
					snakes.snake[0].m_y -= N;
				}
				else snakes.snake[0].m_y += N;
				Sleep(slp);
				isd = isDeath(snakes);
				MoveSnakes(snakes, apl);
				SnakeGetApl(snakes, apl);
			}
			break;
		case'a':
		case'A':
			temp_key = key;
		flagA:
			while (!_kbhit() && !isd)
			{
				if (snakes.snake[0].m_x > snakes.snake[2].m_x)
				{
					snakes.snake[0].m_x += N;
				}
				else snakes.snake[0].m_x -= N;
				Sleep(slp);
				isd = isDeath(snakes);
				MoveSnakes(snakes, apl);
				SnakeGetApl(snakes, apl);
			}
			break;
		case'd':
		case'D':
			temp_key = key;
		flagD:
			while (!_kbhit() && !isd)
			{
				if (snakes.snake[0].m_x < snakes.snake[2].m_x)
				{
					snakes.snake[0].m_x -= N;
				}
				else snakes.snake[0].m_x += N;
				Sleep(slp);
				isd = isDeath(snakes);
				MoveSnakes(snakes, apl);
				SnakeGetApl(snakes, apl);
			}
			break;
		case' ':
		{
			if (slp != 0) slp = 0;
			else slp = 10;
			if (temp_key == 'W'||temp_key == 'w') goto flagW;
			if (temp_key == 'S'|| temp_key == 's') goto flagS;
			if (temp_key == 'A' || temp_key == 'a') goto flagA;
			if (temp_key == 'D' || temp_key == 'd') goto flagD;
		}
		break;
		default:
			break;
		}
	}

	/*if (GetAsyncKeyState(VK_UP))
	{
		snakes.snake[0].m_y--; isd = isDeath(snakes); MoveSnakes(snakes, apl);
	}
	if (GetAsyncKeyState(VK_DOWN))
	{
		snakes.snake[0].m_y++; isd = isDeath(snakes); MoveSnakes(snakes, apl);
	}
	if (GetAsyncKeyState(VK_LEFT))
	{
		snakes.snake[0].m_x--; isd = isDeath(snakes); MoveSnakes(snakes, apl);
	}
	if (GetAsyncKeyState(VK_RIGHT))
	{
		snakes.snake[0].m_x++; isd = isDeath(snakes); MoveSnakes(snakes, apl);
	}*/

}
void PrintSnakes(Snakes& snakes, Apple& apl)
{

	cleardevice();//清屏
	if (isd)//载入死亡背景
	{
		IMAGE Dimg;
		loadimage(&Dimg, TEXT("Dth.jpg"), 480, 480, false);
		putimage(0, 0, &Dimg);
	}
	else
	{
		IMAGE img;  //载入背景图片
		loadimage(&img, TEXT("ButtonBackGround1.png"), 480, 480, false);
		putimage(0, 0, &img);
		//-----------------------------------------------------------
		//文本输入，用于计分
		setbkmode(TRANSPARENT); // 透明显示文字
		settextcolor(BLACK);  // 设置字体颜色
		settextstyle(20, 0, _T("微软雅黑")); // 设置文字大小、字体
		outtextxy(0, 0, TEXT("蛇蛇的长度为："));
		//打印数字，需要先转成字符串
		TCHAR s[20];
		swprintf_s(s, 20, _T("%dm"), snakes.m_savecnt);
		outtextxy(0, 20, s);
		//------------------------------------------------------------

		//画果子
		if (!apl.is_large)
		{
			setfillcolor(BROWN);
			fillrectangle(apl.g_x, apl.g_y, apl.g_x + 10, apl.g_y + 10);
		}
		else
		{
			setfillcolor(RED);
			fillrectangle(apl.g_x, apl.g_y, apl.g_x + 20, apl.g_y + 20);
		}

		//蛇头带有红色边框，后续操作移动的是蛇头
		setfillcolor(RED);
		solidcircle(snakes.snake[0].m_x, snakes.snake[0].m_y, 5);
		//其余部分绿色
		setfillcolor(GREEN);
		for (int i = 1; i < snakes.m_savecnt; ++i)
		{
			if (snakes.snake[i].is_exsist == true)
			{
				solidcircle(snakes.snake[i].m_x, snakes.snake[i].m_y, 5);
			}
		}
	}
	FlushBatchDraw();//此代码在可斜着走模式中可删去
}