#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <iostream>
#include <deque>
#include <string>
#include <map>


/**
 *  Wuzhilong
 *  This is the final version left-lean red-black tree implemented. 
 *  Testing worked well for insert/remove.
 * Fri Feb 5 22:02:04 CST 2016
 */
//g++ -std=c++11 rbtree.cpp

#ifndef __FUNCTION__
#define __FUNCTION__ "fun"
#endif

using namespace std;
class RBTree;
void printRBTree( RBTree* tree );

enum Color{ RED, BLACK};

class Node
{
public:
    int key;
    int data;
    Color color; 

    Node* parent;
    Node* left; 
    Node* right;

    Node():parent(NULL), left(NULL),right(NULL),key(0), color(RED) { };
    Node( int k, Node* p ):parent(p), left(NULL),right(NULL), key(k),color(RED){ };
    bool is_red(){ return (color == RED); };
};

/*
 * public method:
 *  insert();
 *  remove();
 *  search();
 *  min();
 *  max();
 *  predecessor();
 *  successor();
 *  height()
 */

class RBTree
{
public:
    Node* root; 
public:
    RBTree(): root(NULL) {};
public:
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
            root->color = BLACK;
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
            }
        }
        else //if( p->key < child->key )
        {
            if( p->right )
                insert( p->right, child );
            else
            {
                p->right = child;
                child->parent = p;
                cout<<" insert under right of parent("<< p->key <<"), key="<< child->key  <<endl;
            }
        }

        Node* tmp=p;
        //add for red_black tree.
        if( p->right && p->right->is_red() )
        {
            cout<< "insert, p(" << p->key <<") p->right is red. rotate left:\n";
            tmp = rotate_left( p );
        }

        if( tmp->left && tmp->left->is_red() && tmp->left->left && tmp->left->left->is_red() )
        {
            cout<< "insert, p(" << p->key <<") both p->left and left->left are red. rotate right\n";
            tmp = rotate_right( tmp );
        }

        if( tmp->left && tmp->left->is_red() && tmp->right && tmp->right->is_red() )
        {
            cout<< "insert,p(" << p->key <<") p->right and left are red. flip_color \n";
            flip_color( tmp );
        }

        return tmp; 
    };

    Node* Remove(int key )
    {
        if(!root)
        {
            cout<<" remove NULL tree or NULL node!"<< endl;
            return root;
        }
        Node* ptmp = search( root, key );
        if( !ptmp)
        {
            cout<<" key dones not exist in the tree, remove NULL."<<endl; 
            return NULL;
        }
        remove_helper( root, key );
        return ptmp;
    };

