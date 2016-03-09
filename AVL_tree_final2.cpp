#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <iostream>
#include <deque>
#include <string>
#include <assert.h>


/**
 *  Wuzhilong
 *  This is the final version AVL tree implemented. 
 *  Testing worked well for insert/remove.
 * Fri Feb 5 22:02:04 CST 2016
 */
//g++ -std=c++11 AVL_tree.cpp

/*
 * In an AVL tree, the heights of the two child subtrees of any node differ by at most one.
 * Lookup, insertion, and deletion all take O(log n) time in both the average and worst cases.
 */
using namespace std;
class AVLTree;
void printAVLTree( AVLTree* tree );

class Node
{
public:
    int key;
    int data;
    int h; //height of this node's sub tree. h= max( left, right) +1.
    Node* parent;
    Node* left; 
    Node* right;

    Node():parent(NULL), left(NULL),right(NULL),key(0), h(1) { };
    Node( int k, Node* p ):parent(p), left(NULL),right(NULL), key(k),h(1){ };

    //*****************************  for debug ******************************/
    friend ostream& operator << (ostream& os, Node& n )
    {
        os<<"\tN:"<< n.key<<"(h=" <<n.h<< ",p=";
        if( n.parent )
        {
            os<< n.parent->key << ")";
            if( &n == n.parent->left )
                os<< "/";
            else
                os<< "\\";
        }
        else //root
            os<< "(root)";
        os<<"[L:";
        if(n.left)
            os<<n.left->key;
        os<<",R:";
        if(n.right)
            os<<n.right->key;
        os<<"]";
        return os;
    }
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

class AVLTree
{
public:
    Node* root; 
public:
    AVLTree(): root(NULL) {};
public:
    int degree( Node* n  )
    {
        if(!n)
            return 0;
        //cout<<"degree, N:"<< *n<<endl;
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
            //cout << "height of node: "<< *n <<endl;
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
            //cout<<" insert at root: key="<<child->key <<endl;
            return root;
        }

        Node* tmp=p;
        if( p->key == child->key )
        {
            //cout<<"key already exists, key ="<<child->key <<endl;
            return p;
        }
        else if( p->key > child->key )
        {
            if( !(p->left) )
            {
                p->left = child;
                if( !(p->right)) //p has only this child now.
                    p->h++;
                child->parent = p;
             //   cout<<" insert under left of parent(" <<p->key<< "), key="<< child->key  <<endl;
            }
            else
            {
                insert( p->left, child );
                
                //add for AVL, after inserted. it goes up-toward for fixing the balance.
                int lh =  p->left?p->left->h:0;
                int rh = (p->right)? p->right->h:0 ;
                int balance= lh - rh;
                if( balance >= 2) //during rotate, the height of the nodes which are involved in  is recalcuate.
                {
                    if( p->left->key > child->key) // left-left rotate.
                    {
                       tmp = rotate_left_left( p ); 
                    }
                    else //left-right.
                    {
                       tmp = rotate_left_right( p ); 
                    }
                }
                else
                    p->h=1+ ( ( lh > rh)?lh: rh);
            }
        }
        else //if( p->key < child->key )
        {
            if( !(p->right ))
            {
                p->right = child;
                if( !(p->left)) //p has only this child now.
                    p->h++;
                child->parent = p;
              //  cout<<" insert under right of parent("<< p->key <<"), key="<< child->key  <<endl;
            }
            else
            {
                insert( p->right, child );
 
                //add for AVL, after inserted. it goes up-toward for fixing the balance.
                int lh =  p->left?p->left->h:0;
                int rh = (p->right)? p->right->h:0 ;
                int balance= rh - lh;
                if( balance >= 2) //during rotate, the height of the nodes which are involved in  is recalcuate.
                {
                    if( p->right->key < child->key) // right-right rotate.
                    {
                       tmp = rotate_right_right( p ); 
                    }
                    else //right-left.
                    {
                       tmp = rotate_right_left( p ); 
                    }
                }
                else
                    p->h=1+ ( ( lh > rh)?lh: rh);
            }
        }
        return tmp; 
    };

     /**  
      * return the node to should be freed memory by caller.
      */
    Node* remove( int key )
    {
        if(!root )
        {
            //cout<<" remove NULL tree or NULL node!"<< endl;
            return root;
        }
        Node* ptmp = search( root, key );
        if( !ptmp)
        {
            //cout<<" key dones not exist in the tree, remove NULL."<<endl; 
            return NULL;
        }

        remove_helper( root, key );
        return NULL;
    }

