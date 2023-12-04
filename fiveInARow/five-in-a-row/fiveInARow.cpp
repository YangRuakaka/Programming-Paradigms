/****************************************************************
 * Project Name:  fiveInARow
 * File Name:     fiveInARow.cpp
 * File Function: Problem solution
 * Author:        Yukun Yang (杨宇琨)
 * Update Date:   2023/11/19
 ****************************************************************/

 /****************************************************************
  * Solution Description
  ****************************************************************/
//运用Alpha-Beta剪枝算法来对棋局进行拟落子，从而找出最合适的落子位置
//具体原理如下：
//1、对棋盘上所有剩余点进行模拟落子，以对方视角对落子点评分（启发式搜索）
//2、将所有剩余点打完分后，根据分数从高到低排序，先搜索分高的点
//3、因为我们只对我方进行了拟落子，可能出现下在某处可以连成活四分很高
//但对方其实已经眠四了需要我们去防守，而防守点的分数往往不高、因此也需要对手棋拟落子
//4、在我们对某个点进行拟落子的基础上，对剩余点进行对方棋的拟落子
//5、重复1、2步骤，但在1中以我方视角对对方进行评分，在2中逆序排列，先搜分低的点
//6、进行完我方和对手的拟落子后，对整个棋局进行打分（我方视角）
//将每一行每一列每一斜线抽出来作为一个一维字符串来匹配特殊棋型
//7、将这个分数进行两层筛选，在对手拟落子层，会选择所有可落子点中的分值最低点
//而我方拟落子层，会选择对手层搜刮到的所有最低分中最高分作为我们的最佳落子

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>                               // C 语言标准输入输出库
#include <stdlib.h>                              // C 标准通用工具库
#include <string.h>                              // C 风格字符串处理函数库
#include <time.h>                                // C 日期和时间处理函数库
#include<algorithm>                              // C++ 标准模板库（泛型算法组件）
#include <vector>                                // C++ 标准模板库（向量容器）
#include <iostream>                              // C++ 标准模板库（输入输出流）

#define BOARD_SIZE 12
#define TRUE 1
#define FALSE 0
#define START "START"
#define PLACE "PLACE"
#define TURN "TURN"
#define END "END"

#define	CLOCKS_PER_SEC	((clock_t)1000)
#define	CLK_TCK		CLOCKS_PER_SEC

/* Conditional compilation */
#define DEBUG_MODE  1                            // 调试模式开关

typedef int BOOL;

/* Define Status type */
typedef enum {
    Empty,                                       // 空位标志
    Black,                                       // 黑棋标志
    White                                        // 白棋标志
} Status;

/* Define Command type */
struct Command
{
    int x;
    int y;
};

/* Define Move type */
struct Move
{
    int x;
    int y;
    int score;
};

/* Define mystr type */
struct mystr {
    char situation[13];
    int score_M;//When it is my turn
    int score_O;//When it is not my turn
    bool killerMove;
};

/* Define constant variables */
//Win!
const int p0 = 10000000;
//One step left
const int p1 = 1000;
//Two steps left
const int p2 = 100;
const int p3 = 10;
const int p4 = 1;
//Death Value
const int d4 = p1;
const int d3 = p2;
const int d2 = p3;
const int d1 = p4;
//Infinity 
const int infinity = 10000000;
const int L_infinity = -10000000;

/* Define global variables */
Status g_myFlag;
Status g_enemyFlag;
char g_board[BOARD_SIZE][BOARD_SIZE] = { 0 };
//Pattern
struct mystr g_targets_T[] = {
    //Gameover
    {"11111", p0,p0,true},

    //Four
     {"011110",p1, 100 * p1,true},
     {"0111010",p1, d3 + d1,true},
     {"0101110",p1, d3 + d1,true},
     {"0110110",p1,2 * d2,true},
     {"211110",p1, 0,true},
     {"011112",p1, 0,true},
     {"2111010",p1, d1,true},
     {"0101112",p1, d1,true},
     {"2101112",p1, 0,true},
     {"2111012",p1, 0,true},
     {"2110112",p1, 0,true},

