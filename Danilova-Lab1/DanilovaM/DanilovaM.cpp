#include <iostream>
#include <fstream>
#include <string>
#include <limits>
using namespace std;

void PrintMenu() {
	cout << "\n1. Add pipeline information.\n"
		<< "2. Add ks information.\n"
		<< "3. Load pipeline and ks information from file.\n"
		<< "4. View pipeline information.\n"
		<< "5. View ks information.\n"
		<< "6. Save pipeline and ks information to file.\n"
		<< "7. Change attribute 'repair' for a pipeline.\n"
		<< "8. Launch or stop ks ceh (1/0).\n"
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
//Исправлено: убрана ф-ия StreamInit ввиду отсутствия необходимости
template <typename T>
T GetCorrectNumber(T min, T max, string Message)
{
	T vvod;
	cout << Message;
	while ((cin >> vvod).fail() || (vvod <= min) || (vvod>max) )
	{
		cin.clear();
		cin.ignore(10000, '\n');
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
	//исправлено: игнорируется только символ новой строки для getline
	cin.ignore(1, '\n');
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
//изменено: чтение и запись ифнормации и о трубе, и о кс из одного и того же файла
//исправлено: чтение из того же файла, в который записали
void LoadPipeKs(Pipeline& p, Ks& k, bool& PipeInformation, bool& KsInformation)
{
	ifstream fin;
	fin.open("File.txt", ios::in);
	if (fin.is_open())
	{
		fin >> p.id >> p.diameter >> p.length >> p.repear;
		fin >> k.id;
		//добавлено-чтение строки с пробелом из файла
		fin.ignore(1, '\n');
		getline(fin, k.name);
		fin >> k.ceh >> k.cehwork >> k.effective;
		fin.close();
		PipeInformation = KsInformation = true;
	}
	else 
	{
		cout << "An error occurred while reading the file";
		PipeInformation = KsInformation = false;
	}
}

void SavePipeKs(const Pipeline& p, const Ks& k)
{
	ofstream fout;
    fout.open("File.txt", ios::out);
	if (fout.is_open())
	{
		fout << p.id << endl << p.diameter << endl << p.length << endl << p.repear << endl;
		fout << k.id << endl << k.name << endl << k.ceh << endl << k.cehwork << endl << k.effective;
		fout.close();
	}
	else cout << "An error occurred while writing the file";
}
int main()
{
	Pipeline p;
	Ks k; 
	bool PipeInformation = false;
	bool KsInformation = false;
	for ( ; ; ) {
		PrintMenu();
		switch (GetCorrectNumber(-1, 8, "Please, select a number from 0 to 8.\n"))
		{
		case 1: p = InputPipeline(); PipeInformation = true;
			break;
		case 2: k = InputKs(); KsInformation = true;
			break;
		case 3: LoadPipeKs(p, k, PipeInformation, KsInformation);
			break;
		case 4: if (PipeInformation) PrintPipeline(p);
			  else cout << "Pipe information has not been received yet. Input the data by selecting 1 or 3 points.\n ";
			  break;
		case 5: if (KsInformation) PrintKs(k);
			  else cout << "Ks information has not been received yet. Input the data by selecting 2 or 3 points.\n ";
			  break;
		case 6:
			if (PipeInformation && KsInformation) SavePipeKs(p, k);
			else if (!PipeInformation) cout << "Pipe information has not been received yet. Input the data by selecting 1 or 3 points.\n ";
			else cout << "Ks information has not been received yet. Input the data by selecting 2 or 3 points.\n ";
			break;
		case 7: EditPipeline(p);
			break;
		case 8:
			EditKs(k, GetCorrectNumber(-1, 1, "Please, input 1 for launch or 0 for stop: "), k.ceh);
			break;
		 case 0:
			return 0;
			break;
		}

	}

}


