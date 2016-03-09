/** Given a 2D table, with 1/0, to represent the unconnected islands( 1 ).
 * find the number of unconnected lands.
 * Wuzhilong
 * Wed Mar  9 08:54:47 CST 2006
 */



#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <deque>


using namespace std;
const int ROW=10; 
const int COL=10;


void print_table( std::vector< vector<int> >& table );

int reset_point( std::vector<vector<int> >&table, int r, int c,  std::deque< pair<int, int> >& que)
{
    int col = c;
    if( r>=0 && c >= 0 && r<table.size() && c < table[0].size() && table[r][c] )
    {
        table[r][c]=0;
        que.push_back( pair<int, int> (r,c) );
        col = reset_point( table, r, c+1,que); //visit the right direction 
        reset_point( table, r, c-1,que); //visit the right direction 
        reset_point( table, r+1, c,que); //visit the down direction 
        reset_point( table, r-1, c,que); //visit the down direction 
    }
    return col;
}

int find_islands( std::vector<vector<int> >&table )
{
    int islands=0;
    std::deque< pair<int, int> > que;
    for( int i =0; i < table.size(); i++)
    {
        int col_skip = 0;
        for( int j =0;j< table[0].size();j++)
        {
            if ( table[i][j] == 1)
            {
                table[i][j] = 0;
                que.push_back( pair<int, int> (i,j) );
                col_skip = reset_point( table, i, j+1, que );
                reset_point( table, i+1, j, que );
                //j+=col_skip;
                islands++;
                cout<< "island number: "<<islands <<endl;
                cout<< "island number point pair: "<<islands <<endl;
                for( auto x:que )
                    cout<< "(" << x.first <<"," << x.second <<"),";
                    //cout<< x<< ",";
                cout<< "end island number: "<<islands <<endl;
                print_table(table);
            }
            que.clear();
        }
    }
    return islands;
}

void print_table( std::vector< vector<int> >& table )
{
    cout<< "dump table:"<<endl;
    for( int i =0; i < table.size(); i++)
    {
        for( int j =0;j< table[0].size();j++)
        {
             cout<< table[i][j];
        }
        cout<<endl;
    }
    cout<< "end dump table:"<<endl;
}

int main( int arc, char** argv)
{
    std::vector< vector<int> > table( ROW, vector<int> ( COL, 0));
    for( int i =0; i < table.size(); i++)
    {
        for( int j =0;j< table[0].size();j++)
        {
             int random =  rand() %2;
             table[i][j] = random;
        }
    }
    print_table( table );

    int islands = find_islands( table );

    cout<< "find islands:" << islands <<endl;

    print_table( table );

    return 0;
}