     //Three
     {"0011100",p2,d3,true},
     {"0101100",p2,d2 + d1,true},
     {"0011010",p2,d2 + d1,true},
     {"0100110",p2,d2 + d1,false},
     {"0110010",p2,d2 + d1,false},
     {"2011100",p2,0,true},
     {"0011102",p2,0,true},
     {"2111000",p2,0,false},
     {"0001112",p2,0,false},
     {"2101100",p2,0,false},
     {"0011012",p2,0,false},
     {"2110100",p2,0,false},
     {"0010112",p2,0,false},
     {"2100110",p2,d2,false},
     {"0110012",p2,d2,false},
     {"2001112",p2,0,false},
     {"2111002",p2,0,false},
     {"2011102",p2,0,false},
     {"2101102",p2,0,false},
     {"2011012",p2,0,false},
     {"2101012",p2,0,false},
     {"01110",p2,d3,true},
     {"0101010",p2,d2 + d1,false},
     {"2110100",p2,0,false},
     {"0010112",p2,0,false},
     {"2110102",p2,0,false},
     {"2010112",p2,0,false},

     //Two
     {"001100",p3,d2,false},
     {"01010",p3,2 * d1,false},
     {"211000",p3,0,false},
     {"000112",p3,0,false},
     {"210100",p3,0,false},
     {"001012",p3,0,false},
     {"210010",p3,0,false},
     {"010012",p3,0,false},

     //One
     {"00100",p4,d1,false},
};
struct mystr g_targets_F[] = {
    //Gameover
    {"22222", p0,p0,true},

    //Four
    {"022220",p1, 100 * p1,true},
    {"0222020",p1, d3 + d1,true},
    {"0202220",p1, d3 + d1,true},
    {"0220220",p1, 2 * d2,true},
    {"122220",p1, 0,true},
    {"022221",p1, 0,true},
    {"1222020",p1, d1,true},
    {"0202221",p1, d1,true},
    {"1202221",p1, 0,true},
    {"1222021",p1, 0,true},
    {"1220221",p1, 0,true},

    //Three
    {"0022200",p2,d3,true},
    {"0202200",p2,d2 + d1,true},
    {"0022020",p2,d2 + d1,true},
    {"0200220",p2,d2 + d1,false},
    {"0220020",p2,d2 + d1,false},
    {"1022200",p2,0,true},
    {"0022201",p2,0,true},
    {"1222000",p2,0,false},
    {"0002221",p2,0,false},
    {"1202200",p2,0,false},
    {"0022021",p2,0,false},
    {"1220200",p2,0,false},
    {"0020221",p2,0,false},
    {"1200220",p2,d2,false},
    {"0220021",p2,d2,false},
    {"1002221",p2,0,false},
    {"1222001",p2,0,false},
    {"1022201",p2,0,false},
    {"1202201",p2,0,false},
    {"1022021",p2,0,false},
    {"1202021",p2,0,false},
    {"02220",p2,d3,true},
    {"0202020",p2,d2 + d1,false},
    {"1220200",p2,0,false},
    {"0020221",p2,0,false},
    {"1220201",p2,0,false},
    {"1020221",p2,0,false},

    //Two
    {"002200",p3,d2,false},
    {"02020",p3,2 * d1,false},
    {"122000",p3,0,false},
    {"000221",p3,0,false},
    {"120200",p3,0,false},
    {"002021",p3,0,false},
    {"120020",p3,0,false},
    {"020021",p3,0,false},

    //One
    {"00200",p4,d1,false},
};


#if DEBUG_MODE
/***************************************************************************
 Function Name:debug
 Function:To send message while processing
 Input Parameters:str
 Return Value:void
 Documentation：
****************************************************************************/
void debug(const char* str)
{
    printf("DEBUG %s\n", str);
    fflush(stdout);
}

/***************************************************************************
 Function Name:debug
 Function:To send message while processing
 Input Parameters:time
 Return Value:void
 Documentation：
****************************************************************************/
void debug(const double &time)
{
    printf("DEBUG %lf\n", time);
    fflush(stdout);
}

/***************************************************************************
 Function Name:debug
 Function:To send message while processing
 Input Parameters:num
 Return Value:void
 Documentation：
****************************************************************************/
void debug(const int &num)
{
    printf("DEBUG %d\n", num);
    fflush(stdout);
}

