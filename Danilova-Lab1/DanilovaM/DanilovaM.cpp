#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <vector>
#include "Pipeline.h"
#include "Ks.h"
#include "Utils.h"
using namespace std;

void PrintMenu() 
{
	cout << "\n1. Add pipeline information.\n"                    
		<< "2. Add ks information.\n"                             
		<< "3. Delete pipeline information.\n"                    
		<< "4. Delete ks information.\n"                          
		<< "5. Load pipeline and ks information from file.\n"     
		<< "6. View information about all pipelines.\n"           
		<< "7. View information about all ks.\n"                  
		<< "8. Save all information to file.\n"                   
		<< "9. Launch or stop ks ceh.\n"
		<< "10. Batch editing of pipelines.\n"
		<< "11. Pipelines search by filter.\n"                   
		<< "12. Ks search by filter.\n"                           
		<< "0. Exit.\n";
}
void SavePipeline(ofstream& fout, const Pipeline& p)
{
	fout << p.diameter << endl << p.length << endl << p.repear << endl;
}
Pipeline LoadPipeline(ifstream& fin)
{
	Pipeline p;
	fin >> p.diameter >> p.length >> p.repear;
	return p;
}
void SaveKs(ofstream& fout, const Ks& k)
{
	fout << k.name << endl << k.ceh << endl << k.cehwork << endl << k.effective << endl;
}
Ks LoadKs(ifstream& fin)
{
	Ks k;
	fin.ignore(1, '\n');
	getline(fin, k.name);
	fin >> k.ceh >> k.cehwork >> k.effective;
	return k;
}
string AskingForName()
{
	string filename;
	cout << "\nPlease, enter the name: ";
	cin.ignore(1, '\n');
	getline(cin, filename);
	return filename;
}
void LoadAll(vector<Pipeline>& Pipeline_s, vector <Ks>& Ks_s)
{
		ifstream fin;
		fin.open(AskingForName(), ios::in);
		if (fin.is_open())
		{
			int count_pipelines, count_ks;
			fin >> count_pipelines;
			Pipeline_s.reserve(count_pipelines);
			while (count_pipelines--)
				Pipeline_s.push_back(LoadPipeline(fin));
			fin >> count_ks;
			Ks_s.reserve(count_ks);
			while (count_ks--)
				Ks_s.push_back(LoadKs(fin));
			fin.close();
		}
		else cout << "File with this name does not exist.\n";
}
void SaveAll(const vector<Pipeline>& Pipeline_s, const vector <Ks>& Ks_s)
{
	if ((Pipeline_s.size() == 0) or (Ks_s.size() == 0)) cout << "Pipeline or ks information has not been received yet. Input the data by selecting 1, 2 or 5 points.\n ";
	else
	{
		ofstream fout;
		fout.open(AskingForName(), ios::out);
		if (fout.is_open())
		{
			fout << Pipeline_s.size() << endl;
			for (const auto& p : Pipeline_s) SavePipeline(fout, p);
			fout << Ks_s.size() << endl;
			for (const auto& k : Ks_s) SaveKs(fout, k);
			fout.close();
		}
		else cout << "An error occured while writing the file.";
	}
}

template<class C, typename T>
using Filter = bool(*)(const C& pk, T param);
template<class C>
bool CheckById(const C& pc, unsigned int id)
{
	return pc.id == id;
}
bool CheckByRepear(const Pipeline& p, bool repear)
{
	return p.repear == repear;
}
bool CheckByName(const Ks& k, string name)
{
	return k.name == name;
}
bool CheckBy_Percent_Unused_Ceh(const Ks& k, double percent)
{
	return percent <= (double)(100*(k.ceh-k.cehwork)/k.ceh);
}
template<class C, typename T>
vector<int> FindObjectsByFilter(const vector<C>& PK_s, Filter<C, T> f, T param)
{
	vector <int> res;
	int i = 0;
	for (const auto& pk : PK_s)
	{
		if (f(pk, param))
			res.push_back(i);
		i++;
	}
	if (res.size() == 0) cout << "Object not found.\n";
	return res;
}
void BatchEditPipes(vector<Pipeline>& Pipeline_s)
{
	unsigned int choice;
	while (choice = (unsigned int)GetCorrectNumber(-0.5, DBL_MAX, "\nPlease, select correct id of the pipeline you want to edit; enter 0 to stop and go to menu: "))
	{
		for (int i : FindObjectsByFilter(Pipeline_s, CheckById, choice)) Pipeline_s[i].EditPipeline();
	}
}
void EditAllPipes(vector<Pipeline>& Pipeline_s)
{
	for (int i = 0; i <= Pipeline_s.size() - 1; i++) Pipeline_s[i].EditPipeline();
}
template <class C>
void DelPipeKs(vector<C>& PK_s)
{
	int check = 1;
	while (check)
	{
		for (int i : FindObjectsByFilter(PK_s, CheckById, GetCorrectNumber(0u, UINT_MAX, "\nPlease, select correct id of the ks you want to delete: ")))
		{
			PK_s.erase(PK_s.cbegin() + i);
			check = 0;
		}
	}
}

