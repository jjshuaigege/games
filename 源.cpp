#include <graphics.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <stdio.h>
#include <WinUser.h>
#include <mmsystem.h>
#include <math.h>
#define lattsize 30                   //���ӵı߳���С
#define horizontal 12                 //�����ж��ٸ�����
#define vertical 28                   //�����ж��ٸ����ӣ�����4��Ԥ����
#define preparesize 4                  //Ԥ������Ĵ�С��Ĭ��Ϊ4��
#define gu 100;
int latt[vertical][horizontal];//�����ֵ������ӵ����࣬0��ʾ�ո��ӣ�1��ʾ�ƶ��еĸ��ӣ�2��ʾ�̶�֮��ĸ���
struct
{
	int x, y;              //����
	int flag;              //��ǰ�ƶ�ͼ�εľ�����״��19�֣�
} block[preparesize];      //��ʾ4�������ƶ��ķ��飬���Ӱ������ң����ϵ��½�������
struct
{
	int flag[preparesize][preparesize];                //4*4������,flagȡ0��2
} graph_type[19];               //����19��ͼ��
typedef struct
{
	int x, y;
}pt;
struct
{
	pt point[4];
} shift_of_change[19];
int prepare_type;        //ȡֵ��ΧΪ0��18����¼��һ���������ֵ�ͼ������
int grade;               //��¼����
double t;             //��¼��Ϸʱ��
int dlevel;              //��¼��Ϸ�Ѷȵȼ�
//��19��ͼ�δ���graph_type[]����
void initgraphtype()
{
	FILE* fp;
	fp = fopen("./data/����˹����19������.txt", "r");
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
//��֪������̬�ı�ţ���ƫ�����ĺ���
void findshift(int m, int n)   //mΪ�仯ǰ����̬���,nΪ�仯�����̬���
{
	pt temp1[preparesize];         //���Դ�ű仯ǰ4���������
	pt temp2[preparesize];         //���Դ�ű仯��4���������
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
//��ƫ����,19��ͼ�α仯��ƫ��������ʼ����Ӧ������
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
//��ʼ������
void initlatt()
{
	for (int i = preparesize; i < vertical; i++)
		for (int j = 0; j < horizontal; j++)
			latt[i][j] = 0;
}
//������
void drawlatt()
{
	setlinecolor(WHITE);
	for (int i = preparesize; i < vertical; i++)
	{
		for (int j = 0; j < horizontal; j++)
		{
			if (latt[i][j] == 0)
			{
				setfillcolor(RGB(190,190,190));                     //�ո�����ɫ
				fillrectangle(j * lattsize, (i - preparesize) * lattsize, (j + 1) * lattsize, (i - preparesize+1) * lattsize);
			}
			else if (latt[i][j] == 1)
			{
				setfillcolor(BLUE);                        //�˶��еĸ�����ɫ
				fillrectangle(j * lattsize, (i - preparesize) * lattsize, (j + 1) * lattsize, (i - preparesize+1) * lattsize);
			}
			else
			{
				setfillcolor(YELLOW);                     //�̶���ĸ�����ɫ
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
//������֪�ľ����������Ϸ����ɼ���Ԥ�������ɶ�Ӧ��ͼ��
void dtype_based_on_number(int num)
{
	int cnt = 0;
	for (int i = 0; i < preparesize; i++)
		for (int j = horizontal / 2 - preparesize / 2; j < horizontal / 2 + preparesize / 2; j++)
		{
			if (graph_type[num].flag[i][j - (horizontal / 2 - preparesize / 2)] == 2)           //ע���ά�����������ֱ������Ĺ�ϵ��
			{
				latt[i][j] = 1;     //Ϳɫ
				block[cnt].x = j;
				block[cnt].y = i;   //��¼�˶��ķ���λ��
				cnt++;
			}
		}
	for (int i = 0; i < 4; i++)
	{
		block[i].flag = num;       //��¼��ǰ�ƶ�ͼ�ε�����
	}
}
//������7�ִ��ࣩ�ȸ����������19�־������ͣ����ض�Ӧ���
int creategraphnum()
{
	int type = rand() % 7;
	int num,temp;             //num��Ӧ�������࣬temp���Եȸ������ɾ����ͼ��
	switch (type)
	{
	case 0:            //��Ӧgraph_type[0]
		num = 0;
		break;
	case 1:            //��Ӧgraph_type[1]��graph_type[4]
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
//��ͼ����׹ ����׹�ɹ�����1���ŵط���0
int graph_fall()
{
	int flag = 1;       //flagΪ0��ʾ�����ŵأ�Ϊ1��ʾ������׹
	//���ȼ���Ƿ񼴽��ŵ�
	for (int i = 0; i < 4; i++)
	{
		if (block[i].y + 1 == vertical || latt[block[i].y + 1][block[i].x] == 2)
		{
			flag = 0;
			break;
		}
	}
	//ͼ��������׹�Ĵ���
	if (flag == 1)
	{
		for (int i = 3; i >= 0; i--)           //�����ֹ����
		{
			latt[block[i].y][block[i].x] = 0;
			latt[block[i].y+1][block[i].x] = 1;
			block[i].y++;
		}
		return 1;
	}
	//ͼ���ŵصĴ���
	else
	{
		for (int i = 0; i < 4; i++)
		{
			latt[block[i].y][block[i].x] = 2;
		}
		return 0;
	}
}
//����Ƿ������ȥһ��
void checkrow()
{
	int cnt1 = 0;   //cnt1��С�ﵽhorizontal��˵��ĳһ���Ѿ�����
	int cnt2 = 0;   //ĳһ����Ҫ�����ƶ�������,ÿ����һ�����ˣ� cnt2����ֵ��һ
	for (int i = vertical - 1; i >= 0; i--)
	{
		cnt1 = 0;    //����µ�һ�У�cnt1������Ϊ0
		//�ڼ��ĳһ��֮ǰ�ȸ���cnt2�Ĵ�С���������ƶ���ָ��λ��
		for (int j = 0; j < horizontal; j++)
		{
			if (latt[i][j] == 2)
			{
				latt[i][j] = 0;
				latt[i + cnt2][j] = 2;
			}
		}
		//���ĳһ���Ƿ�����
		for (int j = 0; j < horizontal; j++)
		{
			if (latt[i+cnt2][j] == 2)
			{
				cnt1++;
			}
		}
		if (cnt1 == horizontal)     //˵��������Ҫ������ȥ
		{
			for (int j = 0; j < horizontal; j++)
			{
				latt[i+cnt2][j] = 0;
			}
			cnt2++;
		}
		if (cnt2 > 0)
		{
			mciSendString("play ./sound/��ȥ����.wav", 0, 0, 0);
		}
	}
	grade += cnt2 * gu;
}
//�жϸı���״���Ƿ�Խ�����ײ����������,��Խ����ͨ��ƽ�ƽ�������,��ײ���������飬��Ӧ���б��Σ������Խ��б����򷵻�1�����򷵻�0 
int judge_and_repair()
{
	
	e0:pt temp[4];   //��ʱ�����������Դ�ű��κ�4���������
	for(int i=0;i<4;i++)
	{
		temp[i].x = block[i].x + shift_of_change[block[0].flag].point[i].x;
		temp[i].y = block[i].y + shift_of_change[block[0].flag].point[i].y;
	}
	//�ж��Ƿ�Խ��
	for (int i = 0; i < 4; i++)
	{
		if (temp[i].x >= horizontal)    //���ұ�Խ��
		{
			//��Ϊƽ�ƿ����ֻᱻ���鵲ס���ʻ���Ҫ�жϴ�ʱ�Ƿ��ܹ�����ƽ��
			for (int i = 0; i < 4; i++)
			{
				if (block[i].x - 1 < 0 || latt[block[i].y][block[i].x - 1] == 2)
				{
					return 0;
				}

			}
			//���˿�ȷ��ͼ���ܹ�����ƽ�ƣ����ǿ�ʼ����ƽ�Ʋ���
			for (int i = 0; i < 4; i++)
			{
				latt[block[i].y][block[i].x] = 0;
				latt[block[i].y][block[i].x - 1] = 1;
				block[i].x--;
			}
			//�ص�e0�����ж��Ƿ���Ҫ��һ��ƽ��
			goto e0;
		}
		else if (temp[i].x < 0)   //�����Խ��
		{
		//��Ϊƽ�ƿ����ֻᱻ���鵲ס���ʻ���Ҫ�жϴ�ʱ�Ƿ��ܹ�����ƽ��
		for (int i = 0; i < 4; i++)
		{
			if (block[i].x + 1 >= horizontal || latt[block[i].y][block[i].x + 1] == 2)
			{
				return 0;
			}

		}
		//���˿�ȷ��ͼ���ܹ�����ƽ�ƣ����ǿ�ʼ����ƽ�Ʋ���
		for (int i = 3; i >= 0; i--)
		{
			latt[block[i].y][block[i].x] = 0;
			latt[block[i].y][block[i].x + 1] = 1;
			block[i].x++;
		}
		//�ص�e0�����ж��Ƿ���Ҫ��һ��ƽ��
		goto e0;
		}
	}
	//�ж��Ƿ�ײ����������
	for (int i = 0; i < 4; i++)
	{
		if (latt[temp[i].y][temp[i].x] == 2)
		{
			return 0;       // ײ�����������飬��������0
		}
	}
	//���˿�ȷ��ͼ���ܹ����б任����������1
	return 1;
}
//�ж��Ƿ�ʧ�ܣ�����1��ʾ��ʧ�ܣ�����0��ʾ����
int  checklose()
{
	for (int i = 0; i < horizontal; i++)

	{
		if (latt[preparesize][i] == 2)
			return 1;
	}
	return 0;
}
//����ͼ������ƶ���ת��
void control_graph(int control)
{
	int flag1 = -1;    //flag1�ж��Ƿ��ܼ�����׹ 
	int flag2 = -1;    //flag2�ж��Ƿ�任��ײǽ����ײ����������
	//if ((GetAsyncKeyState(VK_LEFT) & 0x8000) || (GetAsyncKeyState('A') & 0x8000) || (GetAsyncKeyState('a') & 0x8000))       //����
	if (control == 'a' || control == 'A')      //����
	{
		//�����ж��Ƿ�ײǽ����������������
		int flag = 1;      //flagΪ1��ʾ������Ϊ0��ʾ���赲
		for (int i = 0; i < 4; i++)
		{
			if (block[i].x - 1 < 0 || latt[block[i].y][block[i].x - 1] == 2)
			{
				flag = 0;
				break;
			}

		}
		if (flag == 1)    //�����ƶ�
		{
			for (int i = 0; i < 4; i++)
			{
				latt[block[i].y][block[i].x] = 0;
				latt[block[i].y][block[i].x - 1] = 1;
				block[i].x--;
			}
		}
	}
	else if (control == 'd' || control == 'D')           //����
	{
		//�����ж��Ƿ�ײǽ����������������
		int flag = 1;      //flagΪ1��ʾ������Ϊ0��ʾ���赲
		for (int i = 0; i < 4; i++)
		{
			if (block[i].x + 1 >= horizontal || latt[block[i].y][block[i].x + 1] == 2)
			{
				flag = 0;
				break;
			}

		}
		if (flag == 1)    //�����ƶ�
		{
			for (int i = 3; i >= 0; i--)
			{
				latt[block[i].y][block[i].x] = 0;
				latt[block[i].y][block[i].x + 1] = 1;
				block[i].x++;
			}
		}
	}
	else if (control == 's' || control == 'S')      //�����½�
	{
		flag1 = graph_fall();
		if (flag1 == 0)
		{
			dtype_based_on_number(prepare_type);            //��Ԥ����������Ԥ���ͼ��
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
				latt[block[i].y][block[i].x] = 0; //ִ������ѭ�������⸲������
			}

			for (int i = 0; i < 4; i++)
			{
				block[i].x = block[i].x + shift_of_change[block[i].flag].point[i].x;
				block[i].y = block[i].y + shift_of_change[block[i].flag].point[i].y;
				latt[block[i].y][block[i].x] = 1;
				//�ı�block[]��flag  
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
//���ݷ�����̬�����Ѷ�
void setdifficulty()    //�ú�����dlevel��ֵ���ı�ǰ�����ã���grade�ó���ǰ���Ѷȵȼ�����dlevel�ó�֮ǰ���Ѷȵȼ�
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
//��Ҫѭ���壨����Ϸ��������
void run_game()
{
	srand((unsigned int)time(NULL));
	setbkcolor(DARKGRAY);
	cleardevice();
	settextstyle(0, 0, _T("����"));
	initlatt();
	initgraphtype();
	initshift();     //һϵ�г�ʼ������  
	int flag;      //�ж��Ƿ������µ�ͼ�Σ�Ϊ0��ʾӦ�������µ�ͼ��
	int fps = 0;       //Ĭ��Ϊ20fps����fpsȡֵΪ0��19,������ÿִ��20�Σ�����ͼ����Ϊʱ�������½�һ���������ܹ���߲�����������
	char control;    //���Խ��ռ���ָ��
	int reason = 0;       //��¼�˳�ԭ��,0�����������У�1��������Ϸʧ�ܶ��˳���2���������˳�
	char gradep[40] = { 0 };      //���ַ��������¼��������ӡ
	int ftemp;            //������ʱ�����Ϸ�ļ�¼�ļ��еĵ�ǰ����
	char finalwords[100] = { 0 };  //��ӡ����ҵ���Ϸ����
	grade = 0;
	t = 0;
	dlevel = 1;
	double temp;         //���ʱ�����ű���
	double clock_s, clock_e;
	double clock_sr=0,clock_er=0;
	int tfs;
	char tfsp[30] = { 0 };         //���ַ��������¼ʱ�䲢��ӡ
	char difsp[30] = { 0 };        //���ַ��������¼�ѶȲ���ӡ
		dtype_based_on_number(creategraphnum());    //��ʼ�������Ϸ�Ԥ�������ɵ�һ��ͼ��
	prepare_type = creategraphnum();             // ��Ԥ�������ʼ��
	outtextxy(440, 10, "��һ��ͼ�Σ�");
	mciSendString("open ./music/��Ϸʱ�ı�������.mp3 alias bgm", 0, 0, 0);
	mciSendString("setaudio bgm volume to 150", 0, 0, 0);                      //������������������ 
	mciSendString("play bgm repeat", 0, 0, 0);
	mciSendString("setaudio ./sound/��ȥ����.wav volume to 50", 0, 0, 0);
	while (1)
	{
		clock_s=clock();
		if (fps == 20)
		{
			fps = 0;
			flag = graph_fall();
			if (flag == 0)
			{
				dtype_based_on_number(prepare_type);            //��Ԥ����������Ԥ���ͼ��
				prepare_type = creategraphnum();                //������һ��ͼ������Ӧ�ı�ţ�������Ԥ��������
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
				mciSendString("play ./sound/��Ϸ��ͣ.wav", 0, 0, 0);
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
		sprintf_s(difsp, "��ǰ�Ѷ�:%d����1��10��", dlevel);
		sprintf_s(gradep, "����:%d", grade);
		outtextxy(440, 200, gradep);
		outtextxy(400, 240, difsp);
		outtextxy(380, 300, "����A����D�����������ƶ�");
		outtextxy(380, 360, "����S�����м����½�");
		outtextxy(380, 420, "����K��������ת����");
		outtextxy(380, 480, "���¿ո����ͣ��Ϸ");
		outtextxy(380, 540, "����B�����ز˵�");
		outtextxy(365, 560, "\(������������ķ��������ᱻ��");
		outtextxy(365, 580, "��\)");
		outtextxy(380, 640, "���ߣ�û�Ļ��Ĳ�ʿ");
		if (reason==1)
		{
			break;
		}
		clock_e = clock();
		t += ((clock_e - clock_s)-(clock_er-clock_sr)) / CLK_TCK;
		tfs = (int)t;
		sprintf_s(tfsp, "��Ϸʱ��:%d��", tfs);
		outtextxy(420, 220, tfsp);
		clock_er = clock_sr = 0;
	}
	mciSendString("close bgm", 0, 0, 0);
	if (reason == 2)    //�����˳�
	{
		return;
	}
	else   //reasonΪ1
	{
		cleardevice();
		FILE* p1;
		p1 = fopen("./data/��߷ּ�¼.txt", "r"); 
		if (p1 == NULL)              //�ļ�������
		{
			p1 = fopen("./data/��߷ּ�¼.txt", "w+");
			fprintf(p1, "%d", grade);
			mciSendString("play ./sound/���Ƽ�¼.wav", 0, 0, 0);
			sprintf_s(finalwords, "���ķ�����%d,��ϲ���������µ���߼�¼�����ķ����ѱ����档", grade);
			outtextxy(10,10,finalwords);
			outtextxy(10, 40, "�밴�¿ո�����˳�");
			fclose(p1);
		}
		else                   //���м�¼
		{
			fscanf(p1, "%d", &ftemp);                //��ȡ��¼�ļ�������
			fclose(p1);
			if (ftemp >= grade)
			{
				mciSendString("play ./sound/δ���Ƽ�¼.wav", 0, 0, 0);
				sprintf_s(finalwords, "���ķ�����%d,���ź���û�д�����߼�¼�����ٽ�������", grade);
				outtextxy(10,10,finalwords);
				outtextxy(10,40,"�밴�¿ո�����˳�");
			}
			else
			{
				p1 = fopen("./data/��߷ּ�¼.txt", "w+");
				fprintf(p1, "%d", grade);
				mciSendString("play ./sound/���Ƽ�¼.wav", 0, 0, 0);
				sprintf_s(finalwords, "���ķ�����%d,��ϲ���������µ���߼�¼�����ķ����ѱ����档", grade);
				outtextxy(10,10,finalwords);
				outtextxy(10,40,"�밴�¿ո�����˳�");
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
int main_menu()                //����0��ʾ��ʼ��Ϸ������1��ʾ�˳���Ϸ
{
	cleardevice();
	setbkcolor(RGB(28,28,28));
	setlinecolor(WHITE);
	settextstyle(42, 20, _T("����"));
	settextcolor(WHITE);
	rectangle(225, 115, 393, 165);     //��ʼ��Ϸ
	rectangle(225, 355, 393, 405);     //��߼�¼
	rectangle(225, 595, 393, 645);     //�˳���Ϸ
	outtextxy(230, 120, "��ʼ��Ϸ");
	outtextxy(230, 360, "��߼�¼");
	outtextxy(230, 600, "�˳���Ϸ");
	MOUSEMSG m;
	//���˵�����
	e0:cleardevice();
	while (1)
	{
		rectangle(225, 115, 393, 165);     //��ʼ��Ϸ
		rectangle(225, 355, 393, 405);     //��߼�¼
		rectangle(225, 595, 393, 645);     //�˳���Ϸ
		m = GetMouseMsg();
		if (m.x > 225 && m.x < 393 && m.y>115 && m.y < 165)
		{
			settextcolor(RED);
			outtextxy(230, 120, "��ʼ��Ϸ");
			settextcolor(WHITE);
			outtextxy(230, 360, "��߼�¼");
			outtextxy(230, 600, "�˳���Ϸ");
			if (m.uMsg == WM_LBUTTONDOWN)
			{
				mciSendString("play ./sound/���������Ч.wav", 0, 0, 0);
				return 0;
			}
		}
		else if(m.x > 225 && m.x < 393 && m.y>355 && m.y < 405)
		{
			settextcolor(RED);
			outtextxy(230, 360, "��߼�¼");
			settextcolor(WHITE);
			outtextxy(230, 120, "��ʼ��Ϸ");
			outtextxy(230, 600, "�˳���Ϸ");
			if (m.uMsg == WM_LBUTTONDOWN)
			{
				mciSendString("play ./sound/���������Ч.wav", 0, 0, 0);
				goto e1;
			}
		}
		else if (m.x > 225 && m.x < 393 && m.y>595 && m.y < 645)
		{
			settextcolor(RED);
			outtextxy(230, 600, "�˳���Ϸ");
			settextcolor(WHITE);
			outtextxy(230, 120, "��ʼ��Ϸ");
			outtextxy(230, 360, "��߼�¼");
			if (m.uMsg == WM_LBUTTONDOWN)
			{
				mciSendString("play ./sound/���������Ч.wav", 0, 0, 0);
				return 1;
			}
		}
		else
		{
			settextcolor(WHITE);
			outtextxy(230, 120, "��ʼ��Ϸ");
			outtextxy(230, 360, "��߼�¼");
			outtextxy(230, 600, "�˳���Ϸ");
		}
	}
	//�鿴�÷ּ�¼����
	e1:cleardevice();
	while (1)
	{
		FILE* p1;
		p1 = fopen("./data/��߷ּ�¼.txt", "r");
		int temp;
		if (p1)
		{
			fscanf(p1, "%d", &temp);
			fclose(p1);
			char finalwords[40] = { 0 };
			sprintf_s(finalwords, "������ߵ÷��ǣ�%d", temp);
			settextcolor(WHITE);
			outtextxy(120, 360, finalwords);
		}
		else
		{
			outtextxy(120,360,"���޼�¼");
		}
		rectangle(10, 10, 100, 60);
		m = GetMouseMsg();
		if (m.x > 10 && m.x < 100 && m.y>10 && m.y < 60)
		{
			settextcolor(RED);
			outtextxy(15, 15, "����");
			if (m.uMsg == WM_LBUTTONDOWN)
			{
				mciSendString("play ./sound/���������Ч.wav", 0, 0, 0);
				goto e0;
			}
		}
		else
		{
			settextcolor(WHITE);
			outtextxy(15, 15, "����");
		}
	}

}
int main()
{
	int flag;          //flag��¼�˵��Ĳ���
	initgraph(600, 720);
	e2:flag=main_menu();
	if (flag == 0)
	{
		run_game();
		goto e2;
	}
	else             //�ڲ˵����˳���Ϸ
	{
		closegraph();     
		return 1;
	}
	return 1;
}