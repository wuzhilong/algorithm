#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <iostream>

using namespace std;

//use as a buffer.
int b[20]={0};

void dump( int a[], int p, int q )
{
    cout<< "dump array:";
    for( int i = p; i<= q; i++)
        printf("a[%d]= %d,", i, a[i] );
    cout << endl;
}

int merge( int a[], int p, int q, int mid )
{
    //cout<< "before merge "<<endl;
    //dump( a, p, q);

    if( p>=q)
    {
        cout << "partition p>=q, no partition.  return p=" << p <<endl;
        return p;
    }

    int i, j, k=0;

    for( i = p, j=mid; i < mid && j <= q; )
    {
        if( a[i] <= a[j] )
        {
           b[k++]= a[i++];
        } 
        else
        {
           b[k++]= a[j++];
        }
    }

    while( i < mid) //if there is remaining of first part.
        b[k++]= a[i++];
        
    while( j <= q) //if there is remaining of second part.
        b[k++]= a[j++];

    for( i = p, k=0; i <= q;  )
        a[i++] = b[k++]; //copy back the sorted( merged ) 
        
    //dump( a, p, q);
    return  mid;
}


void mergesort( int a[], int p, int q )
{
    cout << "mergesort p="<<p <<", q="<<q<<" , q - p=" << q-p << endl;
    if( p >= q )
    {
        printf(" mergesort return: p=%d, q=%d \n", p, q);
        return;
    }

    int mid = (q+p)/2;

    cout << "mergesort divid: mid="<< mid << endl;
    
    mergesort( a, p, mid );
    mergesort( a, mid+1, q );
    merge( a, p, q, mid+1 );
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
    dump(a, 0, size-1 );

    mergesort( a, 0, size-1);

    cout<< endl << endl;
    cout<< "after sort!!!"<< endl;
    dump(a, 0, size-1 );


    delete[] a;
    return 0;
}
