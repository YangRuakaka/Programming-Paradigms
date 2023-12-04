#include<iostream>
#define LEN 105
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

		cout << "请输入股票价格（价格不得小于30，大于100，个数不超过105，不同价格之间用空格分开）" << endl;//显示输入提示

		for (int i = 0; i < LEN; i++) {

			while (cin.peek() == ' ') {

				char ch;

				cin.get(ch);//如果读到空格则忽略

			}//该循环解决输入最后一个数字后，若又输入了空格input则函数无法退出的情况

			if (cin.peek() == '\n' && i != 0)//如果某次循环读到回车则直接跳出函数，停止输入
				return lenth;

			cin >> *(p + i);//每次循环输入一个数

			lenth++;//每次输入一个数，长度加一

			if (cin.good() && *(p + i) >= 30 && *(p + i) <= 100)//每次输入后立即检查该输入是否正确且在范围之内
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
  函数名称：next_rise
  功    能：计算prices数组中每个元素对应的answer的值
  输入参数：prices中各个元素的地址
  返 回 值：int
  说    明：
****************************************************************************/
int next_rise(int* p, int lenth, int* arr)
{
	int* ret = p;//设置一个返回指针，用于与p做比较来计算answer的值
	
	while (true) {

		p++;//指针右移

		if (p - arr >= lenth)//如果读到0则说明为读到上升日期，返回0
			return 0;

		if (*p > *ret)//如果读到上升日期，两指针相减返回answer的值
			return p - ret;
	}

}


int main()
{
	int prices[LEN] = { 0 };

	int answer[LEN] = { 0 };

	int lenth = 0;

	lenth=input(prices);

	for (int i = 0; i < LEN - 1 && prices[i] != 0; i++) {

		answer[i] = next_rise(prices + i, lenth, prices);//计算每个answer的元素

		cout << answer[i] << ' ';//输出每个answer

	}

	cout << endl;

	return 0;
	
}