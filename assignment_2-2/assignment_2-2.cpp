/****************************************************************
 * Problem Description
 ****************************************************************/

//给你⼀个电影的播放时间安排的数组intervals，每部电影的播放时间由其开始和结束的时间标记，
//即 intervals[i] = [starti, endi] 。请返回该影院需要的最少放映厅数量，以确保所有电影都能
//按计划放映
//1 <= intervals.length <= 10 ^ 4
//0 <= starti < endi <= 10 ^ 6

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
	enum time_type { start_time = 0, end_time = 1, over_time = -1 };
	//over_time means when a movie's start_time is later than anther movie's end_time,the end time of the other movie will be replaced as over_time.
	std::vector<std::vector<int>> intervals;
	//Declare a two-dimensional vector to store input data.
public:


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
	int Min_rooms()
	{
		sort();//Sort the vector in ascending order based on its first element
	
		int rooms=1;

		bool room_state;

		for (unsigned int i = 1; i < intervals.size(); i++) {

			room_state = occupied;//Assume every movie needs a new room to display

			for (int j = 0; j < i; j++) {


				if (intervals[i][start_time] > intervals[j][end_time] && intervals[j][end_time] != over_time) {

					intervals[j] = { start_time,over_time };//Flag a movie as it has been over,and will not occupy any rooms
					room_state = vacant;//The movie that has just started can be displayed in the room in which another movie is just over,the room is "vacant"
					break;//No need for another "vacant" room,so stop calculating
				}
				
			}

			if (room_state)
				rooms++;//If there is not an single room "vacant",we have to add a new room for the movie we are discussing

		}

		return rooms;

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

};

int main()
{
	solution s1;//Create a new class

	s1.input();//Input data

	std::cout << "最小放映厅数为：" << s1.Min_rooms() << std::endl;//Display the output

	return 0;
}