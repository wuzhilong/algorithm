/**
Given a lot of customers, which sails a different product items. Find out the longest common items sailed by their customers.

customer1: item1, item2, item3,item4, item5
customer2: item1, item2, item5
customer3: item1, item2, item4
customer4: item2, item3,item4, item6

result: customer, 1, 4 
-- longest same items:  
    item2, item3,item4,

Wuzhilong

The implementation here is using STL set_intersection(), to calculate their same items between two customer.
*/

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <list>
#include <algorithm>


using namespace std;

class vendor
{
public:
    int id;
    list<int> items;
};

void print_vendor( vendor& ven )
{
    cout<<"id:"<<ven.id<<" items:";
    for( auto x:ven.items)
        cout<< x <<",";
    cout<<endl;
}

int main( int arc, char** argv)
{
    const int size = 5; 
    list< vendor* > vens;

    for(int i = 0; i<size;i++ )
    {
       vendor* p = new vendor(); 
       p->id = i+1;
       int inum=2*5; 
       int ran1 = ((double) rand()/(double) RAND_MAX)* inum;
       for( int j=0; j< ran1; j++) 
       {
            int item = ((double) rand()/(double) RAND_MAX)* inum;
            p->items.push_back( item ); 
       }
       p->items.sort();
       p->items.unique();
       vens.push_back(p);
    }

    for( auto x: vens )
        print_vendor( *x);

    vector<vendor*> max_vendor(10);
    list<int> max_items;
    list<int> tmp_items(20);

    list< vendor* >::iterator it, it2;

    for( it = vens.begin(); it != std::prev(vens.end()); it++) 
    {
        it2=it;
        cout<< "round:" << (*it)->id <<endl;
        for( it2++; it2 != vens.end(); it2++) 
        {
            cout<< "internal round:" << (*it2)->id <<endl;
            vendor* a = *it;
            vendor* b = *it2;
            list<int>::iterator reit;
            tmp_items.resize(20);
            reit = std::set_intersection( a->items.begin(), a->items.end(),  b->items.begin(), b->items.end(),tmp_items.begin()); 

            tmp_items.resize( std::distance( tmp_items.begin(), reit ) );
            cout<<" set_intersection result of: vendor= " << a->id <<" b="<< b->id <<" set_intersection items: "<<tmp_items.size() <<endl;
            if( tmp_items.size() > 0 ) 
            {
                for( auto x:tmp_items)
                    cout<< x <<",";
            }
            cout<<endl;
            cout<< "compare to current max_items:" << max_items.size() <<endl;
            if( tmp_items.size() > max_items.size())
            {
                max_vendor.clear();
                max_vendor.push_back(a);
                max_vendor.push_back(b);

                max_items.resize( tmp_items.size());
                std::copy( tmp_items.begin(), tmp_items.end(), max_items.begin());
                cout<<" max result of: " << a->id <<" b="<< b->id  <<endl;
                if( max_items.size() > 0 ) 
                {
                    for( auto x:max_items)
                        cout<< x <<",";
                }
            }
            else if( tmp_items.size() == max_items.size() && std::equal( tmp_items.begin(), tmp_items.end(), max_items.begin()) )
            {
                max_vendor.push_back(a);
                max_vendor.push_back(b);
            }
        }
    }

    std::sort( max_vendor.begin(), max_vendor.end());
    std::unique( max_vendor.begin(), max_vendor.end());

    cout<< "\n\n Final result:\n";
    cout<<" max items of vendor: " ;
    for( auto x: max_vendor )
        cout<< x->id <<", ";

    cout<<" \nsame items of both vendor: " ;
    if( max_items.size() > 0 ) 
    {
        for( auto x:max_items)
            cout<< x <<",";
    }
    return 0;
}
