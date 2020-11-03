#include "Pipeline.h"
#include "Utils.h"
#include <limits>

int Pipeline::MaxId = 0;
Pipeline::Pipeline()
{
	id = ++MaxId;
	length = 0.0;
	diameter = 0;
	repear = false;
}
void Pipeline::EditPipeline()
{
	repear = !repear;
}
std::ostream& operator<<(std::ostream& out, const Pipeline& p)
{
	std::cout << "\nId: " << p.id 
		<< "\tLength: " << p.length
		<< "\tDiameter: " << p.diameter
		<< "\tRepear: " << (p.repear ? "In repear" : "Not in repear") << std::endl;
	return out;
}

std::istream& operator>>(std::istream& in, Pipeline& p)
{
	p.length = GetCorrectNumber(0.0, DBL_MAX, "Input length:");
	p.diameter = GetCorrectNumber(0, INT_MAX, "Input diameter:");
	p.repear = false;
	return in;
}
