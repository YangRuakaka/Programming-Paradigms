/****************************************************************
 * Problem Description
 ****************************************************************/

//给你⼀个整数数组 nums 和两个整数 indexDiff 和 valueDiff 。
//找出是否存在这样的两个下标(i, j) ：
//i != j
//abs(i - j) <= indexDiff
//abs(nums[i] - nums[j]) <= valueDiff
//如果存在，返回 true ；否则，返回 false 。

//2 <= nums.length <= 10 ^ 5
//- 10 ^ 9 <= nums[i] <= 10 ^ 9
//1 <= indexDiff <= nums.length
//0 <= valueDiff <= 10 ^ 9

#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <unordered_set>
#include <cmath>

const int lowerlimit_value = 0;
const int upperlimit_value = 10 * 10 * 10 * 10 * 10 * 10 * 10 * 10 * 10;
const unsigned int lowerlimit_index = 1;
const unsigned int upperlimit_index = 10 * 10 * 10 * 10 * 10;
const unsigned int len = 10 * 10 * 10 * 10 * 10;
const unsigned int upperlimit = 10 * 10 * 10 * 10 * 10 * 10 * 10 * 10 * 10;
const int lowerlimit = -10 * 10 * 10 * 10 * 10 * 10 * 10 * 10 * 10;

class solution {
private:
	std::vector<int> arr;
	//Declare a vector to store input data.
	unsigned int indexDiff;
	 int valueDiff;
	//Declare two variable to store input condition data.
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

			std::cout << "请输入一个整数数组，以空格隔开" << std::endl;//Display the prompt

			int my_input;//Create a variable to hold my input

			for (int i = 0; i < len; i++) {

				while (std::cin.peek() == ' ') {

					char ch;

					std::cin.get(ch);//Skip if a space is found

				}//This loop handles the situation where, after inputting the last number, if another space is entered, the function cannot exit.

				if (std::cin.peek() == '\n' && i != 0)//If a newline character is encountered during a loop, it immediately exits the function, terminating the input
					return;


				std::cin >> my_input;

				if (std::cin.good() && (my_input >= lowerlimit && my_input <= upperlimit)) {
					arr.push_back(my_input);
					//Entering into a new cycle of the loop to input data
				}
				else {

					std::cout << "输入错误，请重新输入!" << std::endl;
					std::cin.clear();
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');//Display the prompt and clear the input stream
					arr.clear();//Clear the whole vector
					break;//Break to start a new while loop
				}

			}//end of for

		}//end of while

	}

	/***************************************************************************
	 Function Name:Diff_input
	 Function:Input data
	 Input Parameters:void
	 Return Value:void
	 Documentation：
	****************************************************************************/
	void Diff_input()
	{

		while (true) {

			std::cout << "请输入两个整数，先输入indexDiff，再输入valueDiff" << std::endl;//Display the prompt
			std::cin >> indexDiff;
			std::cin >> valueDiff;

			//Display the prompt and clear the input stream
			if (std::cin.good() && (indexDiff >= lowerlimit_index && indexDiff <= upperlimit_index)&&(valueDiff >= lowerlimit_value && valueDiff <= upperlimit_value))
				return;
			else {
				std::cout << "输入错误，请重新输入!" << std::endl;
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			
			}

		}//end of while
	}

	/***************************************************************************
	Function Name:hasRepeatedElements_Diif
	Function:Judge whether the queue has repeated elements(conditionally)
	Input Parameters:const std::queue<int> window
	Return Value:bool
	Documentation：It is a function that can only read
	****************************************************************************/
	bool hasRepeatedElements_Diff(const std::queue<int> window)
	{
		std::unordered_set<int> seen; //Store the data we have seen
		std::queue<int> tempQueue = window; //Duplicate the queue

		while (!tempQueue.empty()) {
			//Create a variable to store the element we are checking
			int currentElement = tempQueue.front();
			tempQueue.pop();

			//If the we have seen the element that qualifies return true
			for (int element : seen) {
				if (std::abs(currentElement - element) <= valueDiff)
					return true;

			}

			//Put the element into the 'seen'
			seen.insert(currentElement);
		}

		return false; //Did not see any repeated elements
	}

	/***************************************************************************
	Function Name:hasRepeatedElements_Diff
	Function:Judge whether the vector has repeated elements
	Input Parameters:void
	Return Value:bool
	Documentation：
	****************************************************************************/
	bool hasRepeatedElements_Diff()
	{
		std::queue<int> window;
		//Declare a queue to store the range we are checking

		for (unsigned int i = 0; i < arr.size(); i++) {

			window.push(arr[i]);//Push an element into the queue

			//If the amount of window is bigger than k+1,pop out the first number
			if (window.size() > indexDiff + 1)
				window.pop();

			//If the window has repeated elements return ture
			if (hasRepeatedElements_Diff(window))
				return true;

		}//run all the data only once

		return false;

	}

};

int main()
{
	solution s1;//Create a new class

	s1.input();//Input data

	s1.Diff_input();//Input k

	if (s1.hasRepeatedElements_Diff()) {
		std::cout << "True有重复元素" << std::endl;
	}
	else {
		std::cout << "False无重复元素" << std::endl;
	}

	return 0;
}