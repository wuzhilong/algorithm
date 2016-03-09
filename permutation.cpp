#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>

/**
 * Wuzhilong
 * Thu Mar  3 10:52:26 CST 2006
 * Use three method to implement the permutation a give string.
 */

using namespace std;

/**
 * k : is the prefix index. if k=0, means a full permutation.
 * m: is the size of str.
 */
void permutation( string& str, int k, int m )
{
    //cout<< "\npermutation:";
    if( k == m-1 ) // if the prefix is the last one. print it. end recursion.
        cout<< str<<endl;

    for( int i = k; i < m; i++)
    {
        //cout<< "swap:"<< str[k]<<" = " << str[i] << " k=" << k ;
        swap( str[k], str[i]);  //swap prefix.
        permutation( str, k+1, m ); //increase the prefix size.
        swap( str[k], str[i]);
        //cout<<"\n next prefix increase k="<<k<<endl;
    }
}

/**
 * it is the started string, with acsending sorted.
 * end in the reverse order -- descending sorted.
 */
void dictionary( const string& strin )
{
    string str(strin);
    int s = str.size();
    sort( str.begin(), str.end());
    string endstr(str);
    reverse( endstr.begin(), endstr.end());
    cout<< "dictionary permutation: "<<str<< ", end at: "<< endl;

    int i, j , counter=1; 
    //while( str != endstr )
    
    cout<< "use dictionary permutation, begin:" <<endl;
    cout<< str<<endl;
    while( 1 )
    {
        //find the first position i, where it is less than its following.
        for( i = s-1-1; i>=0; i--)
        {
            if( str[i]<str[i+1])
                break;
        }
        if(i == -1) //not find, hence,the string must already be descending sorted, e,g."edcba"
            break; 
        
        //find out the first position j, which is greater than i. 
        for( j = s-1; j>i; j--)
        {
            if( str[i]<str[j])
            {
                cout<< " i= " << i <<", j= " << j <<" , sort the remaining right part: ["<<j<<"--"<<s-1<<"]"<< endl;
                assert( i>=0 && i<s);
                assert( j>=0 && j<s);

                swap( str[i], str[j]);
                //sort the remaining part after the position (i+1) with acsending order. this is the dictionary permutation followed the current string.
                //actually, in STL implementation. Here it use the reverse(), instead of the sort().  Becausex, after swap, it must be the descending order( max for the remaining part).
                sort( str.begin()+i+1, str.end()); 
                counter++;
                cout<< str<<endl;
                break;
            }
        }
    }
    
    int calc = 1;
    while( s>1) calc *=s--;
    cout<< "end dictionary permutation, counter="<<counter<< " n!="<< calc <<endl;
}

/**
 * STL provide both next_permutation( ) and  prev_permutation().
 * with using the above dictionary method.
 */
void dictionary2( const string& strin )
{
    string str(strin);
    int s = str.size();
    sort( str.begin(), str.end());
    string endstr(str);
    reverse( endstr.begin(), endstr.end());
    cout<< "STL permutation: "<<str<< ", end at: "<< endl;

    int counter = 1;
    cout<< str<<endl;
    while( str != endstr )
    {
        counter++;
        next_permutation( str.begin(), str.end() );
        cout<< str<<endl;
    }

    int calc = 1;
    while( s>1) calc *=s--;
    cout<< "end STL permutation, counter="<<counter<< " n!="<< calc <<endl;
}

int main( int arc, char** argv)
{
    const int size = 10;
    string str={"abcdef"} ; 
    //permutation( str, 2, str.size());
    
    dictionary2( str );
    dictionary( str );
    return 0;
}
