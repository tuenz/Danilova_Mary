#include <iostream>
#include <fstream>
using namespace std;

struct Pipeline
{
	int id;
	int diameter;
	int length;
    int repear;
};

Pipeline InputPipeline()
{
	Pipeline p;
	cout << "Input id:";
	cin >> p.id;
	cout << "Input diameter:";
	cin >> p.diameter;
	cout << "Input length:";
	cin >> p.length;
	cout << "Repear (1/0):";
	cin >> p.repear;
	return p;
}
void PrintPipeline(const Pipeline& p)
{
	cout << "\nId: " << p.id << "\tDiameter: " << p.diameter << "\tLength: " << p.length << "\tRepear: " << p.repear<< endl;
}
void EditPipeline(Pipeline& p)
{
	p.diameter -= 1;
}
Pipeline LoadPipeline()
{
	ifstream fin;
		fin.open("data.txt", ios::in);
	Pipeline p;
	fin >> p.id;
	fin >> p.diameter;
	fin >> p.length;
	fin >> p.repear;
	fin.close();
	return p;
}
void SavePipeline(const Pipeline& p)
{
	ofstream fout;
	fout.open("data.txt",ios:: out);
	fout <<p.id<< endl<<p.diameter << endl << p.length <<endl<< p.repear<<endl;
	fout.close();
}
int main()
{
	Pipeline pipe = InputPipeline();
	PrintPipeline(pipe);
	EditPipeline(pipe);
	PrintPipeline(pipe);
	SavePipeline(pipe);
	return 0;
}


