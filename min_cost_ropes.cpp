/**
Given N ropes of lengths L1, L2, L3, L4...Ln, Join the them to a final one. But with the minimum cost. As The cost for each joining is the length for both join.

Wuzhilong

The implementation here is using a priority_queue to sort the jointed ropes with the remaining.
But the problem may have some confusion, because the final cost must be always the total length. That's the sum of ( L1, L2, L3...Ln).
*/

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <queue>


using namespace std;

void print_arr( std::vector< int>& arr )
{
    cout<< "dump arr: size="<< arr.size() <<endl;
    for( int i =0; i < arr.size(); i++)
    {
        cout<< arr[i]<<",";
    }
    cout<<endl;
    cout<< "end dump arr:"<<endl;
}

int main( int arc, char** argv)
{
    const int size = 100;
    std::priority_queue< int, vector<int>, std::greater<int>  > pq;
    vector<int> vec(size, 0);
    for( int i =0; i < vec.size(); i++)
    {
        int random =  ((double)rand()/(double)RAND_MAX) * size ;
        vec[i] = random;
    }

    print_arr( vec );

    for( int i =0; i < vec.size(); i++)
    {
        pq.push( vec[i] );
    }

    while( pq.size() > 1)
    {
        int a = pq.top();
        pq.pop();
        int b = pq.top();
        pq.pop();
        pq.push( a+ b);
        cout<<" min(a+b): " <<a <<" + "<< b << " = "<< a+b<<endl;
        cout<< "dump priority_queue, size:"<< pq.size() <<endl;
        for( int i = 0; i< pq.size(); i++ )
        {
            //int x = pq.
            //cout<r x<<",";
        }
        cout<< "end: dump priority_queue"<<endl;
    }
    return 0;
}