/***************************************************************************
 Function Name:debug
 Function:To send message while processing
 Input Parameters:const char board[BOARD_SIZE][BOARD_SIZE]
 Return Value:void
 Documentation：
****************************************************************************/
void debug(const char board[BOARD_SIZE][BOARD_SIZE])
{
    //Draw the board
    printf("0  1  2  3  4  5  6  7  8  9  10 11 \n");
    for (int i = 0; i < BOARD_SIZE * 2; i++) {
        if (i % 2)
            std::cout << "------------------------------------" << std::endl;
        else {
            for (int j = 0; j < BOARD_SIZE; j++) {
                if (board[i / 2][j] == Black)
                    std::cout << "○";
                else if (board[i / 2][j] == White)
                    std::cout << "●";
                else
                    std::cout << "  ";

                std::cout << "|";
            }
            std::cout << "  " << i / 2 << std::endl;
        }
    }
    fflush(stdout);
}
#endif // DEBUG_MODE

/***************************************************************************
 Function Name:isInBound
 Function:To check if the cordinate is valid
 Input Parameters:Cordinates
 Return Value:BOOL
 Documentation：
****************************************************************************/
inline BOOL isInBound(const int &x,const int &y)
{
    return x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE;
}

/***************************************************************************
 Function Name:place
 Function:To place the enemyflag
 Input Parameters:cmd Cordinates
 Return Value:void
 Documentation：
****************************************************************************/
inline void place(const struct Command &cmd)
{
    g_board[cmd.x][cmd.y] = g_enemyFlag;
}

/***************************************************************************
 Function Name:start
 Function:To initialize myflag and the board
 Input Parameters:myflag
 Return Value:void
 Documentation：
****************************************************************************/
void start()
{
    memset(g_board, 0, sizeof(g_board));
    int middlePlace = BOARD_SIZE / 2;
    g_board[middlePlace - 1][middlePlace - 1] = static_cast<char>(White);
    g_board[middlePlace][middlePlace] = static_cast<char>(White);
    g_board[middlePlace - 1][middlePlace] = static_cast<char>(Black);
    g_board[middlePlace][middlePlace - 1] = static_cast<char>(Black);
    g_enemyFlag = static_cast<Status>(3 - g_myFlag);
}

/***************************************************************************
 Function Name:checkIfGameEnds
 Function:To check if game ends
 Input Parameters:const char board[BOARD_SIZE][BOARD_SIZE], const BOOL ismy
 Return Value:BOOL
 Documentation：
****************************************************************************/
BOOL checkIfGameEnds(const char board[BOARD_SIZE][BOARD_SIZE], const BOOL &ismy) 
{
    int i, j, count;
    char player;

    if (ismy)
        player = static_cast<char>(g_myFlag);
    else
        player = static_cast<char>(g_enemyFlag);

    //Horizenrtal
    for (i = 0; i < BOARD_SIZE; i++) {
        count = 0;
        for (j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == player) {
                count++;
                if (count == 5)
                    return TRUE;
            }
            else
                count = 0;
        }
    }

    //Vertical
    for (j = 0; j < BOARD_SIZE; j++) {
        count = 0;
        for (i = 0; i < BOARD_SIZE; i++) {
            if (board[i][j] == player) {
                count++;
                if (count == 5)
                    return TRUE;
            }
            else
                count = 0;
        }
    }

    //Diagonal
    for (int i = 0; i <= BOARD_SIZE - 5; i++) {
        for (int j = 0; j <= BOARD_SIZE - 5; j++) {
            //Check the main diagonal.
            if (board[i][j] == player && board[i + 1][j + 1] == player && board[i + 2][j + 2] == player &&
                board[i + 3][j + 3] == player && board[i + 4][j + 4] == player)
                return TRUE;
            //Check the diagonal opposite.
            if (board[i + 4][j] == player && board[i + 3][j + 1] == player && board[i + 2][j + 2] == player &&
                board[i + 1][j + 3] == player && board[i][j + 4] == player)
                return TRUE;
        }
    }

    return FALSE;
}

