/*************************************** GOMOKU GAME *************************************************/


char board[30][30];
int cnt = 0;    //number of chess pices has been put
char pieces[2] = {'X', 'O'};    //player 1, 2's chess

int dxx[8] = {-1,0,-1,-1,1,1,0,1};
int dyy[8] = {0,-1,-1,1,-1,1,1,0};

void init(){
    cnt = 0;
    for(int i = 1; i <= 15; i++)
        for(int j = 1; j <= 15; j++)
            board[i][j] = '*';
}

void printBoard(){
    int i,j;
    for(i = 1; i <= 15; i ++){
        for(j = 1; j <= 15; j++){
            
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
}

int check(int x, int y){
    if(x < 1||x > 15||y < 1||y > 15){
        return 0;
    }
    return 1;
}

int checkInput(int x, int y){
    if(board[x][y] != '*' || (!check(x, y))){
        return 0;
    }
    return 1;
}

int checkWinner(int num, int x, int y){
    if(cnt == 15 * 15)
        return 1;
    //check four directions
    for(int i = 0; i < 4; i++){
        int sum = 1;
        int xx = x + dxx[i];
        int yy = y + dyy[i];
        while(board[xx][yy] == pieces[num] && check(xx,yy) == 1){
            sum++;
            xx = xx+dxx[i];
            yy = yy+dyy[i];
        }
        xx = x + dxx[7-i];
        yy = y + dyy[7-i];
        while(board[xx][yy] == pieces[num] && check(xx,yy) == 1){
            sum++;
            xx = xx+dxx[7-i];
            yy = yy+dyy[7-i];
        }
        if(sum >= 5)
            return 1;
    }
    return 0;
}

int charToInt(char c){
	return (c - '0');
}

void run(){
	char xy[6];
    int x, y;
    int ifPut = 0;
    init();
    int num = 0;    //player number
    while(1){
        printBoard();
        printf("\n");
        printf("player %d please input the location of your chess piece, just like '01 01':", num + 1);
        
        while(1){
            read(0, xy, 6);
            if (xy[0] == 'q'){
            	return;
            }
            if (xy[0] == 'h'){
            	// printf("%s\n", xy);
            	GomokuHelp();
            	break;
            }
            x = charToInt(xy[0]) * 10 + charToInt(xy[1]);
            y = charToInt(xy[3]) * 10 + charToInt(xy[4]);
            if(checkInput(x,y) == 1){
                ifPut = 1;
                break;
            }
            printf("Locatin illegal, please input again: \n");
        }
        if (ifPut == 1){
        	board[x][y] = pieces[num];
            cnt++;
            ifPut = 0;
        }
        if(checkWinner(num,x,y) == 1){
            printf("%d player win! \n", num + 1);
            return;
        }
      
        num = 1 - num;
    }
}

void GomokuHelp(){
	printf("***********************************************\n");
    printf("*  1. Enter 1-15 row number                   *\n");
    printf("*  2. Enter 1-15 col number                   *\n");
    printf("*  3. Player 1 hold 'X' and Palyer 2 hold 'O' *\n");
    printf("*  4. Enter q to quit                         *\n");
    printf("*  5. Enter h to show help menu               *\n");
    printf("***********************************************\n");
}

void Gomoku(){
    printf("***********************************************\n");
    printf("*                    GOMOKU                   *\n");
    printf("***********************************************\n");
    
    GomokuHelp();
    printf("please put your chess pieces within 15 checker!\n");
    run();

    printf("GAME OVER!\n");
    printf("\n");
}


/*************************************** SNAKE GAME *************************************************/

/******************************************************************
                              random                           
*******************************************************************/
int do_rand(unsigned int *value){  
      int quotient, remainder, t;  
  
      quotient = *value / 127773L;  
      remainder = *value % 127773L;  
      t = 16807L * remainder - 2836L * quotient;  
      if (t <= 0)  
      t += 0x7FFFFFFFL;  
      return ((*value = t) % ((unsigned int)0x7FFFFFFFL + 1));  
}

unsigned int next = 1;  

int rand(void)  {  
   return do_rand(&next);  
}  

void srand(unsigned int seed){           //赋初值为种子    
   next = seed;  
}  

/******************************************************************
                              clear                           
*******************************************************************/

void clear(){ //screen cleaner
    clear_screen(0, console_table[current_console].cursor);
    console_table[current_console].crtc_start = 0;
    console_table[current_console].cursor = 0;
}

/******************************************************************
                              sleep                           
*******************************************************************/
void sleep(int pauseTime){
	int i = 0;
	for (i = 0; i < pauseTime*10000; ++i)
	{
		;
	}
}





const int H = 8;   //地图的高
const int L = 16;  //地图的长
char GameMap[8][16];   //游戏地图
int  sum = 1, over = 0;  //蛇的长度, 游戏结束(自吃或碰墙)
int  dx[4] = {0, 0, -1, 1};  //左、右、上、下的方向
int  dy[4] = {-1, 1, 0, 0};

int listener = 0;

struct Snake { //蛇的每个节点的数据类型
    int x, y;  //左边位置
    int now;   //保存当前节点的方向, 0,1,2,3分别为左右上下
}Snake[8*16];

const char Shead = '@';  //蛇头
const char Sbody = 'O';  //蛇身
const char Sfood = '*';  //食物
const char Snode = '.';  //'.'在地图上标示为空


void PrintMap();
void InitMap();  //地图的初始化
void CreateFood(); //在地图上随机产生食物
void UpdateMap();   //刷新显示地图
void Move();   //蛇的移动
void CheckBorder();  //检查蛇头是否越界
void CheckHead(int x, int y);   //检查蛇头移动后的位置情况


/******************************************************************
                        KeyboardListener                           
*******************************************************************/
PUBLIC void ButtonListener(u32 key){ //取出按键,并判断方向
    char output[2] = {'\0', '\0'};
    
    if (!(key & FLAG_EXT)){
        output[0] = key & 0xFF;
        // printf("%c\n", output[0]);
        if (output[0] == 'a' && listener == 1) {
        	Snake[0].now = 0;
        	listener = 0;
        }
        if (output[0] == 'd' && listener == 1) {
        	Snake[0].now = 1;
        	listener = 0;
        }
        if (output[0] == 'w' && listener == 1) {
        	Snake[0].now = 2;
        	listener = 0;
        }
        if (output[0] == 's' && listener == 1) {
        	Snake[0].now = 3;
        	listener = 0;
        }
    }
}

void snake(){
    InitMap();
    UpdateMap();
}

void PrintMap(){
    for(int i = 0; i < H; i++){
        for(int j = 0; j < L; j++){
            printf("%c", GameMap[i][j]);
        }
        printf("\n");
    }
}

void InitMap() {
    // int i, j;
    int hx, hy;
    memset(GameMap, Snode, sizeof(GameMap));  //初始化地图全部为空'.'
    clear();
    // srand(0);   //随机种子
    
    hx = rand() % H;    //产生蛇头
    hy = rand() % L;
    GameMap[hx][hy] = Shead;
    Snake[0].x = hx;
    Snake[0].y = hy;
    Snake[0].now = -1;
    
    printf("\n||***************Snake Game!***************||\n");
    printf("||*****************************************||\n");
    CreateFood();   //随机产生食物
    PrintMap();
    
    printf("Using 'w','s','a','d' to control directions, please press any key to start:\n");
    listener = 1;
    while(listener){};
}

void CreateFood(){ //在地图上随机产生食物
    int fx, fy;
    while(1){
        fx = rand() % H;
        fy = rand() % L;
        
        if(GameMap[fx][fy] == Snode){  //不能出现在蛇所占有的位置
            GameMap[fx][fy] = Sfood;
            break;
        }
    }
}

void UpdateMap(){  //刷新显示地图

    while(1){
    	listener = 1;
        sleep(1000); //延迟半秒(1000为1s),即每半秒刷新一次地图
        printf("Snake[0].now = %d\n", Snake[0].now);
        // if (Snake[0].now != -1){
        //     Move();
        // }
        Move();
        if(over){  //自吃或碰墙即游戏结束
            printf("***************GameOver***************\n");
            break;
        }
        clear();   //清空地图再显示刷新吼的地图
        PrintMap();
    }
}


void Move(){   //蛇的移动
    int i, x, y;
    int t = sum;  //保存当前蛇的长度
    //记录当前蛇头的位置,并设置为空,蛇头先移动
    x = Snake[0].x;  
    y = Snake[0].y;
    GameMap[x][y] = Snode;
    Snake[0].x = Snake[0].x + dx[Snake[0].now];
    Snake[0].y = Snake[0].y + dy[Snake[0].now];
    CheckBorder();   //蛇头是否越界
    CheckHead(x, y);  //蛇头移动后的位置情况,参数为: 蛇头的开始位置
    if(sum == t)  //未吃到食物即蛇身移动哦
        for(i = 1; i < sum; i++){ //从蛇尾节点向前移动,前一个节点作为参照
            if(i == 1){
                GameMap[Snake[i].x][Snake[i].y] = Snode;
            }
            if(i == sum - 1){  //为蛇头后面的蛇身节点,特殊处理
                Snake[i].x = x;
                Snake[i].y = y;
                Snake[i].now = Snake[0].now;
            }
            else   //其他蛇身即走到前一个蛇身位置
            {
                Snake[i].x = Snake[i+1].x;
                Snake[i].y = Snake[i+1].y;
                Snake[i].now = Snake[i+1].now;
            }
            
            GameMap[Snake[i].x][Snake[i].y] = Sbody;
        }
}

void CheckBorder(){  //检查蛇头是否越界
    if(Snake[0].x < 0 || Snake[0].x >= H
       || Snake[0].y < 0 || Snake[0].y >= L)
        over = 1;
}

void CheckHead(int x, int y){  //检查蛇头移动后的位置情况
    if(GameMap[ Snake[0].x][Snake[0].y] == Snode){
        GameMap[ Snake[0].x][Snake[0].y] = Shead;
    }
    else {
        if(GameMap[Snake[0].x][Snake[0].y] == Snode){ //为食物
            GameMap[ Snake[0].x][ Snake[0].y] = Shead;
            Snake[sum].x = x;   //新增加的蛇身为蛇头后面的那个
            Snake[sum].y = y;
            Snake[sum].now = Snake[0].now;
            GameMap[ Snake[sum].x][Snake[sum].y] = Sbody;
            sum++;
            CreateFood();  //食物吃完了马上再产生一个食物
        }
        else {
            over = 1;
        }
    }
}


/*************************************** PUSHBOX GAME *************************************************/

const int ROWS = 10;
const int COLUMNS = 12; //之所以定义为11，是因为字符串的尾部的\0

void DrawMap();
void SwapXY(int *x1, int *y1, int *x2, int *y2);
int IsLose(int boxPositionX, int boxPositionY, char condition); //判断是否已输
void PushBox(int *boxPositionX, int *boxPositionY, int *manPositionX, int *manPositionY); //接收用户的输入，然后做出相应的处理

const char wall = '#';
const char road = ' ';
const char punch = 'X';
const char man = 'O';

void Clear(){
    int i = 0;
    for(i = 0; i < 30; i++)
        printf("\n");
}

char pushBoxMap[10][12] =
{
    {'#','#','#','#','#','#','#','#','#','#','\n','\0'},
    {'#','O',' ','#','#','#','#',' ',' ','#','\n','\0'},
    {'#',' ','X','#','#','#','#',' ',' ','#','\n','\0'},
    {'#',' ',' ',' ',' ',' ',' ',' ',' ','#','\n','\0'},
    {'#','#','#','#','#','#',' ',' ',' ','#','\n','\0'},
    {'#',' ',' ','#','#','#','#',' ',' ','#','\n','\0'},
    {'#',' ',' ',' ',' ',' ',' ',' ',' ','#','\n','\0'},
    {'#',' ',' ',' ','#','#','#','#','#','#','\n','\0'},
    {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ','\n','\0'},
    {'#','#','#','#','#','#','#','#','#','#','\n','\0'},
};


int main(){
    int boxPositionX,boxPositionY;
    int manPositionX,manPositionY;
    
    //定义地图的出口，也就是箱子到达此处就赢了
    int winPositionX = ROWS - 2;
    int winPositionY = COLUMNS - 3;
    int IsWin = 0;
    manPositionX = 1, manPositionY = 1;
    boxPositionX = 2, boxPositionY = 2;
    
    DrawMap();
    
    while(!IsWin){
        int boxNextPositionX = boxPositionX;
        int boxNextPositionY = boxPositionY;
        int manNextPositionX = manPositionX;
        int manNextPositionY = manPositionY;
        //获取用户的输入
        PushBox(&boxNextPositionX, &boxNextPositionY, &manNextPositionX, &manNextPositionY);
        //
        Clear();
        
        //这里处理走之后的效果 TODO：
        if(pushBoxMap[boxNextPositionX][boxNextPositionY] == road
           && pushBoxMap[manNextPositionX][manNextPositionY] == punch) {
            SwapXY(&boxPositionX, &boxPositionY, &boxNextPositionX, &boxNextPositionY);
            SwapXY(&manPositionX, &manPositionY, &manNextPositionX, &manNextPositionY);
            
        }
        if(pushBoxMap[manNextPositionX][manNextPositionY] == road){
            SwapXY(&manPositionX, &manPositionY, &manNextPositionX, &manNextPositionY);
            
        }
        
        DrawMap();
        
        if(boxPositionX == winPositionX && boxPositionY == winPositionY){
            printf("You Are Win!\n");
            IsWin = 1;
            
        }
        if(IsLose(boxPositionX, boxPositionY, wall)){
            printf("You Are Lose!\n");
            break;
        }
    }
    return 0;
}

void DrawMap(){
    for(int i = 0; i < ROWS; i++){
        for (int j = 0; j < COLUMNS; j++) {
            printf("%c", pushBoxMap[i][j]);
        }
    }
}

void SwapXY(int *x1, int *y1, int *x2, int *y2){
    char temp = pushBoxMap[*x1][*y1];
    pushBoxMap[*x1][*y1] = pushBoxMap[*x2][*y2];
    pushBoxMap[*x2][*y2] = temp;
    
    *x1 = *x2;
    *y1 = *y2;
}

int IsLose(int boxPositionX, int boxPositionY, char condition){
    if((pushBoxMap[boxPositionX + 1][boxPositionY] == condition && pushBoxMap[boxPositionX][boxPositionY - 1] == condition)
       ||(pushBoxMap[boxPositionX - 1][boxPositionY] == condition && pushBoxMap[boxPositionX][boxPositionY - 1] == condition)
       ||(pushBoxMap[boxPositionX - 1][boxPositionY] == condition && pushBoxMap[boxPositionX][boxPositionY + 1] == condition)
       ||(pushBoxMap[boxPositionX + 1][boxPositionY] == condition && pushBoxMap[boxPositionX][boxPositionY + 1] == condition)
       ){
        return 1;
    }
    return 0;
}

void PushBox(int *boxPositionX, int *boxPositionY, int *manPositionX, int *manPositionY){
    char direction;
    printf("Please input a direction:([w,a,s,d]stands for[up, left, down, right])\n");
    read(0, direction, 1);
    switch(direction){
        case 'W':
        case 'w':
            (*manPositionX)--;
            (*boxPositionX)--;
            break;
        case 'A':
        case 'a':
            (*manPositionY)--;
            (*boxPositionY)--;
            break;
        case 'S':
        case 's':
            (*manPositionX)++;
            (*boxPositionX)++;
            break;
        case 'D':
        case 'd':
            (*manPositionY)++;
            (*boxPositionY)++;
            break;
        default:
            break;
            //DO NOTHING:
    }
}
