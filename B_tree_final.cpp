#include <stdio.h>
#include <assert.h>
#include <vector>
#include <stdlib.h>
#include <iostream>
#include <deque>
#include <list>
#include <string>

/**
 *  B_tree.cpp
 *  Wuzhilong
 *  This is the final version BTree tree implemented. 
 *  Testing worked well for insert/remove for 200000 keys.
 *  Fri Feb 19 11:12:58 CST 2016
 */
//g++ -std=c++11 B_tree.cpp

#ifndef __FUNCTION__
#define __FUNCTION__ "fun"
#endif

/*as per B-tree definiton. 
 * 1). minimun degree(t),  t>=2            ==> degree is not include the bottom leaf layer.
 * 2). non-root node.keys >= t -1.
 * 3). node.keys <= 2t-1.  That's means internal node has children <= 2t.
 * 4). all leaves have same depth ( height(B_tree))  
 * 5). height(B-tree) <= logt [( n+1)/2]   ==> height is not include the bottom leaf layer.
 *
 *  if t=2, then this B-tree became a  "2-3-4 tree".
 *  For the B+ tree, internal node will not store satellite data, all satellite data are stored in leave node.
 */
const int B_DEGREE = 5;  //minimu degree(t)  =2, then this B-tree became a  "2-3-4 tree".
const int MIN_KEYS = B_DEGREE -1 ;  //minimu degree(t) 
const int MAX_KEYS = 2*B_DEGREE -1 ;  //minimu degree(t) 

using namespace std;
class BTree;

class Node
{
public:
    Node* parent;

    std::vector<int> keys;         //   t-1 <= keys.size <= 2t -1,
    std::vector<Node*> children;   // size= keys.size+1.       keys[i] < any of children[i].keys < keys[i+1]
//    std::vector<void*> data;       // size== keys.size , each key has its sattlite data pointer.
    bool m_leaf;

    Node():parent(NULL), m_leaf(false){ id = id_value++; };
    bool is_leaf(){ return m_leaf; };
    bool is_full() { return keys.size() >= MAX_KEYS; }
/*
    bool add_child( int k, Node* n )
    {
        if( is_full() ) 
        {
            printf("Node[%d-%d] keys is full.\n", keys[0], keys[MAX_KEYS] );
            return false;
        }
        int i = 0;
        while( keys[i]<k) i++;
        //insert this key before position ( i ).
        keys.insert(keys.begin()+i, k );
        children.insert(children.begin()+i, n);
        n->parent = this;
        return true;
    }

    bool remove_child( int pos)
    {
        if( keys.size() <= pos ) 
        {
            printf("Node[%d-%d] keys size: %d is less than pos: %d.\n", keys[0], keys[MAX_KEYS], keys.size(), pos );
            return false;
        }
        keys.erase(keys.begin()+pos );
        children.erase(children.begin()+pos);
        return true;
    }
*/
    //*****************************  for debug ******************************/
    static int id_value; 
    int id; 
    /*
    string toname(string &str)
    {
        if( keys.size() > 0 )
        {
            str="k:"+to_string( keys[0]);
            str+=to_string( keys[keys.size() -1]);
        }
        else
            str="k:Nil";
        return str;
    }
*/
    friend ostream& operator << (ostream& os, Node& n )
    {
        return os;

        os<<"ID:"<< n.id<<" "<<(n.m_leaf?"F":"M");
        if( n.parent )
        {
            int i;
            for(i =0; i< n.parent->children.size();i++ )
            {
                if( &n == n.parent->children[i] )
                    break;
            }
            os<< "<Pid:"<<n.parent->id<<" i="<<i <<">[k:";
        }
        else
            os<< "<Pid:(root)>"<< "[k:";

        for(int i =0; i< n.keys.size();i++ )
        {
            os<< n.keys[i]<<",";
        }
        os<<"]";
        return os;
    }

