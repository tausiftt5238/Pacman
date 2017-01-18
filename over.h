#include "graphics.h"

char result[3][200]={"Mission", "YOU DIED", "Complete"};
char over2[2][200]={"REFUSE TO QUIT", "ABANDON ALL HOPE"};
int cx=20,cy=180,w,h;
int gameover(int type);
int sel(int trigger);
int refx,refy,abax,abay;
struct scoreboard
{
	char name[100];
	int points;
};

int gameover(int type)
{
	cleardevice();
	setlinestyle(SOLID_LINE,0,1);
	int select=0;
	if(type==1)
	{
		setcolor(YELLOW);
		settextstyle(4,HORIZ_DIR,7);
		outtextxy(150,cy-50,result[0]);
		outtextxy(105,cy+textheight(result[0])-50,result[2]);
		delay(1000);
		return 2;
	}
	if(type==2)
	{
		setcolor(RED);
		settextstyle(3,HORIZ_DIR,7);
		outtextxy(140,cy,result[1]);
		h=textheight(result[1]);
		setcolor(CYAN);
		settextstyle(0,HORIZ_DIR,0);
		refx = cx+210,refy = cy+(h+30);
		abax = cx+200,abay = cy+(h+50)+textheight(over2[0]);
		outtextxy(refx,refy,over2[0]);
		outtextxy(abax,abay,over2[1]);
		setlinestyle(SOLID_LINE,0,1);
		setcolor(GREEN);
		rectangle(refx,refy,refx+textwidth(over2[0]),refy+textheight(over2[0]));
	}
	sel(1);
	while(1)
	{
		if(kbhit())
			select=sel(0);
		if(select)
			break;
	}
	setlinestyle(SOLID_LINE,0,1);
	return select;
}

int sel(int trigger)
{
	static int i = 1;
	if(trigger)
	{
		i = 1;
		return 0;
	}
	char a = 0,b = 0;
	b = getch();

	if(b == '\r')
		return i;
	setlinestyle(SOLID_LINE,0,1);
	
	a = getch();
	if(a == KEY_UP && i == 1)
		return 0;
	if(a == KEY_DOWN && i == 2)
		return 0;
	if(a == KEY_DOWN)
		i++;
	if(a == KEY_UP)
		i--;
	if(i == 1)
	{
		setcolor(BLACK);
		rectangle(abax,abay,abax+textwidth(over2[1]),abay+textheight(over2[0]));
		setcolor(GREEN);
		rectangle(refx,refy,refx+textwidth(over2[0]),refy+textheight(over2[0]));
	}
	if(i == 2)
	{
		setcolor(BLACK);
		rectangle(refx,refy,refx+textwidth(over2[0]),refy+textheight(over2[0]));
		setcolor(GREEN);
		rectangle(abax,abay,abax+textwidth(over2[1]),abay+textheight(over2[0]));
	}
	return 0;
}