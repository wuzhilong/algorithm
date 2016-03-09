#include <stdio.h>
//#include "myutil.h"
#include <vector>
#include <stdlib.h>
#include <iostream>

using namespace std;


void dump( int a[], int p, int q )
{
    cout<< "dump array:";
    for( int i = p; i<= q; i++)
        printf("a[%d]= %d,", i, a[i] );
    cout << endl;
}

int partition( int a[], int p, int q )
{
    //cout<< "before partition: pivot="<<a[q]<<endl;
    //dump( a, p, q);

    if( p>=q)
    {
        cout << "partition p>=q, no partition.  return p=" << p <<endl;
        return p;
    }

    int pivot = a[q];
    int itmp;
    int i;
    int mid = p;

    for( i = p; i < q; i++ )
    {
     //   printf("11 a[%d]=%d, mid=%d \n", i, a[i], mid );
        if( a[i] <= pivot && i >= mid )
        {
      //     cout<<"exchange 11\n";
           itmp = a[i]; 
           a[i]= a[mid];
           a[mid]= itmp;
           mid++;
         //  dump( a, p, q);
           continue;
        } 
#if 1
        if( a[i] > pivot && i == p ) // only need to exchange the first element which is greater than the pivort.
        {
       //    cout<<"exchange 22\n";
           itmp = a[i]; 
           a[i]= pivot;
           a[q]= itmp;
          // dump( a, p, q);
        //   printf("22 a[%d]=%d, mid=%d \n", i, a[i], mid );
        }
#endif
    }

    //printf( "after partition: pivot=%d, mid=%d,\n", pivot, mid);
    //dump( a, p, q);
    return  mid;
}


void qsort( int a[], int p, int q )
{
    cout << "qsort p="<<p <<", q="<<q<<" , q - p=" << q-p << endl;
    if( p >= q)
    {
        cout<<" qsort p>=q, return \n";
        return;
    }

    int mid = partition( a, p, q);

    cout << "partion return: mid="<< mid << endl;
    
    //if( mid > p && mid <=q )
    {
        qsort( a, p,mid -1 );
        qsort( a, mid, q );
    }
}


int main( int arc, char** arv )
{

    //int size = 10000;
    int size = 20;
    int* a = new int[size];
    for( int i = 0; i< size; i++)
        //a[i] = (int) ((double)rand()/(double)RAND_MAX * 50 );
        //a[i] = i;
        a[i] = size - i;

    cout<< "before sort!!!"<< endl;

    for( int i = 0; i< size; i++)
        printf(" a[%d]= %d,", i, a[i] );

    //    cout <<"  a[i] =" << a[i] << ",";

    qsort( a, 0, size-1);

    cout<< endl << endl;
    cout<< "after sort!!!"<< endl;

    for( int i = 0; i< size; i++)
        printf(" a[%d]= %d,", i, a[i] );

//        cout <<"  a[i] =" << a[i] << ",";

    cout<< endl << endl;
    //std::vector<int> a;
//    generate_array( a, size );

    delete[] a;
    return 0;
}