/***************************************************************************
 Function Name:isempty
 Function:To check if game ends(Draw)
 Input Parameters:const char board[BOARD_SIZE][BOARD_SIZE]
 Return Value:BOOL
 Documentation：
****************************************************************************/
BOOL isempty(char board[BOARD_SIZE][BOARD_SIZE])
{
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == Empty)
                return FALSE;
        }
    }
    return TRUE;
}

/***************************************************************************
 Function Name:nearby
 Function:To check whether the position is worth searching
 Input Parameters:const char board[BOARD_SIZE][BOARD_SIZE], const int x, const int y
 Return Value:BOOL
 Documentation：
****************************************************************************/
BOOL nearby(const char board[BOARD_SIZE][BOARD_SIZE], const int &x, const int &y)
{
    for (int i = x - 1; isInBound(i, y) && i >= x - 4; i--) {
        if (board[i][y] != Empty) {
            return TRUE;
        }
    }
    for (int i = x + 1; isInBound(i, y) && i < x + 5; i++) {
        if (board[i][y] != Empty) {
            return TRUE;
        }
    }
    for (int i = y - 1; isInBound(x, i) && i >= y - 4; i--) {
        if (board[x][i] != Empty) {
            return TRUE;
        }
    }
    for (int i = y + 1; isInBound(x, i) && i < y + 5; i++) {
        if (board[x][i] != Empty) {
            return TRUE;
        }
    }
    for (int i = x + 1, j = y + 1; isInBound(i, j) && i < x + 5 && j < y + 5; i++, j++) {
        if (board[i][j] != Empty) {
            return TRUE;
        }
    }
    for (int i = x - 1, j = y + 1; isInBound(i, j) && i >= x - 4 && j < y + 5; i--, j++) {
        if (board[i][j] != Empty) {
            return TRUE;
        }
    }
    for (int i = x + 1, j = y - 1; isInBound(i, j) && i < x + 5 && j >= y - 4; i++, j--) {
        if (board[i][j] != Empty) {
            return TRUE;
        }
    }
    for (int i = x - 1, j = y - 1; isInBound(i, j) && i >= x - 4 && j >= y - 4; i--, j--) {
        if (board[i][j] != Empty) {
            return TRUE;
        }
    }

    return FALSE;
}

