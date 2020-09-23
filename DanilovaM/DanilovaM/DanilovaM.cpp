#include <iostream>
#include <fstream>
using namespace std;

struct Pipeline
{
	int diameter;
	int length;
};

Pipeline InputPipeline()
{
	Pipeline p;
	cout << "Input diameter:";
	cin >> p.diameter;
	cout << "Input length:";
	cin >> p.length;
	return p;
}
void PrintPipeline(const Pipeline& p)
{
	cout << "\nDiameter: " << p.diameter << "\tLength: " << p.length << endl;
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
	fin >> p.diameter;
	fin >> p.length;
	fin.close();
	return p;
}
void SavePipeline(const Pipeline& p)
{
	ofstream fout;
	fout.open("data.txt",ios:: out);
	fout << p.diameter << endl << p.length << endl;
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


