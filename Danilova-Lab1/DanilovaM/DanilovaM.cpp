#include <iostream>
#include <fstream>
#include <string>
#include <limits>
using namespace std;

void PrintMenu() {
	cout << "\n1. Add pipeline information.\n"
		<< "2. Add ks information.\n"
		<< "3. Load pipeline information from file.\n"
		<< "4. Load ks information from file.\n"
		<< "5. View pipeline information.\n"
		<< "6. View ks information.\n"
		<< "7. Save pipeline information to file.\n"
		<< "8. Save ks information to file.\n"
		<< "9. Change attribute 'repair' for a pipeline.\n"
		<< "10. Launch or stop ks ceh (1/0).\n"
		<< "0. Exit.\n";
}
struct Pipeline
{
	int id;
	double length;
	int diameter;
	bool repear;
};
struct Ks
{
	int id;
	string name;
	int ceh;
	int cehwork;
	double effective;
};
void StreamInit()
{
	cin.clear();
	cin.ignore(10000, '\n');
}
template <typename T>
T GetCorrectNumber(T min, T max, string Message)
{
	T vvod;
	cout << Message;
	while ((cin >> vvod).fail() || (vvod <= min) || (vvod>max) )
	{
		StreamInit();
		cout << Message;
	}
	return vvod;
}
Pipeline InputPipeline()
{
	Pipeline p;
	p.id = 1;
	p.length = GetCorrectNumber(0.0, DBL_MAX, "Input length:");
	p.diameter = GetCorrectNumber(0, INT_MAX, "Input diameter:");
	p.repear = false;
	return p;
}
Ks InputKs()
{
	Ks k;
	k.id = 1;
	cout << "Input name:";
	StreamInit();
	getline(cin, k.name);
	k.ceh = GetCorrectNumber(0, INT_MAX, "Input a number of ceh:");
	k.cehwork = GetCorrectNumber(-1, k.ceh, "Input a number of ceh in work (<=number of ceh):");
	k.effective = GetCorrectNumber(0.0, DBL_MAX, "Input efficiency:");
	return k;
}
void PrintPipeline(const Pipeline& p)
{
	cout << "\nId: " << p.id 
		 << "\tLength: " << p.length
		 << "\tDiameter: " << p.diameter  
		 << "\tRepear: " << (p.repear ? "In repear" : "Not in repear") << endl;
}
void PrintKs(const Ks& k)
{
	cout << "\nId: " << k.id
		 << "\tName: " << k.name
		 << "\tNumber of ceh: " << k.ceh
		 << "\tNumber of ceh in work: " << k.cehwork
	     << "\tEfficiency: " << k.effective;
}
void EditPipeline(Pipeline& p)
{
	p.repear = !p.repear;
}
void EditKs(Ks& k, int LaunchCeh, int max)
{
	if (LaunchCeh && k.cehwork<max) k.cehwork++;
	if (!LaunchCeh && k.cehwork) k.cehwork--;
}
Pipeline LoadPipeline()
{
	Pipeline p;
	ifstream fin;
	fin.open("InputPipeline.txt", ios::in);
	if (fin.is_open())
	{
		fin >> p.id >> p.diameter >> p.length >> p.repear;
		fin.close();
	}
	return p;
}
Ks LoadKs()
{
	Ks k;
	ifstream fin;
	fin.open("InputKs.txt", ios::in);
	if (fin.is_open())
	{
		fin >> k.id >> k.name >> k.ceh >> k.cehwork >> k.effective;
		fin.close();
	}
	return k;
}
void SavePipeline(const Pipeline& p)
{
	ofstream fout;
	fout.open("OutputPipeline.txt", ios::out);
	if (fout.is_open())
	{
		fout << p.id << endl << p.diameter << endl << p.length << endl << p.repear << endl;
		fout.close();
	}
}
void SaveKS(const Ks& k)
{
	ofstream fout;
	fout.open("OutputKs.txt", ios::out);
	if (fout.is_open())
	{
		fout << k.id << endl << k.name << endl << k.ceh << endl << k.cehwork << endl << k.effective;
		fout.close();
	}
}
int main()
{
	Pipeline p;
	Ks k; 
	bool PipeInformation = false;
	bool KsInformation = false;
	for ( ; ; ) {
		PrintMenu();
		switch (GetCorrectNumber(-1, 10, "Please, select a number from 0 to 10.\n"))
		{
		case 1: p = InputPipeline(); PipeInformation = true;
			break;
		case 2: k = InputKs(); KsInformation = true;
			break;
		case 3: p = LoadPipeline(); PipeInformation = true;
			break;
		case 4: k = LoadKs(); KsInformation = true;
			break;
		case 5: if (PipeInformation) PrintPipeline(p);
			  else cout << "Pipe information has not been received yet. Input the data by selecting 1 or 3 points.\n ";
			  break;
		case 6: if (KsInformation) PrintKs(k);
			  else cout << "Ks information has not been received yet. Input the data by selecting 2 or 4 points.\n ";
			  break;
		case 7:
			if (PipeInformation) SavePipeline(p);
			else cout << "Pipe information has not been received yet. Input the data by selecting 1 or 3 points.\n ";
			break;
		case 8:
			if (KsInformation) SaveKS(k);
			else cout << "Ks information has not been received yet. Input the data by selecting 2 or 4 points.\n ";
			break;
		case 9: EditPipeline(p);
			break;
		case 10:
			EditKs(k, GetCorrectNumber(-1, 1, "Please, input 1 for launch or 0 for stop: "), k.ceh);
			break;
		 case 0:
			return 0;
			break;
		}

	}

}