/***************************************************************************
 Function Name:evaluate_line
 Function:To evaluate the line we processed
 Input Parameters:char* line, BOOL ismy
 Return Value:the score
 Documentation：
****************************************************************************/
int evaluate_line(const char* line,const BOOL &ismy,int& myKillerMove,int& opKillerMove)
{
    //Lenth less than 5 ,exit!
    int size = 0;
    while (line[size] != '\0') {
        size++;
    }
    if (size < 5)
        return 0;

    //If all_zero,exit
    int index = 0;
    while (line[index] != '\0') {
        if (line[index] == '0')
            index++;
        else
            break;
    }
    if (line[index] == '\0')
        return 0;

    int score = 0;//Sum the score

    if (g_myFlag == Black) {
        //My turn
        if (ismy == TRUE) {
            //The pattern that is good for me
            for (unsigned int i = 0; i < sizeof(g_targets_T) / sizeof(g_targets_T[0]); i++) {
                if (strstr(line, g_targets_T[i].situation) != NULL) {
                    myKillerMove += g_targets_T[i].killerMove;
                    if (myKillerMove > 1 && g_targets_T[i].killerMove)
                        score += p1;
                    else
                        score += g_targets_T[i].score_M + g_targets_T[i].score_O;
                    break;
                }

            }
            //The pattern that is bad for me(good for my opponent)
            for (unsigned int i = 0; i < sizeof(g_targets_F) / sizeof(g_targets_F[0]); i++) {
                if (strstr(line, g_targets_F[i].situation) != NULL) {
                    opKillerMove += g_targets_F[i].killerMove;
                    if (opKillerMove > 1 && g_targets_F[i].killerMove)
                        score -= d4;
                    else
                        score -= g_targets_F[i].score_O;
                    break;
                }
            }
        }
        //Opponent's turn
        else {
            //The pattern that is good for me
            for (unsigned int i = 0; i < sizeof(g_targets_T) / sizeof(g_targets_T[0]); i++) {
                if (strstr(line, g_targets_T[i].situation) != NULL) {
                    myKillerMove += g_targets_T[i].killerMove;
                    if (myKillerMove > 1 && g_targets_T[i].killerMove)
                        score += p1;
                    else
                        score += g_targets_T[i].score_O;
                    break;
                }
            }
            //The pattern that is bad for me(good for my opponent)
            for (unsigned int i = 0; i < sizeof(g_targets_F) / sizeof(g_targets_F[0]); i++) {
                if (strstr(line, g_targets_F[i].situation) != NULL) {
                    opKillerMove += g_targets_F[i].killerMove;
                    if (opKillerMove > 1 && g_targets_F[i].killerMove)
                        score -= d4;
                    else
                        score -= g_targets_F[i].score_M + g_targets_F[i].score_O;
                    break;
                }
            }

        }

    }
    else {
        //My turn
        if (ismy == TRUE) {
            //The pattern that is good for me
            for (unsigned int i = 0; i < sizeof(g_targets_F) / sizeof(g_targets_F[0]); i++) {
                if (strstr(line, g_targets_F[i].situation) != NULL) {
                    myKillerMove += g_targets_F[i].killerMove;
                    if (myKillerMove > 1 && g_targets_F[i].killerMove)
                        score += p1;
                    else
                        score += g_targets_F[i].score_M + g_targets_F[i].score_O;
                    break;
                }
            }
            //The pattern that is bad for me(good for my opponent)
            for (unsigned int i = 0; i < sizeof(g_targets_T) / sizeof(g_targets_T[0]); i++) {
                if (strstr(line, g_targets_T[i].situation) != NULL) {
                    opKillerMove += g_targets_T[i].killerMove;
                    if (opKillerMove > 1 && g_targets_T[i].killerMove)
                        score -= d4;
                    else
                        score -= g_targets_T[i].score_O ;
                    break;
                }
            }
        }
        //Opponent's turn
        else {
            //The pattern that is good for me
            for (unsigned int i = 0; i < sizeof(g_targets_F) / sizeof(g_targets_F[0]); i++) {
                if (strstr(line, g_targets_F[i].situation) != NULL) {
                    myKillerMove += g_targets_F[i].killerMove;
                    if (myKillerMove > 1 && g_targets_F[i].killerMove)
                        score += p1;
                    else
                        score += g_targets_F[i].score_O;
                    break;
                }
            }
            //The pattern that is bad for me(good for my opponent)
            for (unsigned int i = 0; i < sizeof(g_targets_T) / sizeof(g_targets_T[0]); i++) {
                if (strstr(line, g_targets_T[i].situation) != NULL) {
                    opKillerMove += g_targets_T[i].killerMove;
                    if (opKillerMove > 1 && g_targets_T[i].killerMove)
                        score -= d4;
                    else
                        score -= g_targets_T[i].score_M + g_targets_T[i].score_O;
                    break;
                }
            }
        }
    }

    return score;

}

