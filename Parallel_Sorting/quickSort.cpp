#include<stdio.h>
#include<omp.h>
#include<stdio.h>
#include<time.h>
#include <cstdlib>
#include <iostream>
#include <stdlib.h>
#include <stdint.h>
#include <string>
#include <fstream>
#include <cassert>

#define SIZE 100000

using namespace std;


void Read_Inp(unsigned long int * inp_arr)
{
      string line;
      ifstream inp_file("inp.txt");
      unsigned long int idx=0;
      cout<<"Begin to read..."<<endl;	
      if (inp_file.is_open())
      {
         while (getline(inp_file,line) && idx <SIZE)
         {
            inp_arr[idx] = strtoul(line.c_str(), NULL, 10);
            idx++;
         }
         inp_file.close();
      }	
}


void swap(long int& a, long int& b)
{
    long int t = a;
    a = b;
    b = t;
}
 

long int partition (unsigned long int* arr, long int low, long int high)
{

    unsigned long int pivot = arr[high];
    long int i = (low - 1);
 	
    for (long int j = low; j <= high- 1; j++)
    {
        
        if (arr[j] <= pivot)
        {
	    i++;
            swap(arr[i], arr[j]);	
        }
    }
    swap(arr[i + 1], arr[high]);
    return (i + 1);
}


void quickSort(unsigned long int* arr, long int low, long int high)
{	
    if (low < high)
    {
      
        long int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}


void quickSort_parallel(unsigned long int* arr, long int low, long int high)
{
    if (low < high)
    {
      
        long int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    
        if((high-low)<1000)
        {   
           quickSort(arr,low,pi - 1);
           quickSort(arr,pi + 1,high);
        }
        else
        {
	   #pragma omp task
	   quickSort_parallel(arr,low,pi-1);
	   #pragma omp task
	   quickSort_parallel(arr,pi+1,high);
	   #pragma omp taskwait
        }

    }

}


void Check_Par(unsigned long int* inp_arr)
{
	#pragma omp parallel for
	for (long int i=0; i <SIZE-1; i++)
	{
		assert(inp_arr[i] <= inp_arr[i+1]);	
	}
}


int main()
{
    cout << "Problem size is " << SIZE << endl;
        unsigned long int * num = new unsigned long int[SIZE];
	Read_Inp(num);
	
#pragma omp parallel
        #pragma omp single
        quickSort(num,0,SIZE-1);
	
        Check_Par(num);
    //for(long int i=0;i<SIZE;i++)
      //{
          //cout<<num[i]<<" ";
      //}
	
	return 0;
}
