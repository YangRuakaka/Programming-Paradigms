#include<iostream>
#include<algorithm>
#include <conio.h>
#define len 5000
using namespace std;


/***************************************************************************
  �������ƣ�input
  ��    �ܣ����û���������ִ�������
  ��������������׵�ַ
  �� �� ֵ���������ָ���
  ˵    ����
****************************************************************************/
int input(int* p)
{
	while (true) {
		int lenth = 0;

		cout << "�������Ʊ�۸񣨼۸񲻵�С��0������1000������������5000����ͬ�۸�֮���ÿո�ֿ���" << endl;//��ʾ������ʾ

		for (int i = 0; i < len; i++) {

			while (cin.peek() == ' ') {

				char ch;

				cin.get(ch);//��������ո������

			}//��ѭ������������һ�����ֺ����������˿ո�input�����޷��˳������

			if (cin.peek() == '\n' && i != 0)//���ĳ��ѭ�������س���ֱ������������ֹͣ����
				return lenth;

			cin >> *(p + i);//ÿ��ѭ������һ����

			lenth++;//ÿ������һ���������ȼ�һ

			if (cin.good() && *(p + i) >= 0 && *(p + i) <= 1000)//ÿ��������������������Ƿ���ȷ���ڷ�Χ֮��
				continue;//������ȷ�������һ��ѭ��

			else {

				cout << "�����������������!" << endl;
				cin.clear();
				cin.ignore(10000, '\n');//�����������ʾ��ʾ�������������

				for (int j = 0; j <= i; j++) {
					*(p + j) = 0;
				}//ͬʱ���֮ǰ������������֣���ʼ������

				break;//����forѭ�������½���whileѭ��������ȫ�µ�һ������
			}

		}//end of for

	}//end of while

}



int main()
{

	int prices[len] = { 0 };
	int max_profit = 0;

	int arr_lenth = input(prices);

	int maxprof_with_stock_buy[len] = { 0 };//maxprof_with_stock_buy[i]��ʾǰi���ҵ�i���������й�Ʊ��������󣨿��Դ����i��������ߵ�i��ʱ�����й�Ʊ���������

	int maxprof_without_stock_sell[len] = { 0 };//maxprof_without_stock_sell[i]��ʾǰi���ҵ�i������󲻳��й�Ʊ�ҵ�i��Ϊ�䶳�ڵ�������󣨿��Դ����i��ʱ���۹�Ʊ���ߵ�i��ʱ�޹�Ʊ���޷�����

	int maxprof_without_stock_cool[len] = { 0 };//maxprof_without_stock_cool[i]��ʾǰi���ҵ�i������󲻳��й�Ʊ�ҵ�i��Ϊ���䶳�ڵ�������󣨡���i���޹�Ʊ�ҿ��Թ����Ʊ����

	int cool_day = 1;

	bool _set;//Ϊ1ʱ�û��Զ��壬Ϊ0ʱȱʡ

	
	cout << "�Ƿ��Զ�����ȴ��ʱ������Ŀȱʡ������Ϊ1�죨��Y�Զ���/��N��ȱʡ�������㣩" << endl;//��չ���֣��ó�����Ը�����ȴ��ʱ��

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
			cout << "��������ȴ�ڵ�ʱ������������Ϊ0����Ŀȱʡ����Ϊ1�죬���������ʵ����ȴ�ڴ���һ������������㣩" << endl;

			cin >> cool_day;

			if (cin.good() && cool_day >= 1)
				break;

			else {
				cout << "�����������������!" << endl;
				cin.clear();
				cin.ignore(10000, '\n');

				cool_day = 1;
			}
		}
	}


	maxprof_with_stock_buy[0] = -prices[0];//�����ʼ����

	
	for (int i = 1; i < arr_lenth; i++) {

		
		maxprof_with_stock_buy[i] = max(maxprof_with_stock_buy[i - 1], maxprof_without_stock_cool[i - 1] - prices[i]);
		//����ʱ������i+1�첻������i+1����������Ϊ��i���������󣨵�i��Ϊ�й�Ʊ״̬��
		//��i+1�칺����i+1����������Ϊ��i�죨��i��Ϊ�մ��ڣ�����������ȥ����Ĺ�Ʊ�۸�

		maxprof_without_stock_sell[i] = max(maxprof_without_stock_sell[i - 1], maxprof_with_stock_buy[i - 1] + prices[i]);
		//����ʱ������i+1�첻������i+1����������Ϊ��i���������󣨵�i��Ϊ�޹�Ʊ״̬��
		//��i+1�칺����i+1����������Ϊ��i�죨��i��Ϊ�й�Ʊ״̬�������������������Ĺ�Ʊ�۸�

		if ((i - cool_day) > 0)
			maxprof_without_stock_cool[i] = max(maxprof_without_stock_sell[i - cool_day], maxprof_without_stock_cool[i - 1]);
		//����ʱ���ɹ������ڵ�������������ǰһ��ҲΪ�ɹ����յ�������������coolday֮ǰ����۵��������ȽϺ�Ľϴ���
	}

	cout << "����ȴ������Ϊ" << cool_day << "��ʱ���ܻ�õ��������Ϊ" << maxprof_without_stock_sell[arr_lenth - 1] << endl;

}