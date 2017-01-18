#include "graphics.h"
#include "over.h"
#include <ctime>
#include <stdlib.h>
#include <string.h>
#include <queue>
#define set(a,b) memset(a,b,sizeof(a))
#define BLUE_CUSTOM COLOR(75,35,235)

int pcx=25,pcy=28,rad=19,x=0,y=0,i=0,maxx=600,maxy=450; //pac's centre, radius, moving centre, screen size
int direction,tcp=89,count=0,active=0,cursed=0,dinky=0,dclyde=0,scored=0,TCstay=0,turns=0,vengeance=0;
int gcx=245,gcy=303; //ghost's centre
int cyan_x=25, cyan_y=28+55*7;
char ch,temp,prevch;
int adj[89][4],path[88]; //Adjacency list. Array for storing path (relatively useless)
int visit[88],parent[88],dist[88];
char TCpoint[100];
int delaytime=5;

int game_1();
void bg(); //background
void point_make(); //Makes points for food and coordinates
void drawpac(int i,char dir,int pcx,int pcy);
int collide();
void pacdeath();
int check_up();
int check_down();
int check_left();
int check_right();
void food_check();
void new_food_make();
void ghost_design();
void move_up();
void move_down();
void move_left();
void move_right();
void (*move)();
void halt(){ return ;}
void adj_list();
void bfs(int source, int n);
void fixpath(int source,int v,int i);
void GRmove(int gdir);
int GRdir(int key);
void ghostto(int color,int gdir,int gcx,int gcy);
void scoreboard(int count);
void resetpoints();
void random_ghost();
void cyan_move();
void cyan_hit_wall();
void cyan_go_rand();
int cyan_up();
int cyan_down();
int cyan_left();
int cyan_right();
void TCgenerate();
void starting();
struct
{
	int x;
	int y;
} points[100]; //Pretty much the basis of motion in this game

struct g_co
{
	int x;
	int y;
	int radius;
	int poly[20];
}ghost[3];

struct foodcord
{
	int x;
	int y;
	int radius;
}food[100];

void sleep(int mseconds)
{
    clock_t goal = mseconds + clock();
    while(goal>clock());
}

