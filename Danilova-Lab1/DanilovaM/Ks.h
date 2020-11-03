#pragma once
#include <string>
class Ks
{
public:
	int id;
	static int MaxId;
	std::string name;
	int ceh;
	int cehwork;
	double effective;

	Ks();
	void EditKs(int LaunchCeh);
	friend std::ostream& operator << (std::ostream& out, const Ks& k);
	friend std::istream& operator >> (std::istream& in, Ks& k);
};

