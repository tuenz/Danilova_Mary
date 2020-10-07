#include <iostream>
#include <fstream>
#include <string>
using namespace std;

void Menu() {
	cout << "Select:\n"
		<< "1. Add pipeline information\n"
		<< "2. Add ks information\n"
		<< "3. View all objects\n"
		<< "4. Change pipeline information\n"
		<< "5. Change ks information\n"
		<< "6. Save all information to file\n"
		<< "7. Load all information from file\n"
		<< "0. Exit\n";
}
struct Pipeline
{
	int id;
	int diameter;
	double length;
    bool repear;
};
struct ks
{
	int id;
	string name;
	int ceh;
	int cehwork;
	double effective;
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
	while (1) {
		int i;
		Menu();
		cin >> i;
		switch (i)
		{
		case 1: 
			break;
		case 2:
			break;
		case 3: 
			break;
		case 4:
			break;
		case 5: 
			break;
		case 6: 
			break;
		case 7: 
			break;
		case 0:
			return 0;
			break;
		default:
			cout << "\nPlease, choose one of the folllowing actions. " << endl;
			break;
		}

	}

}


