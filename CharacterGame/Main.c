#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

CRITICAL_SECTION cs; //临界区
MSG msg;			
UINT_PTR idE;		//定时器ID
int f = 0;			//统计测试
HANDLE thread1;
HANDLE thread2;


void gotoxy(int x,int y)
{
	HANDLE hOut;
	COORD xy={x,y};
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOut,xy);
}


/*
void Cls(int x, int y)
{
	HANDLE hOut;
	COORD xy={0,0};
	int i = 0;
	DWORD byret;
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	for(;i < y; i++)
	{
		xy.Y = i;
		FillConsoleOutputCharacter(hOut,' ',x,xy,&byret);
	}
	
}*/

int SelMenu(int index)
{
	char ch;
	HANDLE hOut;

	while(1)
	{
		
		if(_kbhit())
		{
			ch = _getch();
			EnterCriticalSection(&cs);
			gotoxy(0,0);
			printf("press num = %3d",ch);
			LeaveCriticalSection(&cs);
			switch (ch)
			{
			case 77:
				{
					if (index == 1)
					{
						index = 0;
					}
					else
					{
						index ++;
					}

					break;
				}
			case 75:
				{
					if (index == 0)
					{
						index = 1;
					}
					else
					{
						index --;
					}
					break;
				}
			}

			EnterCriticalSection(&cs);

			switch (index)
			{
			case 0:
				{
					hOut = GetStdHandle(STD_OUTPUT_HANDLE);
					SetConsoleTextAttribute(hOut,BACKGROUND_BLUE | FOREGROUND_INTENSITY);
					gotoxy(2,22);
					printf("[开始游戏]");
					SetConsoleTextAttribute(hOut, FOREGROUND_INTENSITY);
					gotoxy(14,22);
					printf("[结束]");
					
					break;
				}
			case 1:
				{					
					hOut = GetStdHandle(STD_OUTPUT_HANDLE);
					SetConsoleTextAttribute(hOut, FOREGROUND_INTENSITY);
					gotoxy(2,22);
					printf("[开始游戏]");
					SetConsoleTextAttribute(hOut, FOREGROUND_INTENSITY | BACKGROUND_BLUE);
					gotoxy(14,22);
					printf("[结束]");
					SetConsoleTextAttribute(hOut, FOREGROUND_INTENSITY);
					break;
				}
			}
			LeaveCriticalSection(&cs);

			if(ch == 13 && index == 1)
			{
				exit(0);
			}

			if(ch == 13 && index == 0)
			{
				//SetTimer hwnd为Null,KillTimer第二个参数是SetTimer的返回值
				KillTimer(NULL,idE);
				TerminateThread(thread1,-1);
				TerminateThread(thread1,-2);
				system("cls");
				return index;
			}
			
		}
	}
	return index;
}


void drawline(int x,int y,int x1, int y1)
{
	int i;
	int x2,y2;

	HANDLE hOut;
	COORD xy;
	DWORD byret;
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	x2 = x + 2;
	y2 = y + 2;

	gotoxy(x,y);
	printf("┌");

	while(1)
	{
		gotoxy(x+2,y);
		for(i = x + 2; i < x2; i += 2)
			printf("─");

		printf("┐");

		gotoxy(x+2,y2);
		for(i = x + 2; i < x2; i += 2)
			printf("─");

		gotoxy(x,y+1);
		for(i = y + 1; i <= y2; i++)
		{
			printf("│");
			gotoxy(x,i);
		}

		printf("└");

		gotoxy(x2,y+1);
		for(i = y + 1; i <= y2; i++)
		{
			printf("│");
			gotoxy(x2,i);
		}

		printf("┘");
		
		
		for(i = y + 1 ; i < y2; i++)
		{
			//gotoxy(x+2,i);
			/*
			for(j = x + 1; j < x2-1; j++)
			{
				printf(" ");
			}*/

			xy.X = x + 2;
			xy.Y = i;
	
			FillConsoleOutputCharacter(hOut,' ',x2-x-2,xy,&byret);
		}
		//Sleep(20);

		if (x2 <= x1)
		{
			x2 += 2;
		}

		if (y2 <= y1)
		{
			y2 += 2;
		}
		
		if(x2 >= x1 && y2 >= y1)
		{
			break;
			
		}

		Sleep(1);
	}
}



void rowfill(int x,int *p)
{
	int i;
	char ch[100] = {0};

	for(i = 0; i < 33; i++)
	{
		if(p[i] > 19-x )
		{
			strcat( ch,"┃");
		}
		else
		{
			strcat( ch,"  ");
		}
	}

	gotoxy( 3,x);
	printf("%s",ch);
	f++;

	/*
	for(i = 18; i > ( rand() % 17) ; i --)
	{
		printf("┃");
		gotoxy(x,i);
		Sleep(10);
	}*/

}