private:

    /**     x                z  
     *     / \              / \
     *    m   z(r) ==> (r) x   zr
     *       / \          / \ 
     *      zl  zr       m  zl
     */
    Node* rotate_left( Node* x)
    {
        printf( "\nrotate_left, node=%d, before:\n", x->key );
        printRBTree( this );

        Node* z = x->right;
            
        x->right = z->left;
        if( z->left) // zl might be NULL
        {
            x->right->parent = x;
        }
        z->left = x;

        z->parent= x->parent;
        if( x->parent && x->parent->left == x)
            x->parent->left = z;
        else if( x->parent && x->parent->right == x)
            x->parent->right = z;

        x->parent = z;
        z->color=x->color;
        x->color=RED;

        if( root == x)
            root=z;

        printf( "\nrotate_left, node=%d, after:\n", x->key );
        printRBTree( this );
        return z;

    }
     /**    x                z  
     *     / \              / \
    * (r) z   m    ==>     zl  x(r)
     *   / \                  / \ 
     *  zl  zr               zr  m
     */
    Node* rotate_right( Node* x)
    {
        printf( "\nrotate_right, node=%d, before:\n", x->key );
        printRBTree( this );

        Node* z = x->left;
            
        x->left = z->right;
        if( z->right ) //zr might be NULL
        {
            x->left->parent = x;
        }
        z->right = x;
        z->parent= x->parent;
        if( x->parent && x->parent->left == x)
            x->parent->left = z;
        else if( x->parent && x->parent->right == x)
            x->parent->right = z;
        x->parent = z;

        z->color=x->color;
        x->color=RED;
        if( root == x)
            root=z;
        printf( "\nrotate_right, node=%d, after:\n", x->key );
        printRBTree( this );
        return z;
    }


    Node* flip_color( Node* n)
    {
        printf( "\nflip_color, node=%d, before:\n", n->key );
        printRBTree( this );

        if(n && n->left && n->right )
        {
            n->color = (n->color==RED)? BLACK:RED;
            if( n==root )
                n->color=BLACK;
            n->left->color = n->left->color==RED? BLACK:RED;
            n->right->color = n->right->color ==RED? BLACK:RED;
        }
        printf( "\nflip_color, node=%d, after:\n", n->key );
        printRBTree( this );
        return n;
    }


    Node* fixup( Node* n)
    {
        if( !n )
            return NULL;
        Node* tmp = n;
        printf("%s,node(%d)\n", __FUNCTION__, n->key );
        if( tmp->right && tmp->right->is_red())
        {
            printf("%s,node(%d) right child is red. rotate_left.\n", __FUNCTION__, tmp->key );
            tmp = rotate_left( tmp ); //rotate right-leaning reds.
        }
        if( tmp->left && tmp->left->is_red() && tmp->left->left && tmp->left->left->is_red() )
        {
            printf("%s,node(%d) both n->l & n->l->l are red. rotate_right.\n", __FUNCTION__, tmp->key );
            tmp = rotate_right( tmp ); //rotate red-red pairs
        }
        if( tmp->left && tmp->left->is_red() && tmp->right && tmp->right->is_red() )
        {
            printf("%s,node(%d) both n->l & n->r are red. flip_color.\n", __FUNCTION__, tmp->key );
            flip_color(tmp);          //split 4-nodes.
        }
        else
        {
            printf("%s,node(%d) do nothing. ret\n", __FUNCTION__, n->key );
        }
        return tmp;
    }

    Node* move_red_right( Node* n)
    {
        if( !n )
            return NULL;
        printf("%s,node(%d) flip_color firstly.\n", __FUNCTION__, n->key );
        flip_color(n);         
        Node* tmp = n;
        if( n->left && n->left->left && n->left->left->is_red() )
        {
            printf("%s,node(%d) n->l->l is red. rotate_right => flip_color.\n", __FUNCTION__, tmp->key );
            tmp = rotate_right( n ); 
            flip_color( tmp );
        }
        return tmp;
    }

    Node* move_red_left( Node* n)
    {
        if( !n )
            return NULL;
        printf("%s,node(%d) flip_color firstly.\n", __FUNCTION__, n->key );
        flip_color(n);         
        Node* tmp = n;
        if( n->right && n->right->left && n->right->left->is_red() )
        {
            printf("%s,node(%d) then, n->r->l is red. rotate_right=>rotate_left => flip_color.\n", __FUNCTION__, tmp->key );
            tmp->right = rotate_right( tmp->right ); 
            tmp=rotate_left(tmp);
            flip_color( tmp );
        }
        return tmp;
    }


