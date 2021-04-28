#include <graphics.h>		// 引用图形库头文件
#include <conio.h>
#include<math.h>
#include<string>
#include<fstream>
#include<vector>
#include<iostream>
#pragma comment(lib,"Winmm.lib")
using namespace std;

#define High 800
#define Width 590


//函数声明
void getrank();

class Rank
{
public:
	string name;
	int score;
	Rank(string Name = "null", int Score = 0)
	{
		name = Name;
		score = Score;
	}
};
vector<Rank> Ranks(3);

class Enemy
{
public:
	int x, y;
	static int alive_num;
	Enemy(int xx = 0, int yy = 0)
	{
		x = xx;
		y =yy;
	}
};
Enemy Enemies[10];
int Enemy::alive_num=0;

class Enemy_Bullet
{
public:
	int x, y;
	static int alive_num;
	Enemy_Bullet(int xx = 0, int yy = 0)
	{
		x = xx;
		y = yy;
	}
};
Enemy_Bullet Enemy_Bullets[20];
int  Enemy_Bullet::alive_num;

class My_Bullet
{
public:
	int x, y;
	static int alive_num;
	My_Bullet(int xx = 0, int yy = 0, int S = 1)
	{
		x = xx;
		y = yy;
	}
};
My_Bullet My_Bullets[20];
int My_Bullet::alive_num = 0;

clock_t startClock, processClock;

int whichpage = 1;//1表示初始化，即说明菜单界面。2表示游戏界面。3表示游戏中的排行榜界面，4表示说明界面，5表示退出确定界面,6表示游戏内的菜单界面,7表示回到主菜单确定界面,8游戏结束界面；
float position_x, position_y; // 飞机位置
float bullet_x, bullet_y; // 自己子弹位置
int score = 0; // 得分
int Round = 0;//第几轮
//int speed = 1000;
int life = 5;
int duration=40;
bool flag = 0;//表示时间相差小于2；
bool haveinput = 0;
int isfirst = 10000;

IMAGE img_bk,img_rank;//背景图片
IMAGE img_planeNormal1, img_planeNormal2; // 正常飞机图片，1是掩码，2是正常
IMAGE img_planeExplode1, img_planeExplode2; // 爆炸飞机图片，1是掩码，2是正常
IMAGE img_bullet1, img_bullet2; // 子弹图片，1是掩码，2是正常
IMAGE img_enemyPlane1, img_enemyPlane2; // 敌机图片，1是掩码，2是正常
IMAGE img_menu;//初始界面
IMAGE img_showhowtoplay;//说明界面
IMAGE img_confirm;//退出确定界面
IMAGE img_pause;//暂停符号
IMAGE img_home;//房子符号
IMAGE img_confirm2;
IMAGE img_fail;

