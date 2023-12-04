#include<iostream>
#include<algorithm>
#include <conio.h>
#define len 5000
using namespace std;


/***************************************************************************
  函数名称：input
  功    能：将用户输入的数字存入数组
  输入参数：数组首地址
  返 回 值：输入数字个数
  说    明：
****************************************************************************/
int input(int* p)
{
	while (true) {
		int lenth = 0;

		cout << "请输入股票价格（价格不得小于0，大于1000，个数不超过5000，不同价格之间用空格分开）" << endl;//显示输入提示

		for (int i = 0; i < len; i++) {

			while (cin.peek() == ' ') {

				char ch;

				cin.get(ch);//如果读到空格则忽略

			}//该循环解决输入最后一个数字后，若又输入了空格input则函数无法退出的情况

			if (cin.peek() == '\n' && i != 0)//如果某次循环读到回车则直接跳出函数，停止输入
				return lenth;

			cin >> *(p + i);//每次循环输入一个数

			lenth++;//每次输入一个数，长度加一

			if (cin.good() && *(p + i) >= 0 && *(p + i) <= 1000)//每次输入后立即检查该输入是否正确且在范围之内
				continue;//输入正确则进入新一轮循环

			else {

				cout << "输入错误，请重新输入!" << endl;
				cin.clear();
				cin.ignore(10000, '\n');//输入错误则显示提示，并清空输入流

				for (int j = 0; j <= i; j++) {
					*(p + j) = 0;
				}//同时清空之前输入的所有数字，初始化数组

				break;//跳出for循环，重新进入while循环，进行全新的一轮输入
			}

		}//end of for

	}//end of while

}



int main()
{

	int prices[len] = { 0 };
	int max_profit = 0;

	int arr_lenth = input(prices);

	int maxprof_with_stock_buy[len] = { 0 };//maxprof_with_stock_buy[i]表示前i天且第i天结束后持有股票的最大利润（可以代表第i天买入或者第i天时仅持有股票两种情况）

	int maxprof_without_stock_sell[len] = { 0 };//maxprof_without_stock_sell[i]表示前i天且第i天结束后不持有股票且第i天为冷冻期的最大利润（可以代表第i天时出售股票或者第i天时无股票且无法购买）

	int maxprof_without_stock_cool[len] = { 0 };//maxprof_without_stock_cool[i]表示前i天且第i天结束后不持有股票且第i天为非冷冻期的最大利润（“第i天无股票且可以够买股票”）

	int cool_day = 1;

	bool _set;//为1时用户自定义，为0时缺省

	
	cout << "是否自定义冷却期时长，题目缺省条件下为1天（按Y自定义/按N以缺省条件计算）" << endl;//扩展部分，该程序可以更改冷却期时长

	while (true) {

		char ch = _getch();
		if (ch == 'y' || ch == 'Y') {
			_set = 1;
			cout << ch << endl;
			break;
		}
		else if (ch == 'n' || ch == 'N') {
			_set = 0;
			cout << ch << endl;
			break;
		}
		else
			continue;
	}

	if (_set) {

		while (true) {
			cout << "请输入冷却期的时长，不能设置为0（题目缺省条件为1天，本程序可以实现冷却期大于一天的最大利润计算）" << endl;

			cin >> cool_day;

			if (cin.good() && cool_day >= 1)
				break;

			else {
				cout << "输入错误，请重新输入!" << endl;
				cin.clear();
				cin.ignore(10000, '\n');

				cool_day = 1;
			}
		}
	}


	maxprof_with_stock_buy[0] = -prices[0];//数组初始化，

	
	for (int i = 1; i < arr_lenth; i++) {

		
		maxprof_with_stock_buy[i] = max(maxprof_with_stock_buy[i - 1], maxprof_without_stock_cool[i - 1] - prices[i]);
		//更新时，若第i+1天不购买，则i+1天的最大利润为第i天的最大利润（第i天为有股票状态）
		//若i+1天购买，则i+1天的最大利润为第i天（第i天为空窗期）的最大利润减去买入的股票价格

		maxprof_without_stock_sell[i] = max(maxprof_without_stock_sell[i - 1], maxprof_with_stock_buy[i - 1] + prices[i]);
		//更新时，若第i+1天不购买，则i+1天的最大利润为第i天的最大利润（第i天为无股票状态）
		//若i+1天购买，则i+1天的最大利润为第i天（第i天为有股票状态）的最大利润加上卖出的股票价格

		if ((i - cool_day) > 0)
			maxprof_without_stock_cool[i] = max(maxprof_without_stock_sell[i - cool_day], maxprof_without_stock_cool[i - 1]);
		//更新时，可购买日期的最大利润必须是前一天也为可购买日的最大利润和至少coolday之前天出售的最大利润比较后的较大者
	}

	cout << "当冷却期设置为" << cool_day << "天时，能获得的最大利润为" << maxprof_without_stock_sell[arr_lenth - 1] << endl;

}