     /**  
      * return the node to should be freed memory by caller.
      */
    Node* remove_helper( Node* p, int key )
    {
        if( !p )
            return NULL;

        Node* free_node=NULL;
        Node* ptmp = p;
        if( p->key > key )
        {
            // go to left subtree.
            cout<<"remove_helper: delete key="<< key <<"node:"<< *p <<"  >> go to left subtree: "<<endl;
            p->left = remove_helper( p->left, key );
            //int lh =  p->left?p->left->h:0;
            //int rh = (p->right)? p->right->h:0 ;
            int lh =  height( p->left );
            int rh =  height( p->right );
            int balance= lh - rh;
            p->h=1+ ( ( lh > rh)?lh: rh);
           
            if( balance >= 2) // need to do left rotate.
            {
                //since balance >2, p->left must be valid.
                assert( p->left );
                int child_balance =  (p->left->left?( p->left->left->h) :0) - (p->left->right?( p->left->right->h) :0);

                if( child_balance > 0) // left-left rotate.
                {
                   ptmp = rotate_left_left( p ); 
                }
                else //left-right.
                {
                   ptmp = rotate_left_right( p ); 
                }
            }
       
        }
        else if( p->key < key )
        {
            // go to right subtree.
            cout<<"remove_helper: delete key="<< key <<"node:"<< *p <<"  >> go to right subtree: "<<endl;
            p->right = remove_helper( p->right, key );
            //int lh =  p->left?p->left->h:0;
            //int rh = (p->right)? p->right->h:0 ;
            int lh =  height( p->left );
            int rh =  height( p->right );
            int balance= lh - rh;
            p->h=1+ ( ( lh > rh)?lh: rh);

            if( balance <= -2) //during rotate, all involved in node's height is recalcuate.
            {
                //since balance >2, p->left must be valid.
                assert( p->right );
                int child_balance =  (p->right->left?( p->right->left->h) :0) - (p->right->right?( p->right->right->h) :0);
                if( child_balance<0) // right-right rotate.
                {
                   ptmp = rotate_right_right( p ); 
                }
                else //right-left.
                {
                    ptmp = rotate_right_left( p ); 
                }
            }
        }
        else // hit the key in this Node. p->key== key
        {
            cout<<"remove_helper: delete key="<< key <<"node:"<< *p <<"  find the node. "<<endl;
            p->right = remove_helper( p->right, key );
            //1. if it is a leaf. simply remove it, and set its parent to NULL.
            if( !ptmp->left && !ptmp->right )
            {
                cout<<"remove leaf:"<< *ptmp <<endl;
                free_node = transplant( ptmp, NULL );
            }
            //2. if it has only a right child. replace this ptmp with the its right child.
            else if( !ptmp->left && ptmp->right )
            {
                cout<<"remove node:"<< *ptmp<< "replace with Right child:"<< *(ptmp->right) <<endl;
               free_node =  transplant( ptmp, ptmp->right );
            }
            //3. if it has only a left child. replace this ptmp with its left child .
            else if( ptmp->left && !ptmp->right )
            {
                cout<<"remove node:"<< *ptmp<< "replace with Left child:"<< *(ptmp->left) <<endl;
               free_node =  transplant( ptmp, ptmp->left );
            }
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

                 x is min(z), x must have no left child.
           */
            //4. if it has both left and right child. replace this ptmp with predecessor.
            else if( ptmp->left && ptmp->right )
            {
                //4.1 its successor is not its right child. rotate this successor's right child to this successor's parent's left.
                //Node* psuc =  successor(ptmp);
                Node* pmin = min( ptmp->right ); // we find out the min of this target node's right spine.
                cout<<"remove node: key="<< key<< *ptmp<< "has both child, L:"<< *(ptmp->left)<< " R:"<< *(ptmp->right)<< "Min:"<< *pmin <<endl;
                int min=pmin->key; 
                p->key = pmin->key; //replace this target node's key with its successor.
                //4.2 its successor is its right child. same as case2.
                
                ptmp->right = remove_helper( ptmp->right, min );

                //int lh =  p->left?p->left->h:0;
                //int rh = (p->right)? p->right->h:0 ;
                int lh =  height( p->left );
                int rh =  height( p->right );
                int balance= lh - rh;
                p->h=1+ ( ( lh > rh)?lh: rh);
                if( balance <= -2) //during rotate, all involved in node's height is recalcuate.
                {
                    //since balance >2, p->left must be valid.
                    assert( p->right );
                    int child_balance =  (p->right->left?( p->right->left->h) :0) - (p->right->right?( p->right->right->h) :0);
                    if( child_balance<0) // right-right rotate.
                    {
                        ptmp = rotate_right_right( p ); 
                    }
                    else //right-left.
                    {
                        ptmp = rotate_right_left( p ); 
                    }
                }
            }
        }

        if( free_node )
        {
            delete free_node;
            free_node = NULL;
        }
        ptmp->h = height( ptmp );

        return ptmp;
    };

private:

