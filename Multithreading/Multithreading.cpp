// Multithreading.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <thread>
#include <chrono>
#include <iostream>
#include <ctime>
#include <fstream>

using namespace std;
using namespace std::chrono;

//Простое перемножение, по определению
void simple_multiplication(int** A, int** B, int** result, int size)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{//Пробег по каждой паре ij
			result[i][j] = 0;
			for (int k = 0; k < size; k++)
				result[i][j] += A[i][k] * B[k][j];
		}
	}
	return;
}

void fill(int** A, int size)
{
	srand(time(0));
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			A[i][j] = rand() % 100;
}

void thread_calculate(int** A, int** B, int** result, int Lborder, int Rborder, int size)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = Lborder-1; j < Rborder; j++)
		{//Пробег по каждой паре ij в пределах границы по столбцам
			result[i][j] = 0;
			for (int k = 0; k < size; k++)
				result[i][j] += A[i][k] * B[k][j];
		}
	}
	return;
}

int main()
{
	int SIZE = 1000;



	//Каждый поток будет заполнять свои ряды в получившейся матрице.
	int** A = new int*[SIZE];
	for (int i = 0; i < SIZE; i++)
		A[i] = new int[SIZE];

	int** B = new int*[SIZE];
	for (int i = 0; i < SIZE; i++)
		B[i] = new int[SIZE];

	int** C = new int*[SIZE];
	for (int i = 0; i < SIZE; i++)
		C[i] = new int[SIZE];


	fill(A, SIZE);
	fill(B, SIZE);

	int num_threads = 0,j=0;
	cout << "Enter maximum number of threads (takes time): ";
	cin >> j;
	

	//Тут прогнать несколько раз для кол-ва потоков
	for (int num_threads = 1; num_threads <=j; num_threads++)
	{
		int start = clock();
		int rows_per_thread = SIZE / num_threads; //Деление нацело.
		thread* thr = new thread[num_threads];
		//Для всех потоков кроме последнего
		int rows_from = 1;
		for (int i = 0; i < num_threads - 1; i++)
		{
			thr[i] = thread(thread_calculate, A, B, C, rows_from, rows_from + rows_per_thread - 1, SIZE);
			rows_from += (rows_per_thread + 1);
			thr[i].join();

		}
		//Для посленнего потока
		thr[num_threads - 1] = thread(thread_calculate, A, B, C, rows_from, SIZE, SIZE);
		thr[num_threads - 1].join();
		int end = clock();
		ofstream file("output.txt", ios_base::app);
		file << num_threads << " " << end - start << endl;
		for (int i = 0; i < num_threads; i++)
			thr[i].~thread();
		delete [] thr;
	}
    return 0;
}