    void testnode ()
    {
        return ;
        cout<<"\ntestnode:";
        cout<<"ID:"<< id<< " "<<(m_leaf?"F":"M");
        if( parent )
        {
            int i=0; 
            int s = parent->children.size();
            for( i = 0; i < s; i++) 
            {
                if(parent->children[i] == this )
                    break;
            }
            if( i == s) 
            {
                printf("parent <-> child erro, node id:%d\n", id);
                exit(1);
            }
            cout<< "<Pid:"<<parent->id<<" pos:"<<i << ">[k:";
        }
        else
            cout<< "<Pid:(root)>"<< "[k:";

        for(int i =0; i< keys.size();i++ )
        {
            cout<< keys[i]<<",";
        }
        cout<<"]\n";
        return ;
    }

    //***************************** end for debug ******************************/
};

int Node::id_value =0;

/*
 * method:
 *  insert();
 *  delete();
 *  search();
 *  create();
 */

class BTree
{
public:
    Node* root; 
    BTree(): root(NULL) { create(); };

    int degree( Node* n  )
    {
        if( !(n->parent) )
            return 1; //root is degree 1. 
        else 
            return 1+ degree( n->parent );
    }

    int height( Node* n  )
    {
        if( !n )
            return 0; 
        if( n->m_leaf )
            return 1; 
        int h=0;
        int tmp=0;
        for( auto x: n->children )     
        {
            tmp = height(x); 
            h = (tmp > h)? tmp: h;
        }
        return 1+h; 
    }

    int max( Node* n )
    {
        Node* x = n;
        while( x && !(x->is_leaf()) && (x->children.size()> 0) )
        {
            x = x->children[x->children.size() - 1]; 
        }
        if( !x )
            return -1;
        return (x->keys.size()>0)? x->keys[ x->keys.size() - 1]: -1;
    };

    int min( Node* n )
    {
        Node* x = n;
        while( x && !(x->is_leaf()) && (x->children.size()> 0) )
        {
            x = x->children[0]; 
        }
        if( !x )
            return -1;
        return (x->keys.size()>0)? x->keys[0]: -1;
    };
    Node* predecessor( Node* n )
    {
        Node* x = n;
        while( x && !(x->is_leaf()) && (x->children.size()> 0) )
        {
            x = x->children[x->children.size() - 1]; 
        }
        if( !x )
            return NULL;
        return (x->children.size()>0)? x->children[ x->children.size() - 1]: NULL;
    };

    Node* successor( Node* n )
    {
        Node* x = n;
        while( x && !(x->is_leaf()) && (x->children.size()> 0) )
        {
            x = x->children[0]; 
        }
        if( !x )
            return NULL;
        return (x->children.size()>0)? x->children[0]: NULL;
    };

    BTree* create()
    {
        if( !root )
            root = new Node();
        root->m_leaf = true;
        return this;
    };

    /**
     * unlike the BST, AVL and RedBlack tree, it is not nessessary to allocate a node for each key,as one node can store MAX_KEYS keys and its data pointer.
     * we just go downward from root, and split the full node in the downward path. It is only need to new a node when it do splitting.
     * finnally, the target node must be non-full, where it can add this (key,data) into this target node.
     */
    Node* insert( int key, void* data )
    {
        if(!root)
        {
            cout<<" insert into a NULL root, illegal return."<<endl;
            return NULL;
        }
        Node* ret=NULL;
        Node* r= root;

        if( r->is_full() )  //it only here to increase the tree height
        {
            cout<< "root node:" << (*r) <<" is_full, split it!"<<endl;
            Node* s = new Node();  // the 1st place to new Node(). 
            root = s;
            s->m_leaf = false; //root is not leaf now, but in create() is a leaf.
            s->children.push_back( r ); // assign to children[0].
            r->parent=s;
            split_child(s, 0 );
            ret=insert_nonfull( s, key, data );
        }
        else
            ret = insert_nonfull( r, key, data );
        return ret;
    };

    Node* search(Node* n, int k, int& pos )
    {
        if( !n )
            return NULL;
        pos = -1;
        int i=0;
        //cout<<"search: " << k << " in node: "<< *n << endl; 
        for(i=0; i < n->keys.size(); i++) 
        {
            if( n->keys[i] == k )
            {
                pos = i;
                return n;
            }
            else if( n->keys[i]> k )
                break;
        }
        if( n->is_leaf() || (n->children.size() == 0 ) )
            return NULL;
        assert( i < n->children.size());
        return search( n->children[i], k, pos );// go to children.
    };