    /**
     * balance the subtree based from the given starting Node p as the root via rotation. 
     * return the balanced sub tree's root node.
     */
    Node* balance_tree( Node* p)
    {
        if( !p )
            return NULL;
        //cout<< "balance_tree, Node:"<<*p;

        Node* tmp = p;
        int lh =  p->left?p->left->h:0;
        int rh = (p->right)? p->right->h:0 ;
        //int lh =  height( p->left );
        //int rh =  height( p->right );
        int balance= lh - rh;
        p->h=1+ ( ( lh > rh)?lh: rh);
       
        if( balance >= 2) // need to do left rotate.
        {
            //since balance >2, p->left must be valid.
            assert( p->left );
            int child_balance =  (p->left->left?( p->left->left->h) :0) - (p->left->right?( p->left->right->h) :0);

            if( child_balance > 0) // left-left rotate.
            {
               tmp = rotate_left_left( p ); 
            }
            else //left-right.
            {
               tmp = rotate_left_right( p ); 
            }
        }
        else if( balance <= -2) //during rotate, all involved in node's height is recalcuate.
        {
            //since balance >2, p->left must be valid.
            assert( p->right );
            int child_balance =  (p->right->left?( p->right->left->h) :0) - (p->right->right?( p->right->right->h) :0);
            if( child_balance<0) // right-right rotate.
            {
               tmp = rotate_right_right( p ); 
            }
            else //right-left.
            {
               tmp = rotate_right_left( p ); 
            }
        }
        return tmp;

        if( (balance >= 2) || ( balance <= -2) ) //after do balance for this parent, then balance its children. 
        {
            //go banlance its left & right subtree recursively, after two children were balanced..
            if( tmp->left )
            {
         //       cout<< "after balance sub tree: " << *tmp <<" balance its left child: "<< *(tmp->left) <<endl;
                balance_tree( tmp->left);
            }
            if( tmp->right )
            {
          //      cout<< "after balance sub tree: " << *tmp <<" balance its right child: "<< *(tmp->right) <<endl;
                balance_tree( tmp->right);
            }
        }
        return tmp;
    };

 
    /**
     * helper, to transplant  subtree new_node into the old_node place.
     *  both children of old_node will not been move to new node. 
     */
    Node* transplant( Node* old_node, Node* new_node )
    {
        return replace_node( old_node, new_node);
        if( old_node )
            cout<< "before transplant: with old: "<< *old_node;
        else
            cout<< "before transplant:  with old: NULL ";
        if( new_node )
            cout<< ", with new: "<< *new_node<<endl;
        else
            cout<< ", with new: NULL"<<endl;

        if( !(old_node->parent) )  //old node is the root.
            root = new_node;
        else if( old_node->parent->left == old_node )
            old_node->parent->left = new_node;
        else    //if( old_node->parent->right == old_node )
            old_node->parent->right = new_node;

        if( old_node->left)
            old_node->left->parent = new_node;
        if( old_node->right)
            old_node->right->parent = new_node;

        if( new_node)
            new_node->parent = old_node->parent; // must be the last to change,otherwise when new is old's child. it will be new->=new. 

        if( old_node )
            cout<< "after transplant: with old: "<< *old_node;
        else
            cout<< "after transplant:  with old: NULL ";
        if( new_node )
            cout<< ", with new: "<< *new_node<<endl;
        else
            cout<< ", with new: NULL"<<endl;
            
        return old_node;
    };

