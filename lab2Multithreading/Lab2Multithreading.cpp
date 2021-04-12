#include <iostream>
#include <fstream>
#include <string>
#include <omp.h>
#include "conio.h"
using namespace std;

enum FileFormat
{
	PGM,
	PPM

};

void qSort(int* a, int l, int r);
void BrightnessConvertionPGMSinglethread(unsigned int* dataArray, int size);

int main(int argc, char** argv)
{
	

	char* bufIterator = NULL;
	char* buf = NULL;
	char* header = NULL;
	float** table = NULL;
	int NUM_OF_THREADS = 0;
	FileFormat InputFileFormat;//P5 or P6
	int width = 0;
	int height = 0;
	int max = 0;
	unsigned int* dataArray = NULL;
	unsigned char* pixels = NULL;


	//string sNUM_OF_THREADS = argv[2];
	//NUM_OF_THREADS = atoi(argv[2]);
	//string file = argv[1];
	ifstream in("PGM/Warm-and-Cool-Colors_20x20_8x8.pgm"/*argv[1]*/, ios::binary);//matrix.txt
	int size = in.seekg(0, ios::end).tellg();
	if (size == -1)
		throw "File is empty";
	in.seekg(0);
	buf = new char[size + 1];
	in.read(buf, size);
	buf[size] = 0;
	bufIterator = buf;
	in.close();
	string tempString = "";
	pixels =  (unsigned char*)buf;
	int index = 0;//индекс с которого начинаются данные изображения
	int numbers = 0;//кол-во данных для обработки
	
	printf("\n%s", buf);



	string s1(1, buf[0]);
	string s2(1, buf[1]);
	string concatenate = s1 + s2;

	if (concatenate == "P5") {
		InputFileFormat = PGM;
	}
	else if (concatenate == "P6") {
		InputFileFormat = PPM;
	}

	bufIterator += 3;//переходим через 2 символа формата и 1 \n
	if ((int)*bufIterator == 35) {//проверка на комментарии, если есть пропускаем
		bufIterator++;
		while ((int)*bufIterator != 35) {
			bufIterator++;
		}
		bufIterator++;//переходим через #
		bufIterator++;//переходим через \n
	}


	while ((int)*bufIterator != 32) {//пока не найдем пробел считываем длину
		tempString += *bufIterator;
		bufIterator++;
	}
	width = stoi(tempString);
	tempString = "";

	bufIterator++;
	while ((int)*bufIterator != 10) {//пока не найдем \n считываем длину
		tempString += *bufIterator;
		bufIterator++;
	}
	height = stoi(tempString);
	tempString = "";

	bufIterator++;
	while ((int)*bufIterator != 10) {//пока не найдем \n считываем длину
		tempString += *bufIterator;
		bufIterator++;
	}
	max = stoi(tempString);
	bufIterator++;//тут начинаются данные файла

	if (InputFileFormat == PGM) 
	{
		index = bufIterator - buf;

		numbers = size - index;
		dataArray = (unsigned int*)calloc((numbers), sizeof(int));
	
		for (size_t i = 0; i < numbers; i++)
		{
			dataArray[i] = pixels[index + i];
		}

		/*for (size_t i = 0; i < numbers; i++)
		{
			printf("\n%d", dataArray[i]);
		}*/
	
		BrightnessConvertionPGMSinglethread(dataArray, numbers);
	}
	else {

		int index = bufIterator - buf;

		auto numbers = size - index;
		dataArray = (unsigned int*)calloc((numbers), sizeof(int));

		for (size_t i = 0; i < numbers; i++)
		{
			dataArray[i] = pixels[index + i];
		}

		/*for (size_t i = 0; i < numbers; i++)
		{
			printf("\n%d", dataArray[i]);
		}*/
	}

	for (size_t i = 0; i < numbers; i++)
	{
		char val = (char)dataArray[i];
		buf[i + index] = val;
	}

	
		printf("\n%s", buf);

		
		fstream bin("bin.txt", ios::out | ios::binary);
		bin.write(buf, sizeof(char)*size);
		bin.close();

	return 0;
}



void BrightnessConvertionPGMSinglethread(unsigned int* dataArray, int size) 
{
	double percent = (size * 0.39)/100;
	int roundPercent = round(percent);
	int* dataArrayInt = (int*)calloc(size, sizeof(int));
	memcpy(dataArrayInt, dataArray, size*sizeof(int));
	int min = 0;
	int max = 0;

	cout << endl;
	for (int i = 0; i < size; i++) cout << dataArrayInt[i] << " ";
	cout << endl;
	cout << endl;
	for (int i = 0; i < size; i++) cout << dataArray[i] << " ";
	cout << endl;


	if (percent >= 1) {
		qSort(dataArrayInt, 0, size - 1);
		min = dataArrayInt[0 + roundPercent];
		max = dataArrayInt[size - 1 - roundPercent];
		/*cout << endl;
		for (int i = 0; i < size; i++) cout << dataArrayInt[i] << " ";
		cout << endl;*/

		for (size_t i = 0; i < size; i++)
		{
			int y = round((dataArray[i] - min) * 255 / (max - min));
			if (y < 0) {
				y = 0;
			}
			else if (y > 255) {
				y = 255;
			}
			dataArray[i] = y;

		}
		
	}
	else {
		qSort(dataArrayInt, 0, size - 1);
		min = dataArrayInt[0];
		max = dataArrayInt[size-1];
		
		cout << endl;
		for (int i = 0; i < size; i++) cout << dataArray[i] << " ";
		cout << endl;

		for (size_t i = 0; i < size; i++)
		{
			int y = round((dataArray[i] - min) * 255 / (max - min));
			if (y < 0) {
				y = 0;
			}
			else if (y > 255) {
				y = 255;
			}
			dataArray[i] = y;

		}

		cout << endl;
		for (int i = 0; i < size; i++) cout << dataArray[i] << " ";
		cout << endl;

	}

}

void BrightnessConvertionPPMSinglethread()
{

}

void qSort(int* a, int l, int r)
{
	int i = l, j = r, m = a[(l + r) / 2];
	do
	{
		for (; a[i] < m; i++);
		for (; a[j] > m; j--);
		if (i <= j) swap(a[i++], a[j--]);
	} while (i <= j);
	if (l < j) qSort(a, l, j);
	if (i < r) qSort(a, i, r);
}
