#include "Ks.h"
#include "Utils.h"
#include <limits>

int Ks::MaxId = 0;
Ks::Ks()
{
	id = ++MaxId;
	name = "NoName";
	ceh = 0;
	cehwork = 0;
	effective = 0.0;
}
void Ks::EditKs(int LaunchCeh)
{
	if (LaunchCeh && cehwork < ceh) cehwork++;
	if (!LaunchCeh && cehwork) cehwork--;
}
std::ostream& operator<<(std::ostream& out, const Ks& k)
{
	std::cout << "\nId: " << k.id
		<< "\tName: " << k.name
		<< "\tNumber of ceh: " << k.ceh
		<< "\tNumber of ceh in work: " << k.cehwork
		<< "\tEfficiency: " << k.effective;
	return out;
}

std::istream& operator>>(std::istream& in, Ks& k)
{
	std::cout << "Input name:";
	std::cin.ignore(1, '\n');
	getline(std::cin, k.name);
	k.ceh = GetCorrectNumber(0, INT_MAX, "Input a number of ceh:");
	k.cehwork = GetCorrectNumber(-1, k.ceh, "Input a number of ceh in work (<=number of ceh):");
	k.effective = GetCorrectNumber(0.0, DBL_MAX, "Input efficiency:");
	return in;
}