    Node* remove( int key )
    {
        if(!root )
        {
         //   cout<<" remove NULL tree or NULL node!"<< endl;
            return root;
        }
        int pos=0;
        Node* ptmp = search( root, key, pos );
        if( !ptmp)
        {
          //  cout<<" key dones not exist in the tree, remove NULL."<<endl; 
            return NULL;
        }

        ptmp = remove_helper( root, key );
        return ptmp;
    };

private:
   
    /**
     * spilt the full node y=x->children[i].  
     * partition all its keys into two child nodes,y and z.  z is a new node.
     *
     *    [ x.children: 1....m]                                                          [ x.children: 1..............m]                   
     *                      | child.i               =>                                      child.i     /\  i+1             
     *      [y.children: 1...(MAX_KEYS+1)]                          [y.children: 1...[(MAX_KEYS+1)/2]]      [z.children:[(MAX_KEYS+1)/2+1]... (MAX_KEYS+1) ]
     *
     *      x         x              
     *     /  =>     / \           y=x->children[i]    x->keys[i] = y->keys[middle]
     *    y         y   z          z=x->children[i+1]
     */
    Node* split_child( Node* x, int i )
    {
        if(!x)
            return NULL;

        Node* y = x->children[i];
        Node* z = new Node(); //the 2nd( final) place to new Node.
        z->m_leaf = y->m_leaf;
        z->parent=x;

        //printf("%s, before:", __FUNCTION__ );
        //cout << "parent:"<< *x << "  full child:" << *y <<endl;

        int middle = MAX_KEYS/2; //this middle key will be move to parent(x), rather than to z.
        int j = middle+1;
        int q = 0;

        while( j < MAX_KEYS )
        {
            z->keys.push_back( y->keys[j] ); //copy y's upper half keys to z.
        //    z->data.push_back( y->data[j++] ); //copy y's upper data keys to z.
            j++;
        }
        int key_middle = y->keys[middle];  
        for( j = MAX_KEYS-1; j >= middle;j--)  //remove y's upper half keys .
        {
            y->keys.erase( y->keys.begin()+ j );
         //   y->data.erase( y->data.begin()+ j );
        }
        if( !(y->is_leaf() ))
        {
            q = 0;
            for( j = middle+1; j <= MAX_KEYS; j++)  //remove y's upper half children.
            {
                z->children.push_back( y->children[j] ); //copy y's upper half children to z.
                y->children[j]->parent=z; 
            }
            
            for( j = MAX_KEYS; j >middle;j--)  //remove y's upper half children.
                y->children.erase( y->children.begin()+ j );
        }

        //insert z into x's position(i)
        x->children.insert( x->children.begin()+i+1, z ); //x->children[i] already exist. can user insert.

        if( i == x->keys.size())
            x->keys.push_back( key_middle );
        else
            x->keys.insert(x->keys.begin()+i, key_middle);

        //printf("%s, after, middle=%d pos=%d: ", __FUNCTION__, middle, i );
        //cout << "parent:"<< *x << "  2 children:" << *(x->children[i]) << *(x->children[i+1]) <<endl;
        assert( key_middle > x->children[i]->keys[ x->children[i]->keys.size() -1]);
        assert( key_middle < x->children[i+1]->keys[0]);

        assert( key_middle > max(x->children[i]));
        assert( key_middle < min(x->children[i+1]));

        x->testnode();
        x->children[i]->testnode();
        x->children[i+1]->testnode();
        return x;
    }

