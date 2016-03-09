#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <iostream>
#include <deque>
#include <string>

/**
 *  BST_tree.cpp
 *  Wuzhilong
 *  This is the final version BSTTree tree implemented. 
 *  Testing worked well for insert/remove for 200000 nodes.
 *  Tue Feb 16 22:15:22 CST 2016
 */
//g++ -std=c++11 BST_tree.cpp

#ifndef __FUNCTION__
#define __FUNCTION__ "fun"
#endif

using namespace std;
class BSTTree;

void dump( int a[], int p, int q )
{
    cout<< "dump array:";
    for( int i = p; i<= q; i++)
        printf("a[%d]= %d,", i, a[i] );
    cout << endl;
}




class Node
{
public:
    int key;
    int data;

    Node* parent;
    Node* left; 
    Node* right;

    Node():parent(NULL), left(NULL),right(NULL),key(0){ };

    Node( int k, Node* p ):parent(p), left(NULL),right(NULL), key(k) { };
};

/*
 * method:
 *  insert();
 *  delete();
 *  search();
 *  min();
 *  max();
 *  predecessor();
 *  successor();
 */

class BSTTree
{
public:
    Node* root; 
    BSTTree(): root(NULL) {};
    int degree( Node* n  )
    {
        if( !(n->parent) )
            return 1; //root is degree 1. 
        else 
            return 1+ degree( n->parent );
    }


    int height( Node* n  )
    {
        int h_l=0;
        int h_r=0;
        if( !n )
            return 0; 
        else 
        {
            h_l = height( n->left );
            h_r = height( n->right );
        }
        return 1+ (h_l>=h_r ? h_l: h_r);
    }

    Node* min( Node* n )
    {
        if( !n)
            return n;
        Node* p = n;
        while( p->left) 
            p=p->left;
        return p;
    }

    Node* max( Node* n )
    {
        if( !n)
            return n;
        Node* p = n ;
        while( p->right) 
            p=p->right;
        return p;
    }

    Node* predecessor( Node* n )
    {
        if(!n)
            return n;
        return max(n->left);
    };

    Node* successor( Node* n )
    {
        if(!n)
            return n;
        return min(n->right);
    };

    Node* search(Node* n, int k )
    {
        if( !n )
            return NULL;

        if( n->key == k)
            return n;
        else if( n->key > k )
            search( n->left, k );

        else if( n->key < k )
            search( n->right, k );
    };

    Node* insert( Node* p,  Node* child )
    {
        if(!root && !p)
        {
            root=child;
            root->left=root->right = NULL;
            cout<<" insert at root: key="<<child->key <<endl;
            return root;
        }

        if( p->key == child->key )
        {
            cout<<" key exist key ="<<child->key <<endl;
            return p;
        }

        else if( p->key > child->key )
        {
            if( p->left )
                insert( p->left, child );
            else
            {
                p->left = child;
                child->parent = p;
                cout<<" insert under left of parent(" <<p->key<< "), key="<< child->key  <<endl;
                return p;
            }
        }
        else if( p->key < child->key )
        {
            if( p->right )
                insert( p->right, child );
            else
            {
                p->right = child;
                child->parent = p;
                cout<<" insert under right of parent("<< p->key <<"), key="<< child->key  <<endl;
                return p;
            }
        }
        return p;
    };

    Node* remove( int key )
    {
        if(!root )
        {
            cout<<" remove NULL tree or NULL node!"<< endl;
            return root;
        }
        Node* ptmp = search( root, key );
        if( !ptmp)
        {
            //cout<<" key dones not exist in the tree, remove NULL."<<endl; 
            return NULL;
        }

        //1. if it is a leaf. simply remove it, and set its parent to NULL.
        else if( !ptmp->left && !ptmp->right )
        {
            transplant( ptmp, NULL );
        }
        //2. if it has only a right child. replace this ptmp with the its right child.
        else if( !ptmp->left && ptmp->right )
        {
            transplant( ptmp, ptmp->right );
        }
        //3. if it has only a left child. replace this ptmp with its left child .
        else if( ptmp->left && !ptmp->right )
        {
            transplant( ptmp, ptmp->left );
        }
        //4. if it has both left and right child. replace this ptmp with predecessor.
        /*
           p                                     p
           |                                     |
           z(to be removed)     x                x  ( replace z ) 
          / \                    \              / \
         l   r            =>      r     =>     l   r
            / \                  / \              / \
           x                    m                m 
            \
             m     

             x is min(z), x must be have no left child.
             */
  
        else if( ptmp->left && ptmp->right )
        {
            //4.1 its successor is not its right child. rotate this successor's right child to this successor's parent's left.
            Node* psuc =  successor(ptmp);
            if( psuc  != ptmp->right )
            {
                transplant( psuc, psuc->right );
                psuc->right = ptmp->right;
                psuc->right->parent = psuc;
            }
            //4.2 its successor is its right child. same as case2.
            transplant( ptmp, psuc );
            psuc->left = ptmp->left;
            psuc->left->parent = psuc;
        }
        return ptmp;
    };

private:
    /**
     * helper, to transplant  subtree new_node into the old_node place.
     */
    void transplant( Node* old_node, Node* new_node )
    {
        if( !old_node->parent)
            root = new_node;
        else if( old_node->parent->left == old_node )
            old_node->parent->left = new_node;
        else if( old_node->parent->right == old_node )
            old_node->parent->right = new_node;

        if( new_node)
            new_node->parent = old_node->parent;
    };

};