/********************************
 *  not been tested code 
    void RemoveMax()
    {
        root = RemoveMax( root );
        root->color = BLACK;        
    }

    Node*  RemoveMax( Node* n)
    {
        if(!n)
           return NULL;
        Node* tmp = n;
        if( n->left && n->left->is_red())
            tmp = rotate_right( n ); 
        if( !tmp->right )
           return NULL;
        if( n->right && n->right->is_red() && !( n->right->left && n->right->left->is_red()) )
            tmp = move_red_right(n);
        tmp->left = RemoveMax( tmp->left );
        return fixup( tmp );
    }
    **** not been tested code.
********************************/


    /** The min node must be in the bottom of left spine
     *  It must be only has one child or no child. If this min is RED, then it must be a leaf.
     */
    void RemoveMin()
    {
        root = RemoveMin( root );
        root->color = BLACK;        
    }
    Node*  RemoveMin( Node* n)
    {
        if(!n || !(n->left)) //hit the min
        {
           printf("%s,NULL or no left, delete this node(). \n", __FUNCTION__ );
           return NULL;
        }
        printf("%s,node(%d). \n", __FUNCTION__, n->key );
        Node* tmp = n;
        if( n->left && n->left->is_red() && !( n->left->left && n->left->left->is_red()) )
        {
            printf("%s,node(%d). n.l is_red && !(n.l.l.is_red) \n", __FUNCTION__, tmp->key );
            tmp = move_red_left(n);
        }

        printf("%s,node(%d). recursive call\n", __FUNCTION__, tmp->left->key );
        tmp->left = RemoveMin( tmp->left );

        if( tmp->left )
            printf("%s,node->l==(%d). go upward. node==(%d) \n", __FUNCTION__, tmp->left->key, tmp->key );
        else
            printf("%s,node left(%d). deleted, n->l==NULL\n", __FUNCTION__, tmp->key );

        return fixup( tmp ); //fixup up-toward.
    }

    Node* remove_helper( Node* n, int key )
    {
        Node* tmp = n;
        if(tmp && tmp->key > key ) //go to left spine.
        {
            if( tmp->left && tmp->left->is_red() && !( tmp->left->left && tmp->left->left->is_red()) )
                tmp = move_red_left( tmp );
            tmp->left = remove_helper( tmp->left, key );
        }
        else  // equals or greater, go to right spine.
        {
            if( tmp->left && tmp->left->is_red()) 
            {
                printf("%s,node(%d) left child is red. rotate_right.\n", __FUNCTION__, tmp->key );
                tmp = rotate_right( tmp );
            }
            if( tmp->key == key && !(tmp->right )) // the target node has no right child.
                return NULL;
            if( !(tmp->right && tmp->right->is_red())  && !( tmp->right->left && tmp->right->left->is_red()) )
            {
                printf("%s,node(%d) both (n->r) && (n->r->l) not red. move_red_right.\n", __FUNCTION__, tmp->key );
                tmp = move_red_right( tmp );
            }
            if( tmp->key == key ) //the target node to be deleted which is an internal 
            {
                Node* minnode = min( tmp->right ); // we find out the min of this target node's right spine.
                Node* delete_node = tmp;

                printf("%s,node(%d) find key to delete. remove min(n->r)=%d, replace with this min here.\n", __FUNCTION__, tmp->key, minnode->key );
                tmp->right = RemoveMin( tmp->right ); //use the min to replace this target node. ( So, the really being delete 'node' is the min from this tree ).
                replace_node( delete_node, minnode ); //to keep the node data integratity. replace this deleted node with the min one rather than only swap its data and key.
                tmp=minnode;
                printRBTree( this );
            }
            else
            {
                printf("%s,node(%d) recursive go down to (n->r)[%d].\n", __FUNCTION__, tmp->key, tmp->right->key );
                tmp->right = remove_helper( tmp->right, key );
            }
        }
        printf("%s,node(%d) fixup up-toward.\n", __FUNCTION__, tmp->key );
        tmp = fixup( tmp ); //fix up-toward.
        return tmp ; //fix up-toward.
    }
    
    /**
     * helper, to replace new_node into the old_node place.
     */
    void replace_node( Node* old_node, Node* new_node )
    {
        if( !old_node || !new_node )
            return;

        if( !old_node->parent)
        {
            root = new_node;
        }
        else if( old_node->parent->left == old_node )
            old_node->parent->left = new_node;
        else if( old_node->parent->right == old_node )
            old_node->parent->right = new_node;
        
        new_node->color = old_node->color;
        new_node->parent = old_node->parent;
        new_node->left = old_node->left;
        if( new_node->left )
            new_node->left->parent = new_node;
        new_node->right = old_node->right;
        if( new_node->right )
            new_node->right->parent = new_node;
    };
};