     /**
     * insert the key into node(n), which is garranty for non-full.
     */
    Node* insert_nonfull( Node* n, int key, void* data )
    {
        if(!n)
            return NULL;
        int i = 0; 
        if( n->is_leaf())  //insert into this leaf node.
        {
            while ( (i < n->keys.size() ) && key > n->keys[i] ) i++;
            if( i == n->keys.size() )
            {
                n->keys.push_back( key );
          //      n->data.push_back( data);
            }
            else
            {
                n->keys.insert( n->keys.begin()+i, key );
           //     n->data.insert( n->data.begin()+i, data);
            }
            cout<< "insert key:"<<key << "into leaf node:" << (*n) <<endl;
        }
        else
        {
            while ( (i < n->keys.size() ) && key > n->keys[i] ) i++;
            assert( i < n->children.size());
            Node* tmp = n->children[i]; //the target node is this child. 
            if( tmp && tmp->is_full())
            {
                cout<< "split non-leaf node:" << (*n )<<" at pos:"<<i<<endl;
                split_child( n, i);
                if( key > n->keys[i]) //as keys[i] is newly moved up from the target child[i] during splitting.
                    i++;
            }
            cout<< "recursive insert " << key << " into: " << (*n )<<" at child:"<<i<<*(n->children[i])<<endl;
            insert_nonfull( n->children[i], key, data);
        }
        return n;
    };