/*
void printBST( BSTTree* tree )
{
    if( !tree && !tree->root )
        return;

    std::deque< Node* > que;
    std::deque< string > que2;
    que.push_back( tree->root );
    que2.push_back( "(r)\n /\\" );

    string strnull={"NULL"};
    string strend={")"};
    string tmp2("(");
    string tmp;
    while( que.size())
    {
        Node* n = que.front();
        string str = que2.front();

        que.pop_front();
        que2.pop_front();

        printf("\t%d", n->key );
        cout<< "(p=";
        if( n->parent )
            cout<< n->parent->key << strend.c_str();
        else
            cout<< strnull.c_str() << strend.c_str();

        printf("%s", str.c_str() );

        tmp = tmp2 + to_string( n->key ) + ")";
        if( n->left )
        {
            que.push_back( n->left );
            que2.push_back( tmp+"/" );
        }
        if( n->right )
        {
            que.push_back( n->right );
            que2.push_back( tmp+"\\" );
        }
    }
}

string node_cstr( Node* n, string prefix, string post )
{
    return prefix + to_string( n->key) + post;
}
*/



void inorder_tree( BSTTree* tree, Node* n, int de, std::deque<Node*>& que )
{
    if(n) 
    {
        inorder_tree(tree, n->left, de, que);
        if( tree->degree(n) == de )
            que.push_back(n);
        inorder_tree(tree, n->right,de, que);
    }
}

void printBST( BSTTree* tree )
{
    if( !tree && !tree->root )
        return;
    std::deque< Node* > que;
    int h = tree->height(tree->root);
    cout << "\n*****dump tree*********:";
    cout << "\nheight of tree : " << h <<endl; 
    for(int i = 0; i < h; i++)
    {
        inorder_tree( tree, tree->root, i+1, que );
        printf("Layer %2d:", i+1 );
        while( que.size())
        {
            Node* n = que.front();
            que.pop_front();

            if( n->parent )
            {
                if( n == n->parent->left )
                    printf("\t%d[%d]/", n->key,  n->parent->key );
                else if( n->parent->left ) //parent has both children.
                    printf(" \\%d[%d]", n->key,n->parent->key );
                else
                    printf("\t\\%d[%d]", n->key, n->parent->key );
            }
            else //root
                printf("\t%d[root]", n->key);
        }
        cout << endl;
    }
    cout << "\n*****end dump tree*********:\n";
}


int main( int arc, char** arv )
{
    int key; 
    int size = 200000;

    BSTTree bst;
    cout << "insert key to tree: "<<endl;
    //vector<int> vec;
    
    vector<int> vec = { 32,98,195,105,199,49,83,192,69,138,119,157,91,128,158,179,35,151,4,60,34,39,100,27,54,153,74,159,131,123,73,70,88,17,21,48,165,87,16,5,114,15,59,66,134,93,190,166,132,9,109,180,71,184,171,41,110,82,57,147,164,99,120,53,36,160,107,154,196,76,111,56,46,139,104,42,25,31,95,187,92,58,146,61,38,183,198,186,18,13,130,44,189,23,33,19,51,115,143,188,12,1,148,45,40,97,89,144,113,24,8,185,124,162,62,175,79,55,127,136,28,117,112,84,108,0,86,149,168,178,155,10,103,173,152,156,182,50,163,64,133,65,90,72,193,47,191,174,30,137,68,181,150,81,37,26,161,22,106,145,121,129,102,176,52,118,75,29,194,77,78,96,3,200,169,141,85,125,122,101,14,116,140,2,43,170,67,80,177,126,20,94,167,11,197,7,135,6,142,172,63};

  
    cout << endl;
   // for( auto x:vec )
    //    cout<< x <<",";
    cout << endl;

    Node* ptmp ;
    int r=0;
    for( int i = 0; i< size; )
    //for( auto key:vec )
    {
        key = (int) ((double)rand()/(double)RAND_MAX * (size + 10000) );
        //vec.push_back( key );
        ptmp = bst.search( bst.root, key );
        if( !ptmp )
        {
            //cout << key << "  " <<endl;
            Node* p = new Node( key, NULL );
            bst.insert( bst.root, p ); 
            i++;
        }
    }
    cout << "end insert: " <<endl;
    printBST( &bst);

    ptmp = bst.max( NULL);
    cout << "tree max is: "<< ptmp->key  <<endl;

    ptmp = bst.min( NULL);
    cout << "tree min is: "<< ptmp->key  <<endl;

/*
    cout << "search tree for:"<<endl;
    ptmp = bst.search( bst.root, 3666666 );
    if( ptmp )
        cout << "search result is: "<< ptmp->key  <<endl;
    else
        cout << "search result is: NULL"  <<endl;

    int d = 19;
    cout<<"\n\n delelte node: key: "<< d <<endl;
    ptmp = bst.remove( d );
    if( ptmp )
        delete ptmp;
    printBST( &bst);
    cout << "\nheight of tree : " << bst.height( bst.root) <<endl; 
    */

    cout << "\n**********\ninput key to been deleted, non-digtal exit:\n\n " <<endl;
    int d = 1;
    int x;
    while( cin>>d){ if( d == 5) break; };

    d = 1;
//    for( auto x:vec )
    r=0;
    while( bst.root )
    {
        x = (int) ((double)rand()/(double)RAND_MAX * (size+20000) );
  //      cout<<"\n\n delelte node: key: "<< x <<endl;
        //cout<<"running number: "<< d++ <<endl;
        ptmp = bst.remove( x );
        if( ptmp )
        {
            r++;
            delete ptmp;
        }
   //         cout<<"\n\n***************\n successfully delelte node: key: "<< x<<" running NO.="<< d <<endl;
//        printBTree(&bst);
 //       tst_tree(&bst, bst.root);
        d++;
    }
 
    cout<<"running number: "<< d << " ,delete keys:"<<r <<endl;
    cout << "\n**********\n Exit beautifully:\n\n " <<endl;
 
    return 0;
}