/***************************************************************************
 Function Name:evaluate
 Function:To evaluate general board
 Input Parameters:char board[BOARD_SIZE][BOARD_SIZE], BOOL ismy
 Return Value:score
 Documentation：View the board in every one-dimensional line
****************************************************************************/
int evaluate(char board[BOARD_SIZE][BOARD_SIZE],const BOOL &ismy)
{
    int score = 0;
    int myKillerMove = 0;
    int opKillerMove = 0;

    //Horizen
    for (int i = 0; i < BOARD_SIZE; i++) {
        char temp[BOARD_SIZE + 1];//To store a one-dimensional pattern
        int j = 0;
        for (; j < BOARD_SIZE; j++) {
            if (board[i][j] == White)
                temp[j] = '2';
            else if (board[i][j] == Black)
                temp[j] = '1';
            else if (board[i][j] == Empty)
                temp[j] = '0';
        }
        temp[j] = '\0';
        score += evaluate_line(temp, ismy, myKillerMove, opKillerMove);
    }

    //Vertical
    for (int i = 0; i < BOARD_SIZE; i++) {
        char temp[BOARD_SIZE + 1];//To store a one-dimensional pattern
        int j = 0;
        for (; j < BOARD_SIZE; j++) {
            if (board[j][i] == White)
                temp[j] = '2';
            else if (board[j][i] == Black)
                temp[j] = '1';
            else if (board[j][i] == Empty)
                temp[j] = '0';
        }
        temp[j] = '\0';
        score += evaluate_line(temp, ismy, myKillerMove, opKillerMove);
    }


    //Main diagonal
    for (int i = 0; i < BOARD_SIZE; ++i) {
        char temp[BOARD_SIZE + 1];//To store a one-dimensional pattern
        int x = i, y = 0, k = 0;
        while (x >= 0) {
            if (board[x--][y++] == White)
                temp[k++] = '2';
            else if (board[x--][y++] == Black)
                temp[k++] = '1';
            else if (board[x--][y++] == Empty)
                temp[k++] = '0';
        }
        temp[k] = '\0';
        score += evaluate_line(temp, ismy, myKillerMove, opKillerMove);
    }
    for (int j = 1; j < BOARD_SIZE; ++j) {
        char temp[BOARD_SIZE + 1];//To store a one-dimensional pattern
        int x = BOARD_SIZE - 1, y = j;
        int k = 0;
        while (y < BOARD_SIZE) {
            if (board[x--][y++] == White)
                temp[k++] = '2';
            else if (board[x--][y++] == Black)
                temp[k++] = '1';
            else if (board[x--][y++] == Empty)
                temp[k++] = '0';
        }
        temp[k] = '\0';
        score += evaluate_line(temp, ismy, myKillerMove, opKillerMove);
    }

    //Opposite diagonal
    for (int i = 0; i < BOARD_SIZE; ++i) {
        char temp[BOARD_SIZE + 1];//To store a one-dimensional pattern
        int k = 0;
        int x = i, y = 0;
        while (x < BOARD_SIZE) {
       if (board[x++][y++] == White)
                temp[k++] = '2';
            else if (board[x++][y++] == Black)
                temp[k++] = '1';
            else if (board[x++][y++] == Empty)
                temp[k++] = '0';
        }
        temp[k] = '\0';
        score += evaluate_line(temp, ismy, myKillerMove, opKillerMove);
    }
    for (int j = 1; j < BOARD_SIZE; ++j) {
        char temp[BOARD_SIZE + 1];//To store a one-dimensional pattern
        int k = 0;
        int x = 0, y = j;
        while (y < BOARD_SIZE) {
            if (board[x++][y++] == White)
                temp[k++] = '2';
            else if (board[x++][y++] == Black)
                temp[k++] = '1';
            else if (board[x++][y++] == Empty)
                temp[k++] = '0';
        }
        temp[k] = '\0';
        score += evaluate_line(temp, ismy, myKillerMove, opKillerMove);
    }
    return score;
}

