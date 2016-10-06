#include "stdafx.h"
#include "decrypt_string.h"

void DecryptString(std::string& str)
{
	std::string initkey = "d3ixrNuVTMJrqmwpflG9PrUJdsb1zh43gYrVfftT";
	std::string key(initkey);

	int outputcharc = 0;

	// Count the output message character length
	for (int i = 0; i < str.size(); i++)
	{
		if (str[i] == '_')
		{
			outputcharc++;
		}
	}

	// Ensure key length is bigger than the output message length
	while (key.size() < outputcharc)
	{
		key += initkey;
	}

	std::string eschar;
	int charidx = 0;

	std::string output;

	// Decrypt the input string, character by character
	for (int i = 0; i < str.size(); i++)
	{
		if (str[i] == '_')
		{
			while (i + 1 < str.size() && str[i + 1] != '_')
			{
				eschar += str[i + 1];
				i++;
			}
			
			char echar = std::stoi(eschar);
			output += echar ^ key[charidx];

			eschar.clear();
			charidx++;
		}
	}

	// Clear the encrypted input string, ready for decrypted message
	str.clear();

	// Write the output message to the input string
	for (int i = 0; i < output.size(); i++)
	{
		str += output[i];
	}
	output.clear();
}
