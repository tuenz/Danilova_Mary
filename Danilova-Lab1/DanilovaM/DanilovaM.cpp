#include <iostream>
#include <fstream>
#include <string>
using namespace std;

void PrintMenu() {
	cout << "\nSelect, please:\n"
		<< "1. Add pipeline information.\n"
		<< "2. Add ks information.\n"
		<< "3. Load pipeline information from file.\n"
		<< "4. Load ks information from file.\n"
		<< "5. View pipeline information.\n"
		<< "6. View ks information.\n"
		<< "7. Save pipeline information to file.\n"
		<< "8. Save ks information to file.\n"
		<< "9. Change attribute 'repair' for a pipeline.\n"
		<< "10. Launch or stop ks department (l/s).\n"
		<< "0. Exit\n";
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
Pipeline InputPipeline()
{
	Pipeline p;
	p.id = 1;
	//do {
		cout << "Input diameter:";
		cin >> p.diameter;
	//} while ();
	cout << "Input length:";
	cin >> p.length;
	p.repear = false;
	return p;
}
Ks InputKs()
{
	Ks k;
	k.id = 1;
	cout << "Input name:";
	cin.clear();
	cin.ignore(10000, '\n');
	getline(cin, k.name);
	cout << "Input a number of ceh:";
	cin >> k.ceh;
	cout << "Input a number of ceh in work:";
	cin >> k.cehwork;
	cout << "Input efficiency:";
	cin >> k.effective;
	return k;
}
void PrintPipeline(const Pipeline& p)
{
	cout << "\nId: " << p.id 
		 << "\tDiameter: " << p.diameter 
		 << "\tLength: " << p.length 
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
void EditKs(Ks& k, char department)
{
	if (department == (char)"l")
	{
		k.cehwork++;
	}
	else
	{
		k.cehwork--;
	}
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
		fout << k.id << endl << k.name << endl << k.ceh << endl << k.cehwork << k.effective;
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
		int choice;
		PrintMenu();
		cin >> choice;
		switch (choice)
		{
		case 1: p = InputPipeline(); PipeInformation = true;
			break;
		case 2: k = InputKs(); KsInformation = true;
			break;
		case 3: p = LoadPipeline(); PipeInformation = true;
			break;
		case 4: k = LoadKs(); KsInformation = true;
			break;
		case 5: if (PipeInformation) 
		{
			PrintPipeline(p);
		} 
			  else
		{
			cout << "Pipe information has not yet been received. Enter the data by selecting 1 or 3 points.\n ";
		}
			  break;
		case 6: if (KsInformation)
		{
			PrintKs(k);
		}
			else
		{
			cout << "Ks information has not yet been received. Enter the data by selecting 2 or 4 points.\n ";
		}
			  break;
		case 7: 
			if (PipeInformation)
			{
				SavePipeline(p);
			}
			else
			{
				cout << "Pipe information has not yet been received. Enter the data by selecting 1 or 3 points.\n ";
			}
			break;
		case 8:
			if (KsInformation)
		{
				SaveKS(k);
		}
			  else
		{
			cout << "Ks information has not yet been received. Enter the data by selecting 2 or 4 points.\n ";
		}
			break;
		case 9: EditPipeline(p);
			break;
		case 10: cout << "Please, input l or s: ";
			char department;
			cin >> department;
			EditKs(k, department);
			break;
		case 0:
			return 0;
			break;
		default:
			cout << "\nPlease, choose one of the following actions. " << endl;
			break;
		}

	}

}


