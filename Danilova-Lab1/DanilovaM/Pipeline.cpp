#include "Pipeline.h"
#include "Utils.h"
#include <limits>
using namespace std;

int Pipeline::MaxId = 0;
Pipeline::Pipeline()
{
	id = ++MaxId;
	length = 0.0;
	diameter = 0;
	repear = false;
	InputId = 0;
	OutputId = 0;
}
void Pipeline::EditPipeline()
{
	repear = !repear;
}
int Pipeline::GetId() const
{
	return id;
}
ostream& operator << (ostream& out, const Pipeline& p)
{
	cout << "\nId: " << p.id
		<< "\tLength: " << p.length
		<< "\tDiameter: " << p.diameter
		<< "\tRepear: " << (p.repear ? "In repear" : "Not in repear") << endl;
	return out;
}
istream& operator >> (istream& in, Pipeline& p)
{
	p.length = GetCorrectNumber(0.0, DBL_MAX, "Input length:");
	p.diameter = GetCorrectNumber(0, INT_MAX, "Input diameter:");
	p.repear = false;
	return in;
}

ofstream& operator<<(ofstream& fout, const Pipeline& p)
{
	fout << p.id << endl << p.length << endl << p.diameter << endl << p.repear << endl;
	return fout;
}
ifstream& operator>>(std::ifstream& fin, Pipeline& p)
{
	fin >> p.id >> p.length >> p.diameter >> p.repear;
	return fin;
}