    Node* remove_helper( Node* n, int key )
    {
        if( !n)
            return NULL;

       // cout<<"remove_helper, in Node: "<< *n << "  for key=" << key << endl;

        //1. delete from leaf directly.
        if( n->is_leaf())  //insert into this leaf node.
        {
            for( int i = 0; i < n->keys.size();i++ )
            {
                if( key == n->keys[i] ) //hit the position, delete this key and its data.
                {
                    n->keys.erase(n->keys.begin()+ i );
            //        n->data.erase(n->data.begin()+ i );
              //      cout << "found target in leaf Node : "<< *n <<", and erase it.!!!"<<endl;
                    if( n == root && n->keys.size()==0 && n->children.size()==0 ) 
                    {
                        //The last key being deleted. 
             //           cout << "delete last key, and set root(NULL).!!!"<<endl;
                        delete n;
                        root=n=NULL;
                    }
                    return n;
                }
            }
            //cout << "cannot found target : "<< key <<", no deletion.!!!"<<endl;
            return NULL; 
        }
        else
        {
            int i;
            int replace_key = key;
            //void* replace_data;
           // cout << "non-leaf node, key size: "<< n->keys.size()<< endl;
            Node* next = n;
            for( i = 0; i < n->keys.size();i++ )
            {
                if( key <= n->keys[i] ) //hit the position .
                    break;
            }
            //2. it locates in this internal node ( n ).
            if( i < n->keys.size() && key == n->keys[i] )
            {
           //     cout<<"remove_helper, hit key in internal Node: "<< *n << "  at pos=" << i << endl;
                assert( i+1 < n->children.size());
                Node* l= n->children[i];  //left child
                Node* r= n->children[i+1]; //right child.
                //2.a if left child  has at least MIN_KEYS, replace the target with this subtree(left child)'max, which is also predecessor of the current key. recursively delete the replace_key. 
                if( l && l->keys.size() > MIN_KEYS )
                {
                    //replace this target with its predecessor. 
                    replace_key = max(l);
                    //replace_data = l->data[ l->keys.size() -1];
                    n->keys[i] = replace_key;
                    //n->data[i] = replace_data;  // here is not delete the origin data
                    
                    //go-downward to replace the predecessor's predecessor.
                    next = l;
          //          cout<<"remove_helper, in internal Node: replace predecessor key:"<<replace_key<< *n << *next <<  endl;
                }
                //2.b otherwise, if right child  has at least MIN_KEYS, replace the target with this subtree(right child)'min, which is also succecessor of the current key. recursively delete the replace_key. 
                else if( r && r->keys.size() > MIN_KEYS )
                {
                    //replace this target with its succcecessor. 
                    replace_key = min(r);
                    //replace_data = r->data[0];
                    n->keys[i] = replace_key;
                    //n->data[i] = replace_data;  // here is not delete the origin data
                    
                    //go-downward to replace the succcecessor's succcecessor.
                    next = r;
         //           cout<<"remove_helper, in internal Node: replace successor key:"<< replace_key<< *n << *next <<  endl;
                }
                //2.c both predecessor and successor have only MIN_KEYS. hence join them
                else
                {
                    next = join_nodes( n, i ); //join l, r, and key[i], and delete one Node.  it might be degrade the tree. n(root) is delete 
                }
            }
            else //3. go to n's target child. index is (i).
            {
                assert( i < n->children.size());
                Node* x = n->children[i]; 
                assert(x);
        //        cout<<" goto child[" << i <<"] ="<< *x<<endl;
                next = x;
                //3.a if n.children[i].keys == MIN_KEYS, borrow one from either of sibling( left, right) of n.children[i]
                if( x && x->keys.size() <= MIN_KEYS )
                {
                    Node* l = (i==0)? NULL:n->children[i-1]; //left sibling, or NULL when i==0.
                    Node* r = (i+1 >= n->children.size())? NULL:n->children[i+1]; //right sibling, NULL when i==most right,  
     //               cout<< *l << *r<<endl;
                    if( l && l->keys.size() > MIN_KEYS )  
                    {
                        //borrow from left sibling, move up left sibling's max key to n(parent), and move down n->keys[i] to x(most left).  
       //                 cout<<"remove_helper, borrow from left sibling,i="<<i<<endl;
                        int t= ((i==0)?0: i-1);
                        x->keys.insert( x->keys.begin()+0, n->keys[t] );
                        //x->data.insert( x->data.begin()+0, n->data[i] );
                        replace_key = l->keys[ l->keys.size() -1];
                        //replace_data = l->data[ l->keys.size() -1];
                        n->keys[t] = replace_key;
                        l->keys.erase(l->keys.begin() + l->keys.size() -1 );
 
                        //n->data[i] = replace_data;  // here is not delete the origin data
                        //
                        if( !(l->is_leaf()) && l->children.size()>0 )
                        {
                            Node* tmp= l->children[ l->children.size() -1 ];
                            assert( tmp );
                            tmp->parent=x;
                            x->children.insert( x->children.begin()+0, tmp );
                            l->children.erase(l->children.begin() + l->children.size() -1 );
                        }
                   
     //                   cout<<" replace_key="<<replace_key <<" n:"<<*n<<" x:"<< *x <<" left sibling:"<< *l <<  endl;
                    }
                    else if( r && r->keys.size() > MIN_KEYS )
                    {
                        //borrow from right sibling, move up right sibling's min key to n(parent), and move down n->keys[i] to y(most right).  
      //                  cout<<"remove_helper, borrow from right sibling:"<< *r<< " i="<<i<<endl ;
                        int t= ((i==n->keys.size())?n->keys.size()-1: i);
                        x->keys.push_back( n->keys[t] );
                        //x->data.push_back( n->data[i] );
                        replace_key = r->keys[0];
                        //replace_data = r->data[0];
                        n->keys[t] = replace_key;
                        //n->data[i] = replace_data;  // here is not delete the origin data
                        r->keys.erase(r->keys.begin()+0);
                        
                        if( !(r->is_leaf()) && r->children.size()>0 )
                        {
                            Node* tmp= r->children[0];
                            assert( tmp );
                            tmp->parent=x;
                            x->children.push_back(tmp);
                            r->children.erase(r->children.begin()+0);
                        }

   //                     cout<<" after borrow replace_key="<<replace_key << " n:"<<*n<<" x:"<< *x <<" right sibling:"<< *r <<  endl;
                    }
                    //3.c both sibling has only MIN_KEYS, join them and moving down the mediate key from n.
                    else
                    {
    //                    cout<<"remove_helper, join both left&rright sibling ! i=" <<i <<" n:"<<*n<< endl;
                        next = join_nodes( n, (i==0?0:i-1)); //join n, with key[i], it might be degrade the tree. n(root) is delete 
                    }
                }
                replace_key=key; //for this case, use the orginal.
            }
            //recursively go downward .
            return remove_helper( next, replace_key );  
        }
        return n;
    }

/**            
     *      n keys[i]             n                          move down to k[i] from parent n to its n->children[i].  
     *     / \          =>        |          
     *    x   y       keys[x...] keys[i] keys[y...]          joins the keys of ( n->children[i], n->keys[i], n->children[i+1] )
     *
     *                                                       erase n->children[i+1], and delete the Node n->children[i+1]
     *     input: n is the parent node
     *     i: postion
     */
    Node* join_nodes( Node* n, int i )
    {
//        cout<<"join_nodes before at pos: "<<i << " ,key[i]=" << n->keys[i]<< "in Node: "<< *n << endl;
        assert( (i>=0) && (i+1< n->children.size()));
        Node* x = n->children[i];
        Node* y = n->children[i+1];
//        cout<<  *x << *y<< endl;

        x->keys.push_back( n->keys[i]);
        for(int i =0; i< y->keys.size(); i++ )
            x->keys.push_back( y->keys[i]);

        for(int i =0; i< y->children.size(); i++ )
        {   
            y->children[i]->parent = x;
            x->children.push_back( y->children[i]);
        }

        n->children.erase( n->children.begin() + i+1); 
        n->keys.erase( n->keys.begin() + i); 
       
 //       cout<<"join_nodes after at pos: "<<i << " ,key[i]=" << n->keys[i]<<" in Node: "<< *n << *x << endl;
        delete y;

        // if n has no keys, and x is its only child, decrease the height, and change root to x
        if( n==root && n->keys.size() == 0 && n->children.size() == 1 )
        {
  //          cout<<"join_nodes degrade tree, remove root" << endl;
            root = x;
            x->parent = NULL;
            delete n;
            return x;
        }
        return n;
    }

}; //end class B_Tree.