void startup() {//引入图片，音乐，绘图；

	getrank();//读入排行榜文件并存在Rank类

	initgraph(Width,High);
	whichpage = 4;
	setbkmode(TRANSPARENT);//使用透明的输出，也就是文字的背景是不改变的


	mciSendString("open game_music.mp3 alias bkmusic", NULL, 0, NULL);
	mciSendString("play bkmusic repeat", NULL, 0, NULL);  // 引入音乐，并且循环播放

	loadimage(&img_rank, "rank2.png");
	loadimage(&img_bk, "background.jpg");
	loadimage(&img_planeNormal1, "planeNormal_1.jpg");
	loadimage(&img_planeNormal2, "planeNormal_2.jpg");
	loadimage(&img_bullet1, "bullet1.jpg");
	loadimage(&img_bullet2, "bullet2.jpg");
	loadimage(&img_enemyPlane1, "enemyPlane1.jpg");
	loadimage(&img_enemyPlane2, "enemyPlane2.jpg");
	loadimage(&img_planeExplode1, "planeExplode_1.jpg");
	loadimage(&img_planeExplode2, "planeExplode_2.jpg");
	loadimage(&img_menu, "star.jpg");//引入初始化界面
	loadimage(&img_showhowtoplay, "showhowtoplay.jpg");
	loadimage(&img_confirm, "confirm.jpg");
	loadimage(&img_pause, "pause.jpg");
	loadimage(&img_home, "home.jpg");
	loadimage(&img_confirm2, "confirm2.jpg");
	loadimage(&img_fail, "fail.jpg");
	//引入图片，将其通过image保存

	position_x = Width * 0.5;
	position_y = High * 0.7;


	BeginBatchDraw();
}
void getrank() {
	fstream infile("rank.txt");
	string tempstring;int tempscore;
	for (int i = 0;i <= 2;i++)
	{
		infile>>tempstring >> tempscore;
		Ranks[i].name = tempstring;Ranks[i].score = tempscore;
	}
}
void writerank(int Score,char *name) {
	Rank temp(name, Score);
	if (temp.name=="")
	{
		temp.name = "null";
	}
	if (Score > Ranks[0].score)
	{
		Ranks.insert(Ranks.begin(), temp);
		Ranks.pop_back();
	}
	else
	{
		Ranks.insert(Ranks.begin()+1, temp);
		Ranks.pop_back();
	}
	ofstream infile("rank.txt", ios::out);
	for (int i = 0;i <= 2;i++)
	{
		infile << Ranks[i].name << " " << Ranks[i].score << endl;
	}
	infile.close();
}
void showmenu() {

	putimage(0, 0, Width, High, &img_menu, 200, 100);

	//setcolor(YELLOW);
	//settextcolor(BLUE);
	setbkmode(OPAQUE);//文字背景透明

	setfillcolor(YELLOW);
	rectangle(Width / 2 - 60, High / 5, Width / 2 + 80, High / 5 + 40);
	fillcircle(Width / 2 - 80, High / 5 + 20, 20);
	outtextxy(Width / 2 - 30, High / 5+10, "无尽模式");//游戏入口

	setfillcolor(BLUE);
	rectangle(Width / 2 - 60, High / 5+60, Width / 2 + 80, High / 5 + 100);
	fillcircle(Width / 2 - 80, High / 5+80 , 20);
	outtextxy(Width / 2 - 30, High / 5 + 70, "排行榜");//游戏排行榜

	setfillcolor(GREEN);
	rectangle(Width / 2 - 60, High / 5+120, Width / 2 + 80, High / 5 + 160);
	fillcircle(Width / 2 - 80, High / 5 +140, 20);
	outtextxy(Width / 2 - 30, High / 5 + 130, "游戏说明");//游戏说明,里面有制作人员

	setfillcolor(WHITE);
	rectangle(Width / 2 - 60, High / 5+180, Width / 2 + 80, High / 5 + 220);
	fillcircle(Width / 2 - 80, High / 5 + 200, 20);
	outtextxy(Width / 2 - 30, High / 5 + 190, "退出游戏");//退出游戏
}
void showgame() {//游戏内界面
	putimage(0, 0, &img_bk);	// 显示背景	
	char ts[12];
	_itoa_s(score, ts, 10);
	outtextxy(460, 50, "score:");
	outtextxy(500, 50, ts);
	_itoa_s(Round, ts, 10);
	outtextxy(455, 70, "Round:");
	outtextxy(500, 70, ts);
	_itoa_s(life, ts, 10);
	outtextxy(476, 90, "life:");
	outtextxy(500, 90, ts);

	putimage(position_x - 50, position_y - 60, &img_planeNormal1, NOTSRCERASE); 
	putimage(position_x - 50, position_y - 60, &img_planeNormal2, SRCINVERT);//引入自己飞机

	//输出敌方飞机
	for (int i=0;i<=5;i++)
	{
		if (Enemies[i].x + Enemies[i].y)
		{
			putimage(Enemies[i].x, Enemies[i].y, &img_enemyPlane1, NOTSRCERASE);
			putimage(Enemies[i].x, Enemies[i].y, &img_enemyPlane2, SRCINVERT);
		}
	}

	//输出敌方子弹
	for (int i = 0;i <= 19;i++)
	{
		if (Enemy_Bullets[i].x + Enemy_Bullets[i].y)
		{
			putimage(Enemy_Bullets[i].x, Enemy_Bullets[i].y, &img_bullet1, NOTSRCERASE);
			putimage(Enemy_Bullets[i].x, Enemy_Bullets[i].y, &img_bullet2, SRCINVERT);
		}
	}

	//输出我方子弹
	for (int i = 0;i <= 19;i++)
	{
		if (My_Bullets[i].x + My_Bullets[i].y)
		{
			putimage(My_Bullets[i].x, My_Bullets[i].y, &img_bullet1, NOTSRCERASE);
			putimage(My_Bullets[i].x, My_Bullets[i].y, &img_bullet2, SRCINVERT);
		}
	}

}
void showinnermenu() {//游戏内继续，暂停菜单
	putimage(0, 0, &img_bk);
	putimage(Width/2-80,High/2-80, &img_pause);
	putimage(Width / 2 + 80, High / 2 - 80, &img_home);
}
void showrank()
{
	char ts[12];
	putimage(0, 0,&img_rank);
	outtextxy(250, 350, Ranks[0].name.c_str());
	_itoa_s(Ranks[0].score, ts, 10);
	outtextxy(400, 350,ts);//
	outtextxy(250, 480, Ranks[1].name.c_str());
	_itoa_s(Ranks[1].score, ts, 10);
	outtextxy(400, 480, ts);//
	outtextxy(250, 610, Ranks[2].name.c_str());
	_itoa_s(Ranks[2].score, ts, 10);
	outtextxy(400, 610, ts);
}
void showgameover() {//游戏结束
	putimage(0, 0, &img_fail);
	char ts[12];
	_itoa_s(score, ts, 10);
	outtextxy(250, 430, "score:");
	outtextxy(290, 430,ts);
}
void showhowtoplay()
{
	putimage(0, 0, &img_showhowtoplay);
}
void showconfirm()//确定界面
{
	putimage(0, 0,Width,High, &img_confirm,150,200);
}
void showconfirm2()
{
	putimage(0, 0,&img_confirm2);
}
void show() {//屏幕展示
	cleardevice();
	switch (whichpage)
	{
	case 1:showmenu();break;
	case 2:showgame();break;
	case 3:showrank();break;
	case 4:showhowtoplay();break;
	case 5:showconfirm();break;
	case 6:showinnermenu();break;
	case 7:showconfirm2();break;
	case 8:showgameover();break;
	}
	FlushBatchDraw();
}
void cleardate()
{
	isfirst = 10000;

	//清空子弹数据
	for (int i = 0;i <= 19;i++)
	{
		My_Bullets[i].x = My_Bullets[i].y = Enemy_Bullets[i].x = Enemy_Bullets[i].y = 0;
	}

	//清空飞机数据
	position_x = Width * 0.5;
	position_y = High * 0.7;

	//清空敌机数据
	for (int i = 0;i <= 5;i++)
		Enemies[i].x = Enemies[i].y = 0;

	life = 5;
	score = 0;
	Enemy::alive_num = My_Bullet::alive_num = Enemy_Bullet::alive_num = 0;
	haveinput = 0;
}
/*
void Enemy_Bulletmove()
{
	for (int i = 0;i <= 19;i++)
	{
		if (Enemy_Bullets[i].x + Enemy_Bullets[i].y)//还在动的子弹
		{
			if (abs(Enemy_Bullets[i].x - position_x) + abs(Enemy_Bullets[i].y - position_y) <= 10)//敌方子弹集中我方
			{
				life--;
				Enemy_Bullet::alive_num--;
				Enemy_Bullets[i].x = Enemy_Bullets[i].y = 0;
			}
			else if (Enemy_Bullets[i].y >= 770)
			{
				Enemy::alive_num--;
				Enemy_Bullets[i].x = Enemy_Bullets[i].y = 0;
			}
			else
			{
				Enemy_Bullets[i].y += 2;
			}
		}
	}
}
void Enemy_Bullethit(){}
void Enemy_move(){}
void Enemyhit(){}
void Enemy
*/
void updatewithoutinput() {
	duration = 40 - score / 10;
	duration <= 0 ? duration = 1 : duration = duration;
	if (!flag)
	{
		startClock = clock();
		flag = 1;
	}
	processClock = clock();
	if (whichpage == 2&&(processClock-startClock)>=duration)
	{
		flag = 0;
		//敌方子弹移动，并检测是否击中我方飞机。
		for (int i=0;i<=19;i++)
		{
			if (Enemy_Bullets[i].x + Enemy_Bullets[i].y)//还在动的子弹
			{
				if (abs(Enemy_Bullets[i].x - position_x) + abs(Enemy_Bullets[i].y - position_y) <= 50)//敌方子弹集中我方
				{
					if (isfirst<=0)
					{
						life--;
						Enemy_Bullet::alive_num--;
						Enemy_Bullets[i].x = Enemy_Bullets[i].y = 0;
					}
				}
				else if (Enemy_Bullets[i].y >= 770)
				{
					Enemy::alive_num--;
					Enemy_Bullets[i].x = Enemy_Bullets[i].y = 0;
				}
				else
				{
					Enemy_Bullets[i].y +=3;
				}
			}
		}
		//我方子弹移动，判断是否碰到敌方，进行相应处理
		for (int i = 0;i <= 19;i++)
		{
			if (My_Bullets[i].x + My_Bullets[i].y)//还在动的子弹
			{
				for (int j = 0;j <= 5;j++)//敌方飞机
				{
					if (Enemies[j].x + Enemies[j].y)//还在动的敌方飞机
					{
						if (abs(My_Bullets[i].x - Enemies[j].x) + abs(My_Bullets[i].y - Enemies[j].y)<=160)//碰到了
						{
							score++;
							My_Bullet::alive_num--;
							Enemy::alive_num--;
							My_Bullets[i].x = My_Bullets[i].y = 0;
							Enemies[j].x = Enemies[j].y = 0;
							break;
						}
					}
				}
			}
		}
		//我方子弹移动
		for (int i = 0;i <= 19;i++)
		{
			if (My_Bullets[i].x + My_Bullets[i].y)
			{
				if (My_Bullets[i].y <= 30)
				{
					My_Bullets[i].x = My_Bullets[i].y = 0;
					My_Bullet::alive_num--;
				}
				else
				{
					My_Bullets[i].y-= 5;
				}
			}
		}
		
		//敌方飞机移动，检测是否碰到我方，在发射子弹
		for (int i=0;i<=5;i++)
		{
			if (Enemies[i].x + Enemies[i].y)
			{
				if (abs(Enemies[i].x - position_x) + abs(Enemies[i].y - position_y) <= 175)//碰没碰到
				{
					if (isfirst<=0)
					{
						life--;
						Enemy::alive_num--;
						Enemies[i].x = Enemies[i].y = 0;
					}
				}
				else if (Enemies[i].y >= High - 20)//超出边界？
				{
					Enemy::alive_num--;
					Enemies[i].x = Enemies[i].y = 0;
				}
				else
				{
					Enemies[i].y += 1;
					for (int j = 0;j <= 19;j++)
					{
						if (!(Enemy_Bullets[j].x + Enemy_Bullets[j].y) && rand() % 20== 0)
						{
							Enemy_Bullet::alive_num++;
							Enemy_Bullets[j].x = Enemies[i].x+45;
							Enemy_Bullets[j].y = Enemies[i].y + 140;
							break;
						}
					}
					/*int yy = Enemies[i].y;
					if (Enemy_Bullet::alive_num <= 19)
					{
						bool flag = 0;//未找到
						for (int i = 0;i <= 5;i++)
						{
							if (!(Enemies[i].x + Enemies[i].y))
							{
								for (int j = 0;j <= 19;j++)
								{
									if (!(Enemy_Bullets[j].x + Enemy_Bullets[j].y))
									{
										Enemy_Bullet::alive_num++;
										Enemy_Bullets[j].x = Enemies[i].x;
										Enemy_Bullets[j].y = Enemies[i].y + 20;
										flag = 1;
										break;
									}
								}
							}
							if (flag) break;
						}
					}*/
						/*for (int j = 0;j <= 19;j++)
						{
							if (!(Enemy_Bullets[j].x + Enemy_Bullets[j].y) && rand() % 5 == 0)
							{
								Enemy_Bullet::alive_num++;
								Enemy_Bullets[j].x = Enemies[i].x;
								Enemy_Bullets[j].y = Enemies[i].y + 60;
							}
						}*/
					
				}
			}
		}
	}
	//检测敌方是不是空的，是空的重新生成，增加速度，开始下一轮
	int a = 0;
	for(int i=0;i<=5;i++)
		if((Enemies[i].x+Enemies[i].y)) a++;
	if (a<=0)
	{

		Round++;
		for (int i = 0;i <= 5;i++)
		{
			int tempx = rand() % 550 + 15;
			int tempy = rand() % 90 + 10;
			Enemies[i].x = tempx;
			Enemies[i].y = tempy;
			Enemy::alive_num++;
		}
	}


	//判断我方是否生命为零，如果生命为零，则跳转到游戏结束界面，记得处理分数
	if (life <= 0)
	{
		whichpage = 8;
	}
}
void updatewithinput()
{
	//鼠标事件
	while (MouseHit())
	{
		MOUSEMSG m= GetMouseMsg();
		if (whichpage == 1)//主界面
		{
			if ((m.uMsg == WM_LBUTTONDOWN&&Width / 2 - 60 <= m.x && m.x <= Width / 2 + 80 && m.y >= High / 5 && m.y <= High / 5 + 40))
			{
				whichpage = 2;//跳转到游戏界面；
			}
			else if (m.uMsg == WM_LBUTTONDOWN && Width / 2 - 60 <= m.x && m.x <= Width / 2 + 80 && m.y >= High / 5 + 60 && m.y <= High / 5 + 100)
			{
				whichpage = 3;//排行榜界面
			}
			else if (m.uMsg == WM_LBUTTONDOWN && Width / 2 - 60 <= m.x && m.x <= Width / 2 + 80 && m.y >= High / 5 + 120 && m.y <= High / 5 + 160)
			{
				whichpage = 4;//说明界面
			}
			else if (m.uMsg == WM_LBUTTONDOWN && Width / 2 - 60 <= m.x && m.x <= Width / 2 + 80 && m.y >= High / 5 + 180 && m.y <= High / 5 + 220)
			{
				whichpage=5;//确定界面
			}
		}
		else if (whichpage == 2)
		{
				position_x = m.x;
				position_y = m.y;
			if (m.uMsg == WM_LBUTTONDOWN&&My_Bullet::alive_num<=18)
			{
				for (int i = 0;i <= 19;i++)
				{
					if (!(My_Bullets[i].x + My_Bullets[i].y))
					{
						My_Bullets[i].x = position_x;
						My_Bullets[i].y = position_y - 60;
						break;
					}
				}
			}
		}
		else if (whichpage == 3)//排行榜界面
		{
			if (m.uMsg == WM_LBUTTONDOWN && m.x >= 45 && m.x <= 220 && m.y >= 24 && m.y <= 117)
				whichpage = 1;
		}
		else if (whichpage == 4)//说明界面
		{
			if (m.uMsg == WM_LBUTTONDOWN && m.x >= 24 && m.x <= 299 && m.y >= 24 && m.y <= 117)
				whichpage = 1;
		}
		else if (whichpage == 5)//退出界面
		{
			if ((m.uMsg == WM_LBUTTONDOWN && m.y >= 500 && m.y <= 600&&m.x>=100&&m.x<=220))
			{
				exit(0);
			}
			else if (m.uMsg == WM_LBUTTONDOWN&& m.y >= 500 && m.y <= 600 && m.x >= 400 && m.x <= 550)
			{
				whichpage = 1;
			}
		}
		else if (whichpage == 6)//游戏内菜单界面
		{

			if (m.uMsg == WM_LBUTTONDOWN && m.x >= 200 && m.x <= 300 && m.y >= 286 && m.y <= 360)
				whichpage = 2;
			else if (m.uMsg == WM_LBUTTONDOWN && m.x >= 280 && m.x <= 480 && m.y >= 286 && m.y <= 550)
				whichpage = 7;
		}
		else if (whichpage == 7)
		{
			if (m.uMsg == WM_LBUTTONDOWN && m.x >= 135 && m.x <= 220 && m.y >= 400 && m.y <= 700)
			{
				whichpage = 1;
			}
			else if (m.uMsg == WM_LBUTTONDOWN && m.x >= 320 && m.x <= 440 && m.y >= 400 && m.y <= 700)
			{
				whichpage = 2;
			}
		}
		else if (whichpage == 8)
		{
			if (score > Ranks[2].score&&!haveinput)
			{
				char s[10];
				InputBox(s, 10, "Please input your name:");
				haveinput = 1;
				writerank(score, s);
			}
			if (m.uMsg == WM_LBUTTONDOWN && m.x >= 186 && m.x <= 406 && m.y >=465 && m.y <= 530)
			{
				cleardate();
				whichpage = 2;
			}
			else if (m.uMsg == WM_LBUTTONDOWN && m.x >= 189 && m.x <= 410 && m.y >=582 && m.y <=649)
			{
				cleardate();
				whichpage = 1;
			}
		}
	}

	//按下esc事件
	if (_kbhit()&&_getch()==27)
	{
		if (whichpage == 2)
		{
			whichpage = 6;
		}
		else if (whichpage == 6)
		{
			whichpage = 7;
		}
		else
		{
			whichpage = 1;
		}
	}
	
}
int main()
{
	startup();
	while (1)
	{
		isfirst >= 0 ? isfirst-- : isfirst = isfirst;
		show();
		updatewithoutinput();
		updatewithinput();
	}
}