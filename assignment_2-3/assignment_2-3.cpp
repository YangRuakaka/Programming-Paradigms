/****************************************************************
 * Problem Description
 ****************************************************************/

//影院为观众提供了⼀系列电影的播放时间安排intervals，其中每部电影的播放时间由其开始和结束
//的时间标记，即 intervals[i] = [starti, endi] 。由于部分电影有重叠的播放时间，影院决定合
//并这些时间，以提供⼀个新的播放时间表。请你合并所有重叠的播放时间，并返回⼀个不重叠的播放时
//间数组，该数组需恰好覆盖输⼊中的所有时间
//1 <= intervals.length <= 10 ^ 4
//intervals[i].length == 2
//0 <= starti <= endi <= 10 ^ 4

#include <iostream>
#include <vector>
#include <algorithm>


#define vacant 0
#define occupied 1

const unsigned int len = 10 * 10 * 10 * 10;
const unsigned int upperlimit = 10 * 10 * 10 * 10 * 10 * 10;
const unsigned int lowerlimit = 0;


class solution {
private:
	enum time_type { start_time, end_time };
	//over_time means when a movie's start_time is later than anther movie's end_time,the end time of the other movie will be replaced as over_time.
	std::vector<std::vector<int>> intervals;
	//Declare a two-dimensional vector to store input data.
public:



	/***************************************************************************
     Function Name:getintervals
     Function:To get intervals
     Input Parameters:void
     Return Value:private vector variable
     Documentation：
    ****************************************************************************/
	const std::vector<std::vector<int>>& getintervals() const
	{
		return intervals;
	}


	/***************************************************************************
	 Function Name:input
	 Function:Input data
	 Input Parameters:void
	 Return Value:void
	 Documentation：
	****************************************************************************/
	void input()
	{
		while (true) {

			std::cout << "请依次输入每部电影开始和结束的时间，用空格分隔(开始时间不能大于等于结束时间,且必须输入偶数个数据）" << std::endl;//Display the prompt

			int my_input_start;
			int my_input_end;//Declare two variables for storing the data we are currently entering

			for (int i = 0; i < len; i++) {

				while (std::cin.peek() == ' ') {

					char ch;

					std::cin.get(ch);//Skip if a space is found

				}//This loop handles the situation where, after inputting the last number, if another space is entered, the function cannot exit.

				if (std::cin.peek() == '\n' && i != 0)//If a newline character is encountered during a loop, it immediately exits the function, terminating the input
					return;

				std::cin >> my_input_start;
				std::cin >> my_input_end;//Input the data

				if (std::cin.good() && (my_input_start >= lowerlimit && my_input_start <= upperlimit) && my_input_start <= my_input_end && (my_input_end >= lowerlimit && my_input_end <= upperlimit)) {

					std::vector<int> row = { my_input_start ,my_input_end };//Create a new vector to store the entire one-dimensional vector in one go

					intervals.push_back(row);//and push_back the entire vector to the two-dimensional vector

					continue;//Entering into a new cycle of the loop to input data
				}
				else {

					std::cout << "输入错误，请重新输入!" << std::endl;
					std::cin.clear();
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');//Display the prompt and clear the input stream

					intervals.clear();//Clear the whole vector

					break;//Break to start a new while loop
				}

			}//end of for

		}//end of while

	}

	/***************************************************************************
	Function Name:Min_rooms
	Function:Calculate the minimum number of the rooms
	Input Parameters:void
	Return Value:int
	Documentation：
	****************************************************************************/
	void Combine()
	{
		sort();//Sort the vector in ascending order based on its first element

		for (unsigned int i = 1; i < intervals.size(); i++) {

			if (intervals[i][start_time] <= intervals[i - 1][end_time]) {

				intervals[i - 1][end_time] = intervals[i][end_time] > intervals[i - 1][end_time] ? intervals[i][end_time] : intervals[i - 1][end_time];
				//If there is overlap,set the former movie's end_time the longer time of the two movies.

				intervals.erase(intervals.begin() + i);
				//Erase the latter movie as we combine the two movies 

				i--;
				//As we erase a vector,the size of intervals is cut,and we have to reconsider the next movie's index,so "go back a step".
			}

		}

	}

	/***************************************************************************
	 Function Name:sort
	 Function:Sort the vector in ascending order based on its first element
	 Input Parameters:void
	 Return Value:void
	 Documentation：
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

	/***************************************************************************
	 Function Name:Display
	 Function:Display the vector
	 Input Parameters:void
	 Return Value:void
	 Documentation：
	 ****************************************************************************/
	void Display() 
	{
		std::cout << "输出：";

		for (int i = 0; i < intervals.size(); i++) {
			std::cout << "["<< intervals[i][start_time]<<","<< intervals[i][end_time]<<"]";
		}

	}

};

int main()
{
	solution s1;//Create a new class

	s1.input();//Input data

	s1.Combine();//Combine the vector

	s1.Display();//Display the combined vector

	return 0;
}
