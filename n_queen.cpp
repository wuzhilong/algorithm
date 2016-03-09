/**
 * The n-queens puzzle is the problem of placing n queens on an n× n chessboard such that no two queens
 * attack each other.
 * Wuzhilong
 * Tue Mar  8 22:30:35 CST 2006
 */

#include <iostream>
#include <vector>
using namespace std;

const int NUM=8;

vector<vector<int> > table(NUM,vector<int>(NUM,0));

void print_table( std::vector< vector<int> >& table )
{
    cout<< "dump table:"<<endl;
    cout.flags(ios::right);
    for( int i =0; i < table.size(); i++)
    {
        for( int j =0;j< table[0].size();j++)
        {
            cout.width(1);
            cout<< right<< table[i][j]<<",";
        }
        cout<<endl;
    }
    cout<< "end dump table:"<<endl;
}

bool validate_step(int row )
{
    //cout<<"validate_step:row="<<row;
    //print_table(table);

    //compare between every two rows. 
    for( int r1 = row; r1 < NUM-1; r1++ )
    {
        for( int r2 = r1+1; r2 < NUM; r2++ )
        {
            for( int col = 0; col < NUM; col++ )
            {
                if( table[r1][col] == 1)
                {
                    if( table[r2][col] == 1)
                    {
                        cout<<"validate_step fail: same colum="<<col << " in rows r1="<< r1 <<" ,r2="<<r2<<endl;
                        return false; // in the same colum.
                    }
                    //若两个皇后的摆放位置分别是（i,xi）和（j,xj），在棋盘上斜率为-1的斜线上，满足条件i-j=xi-xj;在棋盘上斜率为1的斜线上，满足条件i+j=xi+xj;
                    if( (r2-r1 + col < NUM) && table[r2][ r2-r1+ col] == 1 )
                    {
                        cout<<"validate_step fail: \\ diagonal:"<<col << " in rows r1="<< r1 <<" ,r2="<<r2<<" col2="<<r2-r1+col<<endl;
                        return false; // diagonal. " \ "
                    }
                    if( (col - (r2-r1) >= 0) && table[r2][ col -(r2-r1)] == 1 )
                    {
                        cout<<"validate_step fail: / diagonal:"<<col << " in rows r1="<< r1 <<" ,r2="<<r2<<" col2="<<col -(r2-r1)<<endl;
                        return false; // diagonal. " / "
                    }
                }
            }
        }
    }
    return true;
}

int counter = 0;

bool queen( int row )
{
    if( row<0 )
    {
        counter++;
        cout<<"************ found a solution* counter=" << counter<< " ***********"<<endl;
        print_table( table );
        return true;
    }

    cout<<" queen: row="<<row << " result: "<<endl;
    //print_table( table );

    for(int col = 0; col<NUM; col++)
    {
        table[row][col]=1;
        if( validate_step(row ) ) 
        {
            queen( row-1 );
        }
        table[row][col]=0;
    }
    return false;
}

int main( int argc, char** argv)
{
    queen( NUM-1 );
    
    cout<<"************Finnal solutions counter=" << counter<< " ***********"<<endl;
    return 0;
}
