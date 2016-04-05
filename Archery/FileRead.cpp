#include "FileRead.h"


template <typename T> void FileRead::ReadFromFile(vector<T*> &item, string textfile)
{
	int count = 0;
	float mid1, mid2, las;
	string first, middleOne, middleTwo, last;
	
	ifstream reader;
	reader.open(textfile);

	char output;
	
	if (reader.is_open())
	{
		while (!reader.eof())
		{
			reader >> output;

			if(output == '#')
			{
				getline(reader, first,	   ',');
				getline(reader, middleOne, ',');
				getline(reader, middleTwo, ',');
				getline(reader, last);

				count++;

				cout << first << " -" << middleOne << "kg -" << middleTwo << "efficiency faktor -" << last << "spring constant" << endl;

				stringstream ss(stringstream::in | stringstream::out);
				ss << middleOne << middleTwo << last;

				ss >> mid1;
				ss >> mid2;
				ss >> las;

				item.resize(count);

				item[count - 1] = new T (first, mid1, mid2, las);
				
				
				cout << endl;
			}
			else
				getline(reader, first);  // if the line doesn't begin with the expected symbol, the line is thrown away
		}
	}
	reader.close();
}


void FileRead::Read(vector<Bow*> &bows, vector<Arrow*> &arrows)
{	
	ReadFromFile(bows,"Bows.txt");
	ReadFromFile(arrows,"Arrows.txt");
}
