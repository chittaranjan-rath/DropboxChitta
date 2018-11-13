#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <queue>
#include <cstring>
#include <map>
#include <chrono> 
using namespace std::chrono;
using namespace std;
struct avlnode{
    pair<int,pair<int,int>> data;
    struct avlnode * left;
    struct avlnode * right;
    int height;                 //helpful for balance factor calculation
};
int findHeight(struct avlnode * node){
    if(!node)
        return 0;
    else
        return node->height;
}
struct avlnode * leftRotate (struct avlnode * node){
    struct avlnode * newRoot = node->right;
    struct avlnode * temp = newRoot->left;

    //Rotate
    newRoot->left = node;
    node->right = temp;

    //assign new heights
   
    node->height = 1 + max(findHeight(node->left),findHeight(node->right));
    newRoot->height = 1 + max(findHeight(newRoot->left),findHeight(newRoot->right));

    return newRoot;
}
struct avlnode * rightRotate (struct avlnode * node){
    struct avlnode * newRoot = node->left;
    struct avlnode * temp = newRoot->right;

    //Rotate
    newRoot->right = node;
    node->left = temp;

    //assign new heights
    node->height = 1 + max(findHeight(node->left),findHeight(node->right));
    newRoot->height = 1 + max(findHeight(newRoot->left),findHeight(newRoot->right));

    return newRoot;
}
struct avlnode * insert(struct avlnode *node ,pair<int,pair<int,int>> value){
    if(node==NULL){
        struct avlnode * temp =(struct avlnode *)malloc(sizeof(struct avlnode ));
        temp->data = value;
        temp->left = NULL;
        temp->right = NULL;
        temp->height = 1;
        return temp;
    }
    
    if(node->data > value)
        node->left = insert(node->left, value);
    else if(node->data <= value)
        node->right = insert(node->right, value);
    //else   
        //return node;
    
    node->height = 1 + max(findHeight(node->left),findHeight(node->right));
   
    int balanceFactor = node? (findHeight(node->left) - findHeight(node->right)):0;
    