int game_1()
{
	turns=0,vengeance=0;
	setlinestyle(SOLID_LINE,0,1);
	int over=0;
	char prevch,dir;
	bg();
	cyan_x=25;
	cyan_y=28+55*7;
	count=0;
	if(rand()%2)
		direction = KEY_UP;
	else 
		direction = KEY_RIGHT;
	new_food_make();
	pcx=points[0].x;
	pcy=points[0].y;  //Starts Pac from the upper-left corner
	gcx=points[87].x;
	gcy=points[87].y; //Starts Clyde from the lower-right corner
	int i,gdir=KEY_DOWN;
	dir = KEY_RIGHT;
	move = move_right;  //Starts Pacs motion to be towards the right at the beginning of the game
	ghostto(RED,gdir,gcx,gcy);
	ghostto(CYAN,direction,cyan_x,cyan_y);
	drawpac(i,dir,pcx,pcy);
	starting();
	for(;;)
	{
		scoreboard(count);
		sleep(delaytime);
		int p,g,reach=0,gdir;
		for(p=0; p<88; p++)
			if(points[p].x==pcx&&points[p].y==pcy)  //Finds Pac
				break;
		for(g=0; g<88; g++)
			if(points[g].x==gcx&&points[g].y==gcy)  //Finds Clyde
				break;
		if(dclyde==1) //Resets Clyde to initial position at the end of deactivation period
		{
			gcx=points[87].x;
			gcy=points[87].y;
		}
		if(dinky==1)  //same for Inky
		{
			cyan_x = 25;
			cyan_y = 28+55*7;
		}
		if(turns%40==29)			//Sets position for transient curse
		{
			TCstay=10;
			tcp=(p+15)%88;
		}
		if((turns&&turns%40==0)||(turns%40>=30&&turns%40<=39)) /*TC active every 30th to 40th turn*/
		{
			TCstay--;
			sprintf(TCpoint,"POWER-UP : %d",TCstay);
			printf("%d\n", TCstay);
			active=1;
		}
		else
		{
			setfillstyle(SOLID_FILL,BLACK);
			bar(320,maxy+3,500+textwidth(TCpoint),maxy+3+textwidth(TCpoint));
			active=0;
		}
		if(turns%40==1)				//Needed to remove the TC somehow, in case Pac didn't get it :P
		{
			setfillstyle(SOLID_FILL,BLACK);
			fillellipse(points[tcp].x,points[tcp].y,10,10);
		}
		turns++;                   //Alternative for time
		if(tcp==p&&active)			//Pac ate TC, starting cursed timer, removing the TC
		{
			setfillstyle(SOLID_FILL,BLACK);
			fillellipse(points[tcp].x,points[tcp].y,10,10);
			active=0;
			cursed=20;
			tcp=89;
		}
		food_check();
		if(!dclyde)				//deactivating clyde
		{
			bfs(g,p);
			gdir=GRdir(0);
		}
		
		//Actual movement starts here
		for(i=0;i<55;i++)
		{
			if(i==20)
				new_food_make();
			if(active&&!cursed) //generates TC under these conditions
				TCgenerate();
			if(!dclyde)
			{
				GRmove(gdir);
				ghostto(RED,gdir,gcx,gcy);
			}
			
			if(!dinky)
			{
				cyan_move();
				ghostto(CYAN,direction,cyan_x,cyan_y);
			}
			if(dinky<=15&&dinky>0||dclyde<=15&&dclyde>0) //to create artificial lag :v
				sleep(2);
			sleep(delaytime);
			move();
			drawpac(i,dir,pcx,pcy);
			if(kbhit())
			{
				getch();
				dir = getch();
			}
			over=collide();
			if(cursed&&over) //using the variable 'over' to check which ghost collided, and deactivating it
			{
				vengeance++;
				if(over==1&&!dclyde)
				{
					ghostto(BLACK,0,gcx,gcy);
					dclyde=16;
				}
				if(over==2&&!dinky)
				{
					ghostto(BLACK,0,cyan_x,cyan_y);
					dinky=16;
				}
				over=0;		//removes over in cursed mode, so that game doesn't quit (that'd be embarrasing)
			}
			if(over||count==88)
				break;
		}
		
		if(cursed) //timer for cursed
			cursed--;
		if(dclyde) //deactivation timers for clyde and inky
			dclyde--;
		if(dinky)
			dinky--;

		random_ghost();
		if(over)
		{
			pacdeath();
			break;
		}
		if(count==88)
		{
			break;
		}
		//Game Over

		//Checks for next move (Pac)
		if(dir == KEY_UP)
		{
			if(check_up())
				move = move_up;
			else
				move = halt;
		}
		else if(dir == KEY_DOWN)
		{
			if(check_down())
				move = move_down;
			else
				move = halt;
		}
		else if(dir == KEY_LEFT)
		{
			if(check_left())
				move = move_left;
			else
				move = halt;
		}
		else if(dir == KEY_RIGHT)
		{
			if(check_right())
				move = move_right;
			else
				move = halt;
		}
	}
	cursed=0;
	int g=0;
	if(count==88)
		g=gameover(1);
	if(over)
		g=gameover(2);
	return g;
}

//**********************************************  Background  *********************************************//
void bg()
{
	cleardevice();
	setfillstyle(SOLID_FILL, GREEN);
	bar(0,0,5,maxy); //BOUNDARIES
	bar(0,maxy-5,maxx,maxy);
	bar(maxx-5,0,maxx,maxy);
	bar(0,0,maxx,5);
	bar(55,55,60,275); //WALL1
	bar(55,335,60,maxy-55); //WALL2
	bar(110,55,220,60); //WALL3
	bar(275,55,385,60); //WALL4
	bar(435,55,maxx-55,60); //WALL5
	bar(110,maxy-62,280,maxy-57); //WALL6
	bar(330,maxy-62,maxx-110,maxy-57); //WALL7
	bar(maxx-60,110,maxx-55,220); //WALL8
	bar(maxx-60,280,maxx-55,maxy-60); //WALL9
	bar(110,110,115,maxy-112); //WALL10
	bar(165,110,maxx-165,115); //WALL11
	bar(maxx-115,110,maxx-110,maxy-112); //WALL12
	bar(165,165,170,maxy-112); //WALL13
	bar(maxx-170,165,maxx-165,maxy-112); //WALL14
	bar(220,165,maxx-220,170); //WALL15
	bar(220,220,maxx-220,225); //WALL16
	bar(220,225,225,maxy-112); //WALL17
	bar(maxx-225,225,maxx-220,maxy-112); //WALL18
	bar(275,275,maxx-275,280); //WALL19
	bar(275,maxy-118,maxx-275,maxy-113); //WALL20
	point_make();
	set(adj,-1);
	adj_list();
}

