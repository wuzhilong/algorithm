/** Using two methods to implement the 0-1 knapsack problem.
 * Wuzhilong
 * Wed Feb  9 08:58:19 CST 2006
 */

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <climits>
#include <algorithm>

using namespace std;
const int WEIGHT=10;
class item{
public:
    int value;
    int weight;
    item( int w, int v):value(v),weight(w){};
};

void print_table( std::vector< vector<int> >& table )
{
    cout<< "dump table:"<<endl;
    cout.flags(ios::right);
    for( int i =0; i < table.size(); i++)
    {
        for( int j =0;j< table[0].size();j++)
        {
            cout.width(8);
            cout<< right<< table[i][j]<<",";
        }
        cout<<endl;
    }
    cout<< "end dump table:"<<endl;
}

int knapsack( std::vector< item >& items )
{
    int num = items.size();
    std::vector< vector<int> >  table( num+1, vector<int>( WEIGHT+1, 0));
    std::vector< int >  solution( num,  0);

    int t;
    for( int i = 0; i< num; i++)
    {
        t = i+1;
        for( int j = 1; j<= WEIGHT; j++)
        {
            if( items[i].weight > j ) 
                table[t][j]= table[t-1][j];
            else
            {
                if( items[i].value + table[t-1][j-items[i].weight] > table[t-1][j])
                    table[t][j]= items[i].value + table[ t-1][j-items[i].weight];
                else
                    table[t][j]= table[t-1][j];
            }
        }
    }


    fill( solution.begin(), solution.end(), 0);
    for( auto x:solution)
        cout << x<<",";
    cout<< endl;
    
    //find out the solution.
    int j = WEIGHT;         
    for( int i = num-1; i>= 0; i--)
    {
        t = i+1;
        if ( table[t][j]== table[t-1][j])
            solution[i] = 0;
        else//( table[t][j] == (items[i].value + table[ t-1][j-items[i].weight] ) ) 
        {
            solution[i] = 1;
            j = j - items[i].weight;  // this is very important. to minus the weight.
        }
    }

    int ret = table[num][WEIGHT]; //final maxium value.


    //output the result.
    for( int i = 1; i<= num; i++)
        table[i][0]= items[i-1].value;
    for( int i = 1; i<= WEIGHT; i++)
        table[0][i]= i;

    print_table(table);

    cout<<"weight:";
    cout.flags(ios::right);
    cout.width(8);
 
    for( auto x:items)
        cout << x.weight<<",";
    cout<< endl;
    cout<<" value:";
    cout.flags(ios::right);
    cout.width(8);
    for( auto x:items)
        cout << x.value<<",";
    cout<< endl;

    cout<<"      ";
    cout.flags(ios::right);
    cout.width(8);
    for( auto x:solution)
        cout << x<<",";
    cout<< endl;

    return ret; 
}

int knapsack_recursion( std::vector< item >& items, int Weight, int n )
{
    int num = items.size();
    cout<<"knapsack_recursion: Weight="<<Weight << " , n="<< n <<" items[n].w="<<items[n].weight <<" items[n].v="<<items[n].value <<endl;
    if( n<0 || n>= num) return 0;

    if( Weight <= 0) return 0;

    if( Weight < items[n].weight )
    {
        int r= knapsack_recursion( items, Weight, n-1 ); 
        cout<< "Weight="<<Weight<<" < item[n].weight="<< items[n].weight << " ,return n-1 's result:"<< r<<endl;
        return r;
    }
    else
    {
        int w = Weight - items[n].weight;
        int v = knapsack_recursion( items, w, n-1 ) + items[n].value; 
        int p = knapsack_recursion( items, Weight, n-1); 
        cout<< "val(n-1)="<<p<<" val(n-1, W-w)+v(n)="<< v <<" (Weight-items.w)="<<w << " ,return result:"<< max(v,p) <<endl;
        return max( v, p );
    }
}

int main( int arc, char** argv)
{
    // <weight,value >
//    std::vector< pair<int, int > > items ={ 
//        pair<int,int> (5, 2), 
//        pair<int,int> (8, 3), 
//        pair<int,int> (2, 4), 
//        pair<int,int> (4, 5), 
//        pair<int,int> (3, 6), 
//        pair<int,int> (1, 7), 
//        pair<int,int> (6, 8) };
    std::vector< item > items ={ 
        item(5, 2), 
        item(8, 3), 
        item(2, 4), 
        item(4, 5), 
        item(3, 6), 
        item(1, 7), 
        item(6, 8) };

    //knapsack( items );
    int ret = knapsack_recursion( items, WEIGHT, items.size()-1 );
    cout<<"result of knapsack_recursion: "<< ret << endl;
    return 0;
}