    // left left rotation
    if(balanceFactor > 1 && value < node->left->data )
        return rightRotate(node);
    // left right rotation
    if(balanceFactor > 1 && value > node->left->data ){
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    // right right rotation
    if(balanceFactor < -1 && value > node->right->data)
        return leftRotate(node);
    // right left rotation    
    if(balanceFactor < -1 && value < node->right->data){
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}
/*void inorder(struct avlnode * node){
    if(node->left)
        inorder(node->left);
    printf("%d ",(node->data).first);
    if(node->right)
        inorder(node->right);
}*/
struct avlnode * findInorderSuccessor(struct avlnode * node){
    if(node->left)
        return findInorderSuccessor(node->left);
    return node;
}
/*struct avlnode * deleteNode(struct avlnode * node ,pair<int,pair<int,int>> value){
    if(!node)
        return NULL;
    if(node->data > value)
        node->left = deleteNode(node->left , value);
    else if(node->data < value)
        node->right = deleteNode(node->right ,value);
    else{
        
        struct avlnode *temp = node;
        if(node->right == NULL && node->left == NULL){
            node = NULL;
            free(temp);
            
        }
        else if(node->right == NULL && node->left != NULL){
            node = node->left;
            free(temp);
        }
        else if(node->right != NULL && node->left == NULL){
            node = node->right;
            free(temp);
        }
        else{
            temp = findInorderSuccessor(node->right);
            //cout<<"inoder succ of "<<(node->data).first<<" is "<<(temp->data).first<<endl;
            node->data = temp->data;
            node->right = deleteNode(node->right , temp->data);
        }
    }
    if(!node)
        return node;
    node->height = 1 + max(findHeight(node->left),findHeight(node->right));
    int balanceFactor = node? (findHeight(node->left) - findHeight(node->right)):0;
    int leftBalance = node->left ? (findHeight(node->left->left) - findHeight(node->left->right)):0;
    int rightBalance = node->right ? (findHeight(node->right->left) - findHeight(node->right->right)):0;
    //left left case
    if(balanceFactor > 1 && leftBalance >= 0)
        return rightRotate(node);
    if(balanceFactor > 1 && leftBalance < 0){
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    if(balanceFactor < -1 && rightBalance <= 0)
        leftRotate(node);
     if(balanceFactor < -1 && rightBalance > 0){
         node->right = rightRotate(node->right);
         return leftRotate(node);
     }
     return node;
}*/
int height(struct avlnode *N) 
{ 
    if (N == NULL) 
        return 0; 
    return N->height; 
} 

int getBalance(struct avlnode *N) 
{ 
    if (N == NULL) 
        return 0; 
    return height(N->left) - height(N->right); 
} 

struct avlnode * minValueNode(struct avlnode * node) 
{ 
    struct avlnode * current = node; 
  
    /* loop down to find the leftmost leaf */
    while (current->left != NULL) 
        current = current->left; 
  
    return current; 
} 
  
// Recursive function to delete a node with given key 
// from subtree with given root. It returns root of 
// the modified subtree. 
struct avlnode * deleteNode(struct avlnode * root, pair<int,pair<int,int>> value) 
{ 
    // STEP 1: PERFORM STANDARD BST DELETE 
  
    if (root == NULL) 
        return root; 
    
    // If the key to be deleted is smaller than the 
    // root's key, then it lies in left subtree 
    if ( value < root->data ) 
        root->left = deleteNode(root->left, value); 
  
    // If the key to be deleted is greater than the 
    // root's key, then it lies in right subtree 
    else if( value > root->data ) 
        root->right = deleteNode(root->right, value); 
  
    // if key is same as root's key, then This is 
    // the node to be deleted 
    else
    { 
        // node with only one child or no child 
        if( (root->left == NULL) || (root->right == NULL) ) 
        { 
            struct avlnode  *temp = root->left ? root->left : 
                                             root->right; 
  
            // No child case 
            if (temp == NULL) 
            { 
                temp = root; 
                root = NULL; 
            } 
            else // One child case 
             *root = *temp; // Copy the contents of 
                            // the non-empty child 
            free(temp); 
        } 
        else
        { 
            // node with two children: Get the inorder 
            // successor (smallest in the right subtree) 
            struct avlnode * temp = minValueNode(root->right); 
  
            // Copy the inorder successor's data to this node 
            root->data = temp->data; 
  
            // Delete the inorder successor 
            root->right = deleteNode(root->right, temp->data); 
        } 
    } 
  
    // If the tree had only one node then return 
    if (root == NULL) 
      return root; 
  
    // STEP 2: UPDATE HEIGHT OF THE CURRENT NODE 
    root->height = 1 + max(height(root->left), 
                           height(root->right)); 
  
    // STEP 3: GET THE BALANCE FACTOR OF THIS NODE (to 
    // check whether this node became unbalanced) 
    int balance = getBalance(root); 
  
    // If this node becomes unbalanced, then there are 4 cases 
  
    // Left Left Case 
    if (balance > 1 && getBalance(root->left) >= 0) 
        return rightRotate(root); 
  
    // Left Right Case 
    if (balance > 1 && getBalance(root->left) < 0) 
    { 
        root->left =  leftRotate(root->left); 
        return rightRotate(root); 
    } 
  
    // Right Right Case 
    if (balance < -1 && getBalance(root->right) <= 0) 
        return leftRotate(root); 
  
    // Right Left Case 
    if (balance < -1 && getBalance(root->right) > 0) 
    { 
        root->right = rightRotate(root->right); 
        return leftRotate(root); 
    } 
  
    return root; 
} 



void levelorder (struct avlnode *root){
    struct avlnode * temp = root;
    queue<struct avlnode *> q;
    q.push(temp);
    while(!q.empty()){
        temp = q.front();
        q.pop();
        cout<<(temp->data).first<<" ";
        if(temp->left)
            q.push(temp->left);
        if(temp->right)
            q.push(temp->right);
    }
}
pair<int,pair<int,int>> findmin(struct avlnode *root){
    pair<int,pair<int,int>> p;
    struct avlnode * temp = root;
    if(temp==NULL)
        return p;
    if(temp->left)
        return findmin(temp->left);
    return temp->data;
    
}
int search(struct avlnode *root ,int valtosearch){
    pair<int,pair<int,int>> p;
    struct avlnode * temp = root;
    if(temp==NULL)
        return 0;//value not found
    p = temp->data;
    if(p.first == valtosearch)
        return 1; // value found
    return (search(root->left,valtosearch) || search(root->right,valtosearch));
}
pair<int,pair<int,int>> gp;
void inorder(struct avlnode *root){
    if(root==NULL)
        return;
    if(root->left)
        inorder(root->left);
    gp = root->data;
    cout<<gp.first<<" ";
    if(root->right)
        inorder(root->right);
}
long long mstcost=0;
int main ( int argc, char ** argv ){
    if(argc<3){
    cout<<"invalid parameters"<<endl;
    return 0;
    }
    const char *  sfile=(argv[1]);
    const char *  dfile=(argv[2]);
    auto start = high_resolution_clock::now();
    struct avlnode * root = NULL;
    freopen(sfile,"r",stdin);
    freopen(dfile,"w",stdout);
    int n;
    cin>>n;
    vector<pair<int,int>> vec[n];
    string line;
    int s,d,wt;
    int rnode;
	  rnode=0;
    while(true){
        cin>>s;
        if(cin.fail()==true)
            break;
        cin>>d;
        cin>>wt;
        vec[s].push_back(make_pair(d,wt));
        vec[d].push_back(make_pair(s,wt));
        //cout<<wt<<endl;
    }
    map <int, pair<int,int>> mapofEdges;
	vector<pair<int,pair<int,int>>> DuplicateEdgesToInsert;
    int visited[1000000];
    memset(visited,0,sizeof(visited));
    int minimumEdgeWeight ,source ,destination;
    int i;int cnt=1;
    int v;
    pair<int,pair<int,int>> mini;
    while(cnt<n){
        if(destination==-1){
            cout<<" MST not possible "<<endl;
            return 0;
        }
        pair<int,int> par;
        visited[rnode] = 1;
        for(i=0; i<vec[rnode].size();i++){
            par = vec[rnode][i];
            //cout<<"asd 1"<<endl;
            //cout<<"searching "<<par.second<<endl;
            if(search(root,par.second)==0){
                //cout<<"not found "<<par.second<<endl;
                root = insert(root, make_pair(par.second,make_pair(rnode,par.first)));
                mapofEdges[par.second] = make_pair(rnode,par.first);
            }
            else
            {
                  DuplicateEdgesToInsert.push_back(make_pair(par.second,make_pair(rnode,par.first)));
            }
            pair<int,int> tofind;
            tofind = make_pair(rnode,par.second);

            //cout<<"asd 2"<<endl;
            vector<pair<int,int>>::iterator itr;
            itr = find(vec[par.first].begin(),vec[par.first].end(),tofind);
            if(itr != vec[par.first].end()){
                    vec[par.first].erase(itr);
            }
        }
        //cout<<"inorder is "<<endl;
        //inorder(root);
        //cout<<endl;
        mini = findmin(root);
        minimumEdgeWeight = mini.first;
        root = deleteNode(root, mini);
        //cout<<endl<<"inorder after deletion 1 "<<endl;
        //inorder(root);
        source = mapofEdges[minimumEdgeWeight].first;
        destination = mapofEdges[minimumEdgeWeight].second;
	    mapofEdges[minimumEdgeWeight]=make_pair(-1,-1);
        
        for(int i=0;i<DuplicateEdgesToInsert.size();i++)
        {
            int toInsertWeight = DuplicateEdgesToInsert[i].first;
            //cout<<"searching "<<toInsertWeight<<endl;
            if(search(root,toInsertWeight)==0)
            {
                //cout<<"not found "<<toInsertWeight<<endl;
                root = insert(root, make_pair(toInsertWeight,make_pair(rnode,par.first)));
                mapofEdges[toInsertWeight] = 			
                make_pair(DuplicateEdgesToInsert[i].second.first,
                DuplicateEdgesToInsert[i].second.second);
                int x = DuplicateEdgesToInsert[i].second.first;
                int y = DuplicateEdgesToInsert[i].second.second;
                pair<int,pair<int,int>> tof = make_pair(toInsertWeight,make_pair(x,y));
                vector<pair<int,pair<int,int>>>::iterator itr;
                itr = find(DuplicateEdgesToInsert.begin(),DuplicateEdgesToInsert.end(),tof);
                if(itr != DuplicateEdgesToInsert.end()){
                    DuplicateEdgesToInsert.erase(itr);
                }
            }
	    }


        
        v = mini.second.second;
        //cout<<" destn is "<<destination<<" and visited[destn] is "<<visited[destination]<<endl;
        if(visited[destination]==0){
            cnt++;
             visited[destination] = 1;
              visited[source] =1;
            mstcost+=minimumEdgeWeight;
            cout<<"edge weight " << minimumEdgeWeight <<endl;
            cout<<"mst edge "<<source<<" "<<destination<<endl;
        }    
        else{
            while(visited[destination]==1 && DuplicateEdgesToInsert.size()>0)
	          {
                    //cout<<"inorder is "<<endl;
                    //inorder(root);
                    //cout<<endl;
                    mini = findmin(root);
                    minimumEdgeWeight = mini.first;
                    root = deleteNode(root, mini);
                    //cout<<endl<<"inorder after deeltion 2 "<<endl;
                    //inorder(root);
                    source = mapofEdges[minimumEdgeWeight].first;
                    destination = mapofEdges[minimumEdgeWeight].second;
                    mapofEdges[minimumEdgeWeight]=make_pair(-1,-1);
             
              for(int i=0;i<DuplicateEdgesToInsert.size();i++)
              {
                int toInsertWeight = DuplicateEdgesToInsert[i].first;
                
                if(search(root,toInsertWeight)==0)
                {
                  
                  //V = vEB_tree_insert(V, toInsertWeight, 1, u);
                  root = insert(root, make_pair(toInsertWeight,make_pair(rnode,par.first)));
                  mapofEdges[toInsertWeight] = 			
                  make_pair(DuplicateEdgesToInsert[i].second.first,
                  DuplicateEdgesToInsert[i].second.second);
                  int x = DuplicateEdgesToInsert[i].second.first;
                  int y = DuplicateEdgesToInsert[i].second.second;
                  pair<int,pair<int,int>> tof = make_pair(toInsertWeight,make_pair(x,y));
                    vector<pair<int,pair<int,int>>>::iterator itr;
                            itr = find(DuplicateEdgesToInsert.begin(),DuplicateEdgesToInsert.end(),tof);
                  if(itr != DuplicateEdgesToInsert.end()){
                                  DuplicateEdgesToInsert.erase(itr);
                            }
                  
                }
              }
            }
             if(visited[destination]==0){
                cnt++;
                visited[destination] = 1;
                visited[source] =1;
                mstcost+=minimumEdgeWeight;
                cout<<"edge weight " << minimumEdgeWeight <<endl;
                cout<<"mst edge "<<source<<" "<<destination<<endl;
            }    
            
        }

        
        rnode = destination;
    }
    auto stop = high_resolution_clock::now(); 
    auto duration = duration_cast<microseconds>(stop - start); 
    cout << duration.count() << endl<<"mst cost is "<< mstcost<<endl; 
    return 0;
}