/*******************************************************************
 *                    test code 
*******************************************************************/

void tst_tree( BTree* tree, Node* n )
{
    if(n) 
    {
        n->testnode(); 
        cout<<"tst_tree:"<< *n<<endl;
        assert( n->keys.size()>0 );
        int s = n->keys.size();
        int c = n->children.size();
        if( n->is_leaf() )
            assert( c==0);
        else
            assert( c>0);

        for( int i = 0; i< s && c>0; i++ )
        {
            for( int j = i+1; j < s;j++)
                assert( n->keys[i] < n->keys[i+1]);
            if( n->is_leaf()) 
                continue;
            
            int min =tree->min(n->children[i+1]);
            int max =tree->max(n->children[i]);
            cout<<" i=" << i <<" predecessor max=" << max <<" successor min="<< min << endl;
            assert(  min> n->keys[i]);
            assert(  max< n->keys[i]);
        }
        for( int i = 0; i < n->children.size(); i++ )
            tst_tree(tree, n->children[i]);
    }
}

void traverse_tree( BTree* tree, Node* n, int de, std::deque<Node*>& que )
{
    if(n) 
    {
        if( tree->degree(n) == de )
            que.push_back(n);
        else
            for( int i = 0; i < n->children.size(); i++ )
                traverse_tree(tree, n->children[i], de, que);
    }
}

void printBTree( BTree* tree )
{
    if( !tree && !tree->root )
        return;
    std::deque< Node* > que;
    int h = tree->height(tree->root);
    cout << "\n*****dump tree*********:";
    cout << "\nheight of tree : " << h <<endl; 
    static int pre_parent=0;
    for(int i = 0; i < h; i++)
    {
        traverse_tree( tree, tree->root, i+1, que );
        printf("\nLayer %2d:\n", i+1 );
        while( que.size())
        {
            Node* n = que.front();
            que.pop_front();

            if( n->parent )
            {
                if( n->parent->id != pre_parent)
                    cout<<endl;
                pre_parent = n->parent->id; 
            }
            cout<<*n<<endl;
/*
            printf(" \t{id:%d k:", n->id );
            for( int i=0; i< n->keys.size();i++)
                printf(" %d,", n->keys[i]);

            if( n->parent )
            {
                int k = 0;
                for( k = 0; k< n->parent->children.size(); k++ )
                {
                    if( n == n->parent->children[k])
                        break;
                }
                if( k < n->parent->children.size())
                    printf("P:id=%d i=%d, k[0]=%d",n->parent->id,k,n->parent->keys[0] );
                else
                    printf("P:id=%d i=Ni, k[0]=%d",n->parent->id,n->parent->keys[0] );
            }
            else //root
                printf("\t[root]");
            printf("}" );

*/
        }
        cout << endl;
    }
    cout << "\n*****end dump tree*********:\n";
}


