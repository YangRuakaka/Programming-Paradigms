/****************************************************************
 * Problem Description
 ****************************************************************/

//给你⼀个整数数组 nums 。
//如果任意⼀值在数组中出现⾄少两次，返回 true ；
//如果数组中每个值仅出现⼀次，返回 false 。
//1 <= nums.length <= 10 ^ 5
//- 10 ^ 9 <= nums[i] <= 10 ^ 9

#include <iostream>
#include <vector>
#include <algorithm>

const unsigned int len = 10 * 10 * 10 * 10 * 10;
const unsigned int upperlimit = 10 * 10 * 10 * 10 * 10 * 10 * 10 * 10 * 10;
const int lowerlimit = -10 * 10 * 10 * 10 * 10 * 10 * 10 * 10 * 10;

class solution {
private:
	std::vector<int> arr;
	//Declare a vector to store input data.
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
					//Display the prompt and clear the input stream
					std::cout << "输入错误，请重新输入!" << std::endl;
					std::cin.clear();
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					arr.clear();//Clear the whole vector
					break;//Break to start a new while loop
				}

			}//end of for

		}//end of while

	}

	/***************************************************************************
	Function Name:hasRepeatedElements
	Function:Judge whether the vector has repeated elements
	Input Parameters:void
	Return Value:bool
	Documentation：
	****************************************************************************/
	bool hasRepeatedElements()
	{
		//Using the sort function from the standard library
		std::sort(arr.begin(), arr.end(), [](const int a, const int b) {
			return a < b;//It is a anonymous function used to tell the std::sort how we want to sort our vector
			});

		for (unsigned int i = 0; i < arr.size()-1; i++) {

			//If the element is equal to the next element,it means the vector has repeated elements
			if (arr[i] == arr[i + 1])
				return 1;

		}//run all the data once

		return 0;

	}

};

int main()
{
	solution s1;//Create a new class

	s1.input();//Input data

	if (s1.hasRepeatedElements()) {
		std::cout << "True有重复元素" << std::endl;
	}
	else {
		std::cout << "False无重复元素" << std::endl;
	}

	return 0;
}