//******************************************* HITBOXES *********************************************//
//Checks to the next point in the specified direction to see whether there are walls
int check_up()
{
	int check=0;
	for(int i=pcy; i>=pcy-55; i--)
		if(getpixel(pcx,i)==GREEN)
			check=1;
	if(check==1)
		return 0;
	else
		return 1;
}

int check_down()
{
	int check=0;
	for(int i=pcy; i<=pcy+55; i++)
		if(getpixel(pcx,i)==GREEN)
			check=1;
	if(check==1)
		return 0;
	else
		return 1;
}

int check_left()
{
	int check=0;
	for(int i=pcx; i>=pcx-55; i--)
		if(getpixel(i,pcy)==GREEN)
			check=1;
	if(check==1)
		return 0;
	else
		return 1;
}

int check_right()
{
	int check=0;
	for(int i=pcx; i<=pcx+55; i++)
		if(getpixel(i,pcy)==GREEN)
			check=1;
	if(check==1)
		return 0;
	else
		return 1;
}


//********************************************** FOOD ****************************************************//
void point_make()
{
	int i,j,k;
	points[0].x = food[0].x = 25;
	points[0].y = food[0].y = 28;
	food[0].radius = 4;
	for(i=1,j=55, k = 55;i<88;i++)
	{
		if(i%11==0)
		{
			points[i].x = food[i].x = 25;
			points[i].y = food[i].y = food[i-11].y + j;
			food[i].radius = 4;
		}
		else
		{
			points[i].x = food[i].x = food[i-1].x + k;
			points[i].y = food[i].y = food[i-1].y;
			food[i].radius = 4;
		}
	}
	setcolor(BLACK);
	points[89].x=1000;
	points[89].y=1000;
}

void food_check()  //Removes eaten food so it doesn't regenerate
{
	int i;
	for(i=0;i<88;i++)
	{
		if((((pcx+rad)>=food[i].x+5)&&(pcx-rad)<=food[i].x-5)&&((pcy+rad)>=food[i].y+5&&(pcy-rad)<=food[i].y-5))
		{
			count++;  //Can be used to signal end of game
			food[i].x = 1000;
			food[i].y = 1000;
		}
	}
}
void new_food_make() //Regenarates food in case ghost moves over them
{
	setfillstyle(SOLID_FILL,WHITE);
	int i;
	for(i=0;i<88;i++)
	{
		setcolor(WHITE);
		fillellipse(food[i].x,food[i].y,food[i].radius,food[i].radius);
	}
	setcolor(BLACK);
}

//************************************************* Makeshift AI *******************************************//

void adj_list() //Creates an adjacency list for use in BFS
{
	for(i=0; i<88; i++)
	{
		int a=points[i].x, b=points[i].y;
		int fup=0,fdown=0,fleft=0,fright=0;
		for(int i=b; i>=b-55; i--)
		if(getpixel(a,i)==GREEN)
			fup=1;
		for(int i=b; i<=b+55; i++)
		if(getpixel(a,i)==GREEN)
			fdown=1;
		for(int i=a; i>=a-55; i--)
		if(getpixel(i,b)==GREEN)
			fleft=1;
		for(int i=a; i<=a+55; i++)
		if(getpixel(i,b)==GREEN)
			fright=1;
		int j=0;
		if(!fup)
			adj[i][j++]=i-11;
		if(!fdown)
			adj[i][j++]=i+11;
		if(!fleft)
			adj[i][j++]=i-1;
		if(!fright)
			adj[i][j++]=i+1;
	}
}

