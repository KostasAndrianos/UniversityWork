#include <iostream>
#include <stdlib.h>
#include <map>
#include <array>
#include <string>

std::map<long long int, std::string> stages;
std::map<int, std::string> tens_prefixes;
std::array<std::string, 2> tens_postfixes;
std::map<int, std::string> numbers;

void doMath(int number, long long int num_stage);
void printNum(int current, long long int stage, int mode);
void doMathHelper(long long int input_number, int divider);


int main(int argc, const char *argv[])
{
	long long int num;
	std::cout << "Give a number: ";
	std::cin >> num;

	stages[1000000000] = "billion";
	stages[1000000] = "million";
	stages[1000] = "thousand";
	stages[100] = "hundred";

	tens_prefixes[3] = "thir";
	tens_prefixes[4] = "four";
	tens_prefixes[5] = "fif";
	tens_prefixes[6] = "six";
	tens_prefixes[7] = "seven";
	tens_prefixes[8] = "eigh";
	tens_prefixes[9] = "nine";

	tens_postfixes = {{"teen", "ty"}};

	numbers[0] = "zero";
	numbers[1] = "one";
	numbers[2] = "two";
	numbers[3] = "three";
	numbers[4] = "four";
	numbers[5] = "five";
	numbers[6] = "six";
	numbers[7] = "seven";
	numbers[8] = "eight";
	numbers[9] = "nine";
	numbers[10] = "ten";
	numbers[11] = "eleven";
	numbers[12] = "twelve";
	numbers[20] = "twenty";

	doMath(num, 1000000000);
}

void doMath(int number, long long int num_stage)
{
	int og_number = number;
	int current = number / num_stage;
	number %= num_stage;
	if (current > 0 && (num_stage != 10))
	{
		if(current < 100)
		{
			doMathHelper(current, num_stage);
		}
		else
		{
			if (num_stage == 1000 || num_stage == 100)
				doMath(current, num_stage / 10);
			else
				doMath(current, num_stage / 1000);
		}

		printNum(current, num_stage, 1);
	}
	else if (num_stage == 10)
	{
		doMathHelper(og_number, num_stage);
	}
	if (num_stage != 10)
	{
		if (num_stage == 1000 || num_stage == 100)
			doMath(number, num_stage / 10);
		else
			doMath(number, num_stage / 1000);
	}
}

void doMathHelper(long long int input_number, int divider)
{
	if (input_number <= 10 || input_number == 11 || input_number == 12 || (input_number >= 20 && input_number < 30))
	{
		if (input_number >= 20 && input_number < 30)
		{
			printNum(20, divider, 0);
			printNum(input_number - 20, divider, 0);
		}
		else
		{
			printNum(input_number, divider, 0);
		}
	}
	else if (input_number < 20)
	{
		printNum(input_number % 10, divider, 2);
	}
	else if (input_number < 100)
	{
		printNum(input_number / 10, divider, 3);
		printNum(input_number % 10, divider, 0);
	}
}

void printNum(int current, long long int stage, int mode)
{
	if (mode == 0)
	{
		std::map<int, std::string>::iterator it = numbers.find(current);
		if (it != numbers.end())
			std::cout << it->second << " ";
	}
	else if (mode == 1)
	{
		std::map<long long int, std::string>::iterator it = stages.find(stage);
		if (it != stages.end())
			std::cout << it->second << " ";
	}
	else if (mode == 2 || mode == 3)
	{
		std::map<int, std::string>::iterator it = tens_prefixes.find(current);
		if (it != tens_prefixes.end())
			std::cout << it->second;
	}

	if (mode == 2)
		std::cout << tens_postfixes[0] << " ";
	else if (mode == 3)
		std::cout << tens_postfixes[1] << " ";
}