    /**
     * helper, to replace new_node into the old_node place.
     * both children of old_node will been replalce with new node. 
     */
    Node* replace_node( Node* old_node, Node* new_node )
    {
        if( !old_node  )
            return old_node;
        //cout<< "before replace_node: old:"<< *old_node <<"new:"<<*new_node<<endl;
        if( !(old_node->parent) )
            root = new_node;

        if(!new_node)  //replace with NULL, only happens on leaf
        {
            assert( !(old_node->left || old_node->right) ); 
            if( old_node->parent->left == old_node)  
                old_node->parent->left = NULL;
            else
                old_node->parent->right = NULL;
          //  old_node->parent->h = height( old_node->parent);
         //   cout<< "replace_node: set old to NULL.return!"<<endl;
            return old_node;
        }

        if( old_node->parent == new_node) // new_node is parent,only replace children 
        {
          //  cout<<"replace_node, o->p = n " <<endl;
            new_node->right = old_node->right;
            if( new_node->right )
                new_node->right->parent = new_node;
            new_node->left = old_node->left;
            if( new_node->left )
                new_node->left->parent = new_node;
        }
        else if( old_node->right == new_node  ) // old->right==new
        {
           // cout<<"replace_node, o->r = n " <<endl;
            new_node->left = old_node->left;
            if( new_node->left )
                new_node->left->parent = new_node;

            new_node->parent = old_node->parent;
            if( old_node->parent && old_node->parent->left == old_node)  
                old_node->parent->left = new_node;
            else if( old_node->parent && old_node->parent->right == old_node)  
                old_node->parent->right = new_node;
        }
        else if( old_node->left == new_node  ) //old->left==new
        {
            //cout<<"replace_node, o->l = n " <<endl;
            new_node->right = old_node->right;
            if( new_node->right )
                new_node->right->parent = new_node;

            new_node->parent = old_node->parent;
            if( old_node->parent && old_node->parent->left == old_node)  
                old_node->parent->left = new_node;
            else if( old_node->parent && old_node->parent->right == old_node)  
                old_node->parent->right = new_node;
        }
        else  // normal case, not relationship between two nodes.
        {
            //cout<<"replace_node, normal case, n != o" <<endl;
            new_node->parent = old_node->parent;
            new_node->right = old_node->right;
            new_node->left = old_node->left;
            if( new_node->right )
                new_node->right->parent = new_node;
            if( new_node->left )
                new_node->left->parent = new_node;

            if( old_node->parent && old_node->parent->left == old_node)  
                old_node->parent->left = new_node;
            else if( old_node->parent && old_node->parent->right == old_node)  
                old_node->parent->right = new_node;
        }
        //new_node->h = old_node->h;
//        new_node->h = height(new_node);
        //cout<< "after replace_node: old:"<< *old_node <<"new:"<<*new_node<<endl;
        return old_node;
    };


 /**key(subtree):  A >B >C 
  *
              A  
             / \
            B   z             B  
           / \              /   \
          C   y    ==>     C     A 
         / \              / \   / \ 
        w   x            w  x   y  z 
     */
    
    Node* rotate_left_left( Node* A)
    {
        //if( A->left && A->left->left)
         //   return A;
        //cout<< "\nrotate_left_left, before: "<<*A<<endl;
        //printAVLTree( this );

        Node* B= A->left;
        assert( B );
        Node* y= B->right;
        Node* C= B->left;
        assert( C );

        B->parent = A->parent;
        if( A->parent && A->parent->left == A )
            A->parent->left = B;
        else if( A->parent && A->parent->right == A )
            A->parent->right = B;

        A->parent = B;
        A->left = y;
        B->right = A;
        C->parent = B;
        B->left = C;
        if(y) y->parent = A;

        //here, height(C) do not change.
        A->h = height(A);
        B->h = 1+((A->h > C->h)? A->h: C->h );

        if(A==root)
            root=B;
    
        //cout<< "\nrotate_left_left, after: "<<*A<<endl;
        //printAVLTree( this );
        return B;
    }

