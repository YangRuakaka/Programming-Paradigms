/****************************************************************
 * Problem Description
 ****************************************************************/

//ĳӰԺΪ�����ṩ��һϵ�е�Ӱ�Ĳ���ʱ�䰲�� intervals ��ÿ����Ӱ�Ĳ���ʱ�����俪ʼ�ͽ�����
//ʱ���ǣ��� intervals[i] = [starti, endi] �������ж�һ�����Ƿ��ܹ������������ȫ����
//Ӱ��������κ�һ����
//0 <= intervals.length <= 10 ^ 4
//intervals[i].length == 2
//0 <= starti < endi <= 10 ^ 6


#include <iostream>
#include <vector>
#include <algorithm>

const unsigned int len = 10 * 10 * 10 * 10;
const unsigned int upperlimit = 10 * 10 * 10 * 10 * 10 * 10;
const unsigned int lowerlimit = 0;


class solution {
private:
	enum time_type{start_time,end_time};
	std::vector<std::vector<int>> intervals;
	//Declare a two-dimensional vector to store input data.
public:


    /***************************************************************************
     Function Name:input
     Function:Input data
     Input Parameters:void
     Return Value:void
     Documentation��
    ****************************************************************************/
	void input()
	{
		while (true) {

			std::cout << "����������ÿ����Ӱ��ʼ�ͽ�����ʱ�䣬�ÿո�ָ�(��ʼʱ�䲻�ܴ��ڵ��ڽ���ʱ��,�ұ�������ż�������ݣ�" << std::endl;//Display the prompt
			
			int my_input_start;
			int my_input_end;//Declare two variables for storing the data we are currently entering

			for (int i = 0; i < len; i++) {

				while (std::cin.peek() == ' ') {

					char ch;

					std::cin.get(ch);//Skip if a space is found

				}//This loop handles the situation where, after inputting the last number, if another space is entered, the function cannot exit.

				if (std::cin.peek() == '\n' && i != 0 )//If a newline character is encountered during a loop, it immediately exits the function, terminating the input
					return;

				std::cin >> my_input_start;
				std::cin >> my_input_end;//Input the data

				if (std::cin.good() && (my_input_start >= lowerlimit && my_input_start <= upperlimit) && my_input_start < my_input_end && (my_input_end >= lowerlimit && my_input_end <= upperlimit)) {

					std::vector<int> row = { my_input_start ,my_input_end };//Create a new vector to store the entire one-dimensional vector in one go

					intervals.push_back(row);//and push_back the entire vector to the two-dimensional vector

					continue;//Entering into a new cycle of the loop to input data
				}
				else {

					std::cout << "�����������������!" << std::endl;
					std::cin.clear();
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');//Display the prompt and clear the input stream

					intervals.clear();//Clear the whole vector
				
					break;//Break to start a new while loop
				}

			}//end of for
		
		}//end of while

	}

	/***************************************************************************
	Function Name:CanWatchAllTheMovies
	Function:judge whether one Watch All The Movies without missing one
	Input Parameters:void
	Return Value:bool
	Documentation��
    ****************************************************************************/
	bool CanWatchAllTheMovies()
	{
		sort();//sort the vector in ascending order.

		for (unsigned int i = 0; i + 1 < intervals.size(); i++) {

			//if finding one single movie's end time is later than the next movie's start time,return 0 as it is deemed that one cannot watch all the movies.
			if (intervals[i][end_time] > intervals[i + 1][start_time])
				return 0;
			
		}//run all the data once

		return 1;

	}
	
	/***************************************************************************
     Function Name:sort
     Function:Sort the vector in ascending order based on its first element 
     Input Parameters:void
     Return Value:void
     Documentation��
     ****************************************************************************/
	void sort()
	{
		for (unsigned int iX = 0; iX < intervals.size(); iX++) {
			for (unsigned int iY = iX + 1; iY < intervals.size(); iY++) {

				//if the first element is bigger than its in the rear vector,then exchange the two vector
				if (intervals[iX][start_time] > intervals[iY][start_time]) {

					std::vector<int> tmp;
					//Declare two temporary vector to store the data

					tmp = intervals[iX];
					intervals[iX] = intervals[iY];
					intervals[iY] = tmp;//Exchange two vector

				}
			}
		}
	}

};

int main()
{
	solution s1;//Create a new class

	s1.input();//Input data

	if (s1.CanWatchAllTheMovies())//Display the outcome
		std::cout << "True���ܿ������е�Ӱ" << std::endl;
	else
		std::cout << "False�����ܿ������е�Ӱ" << std::endl;

	return 0;
}