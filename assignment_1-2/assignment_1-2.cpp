#include<iostream>
#include<algorithm>
#define len 105
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

		cout << "请输入股票价格（价格不得小于0，不得大于104，个数不超过105，不同价格之间用空格分开）" << endl;//显示输入提示

		for (int i = 0; i < len; i++) {

			while (cin.peek() == ' ') {

				char ch;

				cin.get(ch);//如果读到空格则忽略

			}//该循环解决输入最后一个数字后，若又输入了空格input则函数无法退出的情况

			if (cin.peek() == '\n'  && i != 0)//如果某次循环读到回车则直接跳出函数，停止输入
				return lenth;

			cin >> *(p + i);//每次循环输入一个数

			lenth++;//每次输入一个数，长度加一

			if (cin.good() && *(p + i) >= 0 && *(p + i) <= 104)//每次输入后立即检查该输入是否正确且在范围之内
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

/***************************************************************************
  函数名称：max_calculate
  功    能：计算假如当天买入能获得的最大值
  输入参数：当前利润最大值，指向数组当天元素的指针,数组长度,数组首地址
  返 回 值：int最大值
  说    明：
****************************************************************************/
int max_calculate(int my_max, int* p, int lenth, int* arr)
{

	for (int* ret = p; p - arr < lenth; p++) {

		my_max = max(*p - *ret, my_max);//比较当前最大利润与当天与之后每天的利润

	}

	return my_max;//返回最大值

}

int main()
{

	int prices[len] = { 0 };
	int max_profit = 0;

	int arr_lenth = input(prices);


	for (int i = 0; i < arr_lenth; i++) {

		max_profit = max_calculate(max_profit,prices+i, arr_lenth, prices);

	}//计算每天能获得的最大受益，并不断迭代最大收益

	cout << max_profit << endl;
	
}