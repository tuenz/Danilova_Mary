#pragma once
#include <iostream>
class Pipeline
{
public:
	int id;
	static int MaxId;
	double length;
	int diameter;
	bool repear;

	Pipeline();
	void EditPipeline();
	friend std::ostream& operator << (std::ostream& out, const Pipeline& p);
	friend std::istream& operator >> (std::istream& in, Pipeline& p);

};