void bfs(int source, int n)  //Breadth First Search
{
	std::queue<int> Q;
	set(visit,0);
	set(parent,-1);
	set(dist,-1);
	visit[source]=1;
	dist[source]=0;
	Q.push(source);
	while(!Q.empty())
	{
		int u=Q.front(),i;
		for(i=0;i<4&&adj[u][i]!=-1;i++)
		{
			int v=adj[u][i];
			if(!visit[v])
			{
				visit[v]=1;
				parent[v]=u;
				dist[v]=dist[u]+1;
				Q.push(v);
			}
		}
		Q.pop();
	}
	set(path,-1);
	fixpath(source, n, dist[n]);
}

void fixpath(int source,int v,int i)  //Recursive function to create shortest path in array
{
	if(v==source)
	{
		path[i]=source;
		return;
	}
	else
	{
		path[i]=v;
		fixpath(source, parent[v], i-1);
	}
}

//************************************************ Clyde **************************************************//

int GRdir(int key)
{
	if(path[key+1]==-1)
		return 0;
	if(points[path[key]].x==points[path[key+1]].x&&points[path[key]].y>points[path[key+1]].y)
		return KEY_UP;
	if(points[path[key]].x==points[path[key+1]].x&&points[path[key]].y<points[path[key+1]].y)
		return KEY_DOWN;
	if(points[path[key]].y==points[path[key+1]].y&&points[path[key]].x>points[path[key+1]].x)
		return KEY_LEFT;
	if(points[path[key]].y==points[path[key+1]].y&&points[path[key]].x<points[path[key+1]].x)
		return KEY_RIGHT;
}

void GRmove(int gdir)
{
	if(gdir==KEY_UP)
		gcy--;
	if(gdir==KEY_DOWN)
		gcy++;
	if(gdir==KEY_LEFT)
		gcx--;
	if(gdir==KEY_RIGHT)
		gcx++;
	if(gdir==0)
		return;
}

void ghostto(int color,int gdir,int gcx,int gcy)
{
	setfillstyle(SOLID_FILL,color);
	int ghost_poly[] = {gcx,gcy+15,gcx+5,gcy+10,gcx+10,gcy+15,gcx+15,gcy+10,gcx+15,gcy-10,gcx+10,gcy-15,gcx+5,gcy-20,gcx-5,gcy-20,gcx-15,gcy-10,
		gcx-15,gcy-5,gcx-15,gcy+10,gcx-10,gcy+15,gcx-5,gcy+10,gcx,gcy+15};
	fillpoly(13,ghost_poly);
	if(gdir)
	{
		setfillstyle(SOLID_FILL,WHITE);
		fillellipse(gcx-7,gcy-5,5,5);
		fillellipse(gcx+7,gcy-5,5,5);
	}
	setfillstyle(SOLID_FILL,BLACK);
	if(gdir == KEY_UP) fillellipse(gcx-7,gcy-7,3,3),fillellipse(gcx+7,gcy-7,3,3);
	if(gdir == KEY_DOWN) fillellipse(gcx-7,gcy-3,3,3),fillellipse(gcx+7,gcy-3,3,3);
	if(gdir == KEY_LEFT) fillellipse(gcx-9,gcy-5,3,3),fillellipse(gcx+5,gcy-5,3,3);
	if(gdir == KEY_RIGHT) fillellipse(gcx-5,gcy-5,3,3),fillellipse(gcx+9,gcy-5,3,3);
}

//************************************************* Pac *************************************************//

