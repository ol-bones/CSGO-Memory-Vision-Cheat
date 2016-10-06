#include "stdafx.h"
#include "scatter_string.h"

ScatterString::ScatterString(const std::string str)
{
	std::cout << &str;
	// Create twice as many character holders as the intended number of characters plus a random 0-5 more
	char_holders = str.length() * 2 + (rand() % 5);
	// Allocate the memory of the char holders, return pointer to start of the allocated memory and cast it as char pointer
	buffer = (char*)malloc(sizeof(char) * char_holders);

	for (unsigned int i = 0; i < str.length(); i++)
	{
		// Get random char holder index
		int rand_holder = rand() % char_holders;

		// Check if random holder already used
		// If used, generate a new index until an unused holder is found
		for (unsigned int j = 0; j < used_holders.size(); j++)
		{
			if (rand_holder == used_holders[j])
			{
				rand_holder = rand() % char_holders;
				j = 0;
			}
		}

		// Store the current character in the unused random holder and record its location
		buffer[rand_holder] = str[i];
		used_holders.push_back(rand_holder);
	}

	// Fill unused holders with random characters
	for (unsigned int i = 0; i < char_holders; i++)
	{
		if (!HolderUsed(i))
		{
			buffer[i] = randchar();
		}
	}
}

bool ScatterString::HolderUsed(int h_idx)
{
	for (unsigned int i = 0; i < used_holders.size(); i++)
	{
		if (used_holders[i] == h_idx)
		{
			return true;
		}
	}
	return false;
}

ScatterString::~ScatterString()
{
	free(buffer);
}

std::string ScatterString::Retrieve()
{ 
	std::string str;

	// Get each character and add it to the new string in order
	for (unsigned int i = 0; i < used_holders.size(); i++)
	{
		str += buffer[used_holders[i]];
	}

	return str;
}

