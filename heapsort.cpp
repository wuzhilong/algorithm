#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;


void dump( int a[], int p, int q )
{
    cout<< "dump array:";
    for( int i = p; i< q; i++)
        printf("a[%d]= %d,", i, a[i] );
    cout << endl;
}

int max_heap( int a[],int size, int i )
{
    int left = i*2;
    int right = i*2+1;
  
    int itmp;
    int largest = i ;
    if( left <= size && a[left -1] > a[i-1] ) 
        largest = left;

    if( right <= size && a[right -1] > a[largest-1] ) 
        largest = right;

    if( largest != i )
    {
        itmp = a[largest - 1];
        a[largest - 1] = a[i-1];
        a[i-1] = itmp;
        max_heap( a, size, largest );
    }

    return a[i-1];
}

void buildheap( int a[], int size )
{
    int mid = (size+1)/2;
    //printf( "mid =%d\n", mid );
    for( int i = mid; i > 0; i--)
    {
        max_heap( a, size, i ); 
    }
}

void heapsort( int a[], int size )
{
    buildheap( a, size );

    int imax;
    for( int newsize = size; newsize > 0; newsize--)
    {
        imax = max_heap( a, newsize, 1 ); 
        a[0]= a[newsize-1];
        a[newsize-1] = imax;
    }
}


int main( int arc, char** arv )
{

    int size = 20;
    int* a = new int[size];
    for( int i = 0; i< size; i++)
        a[i] = (int) ((double)rand()/(double)RAND_MAX * 50 );
        //a[i] = i;
        //a[i] = size - i;

    cout<< "before sort!!!"<< endl;
    dump( a, 0, size);

    heapsort( a, size );

    cout<< endl << endl;
    cout<< "after sort!!!"<< endl;
    dump( a, 0, size);

    delete[] a;
    return 0;
}
