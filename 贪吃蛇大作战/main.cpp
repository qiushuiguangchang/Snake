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
#include<mmsystem.h>//������ý���豸�ӿ�
#pragma comment(lib,"winmm.lib")//���ؾ�̬��
using namespace std;

int APLCNT = 0;
int TheBestScore;//Ŀǰֻ�ܶ�ȡǰһ�εķ�����������߷�
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
	Snake* snake = new Snake[10000];//������� ��ջ�����ܵ������
	int m_savecnt = 0;  //��¼Snake����Чλ�õĸ���
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
	//�����Լ�����Ϸ����
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
	//����ǽ�ڣ���Ϸ����
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
	//���Snake������APL��SnakeLength+=10
	if (!apl.is_large)//С����
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
			APLCNT++;//�о��������Ҳ���õ����int�Ͳ���������ȫ�ֱ�������ֵΪ0
			const int aplcnt = 5;//��aplcnt�����
			if (APLCNT == aplcnt) apl.is_large = true;
		}
	}
	else//�����
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


//###################################   UI����   #########################################
//----------------------------------------------------------------------------------------
void LoadandPut//dxΪͼƬ���ȣ� xΪ����
(
	LPCTSTR name1, double dx1, double dy1, double x1, double y1,
	LPCTSTR name2, double dx2, double dy2, double x2, double y2,
	LPCTSTR name3, double dx3, double dy3, double x3, double y3,
	double k
)
{
	mciSendString(TEXT("open ButtonBKM.mp3"), 0, 0, 0);
	mciSendString(TEXT("play ButtonBKM.mp3 repeat"), 0, 0, 0);

flag://�鿴��߷ֺ�ص���ʼ���������ֱ���˳�

	//�����ܱ���ͼƬ
	IMAGE img0;
	loadimage(&img0, TEXT("ButtonBackGround1.png"), 900, 600, true);
	putimage(0, 0, &img0);

	//˫�±�Ϊ�Ŵ�ͼ�����±�Ϊԭ��Сͼ
	IMAGE img1;
	loadimage(&img1, name1, dx1, dy1, true);
	putimage(x1, y1, &img1);
	IMAGE img11;
	//double k = 1.5;//�Ŵ���
	loadimage(&img11, name1, k * dx1, k * dy1, true);//�Ŵ�k��

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
			putimage(x1 - ((k - 1) / 2) * dx1, y1 - ((k - 1) / 2) * dy1, &img11);//����λ��(���Ĳ���)
			putimage(x2, y2, &img2);
			putimage(x3, y3, &img3);
			FlushBatchDraw();

			if (msg.uMsg == WM_LBUTTONDOWN)
			{
				mciSendString(TEXT("open ButtonEFM.mp3"), 0, 0, 0);
				mciSendString(TEXT("play ButtonEFM.mp3"), 0, 0, 0);
				//����ʹ��ʱ��δ�������滻Ϊ��Ϸ��ڴ���
				//���λ�ò��������һ���Ĵ���
				//----------------------------------------
				if (GameMenu())//�Ӵ˴�������ʼ��Ϸ������СAͷ����ܽ���Ϸ
				{
					GameBeging();
					//�浵����߷֣�
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
			putimage(x2 - ((k - 1) / 2) * dx2, y2 - ((k - 1) / 2) * dy2, &img22);//����λ��(���Ĳ���)
			putimage(x1, y1, &img1);
			putimage(x3, y3, &img3);
			FlushBatchDraw();

			if (msg.uMsg == WM_LBUTTONDOWN)
			{
				mciSendString(TEXT("open ButtonEFM.mp3"), 0, 0, 0);
				mciSendString(TEXT("play ButtonEFM.mp3"), 0, 0, 0);
				//����ʹ��ʱ��δ�������滻Ϊ��Ϸ��ڴ���
				//���λ�ò��������һ���Ĵ���
				//----------------------------------------
				//��������߷֣�
				ifstream ifs;
				ifs.open("TheBestScore.txt", ios::in);
				if (!ifs.is_open())
				{
					cout << "�ļ���ʧ��" << endl;
					return;
				}
				char buf[1024] = { 0 };
				while (ifs.getline(buf, sizeof(buf)))
				{
					cout << "��ǰ����߷�Ϊ��" << buf << endl;			
				}
				goto flag;
			}
		}
		else if (msg.x >= x3 && msg.x <= x3 + dx3 && msg.y >= y3 && msg.y <= y3 + dy3)
		{
			cleardevice();
			putimage(0, 0, &img0);
			putimage(x3 - ((k - 1) / 2) * dx3, y3 - ((k - 1) / 2) * dy3, &img33);//����λ��(���Ĳ���)
			putimage(x1, y1, &img1);
			putimage(x2, y2, &img2);
			FlushBatchDraw();

			if (msg.uMsg == WM_LBUTTONDOWN)
			{
				mciSendString(TEXT("open ButtonEFM.mp3"), 0, 0, 0);
				mciSendString(TEXT("play ButtonEFM.mp3"), 0, 0, 0);
				//����ʹ��ʱ��δ�������滻Ϊ��Ϸ��ڴ���
				//���λ�ò��������һ���Ĵ���
				//----------------------------------------
				cleardevice();
				IMAGE exitimg;
				loadimage(&exitimg, TEXT("fireground.jpg"), 0, 0, true);
				putimage(0, 0, &exitimg);

				//�ı����룬������ʾ��Ϸ�ɹ��˳�
				setbkmode(TRANSPARENT); // ͸����ʾ����
				settextcolor(BLACK);  // ����������ɫ
				settextstyle(50, 0, _T("����")); // �������ִ�С������
				outtextxy(80, 100, TEXT("���˳���Ϸ����\t"));
				outtextxy(80, 350, TEXT("�밴������˳�\t"));

				FlushBatchDraw();
				_getch();
				return;
			}
		}
		//��겻��ָ��λ��ʱ���ϰ�ťԭͼ�����Ŵ�
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
//�򵥵ص��ý���
void Menu()
{
	LoadandPut(TEXT("Start.jpg"), 100, 50, 190, 70,
		TEXT("Load.jpg"), 100, 50, 190, 220,
		TEXT("Exit.jpg"), 100, 50, 190, 370, 1.2);
}
//СAͷ��
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
	IMAGE img;  //���뱳��ͼƬ
	loadimage(&img, TEXT("ButtonBackGround1.png"), 480, 480, false);
	putimage(0, 0, &img);
	//��Ϸ��ʼ
	//������ɹ��Ӹ����߳�
	Apple apl;
	Snakes snakes;
	InitialSnakes(snakes, apl);//��ʼ���ߺ͹���
	PrintSnakes(snakes, apl);
	//��Ϸ������ú���
	BeginBatchDraw();
	while (1)
	{
		KeyOperator(snakes, apl);
		if (isd)
		{
			TheBestScore = snakes.m_savecnt;//��¼��߷�
			HadDied();
			system("pause");
			delete[]snakes.snake;
			EndBatchDraw();
			_getch();
			closegraph();
			return;
		}
		FlushBatchDraw();//�˴�����ڿ�б����ģʽ��������
		//Sleep(1);//��������,�˴�����ڿ�б����ģʽ��������
		SnakeGetApl(snakes, apl);
	}
	EndBatchDraw();
}
int main()
{
	initgraph(480, 480, SHOWCONSOLE);//��ʼ������
	srand((unsigned int)time(NULL));//�������������
	BeginBatchDraw();

	Menu();

	EndBatchDraw();
	return 0;
}
//#######################################���º��������޸�############################################
void InitialSnakes(Snakes& snakes, Apple& apl)
{
	for (int i = 99; i >= 0; --i)//��ʼ�����ߵĳ���Ϊ50,�ĳ�150�����治Ȼ̫����
	{
		snakes.snake[snakes.m_savecnt].m_x = i;
		snakes.snake[snakes.m_savecnt].m_y = 1;
		snakes.snake[snakes.m_savecnt].is_exsist = true;
		snakes.m_savecnt++;
	}
	apl.g_x = rand() % 479 + 1;
	apl.g_y = rand() % 479 + 1;
	setfillcolor(BROWN);
	fillrectangle(apl.g_x, apl.g_y, apl.g_x + 10, apl.g_y + 10);//����������Ĺ���
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
//�ڶ���λ���ڵ���˲�����һ��λ���غϣ�ע�����ʱ���ȵ��������б���
int N = 2;//N���� �������� �� ��ʼ���� (��Ϊÿ���ߵ�İ뾶���ڵ�İ뾶������ֱ���ϸо��߱䳤�ˣ���N�ϴ�ʱ���ֶϵ�)
int slp = 10;//Sleep��ʱ��
char temp_key;
void KeyOperator(Snakes& snakes, Apple& apl)//�ƶ���ͷ
{
	//������벻��б����
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
				//���󴥣�ע��˴���������ŵ���λ�Ƚϣ��۲����ƶ�������
				if (snakes.snake[0].m_y > snakes.snake[2].m_y)
				{
					snakes.snake[0].m_y += N;
				}
				else snakes.snake[0].m_y -= N;
				Sleep(slp);//��������,���������߻ᵼ�¿���
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

	cleardevice();//����
	if (isd)//������������
	{
		IMAGE Dimg;
		loadimage(&Dimg, TEXT("Dth.jpg"), 480, 480, false);
		putimage(0, 0, &Dimg);
	}
	else
	{
		IMAGE img;  //���뱳��ͼƬ
		loadimage(&img, TEXT("ButtonBackGround1.png"), 480, 480, false);
		putimage(0, 0, &img);
		//-----------------------------------------------------------
		//�ı����룬���ڼƷ�
		setbkmode(TRANSPARENT); // ͸����ʾ����
		settextcolor(BLACK);  // ����������ɫ
		settextstyle(20, 0, _T("΢���ź�")); // �������ִ�С������
		outtextxy(0, 0, TEXT("���ߵĳ���Ϊ��"));
		//��ӡ���֣���Ҫ��ת���ַ���
		TCHAR s[20];
		swprintf_s(s, 20, _T("%dm"), snakes.m_savecnt);
		outtextxy(0, 20, s);
		//------------------------------------------------------------

		//������
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

		//��ͷ���к�ɫ�߿򣬺��������ƶ�������ͷ
		setfillcolor(RED);
		solidcircle(snakes.snake[0].m_x, snakes.snake[0].m_y, 5);
		//���ಿ����ɫ
		setfillcolor(GREEN);
		for (int i = 1; i < snakes.m_savecnt; ++i)
		{
			if (snakes.snake[i].is_exsist == true)
			{
				solidcircle(snakes.snake[i].m_x, snakes.snake[i].m_y, 5);
			}
		}
	}
	FlushBatchDraw();//�˴����ڿ�б����ģʽ�п�ɾȥ
}