 /**key(subtree):  A >B >C 
  *
                  A                          A  
                 / \                        / \
                C   z     left_right       B   z   left-left  B  
               / \        ==>             / \               /   \
              w   B                      C   y    ==>      C     A 
                 / \                    / \               / \   / \ 
                x   y                  w   x             w  x   y  z 
     */
    Node* rotate_left_right( Node* A)
    {
       //if( A->left && A->left->right)
        //    return A;
        //cout<< "\nrotate_left_right, before: "<<*A<<endl; 
        //printAVLTree( this );

        Node* C= A->left;
        assert( C );
        Node* B= C->right;
        assert( B );
        Node* x= B->left;

        B->parent = A; 
        B->left = C;
        C->parent = B;
        C->right = x;
        A->left = B;
        if(x) x->parent = C;

        //here, height(A) do not change.
        A->h=height( A );
        B->h=height( B );
        C->h=height( C );

        //B->h = ( C->h > ( y?y->h:0) )? C->h: ( y?y->h:0) + 1;
        //A->h = ( B->h > ( z?z->h:0) )? B->h: ( z?z->h:0) + 1;

        //cout<< "\nrotate_left_right, after: "<<*A<<endl; 
        //printAVLTree( this );

        Node* ret = rotate_left_left( A );
        if(A==root)
            root=B;
        return ret;
    }


 /**key(subtree):  A >B >C 
  *
              C  
             / \
            w   B                      B  
               / \                   /   \
              x   A         ==>     C     A 
                 / \               / \   / \ 
                y   z             w  x   y  z 
     */
    
    Node* rotate_right_right( Node* C)
    {
        //if( C->right && C->right->right)
         //   return C;
        //cout<< "\nrotate_right_right, before: "<<*C<<endl;
        //printAVLTree( this );

        Node* B= C->right;
        assert( B );
        Node* x= B->left;
        Node* A= B->right;
        assert( A );

        if( C->parent && C->parent->left == C )
            C->parent->left = B;
        else if( C->parent && C->parent->right == C )
            C->parent->right = B;
        B->parent = C->parent;
        B->left = C;
        C->parent = B;
        C->right = x;
        if(x) x->parent = C;

        //here, height(A) will not change.
        C->h=height( C );
        B->h = 1+ (( C->h > A->h) ? C->h: A->h) ;

        //C->h = height(C);
        ////B->h = 1+((A->h > C->h)? A->h: C->h );

        if(C==root)
            root=B;
        //cout<< "\nrotate_right_right, after: "<<*C<<endl;
        //printAVLTree( this );
        return B;
    }

 /**key(subtree):  A >B >C 
       C                C               
      / \              / \                           B  
     w   A            w   B                        /   \
        / \              / \             ==>      C     A 
       B   z  ==>       x   A                    / \   / \ 
      / \                  / \                  w  x   y  z 
     x   y                y   z  

         */
    Node* rotate_right_left( Node* C)
    {
        //if( C->right && C->right->left)
         //   return C;
        //cout<< "\nrotate_right_left, before: "<<*C<<endl;
        //printAVLTree( this );

        Node* A= C->right;
        assert( A );
        Node* B= A->left;
        assert( B );
        Node* y= B->right;

        B->parent = C;
        B->right = A;
        A->left = y;
        A->parent = B;
        C->right = B;
        if(y) y->parent = A;

        //here, height(A) will not change.
        A->h=height( A );
        B->h=height( B );
        C->h=height( C );

        //B->h = ( A->h > ( x?x->h:0) )? A->h: ( x?x->h:0) + 1;
        //C->h = ( B->h > ( w?w->h:0) )? B->h: ( w?w->h:0) + 1;

        //cout<< "\nrotate_right_left, after: "<<*C<<endl;
        //printAVLTree( this );

        Node* ret = rotate_right_right( C );
        if(C==root)
            root=B;
        return ret;
    }
};

/******************************************************
 *           test code 
******************************************************/

