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
	string tempString = "";

	
	NUM_OF_THREADS = atoi(argv[3]);
	string inputFile = argv[1];
	string outputFile = argv[2];

	try
	{



		ifstream in(/*"PPM/199624.ppm"*/inputFile, ios::binary);
		size = in.seekg(0, ios::end).tellg();
		if (size == -1)
			throw "File is empty";
		in.seekg(0);
		buf = new char[size + 1];
		in.read(buf, size);
		buf[size] = 0;
		bufIterator = buf;
		in.close();
		
	}
	catch (const char* msg)
	{
		std::cout << msg << std::endl;
		if(buf!= NULL)
		delete buf;
		return 1;
	}
	catch (...)
	{
		std::cout << "Unknown error" << std::endl;
		if (buf != NULL)
			delete buf;
		return 1;
	}
		
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

			start_time = omp_get_wtime();
			BrightnessConvertionPPMMultithread(dataArray, numbers);
			end_time = omp_get_wtime();
			auto timeMulti = end_time - start_time;
			printf("\nTime (%i thread(s)): %f ms\n", NUM_OF_THREADS, timeMulti * 1000);
		}
	}

	

	for (size_t i = 0; i < numbers; i++)
	{
		char val = (char)dataArray[i];
		buf[i + index] = val;
	}
		
		fstream bin(outputFile, ios::out | ios::binary);
		bin.write(buf, sizeof(char)*size);
		bin.close();

		delete buf;
		free(dataArray);


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

#pragma omp for schedule(static)
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

#pragma omp for schedule(static)
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


	free(dataArrayInt);
}

void BrightnessConvertionPPMMultithread(unsigned int* dataArray, int size)
{
	auto sizeOfComponentArray = size / 3;

	int percent = round((size * 0.39) / 100);
	int roundPercent = round(percent / 3);

	int minR = 256;
	int maxR = 0;
	int minG = 256;
	int maxG = 0;
	int minB = 256;
	int maxB = 0;

	int minGlobal = 256;
	int maxGlobal = -1;


	int iterator = 0;
	for (size_t i = 0; i < size; i += 3)
	{

		if (minR > dataArray[i])
			minR = dataArray[i];

		if (minG > dataArray[i + 1])
			minG = dataArray[i + 1];

		if (minB > dataArray[i + 2])
			minB = dataArray[i + 2];

		if (maxR < dataArray[i])
			maxR = dataArray[i];

		if (maxG < dataArray[i + 1])
			maxG = dataArray[i + 1];

		if (maxB < dataArray[i + 2])
			maxB = dataArray[i + 2];



		iterator++;
	}

	int minArr[3] = { minR, minG, minB };
	int maxArr[3] = { maxR, maxG, maxB };


	if (percent >= 1) {

		for (size_t i = 0; i < 3; i++)
		{
			if (minGlobal > minArr[i]) {
				minGlobal = minArr[i];
			}
			if (maxGlobal < maxArr[i]) {
				maxGlobal = maxArr[i];
			}
		}

		int i = 0;

#pragma omp parallel
		{

			#pragma omp for schedule(static)
			for (i = 0; i < size; i++)
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

		}
	}
	else {

		for (size_t i = 0; i < 3; i++)
		{
			if (minGlobal > minArr[i]) {
				minGlobal = minArr[i];
			}
			if (maxGlobal < maxArr[i]) {
				maxGlobal = maxArr[i];
			}
		}

#pragma omp parallel
		{
			int i = 0;
			#pragma omp for schedule(static)
			for (i = 0; i < size; i++)
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

		}

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


	free(dataArrayInt);

}

void BrightnessConvertionPPMSinglethread(unsigned int* dataArray, int size)
{
	auto sizeOfComponentArray = size / 3;

	int percent = round((size * 0.39) / 100);
	int roundPercent = round(percent/3);

	int minR = 256;
	int maxR = 0;
	int minG = 256;
	int maxG = 0;
	int minB = 256;
	int maxB = 0;

	int minGlobal = 256;
	int maxGlobal = -1;

	int iterator = 0;
	for (size_t i = 0; i < size; i+=3)
	{
		
		if (minR > dataArray[i])
			minR = dataArray[i];

		if (minG > dataArray[i+1])
			minG = dataArray[i+1];

		if (minB > dataArray[i+2])
			minB = dataArray[i+2];

		if (maxR < dataArray[i])
			maxR = dataArray[i];

		if (maxG < dataArray[i + 1])
			maxG = dataArray[i+1];

		if (maxB < dataArray[i + 2])
			maxB = dataArray[i + 2];


		iterator++;
	}
	


	if (percent >= 1) {

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

	}
	else {

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
