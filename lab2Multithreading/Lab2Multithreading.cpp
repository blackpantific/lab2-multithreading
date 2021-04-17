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
void BrightnessConvertionPPMSinglethread(unsigned int* dataArray, int size);
void BrightnessConvertionPGMMultithread(unsigned int* dataArray, int size);
void BrightnessConvertionPPMMultithread(unsigned int* dataArray, int size);

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
	int index = 0;//индекс с которого начинаются данные изображения
	int numbers = 0;//кол-во данных для обработки
	int size = 0;

	//string sNUM_OF_THREADS = argv[2];
	//NUM_OF_THREADS = atoi(argv[2]);
	//string file = argv[1];

	try {

	ifstream in("PGM/199624.pgm"/*argv[1]*/, ios::binary);
	size = in.seekg(0, ios::end).tellg();
	if (size == -1)
		throw "File is empty";
	in.seekg(0);
	buf = new char[size + 1];
	in.read(buf, size);
	buf[size] = 0;
	bufIterator = buf;
	in.close();
	string tempString = "";
	pixels = (unsigned char*)buf;
	

	string s1(1, buf[0]);
	string s2(1, buf[1]);
	string concatenate = s1 + s2;

	if (concatenate == "P5") {
		InputFileFormat = PGM;
	}
	else if (concatenate == "P6") {
		InputFileFormat = PPM;
	}
	else {
		throw "Not a valid file";
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


	}
	catch (const char* msg) {
		std::cout << msg << std::endl;

	}
	catch (...) {
		std::cout << "Unknown error" << std::endl;
	}



	float start_time, end_time;

	if (NUM_OF_THREADS == -1) {
		if (InputFileFormat == PGM)
		{
			index = bufIterator - buf;

			numbers = size - index;
			dataArray = (unsigned int*)calloc((numbers), sizeof(int));

			for (size_t i = 0; i < numbers; i++)
			{
				dataArray[i] = pixels[index + i];
			}
			start_time = omp_get_wtime();
			BrightnessConvertionPGMSinglethread(dataArray, numbers);
			end_time = omp_get_wtime();
			auto timeSingle = end_time - start_time;

			printf("\nTime (%i thread(s)): %f ms\n", NUM_OF_THREADS, timeSingle*1000);
		}
		else {

			index = bufIterator - buf;

			numbers = size - index;
			dataArray = (unsigned int*)calloc((numbers), sizeof(int));

			for (size_t i = 0; i < numbers; i++)
			{
				dataArray[i] = pixels[index + i];
			}
			start_time = omp_get_wtime();
			BrightnessConvertionPPMSinglethread(dataArray, numbers);
			end_time = omp_get_wtime();
			auto timeSingle = end_time - start_time;

			printf("\nTime (%i thread(s)): %f ms\n", NUM_OF_THREADS, timeSingle*1000);
		}
	}
	else if (NUM_OF_THREADS == 0) {
		if (InputFileFormat == PGM)
		{
			index = bufIterator - buf;

			numbers = size - index;
			dataArray = (unsigned int*)calloc((numbers), sizeof(int));

			for (size_t i = 0; i < numbers; i++)
			{
				dataArray[i] = pixels[index + i];
			}
			start_time = omp_get_wtime();
			BrightnessConvertionPGMMultithread(dataArray, numbers);
			end_time = omp_get_wtime();
			auto timeMulti = end_time - start_time;
			printf("\nTime (%i thread(s)): %f ms\n", NUM_OF_THREADS, timeMulti*1000);
		}
		else {

			index = bufIterator - buf;

			numbers = size - index;
			dataArray = (unsigned int*)calloc((numbers), sizeof(int));

			for (size_t i = 0; i < numbers; i++)
			{
				dataArray[i] = pixels[index + i];
			}

			start_time = omp_get_wtime();
			BrightnessConvertionPPMMultithread(dataArray, numbers);
			end_time = omp_get_wtime();
			auto timeMulti = end_time - start_time;
			printf("\nTime (%i thread(s)): %f ms\n", NUM_OF_THREADS, timeMulti*1000);
		}
	}
	else if (NUM_OF_THREADS > 0) {

		omp_set_num_threads(NUM_OF_THREADS);

		if (InputFileFormat == PGM)
		{
			index = bufIterator - buf;

			numbers = size - index;
			dataArray = (unsigned int*)calloc((numbers), sizeof(int));

			for (size_t i = 0; i < numbers; i++)
			{
				dataArray[i] = pixels[index + i];
			}

			start_time = omp_get_wtime();
			BrightnessConvertionPGMMultithread(dataArray, numbers);
			end_time = omp_get_wtime();
			auto timeMulti = end_time - start_time;
			printf("\nTime (%i thread(s)): %f ms\n", NUM_OF_THREADS, timeMulti*1000);
		}
		else {

			index = bufIterator - buf;

			numbers = size - index;
			dataArray = (unsigned int*)calloc((numbers), sizeof(int));

			for (size_t i = 0; i < numbers; i++)
			{
				dataArray[i] = pixels[index + i];
			}

			BrightnessConvertionPPMMultithread(dataArray, numbers);
		}
	}

	

	for (size_t i = 0; i < numbers; i++)
	{
		char val = (char)dataArray[i];
		buf[i + index] = val;
	}
		
		fstream bin("bin.txt", ios::out | ios::binary);
		bin.write(buf, sizeof(char)*size);
		bin.close();

	return 0;
}



