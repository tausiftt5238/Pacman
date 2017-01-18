#include "graphics.h"
#include "pacman.h"
#include <cstdio>
#include <cstring>
int selecx = 240,selecy = 180;
int maxhigh=0;
char newgame[] = "NEW GAME";
char how_to_play[] = "HOW TO PLAY";
char exiting[] = "EXIT";
char highscore[] = "HIGHSCORE";
void menu();
int selection(int trigger);
void enter_inky();
void enter_clyd();
void moving_pack(int i);
void leaderboard();
void howtoplay();
void high_input();
int main()
{
	srand(time(0));
	initwindow(maxx,maxy+100,"menu");
	setlinestyle(SOLID_LINE,0,2);
	menu();
	setlinestyle(SOLID_LINE,0,1);
	int select = 0,game=4;
	int i = 0;
	for(int clyd = 0,inky = clyd - selecx;;)
	{
		i++;
		sleep(5);
		moving_pack(i);
		if(kbhit())
			select = selection(0);
		if(select == 1)
		{
			game = game_1();
			if(maxhigh<scored)
				maxhigh = scored;
			if(game==2)
			{
				high_input();
				//break;
				select = 0;
				selection(1);
				menu();
				maxhigh=0;
			}
			if(game == 1)
			{
				continue;
			}
		}
		else if(select == 2)
		{
			howtoplay();
			select = 0;
			selection(1);
			menu();
		}
		else if(select == 3)
		{
			leaderboard();
			select = 0;
			selection(1);
			menu();
		}
		else if(select==4)
			break;
		i %= (2*27);
		maxhigh=0;
	}
	return 0;
}