int main()
{
	vector <Pipeline> Pipeline_s;
	vector <Ks> Ks_s;
	for ( ; ; ) {
		PrintMenu();
		switch (GetCorrectNumber(-1, 12, "Please, select a number from 0 to 12.\n"))
		{
		case 1:
		{
			Pipeline p;
			cin >> p;
			Pipeline_s.push_back(p);
			break;
		}
		case 2: 
		{
			Ks k;
			cin >> k;
			Ks_s.push_back(k);
			break;
		}
		case 3: 
		{
			if (Pipeline_s.size())
			{
				DelPipeKs(Pipeline_s);
			}
			else cout << "Pipe information has not been received yet. Input the data by selecting 1 or 5 points.\n ";
			break;
		}
		case 4: 
		{
			if (Ks_s.size())
			{
				DelPipeKs(Ks_s);
			}
			else cout << "Ks information has not been received yet. Input the data by selecting 2 or 5 points.\n ";
			break;
		}
		case 5: 
		{
			LoadAll(Pipeline_s, Ks_s);
			break;
		}
		case 6:
		{
			if (Pipeline_s.size())
			{ 
				for (const auto& p : Pipeline_s) cout << p << endl; 
			}
			else cout << "Pipe information has not been received yet. Input the data by selecting 1 or 5 points.\n ";
			break;
		}
		case 7: 
 		{
			if (Ks_s.size())
			{
				for (const auto& k : Ks_s) cout << k << endl;
			}
			else cout << "Ks information has not been received yet. Input the data by selecting 2 or 5 points.\n ";
			break;
		}
		case 8:
		{
			SaveAll(Pipeline_s, Ks_s);
			break;
		}
		case 9:
		{
			if (Ks_s.size())
			{
				int check = 1;
				while (check)
				{
					for (int i : FindObjectsByFilter(Ks_s, CheckById, GetCorrectNumber(0u, UINT_MAX, "\nPlease, select correct id of the ks you want to edit: ")))
					{
						Ks_s[i].EditKs(GetCorrectNumber(-1, 1, "Please, input 1 for launch or 0 for stop: "));
						check = 0;
					}
				}
			}
			else cout << "Ks information has not been received yet. Input the data by selecting 2 or 5 points.\n ";
			break;
		}
		case 10:
		{
			if (Pipeline_s.size())
			{
				if (GetCorrectNumber(-1, 1, "Please, input 0 to edit all pipelines or 1 to batch egiting of pipelines: "))
				{
					BatchEditPipes(Pipeline_s);
				}
				else {
					EditAllPipes(Pipeline_s);
				}
			}
			else cout << "Pipe information has not been received yet. Input the data by selecting 1 or 5 points.\n ";
			break;
		}
		case 11:
		{
			if (Pipeline_s.size()) 
			{
				if (GetCorrectNumber(-1, 1, "Please, input 0 for pipeline search by id or 1 for pipeline search by attribute 'in repear': "))
				{
					for (int i : FindObjectsByFilter(Pipeline_s, CheckByRepear, (bool)GetCorrectNumber(-1, 1, "\nPlease, enter 0 to find pipelines not in repear and 1 to find pipelines in repear: ")))
						cout << Pipeline_s[i];
				}
				else
				{
					for (int i : FindObjectsByFilter(Pipeline_s, CheckById, GetCorrectNumber(0u, UINT_MAX, "\nPlease, select correct id of the pipeline you want to find: ")))
						cout << Pipeline_s[i];
				}
			}
			else cout << "Pipe information has not been received yet. Input the data by selecting 1 or 5 points.\n ";
		    break;
		}
		case 12:
		{
			if (Ks_s.size())
			{
				if (GetCorrectNumber(-1, 1, "Please, input 0 for ks search by name or 1 for ks search by the percentage of unused cehworks: "))
				{
					for (int i : FindObjectsByFilter(Ks_s, CheckBy_Percent_Unused_Ceh, (double)GetCorrectNumber(-1, 100, "\nPlease, enter the percentage of unused cehworks(%): ")))
						cout << Ks_s[i];
				}
				else {
					for (int i : FindObjectsByFilter(Ks_s, CheckByName, AskingForName()))
						cout << Ks_s[i];
				}
			}
			else  cout << "Ks information has not been received yet. Input the data by selecting 2 or 5 points.\n ";
			break;
		}
		 case 0:
		 {
			 return 0;
		     break; 
		 }
		}
	}
}