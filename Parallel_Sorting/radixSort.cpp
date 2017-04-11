#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cassert>

#define SIZE 100000

using namespace std;

void Read_Inp(unsigned int * inp_arr)
{
      string line;
      ifstream inp_file("inp.txt");
      unsigned int idx=0;
      if (inp_file.is_open())
      {
         while (getline(inp_file,line))
         {
            inp_arr[idx] = strtoul(line.c_str(), NULL, 10);
            idx++;
         }
         inp_file.close();
      }
}


int getMax(unsigned int *inp_arr, int n)
{
    unsigned int max = inp_arr[0];
    for (int i =1; i<n; i++)
        {
           if (inp_arr[i]>max)
              {
                max = inp_arr[i];
              }
        }
    return max;
}       


void countSort(unsigned int * inp_arr, int n, int digit)
{       
     unsigned int output[n];
     int i;
     int count[10] = {0};
 
     for (i=0; i<n; i++)
        {
           count[(inp_arr[i]/digit)%10]++ ;
        }
     for (i=1; i<10; i++)
        {
           count[i] = count[i]+count[i-1];
        }
     for (i= n-1; i>=0; i--)
        {
           output[count[(inp_arr[i]/digit)%10]-1] = inp_arr[i];
           count[(inp_arr[i]/digit)%10]--;
        }
     for (i=0; i<n; i++)
        {
           inp_arr[i] = output[i];
        }
}


void radixSort(unsigned int * inp_arr, int n)
{        
         int max = getMax(inp_arr, n);
         for (int digit=1; max/digit>0; digit=digit*10)
            {
                countSort(inp_arr, n, digit);
            }
}

int main()
{
     unsigned int *num = new unsigned int[SIZE];
     Read_Inp(num);
     int n= SIZE;
     radixSort(num,n);
     for (int i=0; i<n; i++)
        {
            cout<< num[i]<<" ";
        }
     return 0;
}

