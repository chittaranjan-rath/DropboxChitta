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
struct node 
{ 
    pair<int,pair<int,int>> key;
	//int key; 
	struct node *left, *right; 
}; 

// A utility function to create a new BST node 
struct node *newNode(pair<int,pair<int,int>> item) 
{ 
	struct node *temp = (struct node *)malloc(sizeof(struct node)); 
	temp->key = item; 
	temp->left = temp->right = NULL; 
	return temp; 
} 

// A utility function to do inorder traversal of BST 
void inorder(struct node *root) 
{ 
	if (root != NULL) 
	{ 
		inorder(root->left); 
		printf("%d ", (root->key).first); 
		inorder(root->right); 
	} 
} 

/* A utility function to insert a new node with given key in BST */
struct node* insert(struct node* node, pair<int,pair<int,int>> key) 
{ 
	/* If the tree is empty, return a new node */
	if (node == NULL) return newNode(key); 

	/* Otherwise, recur down the tree */
	if (key < node->key) 
		node->left = insert(node->left, key); 
	else
		node->right = insert(node->right, key); 

	/* return the (unchanged) node pointer */
	return node; 
} 

/* Given a non-empty binary search tree, return the node with minimum 
key value found in that tree. Note that the entire tree does not 
need to be searched. */
struct node * minValueNode(struct node* node) 
{ 
	if(node==NULL)
		return NULL;
	struct node* current = node; 

	/* loop down to find the leftmost leaf */
	while (current->left != NULL) 
		current = current->left; 

	return current; 
} 

int searchbyval(struct node* root,int valtosearch){
    struct node *temp = root; 
    int f=0;
    while (temp != NULL) { 
      if (valtosearch < (temp->key).first) { 
        if (temp->left == NULL) 
          break; 
        else
          temp = temp->left; 
      } 
      else if (valtosearch ==(temp->key).first) { 
        f =1;
        break; 
        
      } 
      else { 
        if (temp->right == NULL) 
          break; 
        else
          temp = temp->right; 
      } 
    } 
  
    return f;
   
    }

/* Given a binary search tree and a key, this function deletes the key 
and returns the new root */
struct node* deleteNode(struct node* root, pair<int,pair<int,int>> key) 
{ 
	// base case 
	if (root == NULL) return root; 

	// If the key to be deleted is smaller than the root's key, 
	// then it lies in left subtree 
	if (key < root->key) 
		root->left = deleteNode(root->left, key); 

	// If the key to be deleted is greater than the root's key, 
	// then it lies in right subtree 
	else if (key > root->key) 
		root->right = deleteNode(root->right, key); 

	// if key is same as root's key, then This is the node 
	// to be deleted 
	else
	{ 
		// node with only one child or no child 
		if (root->left == NULL) 
		{ 
			struct node *temp = root->right; 
			free(root); 
			return temp; 
		} 
		else if (root->right == NULL) 
		{ 
			struct node *temp = root->left; 
			free(root); 
			return temp; 
		} 

		// node with two children: Get the inorder successor (smallest 
		// in the right subtree) 
		struct node* temp = minValueNode(root->right); 

		// Copy the inorder successor's content to this node 
		root->key = temp->key; 

		// Delete the inorder successor 
		root->right = deleteNode(root->right, temp->key); 
	} 
	return root; 
} 

// Driver Program to test above functions 
long long mstcost=0;
int main ( int argc, char ** argv ){
    if(argc<3){
    cout<<"invalid parameters"<<endl;
    return 0;
    }
    const char *  sfile=(argv[1]);
    const char *  dfile=(argv[2]);
	auto start = high_resolution_clock::now();
	struct node *root = NULL; 
   
    freopen(sfile,"r",stdin);
    freopen(dfile,"w",stdout);
    int n;
    cin>>n;
    vector<pair<int,int>> vec[n];
    string line;
    int s,d,wt;
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
    int rnode = 0;




    while(cnt<n){
        if(destination==-1){
            cout<<" MST not possible "<<endl;
            return 0;
        }
        pair<int,int> par;
        visited[rnode] = 1;
        for(i=0; i<vec[rnode].size();i++){
            par = vec[rnode][i];
			if(searchbyval(root,par.second)==0){
                root = insert(root, make_pair(par.second,make_pair(rnode,par.first)));
                mapofEdges[par.second] = make_pair(rnode,par.first);
            }
            else
            {
                  DuplicateEdgesToInsert.push_back(make_pair(par.second,make_pair(rnode,par.first)));
            }
            //root = insert(root, make_pair(par.second,make_pair(rnode,par.first)));
            pair<int,int> tofind;
            tofind = make_pair(rnode,par.second);
            vector<pair<int,int>>::iterator itr;
            itr = find(vec[par.first].begin(),vec[par.first].end(),tofind);
            if(itr != vec[par.first].end()){
                    vec[par.first].erase(itr);
            }
        }
        //inorder(root);
        
		//mini = findmin(root);
        //minimumEdgeWeight = mini.first;
        //root = deleteNode(root, mini);

        struct node * p = minValueNode(root);
		if(p)
        mini = p->key;
		minimumEdgeWeight = mini.first;
        root = deleteNode(root, mini);
        source = mapofEdges[minimumEdgeWeight].first;
        destination = mapofEdges[minimumEdgeWeight].second;
	    mapofEdges[minimumEdgeWeight]=make_pair(-1,-1);
        v = mini.second.second;

		for(int i=0;i<DuplicateEdgesToInsert.size();i++)
        {
            int toInsertWeight = DuplicateEdgesToInsert[i].first;
            if(searchbyval(root,toInsertWeight)==0)
            {

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
					p = minValueNode(root);
					if(p)
        				mini = p->key;
                    //mini = findmin(root);
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
                
                if(searchbyval(root,toInsertWeight)==0)
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