void menu()
{
	cleardevice();
	setcolor(GREEN);
	settextstyle(3,HORIZ_DIR,50);
	outtextxy(80,0,"PROJECT PAC");
	settextstyle(0,HORIZ_DIR,0);
	outtextxy(selecx,selecy,newgame);
	outtextxy(selecx,selecy+textheight(newgame),how_to_play);
	outtextxy(selecx,selecy+textheight(newgame)+textheight(how_to_play),highscore);
	outtextxy(selecx,selecy+textheight(newgame)+textheight(how_to_play)+textheight(highscore),exiting);
	setcolor(GREEN);
	rectangle(selecx,selecy,selecx+textwidth(newgame),selecy+textheight(newgame));
	setcolor(BLUE_CUSTOM);
	settextstyle(3,HORIZ_DIR,1);
	outtextxy(190,490,"BY SAYONTAN AND TAUSIF");
	settextstyle(0,HORIZ_DIR,0);
	
}
int selection(int trigger)
{
	setlinestyle(SOLID_LINE,0,2);
	static int i = 0;
	if(trigger)
	{
		i = 0;
		return 0;
	}
	char a = 0,b = 0;
	b = getch();

	if(b == '\r')
		return i+1;
	
	a = getch();
	if(a == KEY_UP && i == 0)
		return 0;
	if(a == KEY_DOWN && i == 3)
		return 0;
	if(a == KEY_DOWN)
		i++;
	if(a == KEY_UP)
		i--;
	if(i == 0)
	{
		setcolor(BLACK);
		rectangle(selecx,selecy+textheight(newgame),selecx+textwidth(how_to_play),selecy+textheight(newgame)+textheight(how_to_play));
		rectangle(selecx,selecy+textheight(newgame)+textheight(how_to_play),selecx+textwidth(highscore),selecy+textheight(newgame)+textheight(how_to_play)+textheight(highscore));
		rectangle(selecx,selecy+textheight(newgame)+textheight(how_to_play)+textheight(highscore),selecx+textwidth(exiting),selecy+textheight(newgame)+textheight(how_to_play)+textheight(highscore)+textheight(exiting));
		setcolor(GREEN);
		rectangle(selecx,selecy,selecx+textwidth(newgame),selecy+textheight(newgame));
	}
	if(i == 1)
	{
		setcolor(BLACK);
		rectangle(selecx,selecy,selecx+textwidth(newgame),selecy+textheight(newgame));
		rectangle(selecx,selecy+textheight(newgame)+textheight(how_to_play),selecx+textwidth(highscore),selecy+textheight(newgame)+textheight(how_to_play)+textheight(highscore));
		rectangle(selecx,selecy+textheight(newgame)+textheight(how_to_play)+textheight(highscore),selecx+textwidth(exiting),selecy+textheight(newgame)+textheight(how_to_play)+textheight(highscore)+textheight(exiting));
		setcolor(GREEN);
		rectangle(selecx,selecy+textheight(newgame),selecx+textwidth(how_to_play),selecy+textheight(newgame)+textheight(how_to_play));
	}
	if(i == 2)
	{
		setcolor(BLACK);
		rectangle(selecx,selecy,selecx+textwidth(newgame),selecy+textheight(newgame));
		rectangle(selecx,selecy+textheight(newgame),selecx+textwidth(how_to_play),selecy+textheight(newgame)+textheight(how_to_play));
		rectangle(selecx,selecy+textheight(newgame)+textheight(how_to_play)+textheight(highscore),selecx+textwidth(exiting),selecy+textheight(newgame)+textheight(how_to_play)+textheight(highscore)+textheight(exiting));
		setcolor(GREEN);
		rectangle(selecx,selecy+textheight(newgame)+textheight(how_to_play),selecx+textwidth(highscore),selecy+textheight(newgame)+textheight(how_to_play)+textheight(highscore));
	}
	if(i == 3)
	{
		setcolor(BLACK);
		rectangle(selecx,selecy+textheight(newgame)+textheight(how_to_play),selecx+textwidth(highscore),selecy+textheight(newgame)+textheight(how_to_play)+textheight(highscore));
		rectangle(selecx,selecy,selecx+textwidth(newgame),selecy+textheight(newgame));
		rectangle(selecx,selecy+textheight(newgame),selecx+textwidth(how_to_play),selecy+textheight(newgame)+textheight(how_to_play));
		setcolor(GREEN);
		rectangle(selecx,selecy+textheight(newgame)+textheight(how_to_play)+textheight(highscore),selecx+textwidth(exiting),selecy+textheight(newgame)+textheight(how_to_play)+textheight(highscore)+textheight(exiting));
	}
	setlinestyle(SOLID_LINE,0,1);
	return 0;
}
void moving_pack(int i)
{
	setcolor(BLACK);
	static int y = 90;
	static int pcx = 0 ,cyx = -44,inx = -88;
	drawpac(i,KEY_RIGHT,pcx,y);
	ghostto(RED,KEY_RIGHT,cyx,y);
	ghostto(CYAN,KEY_RIGHT,inx,y);
	pcx++;
	cyx++;
	inx++;
	if(pcx == maxx+100+rad)
		pcx = 0;
	if(cyx == maxx+100+rad)
		cyx = 0;
	if(inx == maxx+100+rad)
		inx = 0;
}
void enter_inky()
{
	static int x = 0;
	int y = 350;
	if(x<selecx)
		x++;
	ghostto(CYAN,KEY_RIGHT,x,y);
}
void enter_clyd()
{
	static int x = 0;
	int y = 300;
	if(x<selecx)
		x++;
	ghostto(RED,KEY_RIGHT,x,y);
}
void howtoplay()
{
	cleardevice();
	char welcome[] = "WELCOME TO THE WORLD OF";
	char paccu[] = "PACMAN";
	setcolor(YELLOW);
	settextstyle(3,HORIZ_DIR,3);
	outtextxy(20,10,welcome);
	outtextxy(220,10+textheight(welcome),paccu);
	settextstyle(3,HORIZ_DIR,1);
	char intro[] = "YOU ARE PLAYING AS PACMAN, THE YELLOW BALL SHAPE THINGY.";
	outtextxy(0,100,intro);
	outtextxy(0,100+textheight(intro),"YOUR GOAL IS TO EAT ALL THE FOODS");
	outtextxy(0,100+2*textheight(intro)," WHILE AVOIDING CONTACT WITH THE GHOSTS.");
	outtextxy(0,100+3*textheight(intro),"IF ANY GHOST TOUCHES YOU,IT'S GAME OVER.");
	outtextxy(0,100+4*textheight(intro),"IF YOU EAT ALL THE FOODS, YOU WIN.");
	outtextxy(0,100+6*textheight(intro),"USE DIRECTIONAL KEYS TO MOVE.");
	outtextxy(0,100+8*textheight(intro),"EAT POWER-UPS TO HAVE THE POWER TO EAT GHOSTS INSTEAD.");
	outtextxy(0,100+10*textheight(intro),"PRESS X TO RETURN TO MAIN-MENU.");
	setcolor(BLACK);
	char command;
	for(;;)
	{
		command = getch();
		if(command == 'x' || command == 'X')
			return;
		else 
			continue;
	}
}
void leaderboard()
{
	cleardevice();
	setcolor(BLUE_CUSTOM);
	settextstyle(3,HORIZ_DIR,3);
	outtextxy(180,0,"HIGHSCORE");
	settextstyle(3,HORIZ_DIR,1);
	FILE *fp;
	fp = fopen("scores.txt","r");
	struct scoreboard list[5];
	int i;
	for(i=0;i<5;i++)
	{
		fscanf(fp,"%s%d",list[i].name,&list[i].points);
	}
	int line = 3;
	outtextxy(150,100,"NAME");
	outtextxy(380,100,"SCORE");
	//outtextxy(400,100,"DATE");
	for(i=0;i<5;i++,line++)
	{
		char temp[100];
		sprintf(temp,"%s",list[i].name);
		outtextxy(150,50*line,temp);
		sprintf(temp,"%d",list[i].points);
		outtextxy(380,50*line,temp);
	}
	outtextxy(150,50*line,"PRESS X TO RETURN TO MAIN-MENU.");
	char command;
	for(;;)
	{
		command = getch();
		if(command == 'x' || command == 'X')
			return;
		else 
			continue;
	}
}
void high_input()
{
	cleardevice();
	struct scoreboard tempo;
	FILE *fp;
	char winner[100];
	fp = fopen("scores.txt","r");
	struct scoreboard list[6];
	int i;
	for(i=0;i<5;i++)
	{
		fscanf(fp,"%s%d",list[i].name,&list[i].points);
	}
	fclose(fp);
	if(maxhigh>=list[4].points)
	{
		settextstyle(3,HORIZ_DIR,1);
		char text[10];
		char temp[4];
		int count = 3;
		setcolor(BLUE_CUSTOM);
		//setcolor(BLUE);
		outtextxy(200,200,"ENTER YOUR NAME:");
		outtextxy(200,200+50,"_ _ _");
		temp[0] = getch();
		if(temp[0]>='a' &&temp[0]<='z')
			temp[0] -= 32;
		sprintf(text,"%c _ _ ",temp[0]);
		outtextxy(200,200+50,text);
		temp[1] = getch();
		if(temp[1]>='a' &&temp[1]<='z')
			temp[1] -= 32;
		sprintf(text,"%c %c _ ",temp[0],temp[1]);
		outtextxy(200,200+50,text);
		temp[2] = getch();
		if(temp[2]>='a' &&temp[2]<='z')
			temp[2] -= 32;
		sprintf(text,"%c %c %c ",temp[0],temp[1],temp[2]);
		outtextxy(200,200+50,text);
		temp[3] = 0;
		sleep(500);
		strcpy(list[5].name,temp);
		list[5].points = maxhigh;
	}
	int j;
	for(i=0;i<6;i++)
	{
		for(j=0;j<5;j++)
		{
			if(list[j].points<list[j+1].points)
			{
				tempo = list[j];
				list[j] = list[j+1];
				list[j+1] = tempo;
			}
		}
	}
	fp = fopen("scores.txt","w");
	for(i=0;i<5;i++)
		fprintf(fp,"%s %d\n",list[i].name,list[i].points);
	fclose(fp);
	setcolor(BLACK);
	leaderboard();
}