int main( int arc, char** arv )
{
    //int key; 
    //int size = 10;

    BTree bst;
    cout << "insert key to tree: "<<endl;
/*
    //vector<int> vec;
    vector<int> vec = {42,19,39,45,9,16,38,13,27,23,18,25,47,31,35,7,30 };
   
    cout << endl;
    for( auto x:vec )
        cout<< x <<",";
    cout << endl;

   // for( int i = 0; i< size; i++)
    for( auto key:vec )
    {
        //key = (int) ((double)rand()/(double)RAND_MAX * 50 );
        //vec.push_back( key );

        cout << key << "  " <<endl;
        //Node* p = new Node( key, NULL );
        bst.insert( key, &key); 
    }
    */

    for( int key = 1; key <= 200000; key++ )
        bst.insert( key, &key); 

    cout << "end insert " <<endl;
    printBTree( &bst);
//    tst_tree( &bst, bst.root );

    int max = bst.max( bst.root ); 
    int min = bst.min( bst.root ); 
    cout << "tree max is: "<< max <<endl;
    cout << "tree min is: "<< min <<endl;

    int d = 27;
    Node* ptmp=NULL;
    cout << "\n**********\ninput key to been search, non-digtal exit search:\n\n " <<endl;
    int pos;
/*    while( cin>>d)
    {
        cout<<"\n\n search: key: "<< d <<endl;
        ptmp = bst.search( bst.root, d, pos );
        if( ptmp )
        {
            cout << "search result is: "<< *ptmp << " pos= "<< pos <<endl;
        }
        else
            cout << "search result fail: NULL"  <<endl;
    
        cout << "\n**********\ninput key to been search, non-digtal exit search:\n\n " <<endl;
    }
*/
/*
    vector<int> vec = {32};
    bool e=false;
    while( vec.size() < 200 )
    {
        e=false;
        d = (int) ((double)rand()/(double)RAND_MAX * 250 );
        for( auto x:vec)
        {
            if ( x == d  )
            {
                e=true;
                break;
            }
        }
        if( !e && d <=200 )
            vec.push_back( d );
    }
    cout << endl;
    for( auto x:vec )
        cout<< x <<",";
    cout << endl;
    */

    vector<int> vec = { 32,98,195,105,199,49,83,192,69,138,119,157,91,128,158,179,35,151,4,60,34,39,100,27,54,153,74,159,131,123,73,70,88,17,21,48,165,87,16,5,114,15,59,66,134,93,190,166,132,9,109,180,71,184,171,41,110,82,57,147,164,99,120,53,36,160,107,154,196,76,111,56,46,139,104,42,25,31,95,187,92,58,146,61,38,183,198,186,18,13,130,44,189,23,33,19,51,115,143,188,12,1,148,45,40,97,89,144,113,24,8,185,124,162,62,175,79,55,127,136,28,117,112,84,108,0,86,149,168,178,155,10,103,173,152,156,182,50,163,64,133,65,90,72,193,47,191,174,30,137,68,181,150,81,37,26,161,22,106,145,121,129,102,176,52,118,75,29,194,77,78,96,3,200,169,141,85,125,122,101,14,116,140,2,43,170,67,80,177,126,20,94,167,11,197,7,135,6,142,172,63};


 //   printBTree(&bst);
    cout << "\n**********\ninput key to been deleted, non-digtal exit:\n\n " <<endl;
    int x;
    while( cin>>d){ if( d == 5) break; };

    d = 1;
    //for( auto x:vec )
    while( bst.root && bst.root->keys.size()>0 )
    {
        x = (int) ((double)rand()/(double)RAND_MAX * 201000 );
  //      cout<<"\n\n delelte node: key: "<< x <<endl;
        //cout<<"running number: "<< d++ <<endl;
        ptmp = bst.remove( x );
  //      if( ptmp )
   //         cout<<"\n\n***************\n successfully delelte node: key: "<< x<<" running NO.="<< d <<endl;
//        printBTree(&bst);
 //       tst_tree(&bst, bst.root);
        d++;
    }
 
    cout<<"running number: "<< d <<endl;
    cout << "\n**********\n Exit beautifully:\n\n " <<endl;
    return 0;
}