/***************************************************************************
 Function Name:Heuristic
 Function:To score every place
 Input Parameters:const char board[BOARD_SIZE][BOARD_SIZE], const int x, const int y, const BOOL ismy
 Return Value:score
 Documentation：View the board in four lines based on the position we just placed our flag
****************************************************************************/
int Heuristic(const char board[BOARD_SIZE][BOARD_SIZE], const int &x, const int &y, const BOOL ismy)
{
    char temp[BOARD_SIZE + 1] = "0";
    int i = 0;
    int score = 0;

    int myKillerMove = 0;
    int opKillerMove = 0;

    //Horizental
    int Y = y - 4;
    if (Y < 0)
        Y = 0;
    for (; i < 9 && Y < BOARD_SIZE && Y < y + 5; Y++, i++) {
        if (board[x][Y] == White)
            temp[i] = '2';
        else if (board[x][Y] == Black)
            temp[i] = '1';
        else if (board[x][Y] == Empty)
            temp[i] = '0';
    }
    temp[i] = '\0';
    score += evaluate_line(temp, ismy, myKillerMove, opKillerMove);

    //Reset
    i = 0;
    for (int k = 0; k < BOARD_SIZE; k++) {
        temp[k] = 0;
    }

    //Vertical
    int X = x - 4;
    if (X < 0)
        X = 0;
    for (; i < 9 && X < BOARD_SIZE && X < x + 5; X++, i++) {
        if (board[X][y] == White)
            temp[i] = '2';
        else if (board[X][y] == Black)
            temp[i] = '1';
        else if (board[X][y] == Empty)
            temp[i] = '0';
    }
    temp[i] = '\0';
    score += evaluate_line(temp, ismy, myKillerMove, opKillerMove);

    //Reset
    i = 0;
    for (int k = 0; k < BOARD_SIZE; k++) {
        temp[k] = 0;
    }

    //Diagonal
    X = x;
    Y = y;
    while (X > 0 && Y > 0 && X > x - 4 && Y > y - 4) {
        X--;
        Y--;
    }
    for (; Y < BOARD_SIZE && X < BOARD_SIZE && X < x + 5 && Y < y + 5; X++, Y++, i++) {
        if (board[X][Y] == White)
            temp[i] = '2';
        else if (board[X][Y] == Black)
            temp[i] = '1';
        else if (board[X][Y] == Empty)
            temp[i] = '0';
    }
    temp[i] = '\0';
    score += evaluate_line(temp, ismy, myKillerMove, opKillerMove);

    //Reset
    i = 0;
    for (int k = 0; k < BOARD_SIZE; k++) {
        temp[k] = 0;
    }

    //Diagonal
    X = x;
    Y = y;
    while (X < BOARD_SIZE && Y > 0 && X < x + 5 && Y > y - 4) {
        X++;
        Y--;
    }
    for (; Y < BOARD_SIZE && X >= 0 && X >= x - 4 && Y < y + 5; X--, Y++, i++) {
        if (board[X][Y] == White)
            temp[i] = '2';
        else if (board[X][Y] == Black)
            temp[i] = '1';
        else if (board[X][Y] == Empty)
            temp[i] = '0';
    }
    temp[i] = '\0';
    score += evaluate_line(temp, ismy, myKillerMove, opKillerMove);

    return score;
}

/***************************************************************************
 Function Name:alpha_beta
 Function:To calculate the bestmove by alpha-beta
 Input Parameters:char board[BOARD_SIZE][BOARD_SIZE], int alpha, int beta, BOOL Player, int maxdepth
 Return Value:a move with a score
 Documentation：
****************************************************************************/
struct Move alpha_beta(char board[BOARD_SIZE][BOARD_SIZE], int alpha, int beta,const BOOL &Player,const int &maxdepth)
{
    std::vector<Move> empty;
    Move bestmove;

    if (isempty(board) || checkIfGameEnds(board, Player) == TRUE) {
        bestmove.x = -1;
        bestmove.y = -1;
        bestmove.score = evaluate(board, Player);
        return bestmove;
    }

    //Check all the empty place and give a score
    for (int i = 0; i < BOARD_SIZE - 0; i++) {
        for (int j = 0; j < BOARD_SIZE - 0; j++) {
            if (board[i][j] == Empty && nearby(board, i, j)) {
                if (Player == TRUE)
                    board[i][j] = g_myFlag;
                else
                    board[i][j] = g_enemyFlag;

                int score = Heuristic(board, i, j, 1 - Player);
                empty.push_back({ i,j,score });

                board[i][j] = Empty;
            }
        }
    }

    //My turn,so we check the place that has high score
    if (Player == TRUE) {
        std::sort(empty.begin(), empty.end(), [](const Move& a, const Move& b) {
            return a.score > b.score;
            });
    }
    //Opponent's turn,so fisrt consider the place has lower score
    else {
        std::sort(empty.begin(), empty.end(), [](const Move& a, const Move& b) {
            return a.score < b.score;
            });
    }

    //Reach at the bottom of the Recursion
    if (maxdepth == 0) {
        bestmove.x = empty[0].x;
        bestmove.y = empty[0].y;
        bestmove.score = evaluate(board, Player);//Evaluate the general
        return bestmove;
    }

