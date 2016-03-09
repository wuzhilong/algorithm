/** LCS -- longest common substring problem
 * given two strings, then find out their longest consective common substring.
 *
 * Wuzhilong
 * Wed Feb  19 08:56:23 CST 2006
 */

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

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



string findlongestsubstr( const string& a, const string& b)
{
    std::vector<vector<int> >  table( a.size(), vector<int>( b.size(),0)) ;
    for( int j =0;j< b.size();j++)
    {
        table[0][j] = (a[0]==b[j]?1:0); 
    }

    for( int i =1; i < a.size(); i++)
    {
        table[i][0] = (a[i]==b[0]?1:0); 
        for( int j =1;j< b.size();j++)
        {
            table[i][j] = (a[i]==b[j]? table[i-1][j-1]+1:0); 
        }
    }


    //output the table.
    for( int i =0; i < a.size(); i++)
    {
        for( int j =0;j< b.size();j++)
        {
            if (j==0 && i==0)
            {
                cout <<"  ";
                for( int k =0; k < a.size(); k++)
                    cout<< b[k]<< ","; 
                cout<<endl;
            }
            if( j == 0)
                cout<< a[i] << " ";
            cout<< table[i][j]<<",";
        }
        cout<<endl;
    }
    string ret;
    return ret;
}


int main( int arc, char** argv)
{
    string a ( "aaaaaaaaaihadsfkjkkkjkfsdaskj");
    string b ( "bbbbbbaaadfdsjglskj");
    findlongestsubstr( a, b );
    return 0;
}
