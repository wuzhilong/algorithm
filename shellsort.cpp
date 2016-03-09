#include <stdio.h>
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

/**
 * When the dk is 1, it is the insert sort.
 */
int shellinsertsort( int a[], int p, int q, int dk )
{
    cout << "shellinsertsort before dk ="<<dk <<endl;
    dump( a, p, q);

    int i, j;
    for( i = p+dk ; i <= q; i++ )
    {
        if( a[i] <= a[i-dk] )//a[0] ...  a[i-dk] are sorted now. it need to insert x into the 0..[i-ak].
        {
           int x = a[i];
           a[i]=a[i-dk];

           j=i; 
           //find out the positon(j) to put x.
           while( j-dk >= p) 
           {
               if( a[j-dk] > x ) //find the next one.
               {
                   a[j]=a[j-dk]; //advance each element which is greater than x.
                   j-=dk;
               }
               else
               {
                  break; 
               }
           }
           a[j]=x; //j is the position which is the last one greater than x. 
        } 
    }
        
    cout << "shellinsertsort after dk ="<<dk <<endl;
    dump( a, p, q);
    return  0;
}


void shellsort( int a[], int p, int q )
{
    cout << "shellsort p="<<p <<", q="<<q<<" , q - p=" << q-p << endl;
    if( p >= q )
    {
        printf(" shellsort return: p=%d, q=%d \n", p, q);
        return;
    }

    int dk = (q-p)/2+1;
   
    while( dk >= 1 )
    {
        shellinsertsort( a, p, q, dk );
        dk/=2;
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
    dump(a, 0, size-1 );

    shellsort( a, 0, size-1);

    cout<< endl << endl;
    cout<< "after sort!!!"<< endl;
    dump(a, 0, size-1 );


    delete[] a;
    return 0;
}
