#pragma once
#include <vector>
#include <array>

class Character
{
public:
    virtual int get_row() = 0;
	virtual void set_row(int) = 0;

	virtual int get_col() = 0;
	virtual void set_col(int) = 0;

	virtual char get_repr() = 0;

protected:
	int _row, _col;
	std::vector<std::array<int, 2>> m_legal_moves;
	char _repr;
};