/*
void printAVLTree( AVLTree* tree )
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
    cout << "dump tree:\n";

    while( que.size())
    {
        Node* n = que.front();
        string str = que2.front();

        que.pop_front();
        que2.pop_front();

        printf("\t%d(h=%d,", n->key, n->h );

        cout<< "p=";
        if( n->parent )
            cout<< n->parent->key << strend.c_str();
        else
            cout<< strnull.c_str() << strend.c_str();


        printf("%s", str.c_str() );

        tmp = tmp2 + to_string( n->key )+  ")";
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

void inorder_tree( AVLTree* tree, Node* n, int de, std::deque<Node*>& que )
{
    if(n) 
    {
        inorder_tree(tree, n->left, de, que);
        if( tree->degree(n) == de )
            que.push_back(n);
        inorder_tree(tree, n->right,de, que);
    }
}

void validate_tree( AVLTree* tree, Node* n )
{
    if(n) 
    {
       // cout<< n->key <<endl; 
        int hl = tree->height( n->left );
        int hr = tree->height( n->right); 
        int h = tree->height( n); 
        int b = abs(hl-hr ); 

        assert( h==n->h);
        //cout<<"validate_tree:"<< *n<<" hl="<<hl<<", hr="<<hr<<" ,b= "<< b<<endl;
        assert( b<2 );
        if( n->left )
        {
            Node* pre_max= tree->max( n->left );
            //assert( n->key > pre_max);
            assert( n->key > n->left->key);
            assert( n->left->parent == n );
        }
        if( n->right )
        {
            //int suc_min = tree->min( n->right );
            //assert( n->key >suc_min);
            assert( n->key < n->right->key);
            assert( n->right->parent == n );
        }
        validate_tree(tree, n->left);
        validate_tree(tree, n->right);
    }
}

void printAVLTree( AVLTree* tree )
{
        //return;
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
        //cout<<"degree"<< i <<"que size:\t"<<que.size()<<endl;
        while( que.size())
        {
            Node* n = que.front();
            assert( n );
            que.pop_front();
            if( n->parent )
            {
                if( n == n->parent->left )
                    printf("\t%d(h:%d)[p:%d]/", n->key,n->h, n->parent->key );
                else if( n->parent->left ) //parent has both children.
                    printf(" \\%d(h:%d)[p:%d]", n->key,n->h, n->parent->key );
                else
                    printf("\t\\%d(h:%d)[p:%d]", n->key,n->h, n->parent->key );
            }
            else //root
                printf("\t%d(h:%d) [root]", n->key,n->h );
        }
        cout << endl;
    }
    cout << "\n*****end dump tree*********:\n";
}

int main( int arc, char** arv )
{
    int key; 
    int size = 2000000;
    
    int d;
    Node* ptmp;

    AVLTree avltree;
    cout << "insert key to tree: "<<endl;
    //vector<int> vec;
    vector<int> vec = { 32,98,195,105,199,49,83,192,69,138,119,157,91,128,158,179,35,151,4,60,34,39,100,27,54,153,74,159,131,123,73,70,88,17,21,48,165,87,16,5,114,15,59,66,134,93,190,166,132,9,109,180,71,184,171,41,110,82,57,147,164,99,120,53,36,160,107,154,196,76,111,56,46,139,104,42,25,31,95,187,92,58,146,61,38,183,198,186,18,13,130,44,189,23,33,19,51,115}; //143,188,12,1,148,45,40,97,89,144,113,24,8,185,124,162,62,175,79,55,127,136,28,117,112,84,108,0,86,149,168,178,155,10,103,173,152,156,182,50,163,64,133,65,90,72,193,47,191,174,30,137,68,181,150,81,37,26,161,22,106,145,121,129,102,176,52,118,75,29,194,77,78,96,3,200,169,141,85,125,122,101,14,116,140,2,43,170,67,80,177,126,20,94,167,11,197,7,135,6,142,172,63};


    int i=0;
    size = vec.size();
    //for( i = 0; i< size; )
    for( auto k:vec )
    {
        //int k= (int) ((double)rand()/(double)RAND_MAX * (size * 1.1 ) );
        
        ptmp = avltree.search( avltree.root, k );
        if( !ptmp )
        {
            //cout <<"insert key:"<< k << " , Number: "<< i <<endl;
            Node* p = new Node( k, NULL );
            avltree.insert( avltree.root, p ); 

           // printAVLTree( &avltree);
           // validate_tree( &avltree, avltree.root );
            i++;
        }
    }
    /*vector<int> vec1;
    //int i=1;
    for( int i = 0; i< size; )
    {
        key = (int) ((double)rand()/(double)RAND_MAX * (size+10) );
        
        ptmp = avltree.search( avltree.root, key );
        if( ptmp )
        {
             vec1.push_back( key );
            //cout <<"insert key:"<< key << "  " <<endl;
           // Node* p = new Node( key, NULL );
            //avltree.insert( avltree.root, p ); 
            i++;
        }
    }

    for( auto x:vec1 )
        cout<< x <<",";
    cout << endl;
    */

    printAVLTree( &avltree);
    validate_tree( &avltree, avltree.root );
    cout << "\n**********\nend insert: "<< i <<" keys inserted. \n\n " <<endl;