void drawpac(int i,char dir,int pcx,int pcy)
{
	if(cursed)
		setfillstyle(SOLID_FILL,WHITE);
	else
		setfillstyle(SOLID_FILL,YELLOW);
	fillellipse(pcx,pcy,rad,rad);
	setfillstyle(SOLID_FILL,BLACK);
	if(i<27)
	{
		if(dir==KEY_RIGHT)
			pieslice(pcx,pcy,-45,45,rad);
		if(dir==KEY_UP)
			pieslice(pcx,pcy,45,135,rad);
		if(dir==KEY_LEFT)
			pieslice(pcx,pcy,135,225,rad);
		if(dir==KEY_DOWN)
			pieslice(pcx,pcy,225,-45,rad);
	}
}

void pacdeath()
{
	int i;
	setlinestyle(SOLID_LINE,0,2);
	setfillstyle(SOLID_FILL,YELLOW);
	fillellipse(pcx,pcy,rad,rad);
	for(i=1;i<175;i++)
	{
		sleep(10);
		pieslice(pcx,pcy,i,-i,rad);
	}
	setfillstyle(SOLID_FILL,BLACK);
	fillellipse(pcx,pcy,rad,rad);
	setlinestyle(SOLID_LINE,0,1);
}

void move_up()
{
	--pcy;
}
void move_down()
{
	++pcy;
}
void move_left()
{
	--pcx;
}
void move_right()
{
	++pcx;
}


//*************************************************** Collision *********************************************//

int collide()
{
	int dx[4]={0,0,rad+2,-(rad+2)};
	int dy[4]={rad+2,-(rad+2),0,0};
	for(int i=0;i<4;i++)
	{
		int color=getpixel(pcx+dx[i],pcy+dy[i]);
		if(color==RED)
			return 1;
		if(color==CYAN)
			return 2;
	}
	return 0;
}

//************************************************** Inky *************************************************//

void cyan_hit_wall()
{
	if(direction == KEY_UP)
	{
		if(!cyan_up())
		{
			if(rand()%2)
			{
				if(cyan_right())
					direction = KEY_RIGHT;
				else if(cyan_left())
					direction = KEY_LEFT;
				else
					direction = KEY_DOWN;
			}
			else
			{
				if(cyan_left())
					direction = KEY_LEFT;
				else if(cyan_right())
					direction = KEY_RIGHT;
				else
					direction = KEY_DOWN;
			}
		}
	}
	else if(direction == KEY_DOWN)
	{
		if(!cyan_down())
		{
			if(rand()%2)
			{
				if(cyan_right())
					direction = KEY_RIGHT;
				else if(cyan_left())
					direction = KEY_LEFT;
				else
					direction = KEY_UP;
			}
			else
			{
				if(cyan_left())
					direction = KEY_LEFT;
				else if(cyan_right())
					direction = KEY_RIGHT;
				else
					direction = KEY_UP;
			}
		}
	}
	else if(direction == KEY_LEFT)
	{
		if(!cyan_left())
		{
			if(rand()%2)
			{
				if(cyan_up())
					direction = KEY_UP;
				else if(cyan_down())
					direction = KEY_DOWN;
				else
					direction = KEY_RIGHT;
			}
			else
			{
				if(cyan_down())
					direction = KEY_DOWN;
				else if(cyan_up())
					direction = KEY_UP;
				else
					direction = KEY_RIGHT;
			}
		}
	}
	else if(direction == KEY_RIGHT)
	{
		if(!cyan_right())
		{
			if(rand()%2)
			{
				if(cyan_up())
					direction = KEY_UP;
				else if(cyan_down())
					direction = KEY_DOWN;
				else
					direction = KEY_LEFT;
			}
			else
			{
				if(cyan_down())
					direction = KEY_DOWN;
				else if(cyan_up())
					direction = KEY_UP;
				else
					direction = KEY_LEFT;
			}
		}
	}
}

