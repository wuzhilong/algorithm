/**
 * Given two words (start and end), and a dictionary, find the length of shortest transformation sequence
 * from start to end, such that:
 * • Only one letter can be changed at a time
 * • Each intermediate word must exist in the dictionary
 *
 * For example, Given:
 * start = "hit"
 * end = "cog"
 * dict = ["hot","dot","dog","lot","log"]
 * As one shortest transformation is "hit" -> "hot" -> "dot" -> "dog" -> "cog", return its length 5.
 * Note:
 * • Return 0 if there is no such transformation sequence.
 * • All words have the same length.
 * • All words contain only lowercase alphabetic characters.
 */

#include <iostream>
#include <vector>
#include <string>
#include <deque>
#include <deque>
#include <algorithm>

using namespace std;

string start_str={"hit"};
string end_str={"cog"};
vector<string> dict= {"hot","dot","dog","lot","log"};
deque<string> mindq;

bool DFS( deque< string> & que, string& next)
{
    bool bfound = false;
    cout<< "DFS: "<< next<<endl;
    //while( que.size() > 0)
    {
        if(que.size()> 0 &&  que.end() != find( que.begin(), que.end(), next )) 
        {
            cout<< "DFS: que size="<< que.size()<< " string="<< next<<" find in the que,return false!!" <<endl;
            return false;
        }


        string strtmp = next;
        if( strtmp == end_str )
            return (bfound=true);

        int n = strtmp.size();
        for( int i = 0; i< n; i++)
        {
            string src1= strtmp;
            src1[i]='a';
            string dest1=end_str;
            dest1[i]='a';
            if( src1 == dest1 )
            {
                que.push_back(next);
                if((  mindq.size() > 0 && que.size() < mindq.size())  || mindq.size() == 0  )
                {
                    mindq.resize( que.size());
                    copy( que.begin(), que.end(), mindq.begin());
                }
                cout<< "found que: ";
                for( auto x: que)
                    cout<< x<<", ";
                cout <<end_str<<endl;
                que.pop_back();
                return (bfound=true);
            }
        }

        for( int i = 0; i< n; i++)
        {
            string src1= strtmp;
            src1[i]='a';
            cout<< next << " change pos: "<<i<< " in string:" << strtmp;
            for( int j = 0; j< dict.size(); j++)
            {
                cout<<" dict["<<j<< "]= " << dict[j] <<endl;
                if( dict[j].size() != strtmp.size() || dict[j] == strtmp )
                {
                    cout<<" size not equal or same str"<<endl;
                    continue;
                }

                string dest1=dict[j];
                dest1[i]='a';
                if( src1 == dest1 && (que.end() == find( que.begin(), que.end(), dict[j] )))
                {
                    que.push_back(next);
                    bfound = DFS( que, dict[j] );
                    que.pop_back();
                    //if( bfound )
                    //    return bfound;
                }
            }
        }
    }
    //return bfound;
}

vector<string> visited;
bool BFS_check_reached( deque< string> & que, string& next)
{
    cout<< "BFS_check_reached: "<< next<<endl;
        
        int n = next.size();
        for( int i = 0; i< n; i++)
        {
            string src1= next;
            src1[i]='a';
            string dest1=end_str;
            dest1[i]='a';
            if( src1 == dest1 )
            {
                que.push_back(next);
                if((  mindq.size() > 0 && que.size() < mindq.size())  || mindq.size() == 0  )
                {
                    mindq.resize( que.size());
                    copy( que.begin(), que.end(), mindq.begin());
                }
                cout<< "found que: ";
                for( auto x: que)
                    cout<< x<<", ";
                cout <<end_str<<endl;
                que.pop_back();
                return true;
            }
        }
        return false;
}

void BFS( deque< string> & que, string& next)
{
    cout<< "BFS: "<< next<<endl;
         deque< string>  quetmp;
        que.push_back(next);
        int n = next.size();
        for( int i = 0; i< n; i++)
        {
            string src1= next;
            src1[i]='a';
            cout<< next << " change pos: "<<i <<endl;
            for( int j = 0; j< dict.size(); j++)
            {
                cout<<" dict["<<j<< "]= " << dict[j] <<endl;
                if( dict[j].size() != next.size() || dict[j] == next )
                {
                    cout<<" size not equal or same str"<<endl;
                    continue;
                }

                string dest1=dict[j];
                dest1[i]='a';
                if( src1 == dest1 && (que.end() == find( que.begin(), que.end(), dict[j] ))
                && (visited.end() == find( visited.begin(), visited.end(), dict[j] )))
                {
                    visited.push_back( dict[j]);
                    if(! BFS_check_reached(que, dict[j] ))
                    {
                        quetmp.push_back(dict[j]);
                    }
                }
            }
        }

        cout<< "This round tmp que: ";
        for( auto x:quetmp)
            cout<< x<<", ";
        cout <<endl;

        if(que.size()> 0 ) 
        {
            cout<< "DFS: que size="<< que.size() <<" :";
                for( auto x: que)
                    cout<< x<<", ";
                cout <<endl;
        }

        while( quetmp.size()>0)
        {
            string strtmp = quetmp.front();
            quetmp.pop_front();
            BFS(que,  strtmp);
        }

        que.pop_back();
}


int main( int argc, char** argv )
{
    deque<string> dq, quetmp;

    //bool bret = DFS(dq, start_str ); 
    BFS(dq,  start_str ); 
    //if( bret )
    {
        cout<< "found min que: ";
        for( auto x:mindq)
            cout<< x<<", ";
        cout <<end_str<<endl;
    }
    //else
    //    cout <<" not found, failure!"<<endl;
    return 0;
}