void BrightnessConvertionPGMMultithread(unsigned int* dataArray, int size) {

	double percent = (size * 0.39) / 100;
	int roundPercent = round(percent);
	int* dataArrayInt = (int*)calloc(size, sizeof(int));
	memcpy(dataArrayInt, dataArray, size * sizeof(int));
	int min = 0;
	int max = 0;

	if (percent >= 1) {
		qSort(dataArrayInt, 0, size - 1);
		min = dataArrayInt[0 + roundPercent/2];
		max = dataArrayInt[size - 1 - roundPercent/2];
		
		

#pragma omp parallel
		{

		int i = 0;

#pragma omp for schedule(static, 1)
		for (i = 0; i < size; i++)
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

	}
	else {
		qSort(dataArrayInt, 0, size - 1);
		min = dataArrayInt[0];
		max = dataArrayInt[size - 1];


#pragma omp parallel
		{
			int i = 0;

#pragma omp for schedule(static, 1)
			for (i = 0; i < size; i++)
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

	}

}

void BrightnessConvertionPPMMultithread(unsigned int* dataArray, int size)
{
	auto sizeOfComponentArray = size / 3;

	int percent = round((size * 0.39) / 100);
	int roundPercent = round(percent / 3);
	int* dataArrayIntR = (int*)calloc(sizeOfComponentArray, sizeof(int));
	int* dataArrayIntG = (int*)calloc(sizeOfComponentArray, sizeof(int));
	int* dataArrayIntB = (int*)calloc(sizeOfComponentArray, sizeof(int));


	/*cout << endl;
	for (int i = 0; i < size; i++) cout << dataArray[i] << " ";
	cout << endl;
	cout << endl;*/

	int iterator = 0;
	for (size_t i = 0; i < size; i += 3)
	{
		dataArrayIntR[iterator] = dataArray[i];
		//cout << dataArrayIntR[iterator] << " ";
		dataArrayIntG[iterator] = dataArray[i + 1];
		//cout << dataArrayIntG[iterator] << " ";
		dataArrayIntB[iterator] = dataArray[i + 2];
		//cout << dataArrayIntB[iterator] << " ";

		iterator++;
	}

	int minR = 0;
	int maxR = 0;
	int minG = 0;
	int maxG = 0;
	int minB = 0;
	int maxB = 0;

	int minGlobal = -1;
	int maxGlobal = 256;



	//cout << endl;
	//cout << endl;
	//for (int i = 0; i < sizeOfComponentArray; i++) cout << dataArrayIntR[i] << " ";
	//cout << endl;
	//cout << endl;
	//for (int i = 0; i < sizeOfComponentArray; i++) cout << dataArrayIntG[i] << " ";
	//cout << endl;
	//cout << endl;
	//for (int i = 0; i < sizeOfComponentArray; i++) cout << dataArrayIntB[i] << " ";
	//cout << endl;


	if (percent >= 1) {

		qSort(dataArrayIntR, 0, sizeOfComponentArray - 1);
		minR = dataArrayIntR[0 + roundPercent];
		maxR = dataArrayIntR[sizeOfComponentArray - 1 - roundPercent];
		/*cout << endl;
		for (int i = 0; i < sizeOfComponentArray; i++) cout << dataArrayIntR[i] << " ";
		cout << endl;*/

		qSort(dataArrayIntG, 0, sizeOfComponentArray - 1);
		minG = dataArrayIntG[0 + roundPercent];
		maxG = dataArrayIntG[sizeOfComponentArray - 1 - roundPercent];
		/*cout << endl;
		for (int i = 0; i < sizeOfComponentArray; i++) cout << dataArrayIntG[i] << " ";
		cout << endl;*/

		qSort(dataArrayIntB, 0, sizeOfComponentArray - 1);
		minB = dataArrayIntB[0 + roundPercent];
		maxB = dataArrayIntB[sizeOfComponentArray - 1 - roundPercent];
		/*cout << endl;
		for (int i = 0; i < sizeOfComponentArray; i++) cout << dataArrayIntB[i] << " ";
		cout << endl;*/

		int minArr[3] = { minR, minG, minB };
		int maxArr[3] = { maxR, maxG, maxB };

		for (size_t i = 0; i < 3; i++)
		{
			if (minGlobal < minArr[i]) {
				minGlobal = minArr[i];
			}
			if (maxGlobal > maxArr[i]) {
				maxGlobal = maxArr[i];
			}
		}


		/*minGlobal = round((minR + minG + minB) / 3);
		maxGlobal = round((maxR + maxG + maxB) / 3);*/

		cout << endl;
		for (int i = 0; i < 100; i++) cout << dataArray[i] << " ";
		cout << endl;

		for (size_t i = 0; i < size; i++)
		{
			int y = round((dataArray[i] - minGlobal) * 255 / (maxGlobal - minGlobal));
			if (y < 0) {
				y = 0;
			}
			else if (y > 255) {
				y = 255;
			}
			dataArray[i] = y;

		}

		cout << endl;
		for (int i = 0; i < 100; i++) cout << dataArray[i] << " ";
		cout << endl;


	}
	else {
		qSort(dataArrayIntR, 0, sizeOfComponentArray - 1);
		minR = dataArrayIntR[0];
		maxR = dataArrayIntR[sizeOfComponentArray - 1];
		cout << endl;
		for (int i = 0; i < sizeOfComponentArray; i++) cout << dataArrayIntR[i] << " ";
		cout << endl;

		qSort(dataArrayIntG, 0, sizeOfComponentArray - 1);
		minG = dataArrayIntG[0];
		maxG = dataArrayIntG[sizeOfComponentArray - 1];
		cout << endl;
		for (int i = 0; i < sizeOfComponentArray; i++) cout << dataArrayIntG[i] << " ";
		cout << endl;

		qSort(dataArrayIntB, 0, sizeOfComponentArray - 1);
		minB = dataArrayIntB[0];
		maxB = dataArrayIntB[sizeOfComponentArray - 1];
		cout << endl;
		for (int i = 0; i < sizeOfComponentArray; i++) cout << dataArrayIntB[i] << " ";
		cout << endl;

		int minArr[3] = { minR, minG, minB };
		int maxArr[3] = { maxR, maxG, maxB };

		for (size_t i = 0; i < 3; i++)
		{
			if (minGlobal > minArr[i]) {
				minGlobal = minArr[i];
			}
			if (maxGlobal < maxArr[i]) {
				maxGlobal = maxArr[i];
			}
		}


		cout << endl;
		for (int i = 0; i < size; i++) cout << dataArray[i] << " ";
		cout << endl;

		for (size_t i = 0; i < size; i++)
		{
			int y = round((dataArray[i] - minGlobal) * 255 / (maxGlobal - minGlobal));
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

void BrightnessConvertionPGMSinglethread(unsigned int* dataArray, int size) 
{
	double percent = (size * 0.39)/100;
	int roundPercent = round(percent);
	int* dataArrayInt = (int*)calloc(size, sizeof(int));
	memcpy(dataArrayInt, dataArray, size*sizeof(int));
	int min = 0;
	int max = 0;

	if (percent >= 1) {
		qSort(dataArrayInt, 0, size - 1);
		min = dataArrayInt[0 + roundPercent/2];
		max = dataArrayInt[size - 1 - roundPercent/2];

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

}

void BrightnessConvertionPPMSinglethread(unsigned int* dataArray, int size)
{
	auto sizeOfComponentArray = size / 3;

	int percent = round((size * 0.39) / 100);
	int roundPercent = round(percent/3);
	int* dataArrayIntR = (int*)calloc(sizeOfComponentArray, sizeof(int));
	int* dataArrayIntG = (int*)calloc(sizeOfComponentArray, sizeof(int));
	int* dataArrayIntB = (int*)calloc(sizeOfComponentArray, sizeof(int));
	
	
	/*cout << endl;
	for (int i = 0; i < size; i++) cout << dataArray[i] << " ";
	cout << endl;
	cout << endl;*/

	int iterator = 0;
	for (size_t i = 0; i < size; i+=3)
	{
		dataArrayIntR[iterator] = dataArray[i];
		//cout << dataArrayIntR[iterator] << " ";
		dataArrayIntG[iterator] = dataArray[i + 1];
		//cout << dataArrayIntG[iterator] << " ";
		dataArrayIntB[iterator] = dataArray[i + 2];
		//cout << dataArrayIntB[iterator] << " ";

		iterator++;
	}
	
	int minR = 0;
	int maxR = 0;
	int minG = 0;
	int maxG = 0;
	int minB = 0;
	int maxB = 0;

	int minGlobal = -1;
	int maxGlobal = 256;



	//cout << endl;
	//cout << endl;
	//for (int i = 0; i < sizeOfComponentArray; i++) cout << dataArrayIntR[i] << " ";
	//cout << endl;
	//cout << endl;
	//for (int i = 0; i < sizeOfComponentArray; i++) cout << dataArrayIntG[i] << " ";
	//cout << endl;
	//cout << endl;
	//for (int i = 0; i < sizeOfComponentArray; i++) cout << dataArrayIntB[i] << " ";
	//cout << endl;


	if (percent >= 1) {

		qSort(dataArrayIntR, 0, sizeOfComponentArray - 1);
		minR = dataArrayIntR[0 + roundPercent];
		maxR = dataArrayIntR[sizeOfComponentArray - 1 - roundPercent];
		/*cout << endl;
		for (int i = 0; i < sizeOfComponentArray; i++) cout << dataArrayIntR[i] << " ";
		cout << endl;*/

		qSort(dataArrayIntG, 0, sizeOfComponentArray - 1);
		minG = dataArrayIntG[0 + roundPercent];
		maxG = dataArrayIntG[sizeOfComponentArray - 1 - roundPercent];
		/*cout << endl;
		for (int i = 0; i < sizeOfComponentArray; i++) cout << dataArrayIntG[i] << " ";
		cout << endl;*/

		qSort(dataArrayIntB, 0, sizeOfComponentArray - 1);
		minB = dataArrayIntB[0 + roundPercent];
		maxB = dataArrayIntB[sizeOfComponentArray - 1 - roundPercent];
		/*cout << endl;
		for (int i = 0; i < sizeOfComponentArray; i++) cout << dataArrayIntB[i] << " ";
		cout << endl;*/

		int minArr[3] = { minR, minG, minB };
		int maxArr[3] = { maxR, maxG, maxB };

		for (size_t i = 0; i < 3; i++)
		{
			if (minGlobal < minArr[i]) {
				minGlobal = minArr[i];
			}
			if (maxGlobal > maxArr[i]) {
				maxGlobal = maxArr[i];
			}
		}


		/*minGlobal = round((minR + minG + minB) / 3);
		maxGlobal = round((maxR + maxG + maxB) / 3);*/

		cout << endl;
		for (int i = 0; i < 100; i++) cout << dataArray[i] << " ";
		cout << endl;

		for (size_t i = 0; i < size; i++)
		{
			int y = round((dataArray[i] - minGlobal) * 255 / (maxGlobal - minGlobal));
			if (y < 0) {
				y = 0;
			}
			else if (y > 255) {
				y = 255;
			}
			dataArray[i] = y;

		}

		cout << endl;
		for (int i = 0; i < 100; i++) cout << dataArray[i] << " ";
		cout << endl;


	}
	else {
		qSort(dataArrayIntR, 0, sizeOfComponentArray - 1);
		minR = dataArrayIntR[0];
		maxR = dataArrayIntR[sizeOfComponentArray - 1];
		cout << endl;
		for (int i = 0; i < sizeOfComponentArray; i++) cout << dataArrayIntR[i] << " ";
		cout << endl;

		qSort(dataArrayIntG, 0, sizeOfComponentArray - 1);
		minG = dataArrayIntG[0];
		maxG = dataArrayIntG[sizeOfComponentArray - 1];
		cout << endl;
		for (int i = 0; i < sizeOfComponentArray; i++) cout << dataArrayIntG[i] << " ";
		cout << endl;

		qSort(dataArrayIntB, 0, sizeOfComponentArray - 1);
		minB = dataArrayIntB[0];
		maxB = dataArrayIntB[sizeOfComponentArray - 1];
		cout << endl;
		for (int i = 0; i < sizeOfComponentArray; i++) cout << dataArrayIntB[i] << " ";
		cout << endl;

		int minArr[3] = { minR, minG, minB };
		int maxArr[3] = { maxR, maxG, maxB };

		for (size_t i = 0; i < 3; i++)
		{
			if (minGlobal > minArr[i]) {
				minGlobal = minArr[i];
			}
			if (maxGlobal < maxArr[i]) {
				maxGlobal = maxArr[i];
			}
		}


		cout << endl;
		for (int i = 0; i < size; i++) cout << dataArray[i] << " ";
		cout << endl;

		for (size_t i = 0; i < size; i++)
		{
			int y = round((dataArray[i] - minGlobal) * 255 / (maxGlobal - minGlobal));
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
