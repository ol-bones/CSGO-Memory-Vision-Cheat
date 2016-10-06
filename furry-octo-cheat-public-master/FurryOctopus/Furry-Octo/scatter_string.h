#pragma once
#include "stdafx.h"
#include "rand_char.h"

class ScatterString
{
public:
	ScatterString(const std::string str);
	~ScatterString();

	std::string Retrieve();

private:
	char* buffer;
	unsigned int char_holders;
	std::vector<int> used_holders;

	bool HolderUsed(int h_idx);
};

