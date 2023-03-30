#include <graphics.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <stdio.h>
#include <WinUser.h>
#include <mmsystem.h>
#include <math.h>
#define lattsize 30                   //格子的边长大小
#define horizontal 12                 //横向有多少个格子
#define vertical 28                   //纵向有多少个格子（包括4个预备）
#define preparesize 4                  //预备区域的大小（默认为4）
#define gu 100;
int latt[vertical][horizontal];//数组的值代表格子的种类，0表示空格子，1表示移动中的格子，2表示固定之后的格子
struct
{
	int x, y;              //坐标
	int flag;              //当前移动图形的具体形状（19种）
} block[preparesize];      //表示4个正在移动的方块，格子按从左到右，从上到下进行排序
struct
{
	int flag[preparesize][preparesize];                //4*4的区域,flag取0或2
} graph_type[19];               //共有19种图形
typedef struct
{
	int x, y;
}pt;
struct
{
	pt point[4];
} shift_of_change[19];
int prepare_type;        //取值范围为0到18，记录下一个即将出现的图形种类
int grade;               //记录分数
double t;             //记录游戏时间
int dlevel;              //记录游戏难度等级
//将19种图形传入graph_type[]数组
void initgraphtype()
{
	FILE* fp;
	fp = fopen("./data/俄罗斯方块19种类型.txt", "r");
	if (fp == NULL)
		exit(1);
	else 
	{
		for (int k = 0; k < 19; k++)
			for (int i = 0; i < preparesize; i++)
				for (int j = 0; j < preparesize; j++)
				{
					fscanf(fp, "%d", &graph_type[k].flag[i][j]);
				}
		fclose(fp);
	}
}
//已知两种形态的编号，求偏移量的函数
void findshift(int m, int n)   //m为变化前的形态编号,n为变化后的形态编号
{
	pt temp1[preparesize];         //用以存放变化前4个点的坐标
	pt temp2[preparesize];         //用以存放变化后4个点的坐标
	int cnt1 = 0, cnt2 = 0;
	for (int j = 0; j < preparesize; j++)
		for (int k = 0; k < preparesize; k++)
		{
			if (graph_type[m].flag[j][k] == 2)
			{
				temp1[cnt1].x = k; temp1[cnt1].y = j; cnt1++;
			}
			if (graph_type[n].flag[j][k] == 2)
			{
				temp2[cnt2].x = k; temp2[cnt2].y = j; cnt2++;
			}
		}
	for (int i = 0; i < 4; i++)
	{
		shift_of_change[m].point[i].x = temp2[i].x - temp1[i].x;
		shift_of_change[m].point[i].y = temp2[i].y - temp1[i].y;
	}
	return;
}
//求偏移量,19种图形变化的偏移量，初始化相应的数组
void initshift()
{
	for(int i = 0; i < 19; i++ )
	{
		int cnt1 = 0,cnt2=0;
		if (i == 0)
		{
			findshift(i, i);
		}
		else if (i >= 1 && i <= 4)
		{
			findshift(i, i % 4 + 1);
		}
		else if (i >= 5 && i <= 8)
		{
			findshift(i, i % 4 + 5);
		}
		else if (i >= 9 && i <= 12)
		{
			findshift(i, i % 4 + 9);
		}
		else if (i >= 13 && i <= 14)
		{
			findshift(i, i % 2 + 13);
		}
		else if (i >= 15 && i <= 16)
		{
			findshift(i, i % 2 + 15);
		}
		else if (i >= 17 && i <= 18)
		{
			findshift(i, i % 2 + 17);
		}
	}
}
//初始化格子
void initlatt()
{
	for (int i = preparesize; i < vertical; i++)
		for (int j = 0; j < horizontal; j++)
			latt[i][j] = 0;
}
//画格子
void drawlatt()
{
	setlinecolor(WHITE);
	for (int i = preparesize; i < vertical; i++)
	{
		for (int j = 0; j < horizontal; j++)
		{
			if (latt[i][j] == 0)
			{
				setfillcolor(RGB(190,190,190));                     //空格子颜色
				fillrectangle(j * lattsize, (i - preparesize) * lattsize, (j + 1) * lattsize, (i - preparesize+1) * lattsize);
			}
			else if (latt[i][j] == 1)
			{
				setfillcolor(BLUE);                        //运动中的格子颜色
				fillrectangle(j * lattsize, (i - preparesize) * lattsize, (j + 1) * lattsize, (i - preparesize+1) * lattsize);
			}
			else
			{
				setfillcolor(YELLOW);                     //固定后的格子颜色
				fillrectangle(j * lattsize, (i - preparesize) * lattsize, (j + 1) * lattsize, (i - preparesize+1) * lattsize);
			}
		}
	}
	for(int i=0;i<preparesize;i++)
		for (int j = 0; j < preparesize; j++)
		{
			if(graph_type[prepare_type].flag[i][j] == 2)
			{
				setfillcolor(RED);
				fillrectangle((j + 2 + horizontal) * lattsize, (i + 1) * lattsize, (j + 3 + horizontal) * lattsize, (i + 2) * lattsize);
			}
			else
			{
				setfillcolor(RGB(190,190,190));
				fillrectangle((j + 2 + horizontal) * lattsize, (i + 1) * lattsize, (j + 3 + horizontal) * lattsize, (i + 2) * lattsize);
			}
		}
}
//根据已知的具体类型在上方不可见的预备区生成对应的图形
void dtype_based_on_number(int num)
{
	int cnt = 0;
	for (int i = 0; i < preparesize; i++)
		for (int j = horizontal / 2 - preparesize / 2; j < horizontal / 2 + preparesize / 2; j++)
		{
			if (graph_type[num].flag[i][j - (horizontal / 2 - preparesize / 2)] == 2)           //注意二维数组的行列与直角坐标的关系！
			{
				latt[i][j] = 1;     //涂色
				block[cnt].x = j;
				block[cnt].y = i;   //记录运动的方块位置
				cnt++;
			}
		}
	for (int i = 0; i < 4; i++)
	{
		block[i].flag = num;       //记录当前移动图形的类型
	}
}
//（根据7种大类）等概率随机生成19种具体类型，返回对应编号
int creategraphnum()
{
	int type = rand() % 7;
	int num,temp;             //num对应具体种类，temp用以等概率生成具体的图形
	switch (type)
	{
	case 0:            //对应graph_type[0]
		num = 0;
		break;
	case 1:            //对应graph_type[1]到graph_type[4]
		temp = rand() % 4;
		num = temp + 1;
		break;
	case 2:
		temp = rand() % 4;
		num = temp + 5;
		break;
	case 3:
		temp = rand() % 4;
		num = temp + 9;
		break;
	case 4:
		temp = rand() % 2;
		num = temp + 13;
		break;
	case 5:
		temp = rand() % 2;
		num = temp + 15;
		break;
	case 6:
		temp = rand() % 2;
		num = temp + 17;
		break;
	}
	return num;
}
//让图形下坠 ，下坠成功返回1，着地返回0
int graph_fall()
{
	int flag = 1;       //flag为0表示即将着地，为1表示正常下坠
	//首先检查是否即将着地
	for (int i = 0; i < 4; i++)
	{
		if (block[i].y + 1 == vertical || latt[block[i].y + 1][block[i].x] == 2)
		{
			flag = 0;
			break;
		}
	}
	//图像正常下坠的处理
	if (flag == 1)
	{
		for (int i = 3; i >= 0; i--)           //逆序防止覆盖
		{
			latt[block[i].y][block[i].x] = 0;
			latt[block[i].y+1][block[i].x] = 1;
			block[i].y++;
		}
		return 1;
	}
	//图像着地的处理
	else
	{
		for (int i = 0; i < 4; i++)
		{
			latt[block[i].y][block[i].x] = 2;
		}
		return 0;
	}
}
//检查是否可以消去一行
void checkrow()
{
	int cnt1 = 0;   //cnt1大小达到horizontal，说明某一行已经满了
	int cnt2 = 0;   //某一行需要向下移动的行数,每发现一行满了， cnt2的数值加一
	for (int i = vertical - 1; i >= 0; i--)
	{
		cnt1 = 0;    //检查新的一行，cnt1重新置为0
		//在检查某一行之前先根据cnt2的大小，将该行移动到指定位置
		for (int j = 0; j < horizontal; j++)
		{
			if (latt[i][j] == 2)
			{
				latt[i][j] = 0;
				latt[i + cnt2][j] = 2;
			}
		}
		//检查某一行是否满了
		for (int j = 0; j < horizontal; j++)
		{
			if (latt[i+cnt2][j] == 2)
			{
				cnt1++;
			}
		}
		if (cnt1 == horizontal)     //说明该行需要进行消去
		{
			for (int j = 0; j < horizontal; j++)
			{
				latt[i+cnt2][j] = 0;
			}
			cnt2++;
		}
		if (cnt2 > 0)
		{
			mciSendString("play ./sound/消去方块.wav", 0, 0, 0);
		}
	}
	grade += cnt2 * gu;
}
//判断改变形状后是否越界或者撞到其他方块,若越界则通过平移进行修正,若撞到其他方块，则不应进行变形，若可以进行变形则返回1，否则返回0 
int judge_and_repair()
{
	
	e0:pt temp[4];   //临时变量数组用以存放变形后4个点的坐标
	for(int i=0;i<4;i++)
	{
		temp[i].x = block[i].x + shift_of_change[block[0].flag].point[i].x;
		temp[i].y = block[i].y + shift_of_change[block[0].flag].point[i].y;
	}
	//判断是否越界
	for (int i = 0; i < 4; i++)
	{
		if (temp[i].x >= horizontal)    //在右边越界
		{
			//因为平移可能又会被方块挡住，故还需要判断此时是否能够进行平移
			for (int i = 0; i < 4; i++)
			{
				if (block[i].x - 1 < 0 || latt[block[i].y][block[i].x - 1] == 2)
				{
					return 0;
				}

			}
			//至此可确定图形能够进行平移，于是开始进行平移操作
			for (int i = 0; i < 4; i++)
			{
				latt[block[i].y][block[i].x] = 0;
				latt[block[i].y][block[i].x - 1] = 1;
				block[i].x--;
			}
			//回到e0处，判断是否需要进一步平移
			goto e0;
		}
		else if (temp[i].x < 0)   //在左边越界
		{
		//因为平移可能又会被方块挡住，故还需要判断此时是否能够进行平移
		for (int i = 0; i < 4; i++)
		{
			if (block[i].x + 1 >= horizontal || latt[block[i].y][block[i].x + 1] == 2)
			{
				return 0;
			}

		}
		//至此可确定图形能够进行平移，于是开始进行平移操作
		for (int i = 3; i >= 0; i--)
		{
			latt[block[i].y][block[i].x] = 0;
			latt[block[i].y][block[i].x + 1] = 1;
			block[i].x++;
		}
		//回到e0处，判断是否需要进一步平移
		goto e0;
		}
	}
	//判断是否撞到其他方块
	for (int i = 0; i < 4; i++)
	{
		if (latt[temp[i].y][temp[i].x] == 2)
		{
			return 0;       // 撞到了其他方块，函数返回0
		}
	}
	//至此可确定图形能够进行变换，函数返回1
	return 1;
}
//判断是否失败，返回1表示已失败，返回0表示正常
int  checklose()
{
	for (int i = 0; i < horizontal; i++)

	{
		if (latt[preparesize][i] == 2)
			return 1;
	}
	return 0;
}
//控制图像进行移动和转向
void control_graph(int control)
{
	int flag1 = -1;    //flag1判断是否能继续下坠 
	int flag2 = -1;    //flag2判断是否变换后撞墙或者撞到其他方块
	//if ((GetAsyncKeyState(VK_LEFT) & 0x8000) || (GetAsyncKeyState('A') & 0x8000) || (GetAsyncKeyState('a') & 0x8000))       //左移
	if (control == 'a' || control == 'A')      //左移
	{
		//首先判断是否撞墙或者碰到其他方块
		int flag = 1;      //flag为1表示正常，为0表示被阻挡
		for (int i = 0; i < 4; i++)
		{
			if (block[i].x - 1 < 0 || latt[block[i].y][block[i].x - 1] == 2)
			{
				flag = 0;
				break;
			}

		}
		if (flag == 1)    //可以移动
		{
			for (int i = 0; i < 4; i++)
			{
				latt[block[i].y][block[i].x] = 0;
				latt[block[i].y][block[i].x - 1] = 1;
				block[i].x--;
			}
		}
	}
	else if (control == 'd' || control == 'D')           //右移
	{
		//首先判断是否撞墙或者碰到其他方块
		int flag = 1;      //flag为1表示正常，为0表示被阻挡
		for (int i = 0; i < 4; i++)
		{
			if (block[i].x + 1 >= horizontal || latt[block[i].y][block[i].x + 1] == 2)
			{
				flag = 0;
				break;
			}

		}
		if (flag == 1)    //可以移动
		{
			for (int i = 3; i >= 0; i--)
			{
				latt[block[i].y][block[i].x] = 0;
				latt[block[i].y][block[i].x + 1] = 1;
				block[i].x++;
			}
		}
	}
	else if (control == 's' || control == 'S')      //加速下降
	{
		flag1 = graph_fall();
		if (flag1 == 0)
		{
			dtype_based_on_number(prepare_type);            //在预备区生成所预测的图形
			prepare_type = creategraphnum();
		}

	}
	else if (control == 'k' || control == 'K')
	{
		int mark = -1;
		mark = judge_and_repair();
		if (mark == 1)
		{
			for (int i = 0; i < 4; i++)
			{
				latt[block[i].y][block[i].x] = 0; //执行两遍循环，避免覆盖现象
			}

			for (int i = 0; i < 4; i++)
			{
				block[i].x = block[i].x + shift_of_change[block[i].flag].point[i].x;
				block[i].y = block[i].y + shift_of_change[block[i].flag].point[i].y;
				latt[block[i].y][block[i].x] = 1;
				//改变block[]的flag  
				if (block[i].flag == 0)
				{
					;
				}
				else if (block[i].flag >= 1 && block[i].flag <= 4)
				{
					block[i].flag = block[i].flag % 4 + 1;
				}
				else if (block[i].flag >= 5 && block[i].flag <= 8)
				{
					block[i].flag = block[i].flag % 4 + 5;
				}
				else if (block[i].flag >= 9 && block[i].flag <= 12)
				{
					block[i].flag = block[i].flag % 4 + 9;
				}
				else if (block[i].flag >= 13 && block[i].flag <= 14)
				{
					block[i].flag = block[i].flag % 2 + 13;
				}
				else if (block[i].flag >= 15 && block[i].flag <= 16)
				{
					block[i].flag = block[i].flag % 2 + 15;
				}
				else if (block[i].flag >= 17 && block[i].flag <= 18)
				{
					block[i].flag = block[i].flag % 2 + 17;
				}

			}
		}
	}
}
//根据分数动态调整难度
void setdifficulty()    //该函数在dlevel的值被改变前被调用，由grade得出当前的难度等级，由dlevel得出之前的难度等级
{
	int temp=1+grade/(1000);
	if (temp <= dlevel||dlevel==10)
	{
		return;
	}
	else if (temp == 2)
	{
		mciSendString("play ./dls/1.wav", 0, 0, 0);
		dlevel = 2;
	}
	else if (temp == 3)
	{
		mciSendString("play ./dls/2.wav", 0, 0, 0);
		dlevel = 3;
	}
	else if (temp == 4)
	{
		mciSendString("play ./dls/3.wav", 0, 0, 0);
		dlevel = 4;
	}
	else if (temp == 5)
	{
		mciSendString("play ./dls/4.wav", 0, 0, 0);
		dlevel = 5;
	}
	else if (temp == 6)
	{
		mciSendString("play ./dls/5.wav", 0, 0, 0);
		dlevel = 6;
	}
	else if (temp == 7)
	{
		mciSendString("play ./dls/6.wav", 0, 0, 0);
		dlevel = 7;
	}
	else if (temp == 8)
	{
		mciSendString("play ./dls/7.wav", 0, 0, 0);
		dlevel = 8;
	}
	else if(temp == 9)
	{
		mciSendString("play ./dls/8.wav", 0, 0, 0);
		dlevel = 9;
	}
	else
	{
		mciSendString("play ./dls/9.wav", 0, 0, 0);
		dlevel = 10;
	}
}
//主要循环体（让游戏跑起来）
void run_game()
{
	srand((unsigned int)time(NULL));
	setbkcolor(DARKGRAY);
	cleardevice();
	settextstyle(0, 0, _T("楷体"));
	initlatt();
	initgraphtype();
	initshift();     //一系列初始化工作  
	int flag;      //判断是否生成新的图形，为0表示应该生成新的图形
	int fps = 0;       //默认为20fps，即fps取值为0到19,主函数每执行20次，才让图形因为时间流逝下降一格，这样做能够提高操作的灵敏度
	char control;    //用以接收键盘指令
	int reason = 0;       //记录退出原因,0代表正常运行，1代表因游戏失败而退出，2代表主动退出
	char gradep[40] = { 0 };      //该字符串数组记录分数并打印
	int ftemp;            //用以临时存放游戏的记录文件中的当前内容
	char finalwords[100] = { 0 };  //打印对玩家的游戏评价
	grade = 0;
	t = 0;
	dlevel = 1;
	double temp;         //存放时间缩放倍率
	double clock_s, clock_e;
	double clock_sr=0,clock_er=0;
	int tfs;
	char tfsp[30] = { 0 };         //该字符串数组记录时间并打印
	char difsp[30] = { 0 };        //该字符串数组记录难度并打印
		dtype_based_on_number(creategraphnum());    //初始化，在上方预备区生成第一个图形
	prepare_type = creategraphnum();             // 对预测变量初始化
	outtextxy(440, 10, "下一个图形：");
	mciSendString("open ./music/游戏时的背景音乐.mp3 alias bgm", 0, 0, 0);
	mciSendString("setaudio bgm volume to 150", 0, 0, 0);                      //调音量操作！！！！ 
	mciSendString("play bgm repeat", 0, 0, 0);
	mciSendString("setaudio ./sound/消去方块.wav volume to 50", 0, 0, 0);
	while (1)
	{
		clock_s=clock();
		if (fps == 20)
		{
			fps = 0;
			flag = graph_fall();
			if (flag == 0)
			{
				dtype_based_on_number(prepare_type);            //在预备区生成所预测的图形
				prepare_type = creategraphnum();                //生成下一个图形所对应的编号，保存在预备变量中
			}
		}
		if (_kbhit())
		{
			control = _getch();
			if (control == 'b' || control == 'B')
			{
				reason = 2;
				break;
			}
			else if (control == ' ')
			{
				mciSendString("play ./sound/游戏暂停.wav", 0, 0, 0);
				mciSendString("pause bgm", 0, 0, 0);
				clock_sr = clock();
				do
				{
						control = _getch();
				} while (control != ' ');
				clock_er = clock();
				mciSendString("resume bgm", 0, 0, 0);
			}
			else
				control_graph(control);
		}
		checkrow();
		drawlatt();
		setdifficulty();
		temp = pow(1.2, (double)dlevel);
		Sleep(40/temp);
		fps++;
		reason = checklose();
		sprintf_s(difsp, "当前难度:%d级（1到10）", dlevel);
		sprintf_s(gradep, "分数:%d", grade);
		outtextxy(440, 200, gradep);
		outtextxy(400, 240, difsp);
		outtextxy(380, 300, "按下A键和D键进行左右移动");
		outtextxy(380, 360, "按下S键进行加速下降");
		outtextxy(380, 420, "按下K键进行旋转变形");
		outtextxy(380, 480, "按下空格键暂停游戏");
		outtextxy(380, 540, "按下B键返回菜单");
		outtextxy(365, 560, "\(若如此做，您的分数将不会被保");
		outtextxy(365, 580, "存\)");
		outtextxy(380, 640, "作者：没文化的博士");
		if (reason==1)
		{
			break;
		}
		clock_e = clock();
		t += ((clock_e - clock_s)-(clock_er-clock_sr)) / CLK_TCK;
		tfs = (int)t;
		sprintf_s(tfsp, "游戏时长:%d秒", tfs);
		outtextxy(420, 220, tfsp);
		clock_er = clock_sr = 0;
	}
	mciSendString("close bgm", 0, 0, 0);
	if (reason == 2)    //主动退出
	{
		return;
	}
	else   //reason为1
	{
		cleardevice();
		FILE* p1;
		p1 = fopen("./data/最高分纪录.txt", "r"); 
		if (p1 == NULL)              //文件不存在
		{
			p1 = fopen("./data/最高分纪录.txt", "w+");
			fprintf(p1, "%d", grade);
			mciSendString("play ./sound/打破记录.wav", 0, 0, 0);
			sprintf_s(finalwords, "您的分数是%d,恭喜您创造了新的最高记录！您的分数已被保存。", grade);
			outtextxy(10,10,finalwords);
			outtextxy(10, 40, "请按下空格键以退出");
			fclose(p1);
		}
		else                   //已有记录
		{
			fscanf(p1, "%d", &ftemp);                //读取记录文件的内容
			fclose(p1);
			if (ftemp >= grade)
			{
				mciSendString("play ./sound/未打破记录.wav", 0, 0, 0);
				sprintf_s(finalwords, "您的分数是%d,很遗憾您没有打破最高记录，请再接再厉。", grade);
				outtextxy(10,10,finalwords);
				outtextxy(10,40,"请按下空格键以退出");
			}
			else
			{
				p1 = fopen("./data/最高分纪录.txt", "w+");
				fprintf(p1, "%d", grade);
				mciSendString("play ./sound/打破记录.wav", 0, 0, 0);
				sprintf_s(finalwords, "您的分数是%d,恭喜您创造了新的最高记录！您的分数已被保存。", grade);
				outtextxy(10,10,finalwords);
				outtextxy(10,40,"请按下空格键以退出");
				fclose(p1);
			}
		}
		while (1)
		{
			control = _getch();
			if (control == ' ')
				return;
		}
	}
	_getch();
	closegraph();
}
int main_menu()                //返回0表示开始游戏，返回1表示退出游戏
{
	cleardevice();
	setbkcolor(RGB(28,28,28));
	setlinecolor(WHITE);
	settextstyle(42, 20, _T("楷书"));
	settextcolor(WHITE);
	rectangle(225, 115, 393, 165);     //开始游戏
	rectangle(225, 355, 393, 405);     //最高记录
	rectangle(225, 595, 393, 645);     //退出游戏
	outtextxy(230, 120, "开始游戏");
	outtextxy(230, 360, "最高记录");
	outtextxy(230, 600, "退出游戏");
	MOUSEMSG m;
	//主菜单界面
	e0:cleardevice();
	while (1)
	{
		rectangle(225, 115, 393, 165);     //开始游戏
		rectangle(225, 355, 393, 405);     //最高记录
		rectangle(225, 595, 393, 645);     //退出游戏
		m = GetMouseMsg();
		if (m.x > 225 && m.x < 393 && m.y>115 && m.y < 165)
		{
			settextcolor(RED);
			outtextxy(230, 120, "开始游戏");
			settextcolor(WHITE);
			outtextxy(230, 360, "最高记录");
			outtextxy(230, 600, "退出游戏");
			if (m.uMsg == WM_LBUTTONDOWN)
			{
				mciSendString("play ./sound/鼠标点击的音效.wav", 0, 0, 0);
				return 0;
			}
		}
		else if(m.x > 225 && m.x < 393 && m.y>355 && m.y < 405)
		{
			settextcolor(RED);
			outtextxy(230, 360, "最高记录");
			settextcolor(WHITE);
			outtextxy(230, 120, "开始游戏");
			outtextxy(230, 600, "退出游戏");
			if (m.uMsg == WM_LBUTTONDOWN)
			{
				mciSendString("play ./sound/鼠标点击的音效.wav", 0, 0, 0);
				goto e1;
			}
		}
		else if (m.x > 225 && m.x < 393 && m.y>595 && m.y < 645)
		{
			settextcolor(RED);
			outtextxy(230, 600, "退出游戏");
			settextcolor(WHITE);
			outtextxy(230, 120, "开始游戏");
			outtextxy(230, 360, "最高记录");
			if (m.uMsg == WM_LBUTTONDOWN)
			{
				mciSendString("play ./sound/鼠标点击的音效.wav", 0, 0, 0);
				return 1;
			}
		}
		else
		{
			settextcolor(WHITE);
			outtextxy(230, 120, "开始游戏");
			outtextxy(230, 360, "最高记录");
			outtextxy(230, 600, "退出游戏");
		}
	}
	//查看得分纪录界面
	e1:cleardevice();
	while (1)
	{
		FILE* p1;
		p1 = fopen("./data/最高分纪录.txt", "r");
		int temp;
		if (p1)
		{
			fscanf(p1, "%d", &temp);
			fclose(p1);
			char finalwords[40] = { 0 };
			sprintf_s(finalwords, "您的最高得分是：%d", temp);
			settextcolor(WHITE);
			outtextxy(120, 360, finalwords);
		}
		else
		{
			outtextxy(120,360,"暂无记录");
		}
		rectangle(10, 10, 100, 60);
		m = GetMouseMsg();
		if (m.x > 10 && m.x < 100 && m.y>10 && m.y < 60)
		{
			settextcolor(RED);
			outtextxy(15, 15, "返回");
			if (m.uMsg == WM_LBUTTONDOWN)
			{
				mciSendString("play ./sound/鼠标点击的音效.wav", 0, 0, 0);
				goto e0;
			}
		}
		else
		{
			settextcolor(WHITE);
			outtextxy(15, 15, "返回");
		}
	}

}
int main()
{
	int flag;          //flag记录菜单的操作
	initgraph(600, 720);
	e2:flag=main_menu();
	if (flag == 0)
	{
		run_game();
		goto e2;
	}
	else             //在菜单处退出游戏
	{
		closegraph();     
		return 1;
	}
	return 1;
}