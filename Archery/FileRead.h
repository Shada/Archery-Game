#include "stdafx.h"
#include "Bow.h"
#include "Arrow.h"

class FileRead
{
private:
	template <typename T> static void ReadFromFile(vector<T*> &item, string textfile);
public:
	static void Read(vector<Bow*> &bows, vector<Arrow*> &arrows);
};