/*
    Node* ptmp = avltree.max( NULL);
    cout << "\ntree max is: "<< ptmp->key  <<endl;

    ptmp = avltree.min( NULL);
    cout << "\ntree min is: "<< ptmp->key  <<endl;

    cout << "\nroot of tree : " << avltree.root->key <<endl; 

    cout << "\nheight of tree : " << avltree.height( avltree.root ) <<endl; 

    cout << "\nsearch tree for:"<<endl;
    ptmp = avltree.search( avltree.root, 9 );
    if( ptmp )
        cout << "\nsearch result is: "<< ptmp->key  <<endl;
    else
        cout << "\nsearch result is: NULL"  <<endl;

    int d = 16;
    cout<<"\n\n delelte node: key: "<< d <<endl;
    ptmp = avltree.remove( d );
    if( ptmp )
        delete ptmp;
    printAVLTree(&avltree);

    cout << "\nheight of tree : " << avltree.height( avltree.root) <<endl; 
    */

    //vector<int> vec1={ 177,83,165,168,192,41,70,162,58,116,100,132,76,108,200,193,134,151,29,128,3,51,28,169,33,84,27,22,46,108,177,129,62,134,110,104,61,162,111,162,84,188,59,74,170,193,14,200,110,18,40,139,187,73,13,4,96,13,50,190,179,56,113,79,160,108,140,112,8,92,196,196,152,59,155,135,74,145,35,92,185,174,69,48,188,73,144,124,138,181,92,194,84,171,144,192,101,45,200,194,31,185,135,91,130,59,165,64,94,47,39,58,117,87,35,191,21,26,104,160,197,144,80,158,77,62,49,123,51,32,154,26,167,34,157,15,200,11,110,37,50,168,154,138,134,160,19,28,109,16,14,43,97,172,120,159,10,33,43,187,26,11,183,15,0,194,125,38,34,82,192,172,75,116,122,95,145,21,112,159,64,121,185,157,132,7,157,175,195,184,175,156,190,140,104,34,175,187,16,137,52};

    vector<int> vec1={ 146,16,25,31,13,4,70,46,153,184,190,164,21,9,23,130,99,196,157,110,98,39,91,187,158,5,119,69,115,27,109,61,147,123,53,180,93,19,159,104,132,139,165,186,54,88,92,18,199,189,166,83,35,36,183,17,105,32,66,192,74,33,38,42,131,171,114,76,71,160,134,49,15,44,198,107,34,154,82,100,120,128,87,58,56,60,179,95,73,57,51,59,48,111,138,41,195,151 };

    d = 0;
  /*  for( auto x:vec1 )
    {
        for( auto y:vec )
            if(x==y)
            {
                cout<< "x = "<<x<< " y= "<< y<<endl;
                d++;
            }
    }
    */
    cout<<"vector1 contains number: "<< d <<"  keys of vec"<<endl;

    cout << "\n**********\ninput key to been deleted, non-digtal exit:\n\n " <<endl;
    int x;
    while( cin>>d){ if( d == 5) break; };

    d = 1;
    int r=1;
    for( auto k:vec1 )
    //for(int j = 0; j< size; )
    {
        //int k = (int) ((double)rand()/(double)RAND_MAX * (size*1.2) );
        /*if( j >= vec.size()) 
            j = vec.size() -1;
            */
        //x=vec1[j];
        //vec.erase( vec.begin() + j );
        //vec1.push_back(x);
        
        //cout<<"going to delete, running number: "<< d <<" delete keys: "<< x <<endl;

        ptmp = avltree.remove( k );
        if( ptmp )
        {
            delete ptmp;
         //   j++;
            r++;
        }
        d++;
        //cout<<"\n\n after delete, before verify!!! running number: "<< d <<","<<r<<" keys being removed" <<" ,deleted key="<< x <<endl;
        //printAVLTree(&avltree);
        validate_tree(&avltree, avltree.root);
        cout<<"\n\n successfully delete!!!! running NO.: "<< d <<", "<<r<<" keys being removed" <<" ,deleted key="<< k <<endl;
    }
 
    cout<<"running number: "<< d <<" delete keys: "<<r <<endl;
    cout << "\n\n\n**********\n Exit beautifully:\n\n " <<endl;
    return 0;
}