VOID CALLBACK TimerProc(
						HWND hwnd,         // handle to window
						UINT uMsg,         // WM_TIMER message
						UINT_PTR idEvent,  // timer identifier
						DWORD dwTime       // current system time
						)
{
	EnterCriticalSection(&cs);
	gotoxy(70,1);
	printf("F: %d",f);
	LeaveCriticalSection(&cs);
	f = 0;
	//KillTimer(NULL,idEvent);
	//printf("%d",idEvent);
	//KillTimer(NULL,idE);
}



DWORD WINAPI mesg(  LPVOID lpParameter)
{
	idE = SetTimer(NULL,1,1000,TimerProc);
	while(GetMessage(&msg, NULL, 0, 0))
	{
		DispatchMessage(&msg);
	}
}



DWORD WINAPI Music(  LPVOID lpParameter)
{
	int i;
	//int x,y,w,h;
	int p[33];
	HANDLE hOut;
	COORD home;
	//DWORD byret;

	home.X = 3;

	hOut = GetStdHandle( STD_OUTPUT_HANDLE);
	//srand( (unsigned)time( NULL ));	
	while(1)
	{
		/*
		for(i = 3; i < 70; i+=2)
		{
			rowfill( i);
		}
		*/

		for(i = 0 ;i < 33; i++)
		{
			p[i] = rand() % 10;
		}

		for(i = 18; i >= 8; i--)
		{
			EnterCriticalSection(&cs);
			rowfill(i,p);
			LeaveCriticalSection(&cs);
			//f++;
			Sleep((rand() % 10) + 20);
		}

		Sleep((rand() % 800));
/*
		x = (rand() % 70) + 2 ;
		y = (rand() % 3) + 2 ;
		w = rand() % (74-x);
		h = rand() % (6-y);
		

		drawline(x,y,w,h);*/
		/*
		for(j = 3; j <= 8; j++)
		{
			home.Y = j;
			FillConsoleOutputCharacter( hOut,' ',70,home,&byret);
		}*/
	}
}



void init()
{
	int i = 0;
	HANDLE hOut;
	int index = 0;
	
	gotoxy(2,23);
	
	srand( (unsigned)time( NULL));

	for(i = 2 ; i <= 76; i++)
		printf("-");

	hOut = GetStdHandle( STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute( hOut,BACKGROUND_BLUE | FOREGROUND_INTENSITY);

	gotoxy(2,22);
	printf("[开始游戏]");
	SetConsoleTextAttribute( hOut, FOREGROUND_INTENSITY);
	gotoxy(14,22);
	printf("[结束]");

	drawline(1,1,76,18);
	

	
	thread1 = CreateThread(NULL,0,Music,NULL,0,NULL);	
	thread2 = CreateThread(NULL,0,mesg,NULL,0,NULL);		

	index = SelMenu(index);

	if(index == 0)
	{
		StarGame();
	}
}


int main()
{
	char a[2];
	char m[50];
	HWND hwnd;
	
	//获取屏幕长宽
	int nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	HMENU h;

	//隐藏光标
	CONSOLE_CURSOR_INFO cci;
	cci.bVisible = FALSE;
	cci.dwSize = sizeof(cci);	
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);

	
	InitializeCriticalSection(&cs);
	
	//控制台句柄
	hwnd = GetConsoleWindow();
	
	//屏蔽popup
	h = GetSystemMenu(hwnd, FALSE);
	DeleteMenu( h,5,MF_BYPOSITION);
	DeleteMenu( h,5,MF_BYPOSITION);
	DeleteMenu( h,5,MF_BYPOSITION);
	DeleteMenu( h,5,MF_BYPOSITION);
	DeleteMenu( h,5,MF_BYPOSITION);

	//屏蔽拖动大小及设置行数列数
	SetWindowPos(GetConsoleWindow(),HWND_NOTOPMOST,(int)(nScreenWidth*0.25),(int)(nScreenHeight*0.25),0,0,SWP_NOSIZE);
	system("mode con cols=80 lines=25");
	system("title CharacterGame");

	//初始化
	init();

	/*

	file = fopen("Main.ini","r");

	if( NULL == file)
	{
		fprintf(stderr,"Can't open file \"Main.ini\"");
		getchar();
		exit(-1);
	}

	
	fclose(file);*/
	system("pause");
	return 0;
}