    //My turn
    if (Player == TRUE) {
        bestmove.score = L_infinity;//For this situation,initialize its score really low
        //Check every place that is empty
        for (unsigned int i = 0; i < empty.size(); i++) {
            board[empty[i].x][empty[i].y] = g_myFlag;//Intended move(Exploratory)
            Move move = alpha_beta(board, alpha, beta, FALSE, maxdepth - 1);//Turn into opponent's view
            board[empty[i].x][empty[i].y] = Empty;//Remove our flag
            move.x = empty[i].x;//Record current position
            move.y = empty[i].y;
            if (move.score > bestmove.score)//If the move is better than ever
                bestmove = move;//then it is the bestmove
            alpha = std::max(alpha, bestmove.score);//Record the alpha
            if (beta <= alpha)//If alpha is greater than beta,break the loop
                break;
        }

    }
    //Opponent's turn
    else {
        bestmove.score = infinity;//For this situation,initialize its score really high
        //Check every place that is empty
        for (unsigned int i = 0; i < empty.size(); i++) {
            board[empty[i].x][empty[i].y] = g_enemyFlag;//Intended move(Exploratory)
            Move move = alpha_beta(board, alpha, beta, TRUE, maxdepth - 1);//Turn into my view
            board[empty[i].x][empty[i].y] = Empty;//Remove the flag
            move.x = empty[i].x;//Record current position
            move.y = empty[i].y;
            if (move.score < bestmove.score)//If the position is the worst ever(for me)
                bestmove = move;//then the opponent is happy and regard it as the bestmove
            beta = std::min(beta, bestmove.score);//Record the beta
            if (beta <= alpha) //If alpha is greater than beta,break the loop
                break;
        }
    }
    return bestmove;
}

/***************************************************************************
 Function Name:turn
 Function:To calculate and place my flag
 Input Parameters:
 Return Value:void
 Documentation：
****************************************************************************/
void turn()
{
    if (g_board[7][7] == Empty && g_myFlag == Black) {
        g_board[7][7] = g_myFlag;
        printf("%d %d\n", 7, 7);
        fflush(stdout);
    }//The first step of Black
    else if (g_board[7][7] == Black && g_myFlag == White && g_board[6][7] == Empty) {
        g_board[6][7] = g_myFlag;
        printf("%d %d\n", 6, 7);
        fflush(stdout);
    }//The first step of White
    else if (g_board[4][4] == Black && g_myFlag == White && g_board[4][5] == Empty) {
        g_board[4][5] = g_myFlag;
        printf("%d %d\n", 4, 5);
        fflush(stdout);
    }//The first step of White
    else {
        struct Move bestmove = alpha_beta(g_board, L_infinity, infinity, TRUE, 2);
#if DEBUG_MODE      
        debug("score:");
        debug(bestmove.score);
        debug("\n");
#endif // DEBUG_MODE
        g_board[bestmove.x][bestmove.y] = g_myFlag;
        printf("%d %d\n", bestmove.x, bestmove.y);
        fflush(stdout);
    }

}

/***************************************************************************
 Function Name:end
 Function:To exit
 Input Parameters:
 Return Value:void
 Documentation：
****************************************************************************/
void end(int x)
{
    exit(0);
}

/***************************************************************************
 Function Name:loop
 Function:To react to the command
 Input Parameters:
 Return Value:void
 Documentation：
****************************************************************************/
void loop()
{
    char tag[10] = { 0 };
    struct Command command =
    {
        0,
        0,
    };
 
    while (TRUE)
    {
        memset(tag, 0, sizeof(tag));
        scanf("%s", tag);
        if (strcmp(tag, START) == 0)
        {
            scanf("%d", &g_myFlag);
            start();
            printf("OK\n");
            fflush(stdout);
        }
        else if (strcmp(tag, PLACE) == 0)
        {
            scanf("%d %d", &command.x, &command.y);
            place(command);
        }
        else if (strcmp(tag, TURN) == 0)
        {
#if DEBUG_MODE    
            clock_t starttime, finishtime;
            double Times;
            starttime = clock();
#endif // DEBUG_MODE
            turn();
#if DEBUG_MODE 
            finishtime = clock();
            Times = (double)(finishtime - starttime) / CLOCKS_PER_SEC;
            debug("duration:"); debug(Times); debug("\n");
            debug(g_board);
#endif // DEBUG_MODE
        }
        else if (strcmp(tag, END) == 0)
        {
            int status;
            scanf("%d", &status);
            end(status);
        }
    }
}

int main(int argc, char* argv[])
{
    loop();
    return 0;
}