/*
void printRBTree( RBTree* tree )
{
    if( !tree && !tree->root )
        return;

    std::deque< Node* > que;
    std::deque< string > que2;
    que.push_back( tree->root );
    que2.push_back( "(root)\n /\\" );

    string strnull={"NULL"};
    string strend={")"};
    string tmp2("(");
    string tmp;
    string bstr="B";
    string rstr="R";
    cout << "dump tree:\n";

    while( que.size())
    {
        Node* n = que.front();
        string str = que2.front();

        que.pop_front();
        que2.pop_front();

        printf("\t%d%c", n->key, n->color==RED? 'R':'B' );

        cout<< "p=";
        if( n->parent )
            cout<< n->parent->key << strend.c_str();
        else
            cout<< strnull.c_str() << strend.c_str();

        printf("%s", str.c_str() );

        tmp = tmp2 + to_string( n->key )+ ( n->is_red()?rstr:bstr) + ")";
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
*/

void inorder_tree( RBTree* tree, Node* n, int de, std::deque<Node*>& que )
{
    if(n) 
    {
        inorder_tree(tree, n->left, de, que);
        if( tree->degree(n) == de )
            que.push_back(n);
        inorder_tree(tree, n->right,de, que);
    }
}

void printRBTree( RBTree* tree )
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
                    printf("\t%d(%c)[%d]/", n->key, n->color==RED? 'R':'B', n->parent->key );
                else if( n->parent->left ) //parent has both children.
                    printf(" \\%d(%c)[%d]", n->key, n->color==RED? 'R':'B', n->parent->key );
                else
                    printf("\t\\%d(%c)[%d]", n->key, n->color==RED? 'R':'B', n->parent->key );
            }
            else //root
                printf("\t%d(%c)[root]", n->key, n->color==RED? 'R':'B');
        }
        cout << endl;
    }
    cout << "\n*****end dump tree*********:\n";
}

int main( int arc, char** arv )
{
    //int key; 
    //int size = 20;

    RBTree rbtree;
    cout << "insert key to tree: "<<endl;
    //vector<int> vec;
    vector<int> vec = {42,19,39,39,45,9,16,38,13,27,23,31,18,25,47,45,31,35,7,30 };
    //for( int i = 0; i< size; i++)
    for( auto key:vec )
    {
        //key = (int) ((double)rand()/(double)RAND_MAX * 50 );
        //vec.push_back( key );
        
        cout <<"insert key:"<< key << "  " <<endl;
        Node* p = new Node( key, NULL );
        rbtree.insert( rbtree.root, p ); 
        printRBTree( &rbtree);
    }

    cout << endl;
    for( auto x:vec )
        cout<< x <<",";
    cout << endl;

    cout << "\n**********\nend insert:\n\n " <<endl;
    printRBTree( &rbtree);

    Node* ptmp = rbtree.max( NULL);
    cout << "\ntree max is: "<< ptmp->key  <<endl;

    ptmp = rbtree.min( NULL);
    cout << "\ntree min is: "<< ptmp->key  <<endl;

    cout << "\nheight of tree : " << rbtree.height( rbtree.root ) <<endl; 

    cout << "\nsearch tree for:"<<endl;
    ptmp = rbtree.search( rbtree.root, 9 );
    if( ptmp )
        cout << "\nsearch result is: "<< ptmp->key  <<endl;
    else
        cout << "\nsearch result is: NULL"  <<endl;

    int d = 27;
    cout << "\n**********\ninput key to been deleted, non-digtal exit:\n\n " <<endl;
    while( cin>>d)
    {
        cout<<"\n\n delelte node: key: "<< d <<endl;
        ptmp = rbtree.Remove( d );
        if( ptmp )
            delete ptmp;
        cout<<"\n\n***************\n result delelte node: key: "<< d <<endl;
        printRBTree(&rbtree);
    }
    return 0;
}

