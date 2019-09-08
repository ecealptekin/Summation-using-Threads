#include <iostream>
#include <string>
#include <fstream>
#include <thread>
#include <mutex>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <random>
#include <time.h>
#include <sstream>

using namespace std;

//Ece Alptekin
//24156
//Homework 7 – Summation using Threads

mutex MyMutex;
int sum = 0; //global variable for the sum
int countind = 0; //global variable for the array index

void GetInput(int * &DynArray, int & min, int & max,int & size)
{ //this function opens the file and then creates a dynamic array 
	ifstream in;
	string filename;
	cout << "Please enter the file name." << endl;
	cin >> filename;
	in.open(filename);

	while(in.fail())
	{
	  cout << "Please enter the file name." << endl;
	  cin >> filename;
	  in.open(filename);
	}

	cout << "Please enter the wait range of threads." << endl;
	cin >> min >> max;

	in >> size;
	DynArray = new int[size];

	int temp;
	for(int i=0; i<size; i++)
	{
		in >> temp;
		DynArray[i] = temp;
	}
}

void DisplayTime()
{
	time_t tt = chrono::system_clock::to_time_t(chrono::system_clock::now()); 
	struct tm *ptm = new struct tm; 
	localtime_s(ptm, &tt); 
	 
	cout << "\n";
    cout << "Starting reading the array at " << put_time(ptm,"%X") << endl;
    cout << "Array stored in the memory. Starting the summation at " << put_time(ptm,"%X") << endl;
	cout << "\n";
}

int random_range(const int & min, const int & max) 
{
static mt19937 generator(time(0));
uniform_int_distribution<int> distribution(min, max);
return distribution(generator);
}

void increment(const int & minimum, const int & maximum,int id,const int & size,int * DynArray)
{ //thread function
	while(countind < size)
	{
	int time_in_seconds = random_range(minimum,maximum);
	this_thread::sleep_for(chrono::seconds(time_in_seconds));
	
	MyMutex.lock();
	if(countind < size)
	{
	sum = sum + DynArray[countind]; //adds the element to the sum

	ostringstream output; 
	time_t tt = chrono::system_clock::to_time_t(chrono::system_clock::now()); 
	struct tm *ptm = new struct tm; 
	localtime_s(ptm, &tt); 

	output << "Thread " <<id<< " added number index " << countind << " to the total sum at " << put_time(ptm,"%X") << endl << "Current sum: " << sum << endl;
	cout << output.str();

	countind++; //increases the index of the array
   
	MyMutex.unlock();
	}

	else
	{
		MyMutex.unlock();
	}

  }
}

void Print()
{
	time_t tt = chrono::system_clock::to_time_t(chrono::system_clock::now()); //displays current time
	struct tm *ptm = new struct tm; 
	localtime_s(ptm, &tt); 

	cout << "\n";
	cout << "Adding finished at " << put_time(ptm,"%X") << endl;
    cout << "Sum: " << sum << endl;
}

int main()
{
	int * DynArray;
	int minimum,maximum,size;
	GetInput(DynArray,minimum,maximum,size);
	DisplayTime();

	thread thr1(increment,minimum,maximum,1,size,DynArray);
	thread thr2(increment,minimum,maximum,2,size,DynArray);
	thread thr3(increment,minimum,maximum,3,size,DynArray);
	thread thr4(increment,minimum,maximum,4,size,DynArray);
	thread thr5(increment,minimum,maximum,5,size,DynArray);

	thr1.join();
	thr2.join();
	thr3.join();
	thr4.join();
	thr5.join();

	Print();

	delete DynArray; //deallocate dynamic memory

	cin.get();
	cin.ignore();
	return 0;
}