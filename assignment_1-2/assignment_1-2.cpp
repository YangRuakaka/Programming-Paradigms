#include<iostream>
#include<algorithm>
#define len 105
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

		cout << "�������Ʊ�۸񣨼۸񲻵�С��0�����ô���104������������105����ͬ�۸�֮���ÿո�ֿ���" << endl;//��ʾ������ʾ

		for (int i = 0; i < len; i++) {

			while (cin.peek() == ' ') {

				char ch;

				cin.get(ch);//��������ո������

			}//��ѭ������������һ�����ֺ����������˿ո�input�����޷��˳������

			if (cin.peek() == '\n'  && i != 0)//���ĳ��ѭ�������س���ֱ������������ֹͣ����
				return lenth;

			cin >> *(p + i);//ÿ��ѭ������һ����

			lenth++;//ÿ������һ���������ȼ�һ

			if (cin.good() && *(p + i) >= 0 && *(p + i) <= 104)//ÿ��������������������Ƿ���ȷ���ڷ�Χ֮��
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

/***************************************************************************
  �������ƣ�max_calculate
  ��    �ܣ�������統�������ܻ�õ����ֵ
  �����������ǰ�������ֵ��ָ�����鵱��Ԫ�ص�ָ��,���鳤��,�����׵�ַ
  �� �� ֵ��int���ֵ
  ˵    ����
****************************************************************************/
int max_calculate(int my_max, int* p, int lenth, int* arr)
{

	for (int* ret = p; p - arr < lenth; p++) {

		my_max = max(*p - *ret, my_max);//�Ƚϵ�ǰ��������뵱����֮��ÿ�������

	}

	return my_max;//�������ֵ

}

int main()
{

	int prices[len] = { 0 };
	int max_profit = 0;

	int arr_lenth = input(prices);


	for (int i = 0; i < arr_lenth; i++) {

		max_profit = max_calculate(max_profit,prices+i, arr_lenth, prices);

	}//����ÿ���ܻ�õ�������棬�����ϵ����������

	cout << max_profit << endl;
	
}