void cyan_go_rand()
{
	int random = rand()%3;
	if(direction == KEY_UP)
	{
		if(!random)
		{
			if(cyan_right())
				direction = KEY_RIGHT;
			else if(cyan_left())
				direction = KEY_LEFT;
		}
		else if(random==1)
		{
			if(cyan_left())
				direction = KEY_LEFT;
			else if(cyan_right())
				direction = KEY_RIGHT;
		}
		else
			return;
	}
	else if(direction == KEY_DOWN)
	{
		if(!random)
		{
			if(cyan_right())
				direction = KEY_RIGHT;
			else if(cyan_left())
				direction = KEY_LEFT;
		}
		else if(random == 1)
		{
			if(cyan_left())
				direction = KEY_LEFT;
			else if(cyan_right())
				direction = KEY_RIGHT;
		}
		else
			return;
	}
	else if(direction == KEY_LEFT)
	{
		if(!random)
		{
			if(cyan_up())
				direction = KEY_UP;
			else if(cyan_down())
				direction = KEY_DOWN;
		}
		else if(random == 1)
		{
			if(cyan_down())
				direction = KEY_DOWN;
			else if(cyan_up())
				direction = KEY_UP;
		}
		else
			return;
	}
	else if(direction == KEY_RIGHT)
	{
		if(!random)
		{
			if(cyan_up())
				direction = KEY_UP;
			else if(cyan_down())
				direction = KEY_DOWN;
		}
		else if(random == 1)
		{
			if(cyan_down())
				direction = KEY_DOWN;
			else if(cyan_up())
				direction = KEY_UP;
		}
		else
			return;
	}
}

void cyan_move()
{
	if(direction == KEY_UP)
		cyan_y--;
	else if(direction == KEY_DOWN)
		cyan_y++;
	else if(direction == KEY_LEFT)
		cyan_x--;
	else if(direction == KEY_RIGHT)
		cyan_x++;
}

void random_ghost()
{
	cyan_go_rand();
	cyan_hit_wall();
}

int cyan_up()
{
	int check=0;
	for(int i=cyan_y; i>=cyan_y-55; i--)
		if(getpixel(cyan_x,i)==GREEN)
			check=1;
	if(check==1)
		return 0;
	else
		return 1;
}

int cyan_down()
{
	int check=0;
	for(int i=cyan_y; i<=cyan_y+55; i++)
		if(getpixel(cyan_x,i)==GREEN)
			check=1;
	if(check==1)
		return 0;
	else
		return 1;
}

int cyan_left()
{
	int check=0;
	for(int i=cyan_x; i>=cyan_x-55; i--)
		if(getpixel(i,cyan_y)==GREEN)
			check=1;
	if(check==1)
		return 0;
	else
		return 1;
}

int cyan_right()
{
	int check=0;
	for(int i=cyan_x; i<=cyan_x+55; i++)
		if(getpixel(i,cyan_y)==GREEN)
			check=1;
	if(check==1)
		return 0;
	else
		return 1;
}

//******************************************** Transient Curse ********************************************//

void TCgenerate()
{
		setfillstyle(SOLID_FILL, MAGENTA);
		fillellipse(points[tcp].x,points[tcp].y,10,10);
}
//************************************************** Misc *************************************************//
void scoreboard(int count)
{
	setcolor(WHITE);
	settextstyle(3,HORIZ_DIR,3);
	char num[100];
	char score[] = "Score:";
	scored = count*100 - turns*10 + vengeance*200;
	sprintf(num,"%d",scored);
	outtextxy(0,maxy+3,score);
	outtextxy(textwidth(score),maxy+3,num);
	bar(320,maxy+3,500+textwidth(TCpoint),maxy+3+textwidth(TCpoint));
	setcolor(MAGENTA);
	if(TCstay)
		outtextxy(320,maxy+3,TCpoint);
	setcolor(BLACK);
}
void starting()
{
	setcolor(YELLOW);
	settextstyle(3,HORIZ_DIR,3);
	char three[] = "3...";
	char two[] = "2...";
	char one[] = "1...";
	int i;
	for(i=0;i<3;i++)
	{
		if(i == 0)
			outtextxy(300,maxy+3,three);
		else if(i == 1)
			outtextxy(300,maxy+3+textheight(three),two);
		else if(i == 2)
			outtextxy(300,maxy+3+textheight(three)+textheight(two),one);
		sleep(500);
	}
	setcolor(BLACK);
	setfillstyle(SOLID_FILL,BLACK);
	bar(300,maxy+3,300+textwidth(three),maxy+3+textheight(three)+textheight(two)+textheight(one));
}