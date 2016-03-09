#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <climits>
#include <algorithm>


/**
 * Given a m × n grid filled with non-negative numbers, find a path from top left to bottom right which
 * minimizes the sum of all numbers along its path.
 * Note: You can only move either down or right at any point in time
 *
 *   Wuzhilong
 *   Fri Mar  4 09:06:02 CST 2016
 */

using namespace std;
const int ROW=5; 
const int COL=5;
static int total_counter = 0;
static int counter = 0;

void print_table( std::vector< vector<int> >& table );

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

int DP_find_min_table_without_recursion( std::vector<vector<int> >&min_table, std::vector<vector<int> >&table, int r, int c )
{
    total_counter++;
    if( r ==0 && c == 0)
    {
        cout<<"find_min_table: reach[0,0] return:"<< table[0][0]<<endl;
        min_table[0][0]=1;
        return table[0][0];
    }

    if( r < 0 || c < 0)
    {
        cout<<"find_min_table: out bound return:"<< INT_MAX<<endl;
        return INT_MAX;
    }

    counter++;
    min_table[0][0]= table[0][0]; 

    for( int i =1; i < min_table.size(); i++)
        min_table[i][0]= table[i-1][0]+table[i][0];
    for( int i =1; i < min_table[0].size(); i++)
        min_table[0][i]= table[0][i-1]+table[0][i];

    for( int i =1; i < table.size(); i++)
    {
        for( int j =1;j< table[0].size();j++)
        {
            min_table[i][j] = table[i][j] +  min( min_table[i-1][j] , min_table[i][j-1] );
        }
        cout<<endl;
    }
 
    int ret = min_table[r][c];
    cout<<"DP_find_min_table_without_recursion: m x n: "<< r<<","<<c<<" return: "<< ret <<endl;
    return ret; 
}


list< pair<int,int> > min_path;
int DP_find_min_table( std::vector<vector<int> >&min_table, std::vector<vector<int> >&table, int r, int c )
{
    total_counter++;
    if( r ==0 && c == 0)
    {
        cout<<"find_min_table: reach[0,0] return:"<< table[0][0]<<endl;
        min_table[0][0]=1;
        return table[0][0];
    }

    if( r < 0 || c < 0)
    {
        cout<<"find_min_table: out bound return:"<< INT_MAX<<endl;
        return INT_MAX;
    }
    if( min_table[r][c] != INT_MAX)
        return min_table[r][c];

    counter++;
   
    int t1 = DP_find_min_table(min_table, table, r -1, c);
    int t2 = DP_find_min_table(min_table, table, r, c-1);
    int t = min( t1, t2);
    int ret =  t + table[r][c] ;
/* cannot work.
    if( t1 < t2 && r-1>=0 && c>=0 )
        min_path.push_front( std::pair<int,int>( r-1,c));
    else if( t1 > t2 && c-1>=0 && r>=0 )
        min_path.push_front( std::pair<int,int>( r,c-1));
        */

    min_table[r][c] = ret;
    cout<<"find_min_table: m x n: "<< r<<","<<c<<" min(a=" <<t1<<" ,t="<<t2<< ", table[][]="<<table[r][c] <<" return: "<< ret <<endl;
    return ret; 
}



int find_min_table( std::vector<vector<int> >&min_table, std::vector<vector<int> >&table, int r, int c )
{
    total_counter++;
    if( r ==0 && c == 0)
    {
        cout<<"find_min_table: reach[0,0] return:"<< table[0][0]<<endl;
        min_table[0][0]=1;
        return table[0][0];
    }

    if( r < 0 || c < 0)
    {
        cout<<"find_min_table: out bound return:"<< INT_MAX<<endl;
        return INT_MAX;
    }

    counter++;
   
    int t1 = find_min_table(min_table, table, r -1, c);
    int t2 = find_min_table(min_table, table, r, c-1);
    int t = min( t1, t2);
    int ret =  t + table[r][c] ;

    min_table[r][c] = ret;
    cout<<"find_min_table: m x n: "<< r<<","<<c<<" min(a=" <<t1<<" ,t="<<t2<< ", table[][]="<<table[r][c] <<" return: "<< ret <<endl;
    return ret; 
}

int main( int arc, char** argv)
{
    std::vector< vector<int> > table( ROW, vector<int> ( COL, 100));
    std::vector< vector<int> > min_table( ROW, vector<int> ( COL, INT_MAX));

    for( int i =0; i < table.size(); i++)
    {
        for( int j =0;j< table[0].size();j++)
        {
             int random =  ((double)rand()/(double)RAND_MAX)* 20;
             //table[i][j] = random;
             if(i==j){
             table[i][j] = 2;
             if( i-1 >= 0)
             table[i-1][j] = 2;
             if( j-1 >= 0)
             table[i][j-1] = 1;
             }
        }
    }
    
    print_table( table );

    int min = find_min_table(min_table, table, ROW-1, COL-1 ); 
    cout<< "find min:" << min<<"total_counter:" <<total_counter <<" counter: "<<counter <<endl;
    //int min = DP_find_min_table(min_table, table, ROW-1, COL-1 ); 
    //int min = DP_find_min_table_without_recursion(min_table, table, ROW-1, COL-1 ); 
    cout<< "DP find min:" << min<<"total_counter:" <<total_counter <<" counter: "<<counter <<endl;
    print_table( table );
    print_table( min_table );

    for( int i =0; i < table.size(); i++)
    {
        for( int j =0;j< table[0].size();j++)
        {
             table[i][j] = 0;
        }
    }

    int i =0;
    int j =0;
    table[0][0] = 1;
    while( i < table.size()-1 || j < table[0].size()-1)
    {
        //cout<<"i, "<<i <<"j =" <<j <<endl;
        if( i+1<ROW && j+1 < COL && min_table[i+1][j] < min_table[i][j+1] )
        {
            table[i+1][j] = 1;
            i++;
        }
        else if( i+1<ROW && j+1 < COL && min_table[i+1][j] > min_table[i][j+1] )
        {
            table[i][j+1] = 1;
            j++;
        }
        else if( i==ROW-1 && j+1 < COL ) 
        {  
            table[i][j+1] = 1;
            j++;
        }
        else if( i+1<ROW && j == COL-1 ) 
        {  
            table[i+1][j] = 1;
            i++;
        }
    }


    cout<< "min min_table:" << min <<endl;
    print_table( table );
    
    cout<< "min min_path:" << min_path.size() <<endl;
    for( auto it=min_path.begin(); it != min_path.end();it++)
        cout<<"("<< (*it).first <<", "<< (*it).second <<")" <<endl;

    return 0;
}
