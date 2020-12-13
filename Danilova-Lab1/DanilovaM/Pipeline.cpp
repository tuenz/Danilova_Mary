#include "Pipeline.h"
#include "Utils.h"
#include <limits>
#include <cmath>
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
	Weight = 0;
	Throughput = 0;
}
void Pipeline::EditPipeline()
{
	repear = !repear;
}
int Pipeline::GetId() const
{
	return id;
}
int Pipeline::GetWeight() const
{
	return (int)length;
}
int Pipeline::GetThroughput() const
{
	return (int)sqrt(pow(diameter, 5)/length);
}
ostream& operator << (ostream& out, const Pipeline& p)
{
	cout << "\nId: " << p.id
		<< "\tLength: " << p.length
		<< "\tDiameter: " << p.diameter
		<< "\tRepear: " << (p.repear ? "In repear" : "Not in repear")
		<< "\tInputId: " << p.InputId
		<< "\tOutputId: " << p.OutputId
		<< "\tWeight: " << p.Weight
		<< "\tThroughput:" << p.Throughput << endl;
	return out;
}
istream& operator >> (istream& in, Pipeline& p)
{
	p.length = GetCorrectNumber(0.0, DBL_MAX, "Input length:");
	p.diameter = GetCorrectNumber(0, INT_MAX, "Input diameter:");
	p.repear = false;
	p.Weight = p.GetWeight();
	p.Throughput = p.GetThroughput();
	return in;
}

ofstream& operator<<(ofstream& fout, const Pipeline& p)
{
	fout << p.id << endl << p.length << endl << p.diameter << endl << p.repear << endl << p.InputId << endl << p.OutputId << endl << p.Weight << endl << p.Throughput << endl;
	return fout;
}
ifstream& operator>>(std::ifstream& fin, Pipeline& p)
{
	fin >> p.id >> p.length >> p.diameter >> p.repear >> p.InputId >> p.OutputId >> p.Weight >> p.Throughput;
	return fin;
}