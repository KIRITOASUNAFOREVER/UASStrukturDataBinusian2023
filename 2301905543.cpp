#include<string.h>
#include<stdio.h>
#include<stdlib.h>
# define my_sizeof(type) ((char *)(&type+1)-(char*)(&type)) 

struct Tree{
	int key;
	int height;
	char name[35];
	int desc; 
	Tree *left, *right;
}*root=NULL;

int getMax (int height1, int height2){
	if (height1 > height2){
		return height1;
	}else{
		return height2;
	}
}

int getHeight(Tree *curr){
	if (curr==NULL){
		return 0;
	}else{
		return curr->height;
	}
}

int getBalanceFactor(Tree *curr){
	if (curr==NULL){
		return 0;
	}else{
		return getHeight(curr->left)-getHeight(curr->right);
	}
}

struct Tree *leftRotation(Tree *curr){
	Tree *child = curr->right;
	Tree *grandChild = child->left;
	
	child->left = curr;
	curr->right = grandChild;
	
	curr->height = getMax(getHeight(curr->left), getHeight(curr->right))+1;
	child->height = getMax(getHeight(child->left), getHeight(child->right))+1;
	
	return child;
}

struct Tree *rightRotation(Tree *curr){
	Tree *child = curr->left;
	Tree *grandChild = child->right;
	
	child->right = curr;
	curr->left = grandChild;
	
	curr->height = getMax(getHeight(curr->left), getHeight(curr->right))+1;
	child->height = getMax(getHeight(child->left), getHeight(child->right))+1;
	
	return child;
}
struct Tree *insertNode(Tree *curr, int value, char name[]){
	if (curr==NULL){
		curr = (Tree *)malloc(sizeof(struct Tree));
		curr->key =  value;
		strcpy(curr->name,name);
		curr->height = 1;
		curr->desc = 0;
		curr->left = curr->right = NULL;
		return curr;
	}
	else if (value < curr->key){
		curr->left = insertNode(curr->left, value,name);
		curr->desc++; 
	}
	else if (value > curr->key){
		curr->right = insertNode(curr->right, value,name);
		curr->desc++; 
	}
	else if(value == curr->key){
		return curr;
	}
	curr->height = getMax(getHeight(curr->left), getHeight(curr->right))+1;
	int balanceFactor = getBalanceFactor(curr);
	
	if (balanceFactor < -1 && value > curr->right->key){// RR case
		return leftRotation(curr);
	}
	
	if (balanceFactor < -1 && value < curr->right->key){//RL Case
		curr->right = rightRotation(curr->right);
		return leftRotation(curr);
	}
	
	if (balanceFactor > 1 && value < curr->left->key){// LL Case
		return rightRotation(curr);
	}
	
	if (balanceFactor > 1 && value > curr->left->key){ // LR Case
		curr->left = leftRotation(curr->left);
		return rightRotation(curr);
	}
	return curr;
}

struct Tree *preDecessor(Tree *curr){
	curr= curr->left;
	while(curr->right){
		curr=curr->right;
	}
	
	return curr;
}
struct Tree *deleteNode(Tree *curr, int value,char name[],int desc){
	if (curr==NULL){
		return curr;
	}
	else if (value < curr->key){
		curr->left = deleteNode(curr->left, value,name,desc);
	}
	else if (value > curr->key){
		curr->right = deleteNode(curr->right, value,name,desc);
	}
	else{
		Tree *temp = NULL;
		if (curr->left ==NULL || curr->right==NULL){ // 1 CHILD OR NO CHILD
			if (curr->left==NULL){
				temp = curr->right;
			}
			else{
				temp = curr->left;
			}
			
			if (temp==NULL){ // NO CHILD
				temp=curr;
				curr=NULL;
			}else{ // HAVE 1 CHILD
				*curr = *temp;
			}
			free(temp);
		}
		else{ // HAVE 2 CHILD
			temp = preDecessor(curr);
			curr->key = temp->key;
			strcpy(curr->name,temp->name);
			curr->desc = desc;
			curr->left = deleteNode(curr->left, temp->key, temp->name,temp->desc);
		}
	}
	
	if (curr==NULL){
		return curr;
	}
	
	curr->height = getMax(getHeight(curr->left), getHeight(curr->right))+1;
	int balanceFactor = getBalanceFactor(curr);
	
	if (balanceFactor >1 && getBalanceFactor(curr->left)<0){//LR Case
		curr->left = leftRotation(curr->left);
		return rightRotation(curr);
	}
	
	if (balanceFactor > 1 && getBalanceFactor(curr->left)>=0){//LL Case
		return rightRotation(curr);
	}
	
	if (balanceFactor < -1 && getBalanceFactor(curr->right)<=0){// RR Case
		return leftRotation(curr);
	}
	
	if (balanceFactor < -1 && getBalanceFactor(curr->right)>0){//RL Case
		curr->right = rightRotation(curr->right);
		return leftRotation(curr);
	}
	
	return curr;
}

void inOrder(Tree *curr) 
{
    if(curr!=NULL)
    {
        inOrder(curr->left);
        printf("%d %s\n",curr->key,curr->name);
        inOrder(curr->right);
    }
}

int count(Tree *tree)
{
    int c =  1;             
    if (tree ==NULL)
        return 0;
    else
    {
        c += count(tree->left);
        c += count(tree->right);
        return c;
    }
}

struct Tree *search(Tree **node, int value){
	if((*node)!=NULL){
		if(value<(*node)->key){
			search(&(*node)->left,value);
		}else if(value>(*node)->key){
			search(&(*node)->right,value);
		}else{
			printf("%d %s\n", (*node)->key,(*node)->name);
		}
	}
}

int CountGreater(struct Tree* root, int x,char name[]) 
{ 
    int res = 0; 

    while (root != NULL) { 
        int desc = (root->right != NULL) ?  
                   root->right->desc : -1; 
        if (root->key > x) { 
            res = res + desc + 1 + 1; 
            root = root->left; 
        } else if (root->key < x) 
            root = root->right; 
        else { 
            res = res + desc + 1; 
            break; 
        } 
    } 
    return res; 
}

void maxDiffUtil(struct Tree *ptr, int k, int &min_diff, int &min_diff_key) 
{ 
    if (ptr == NULL) 
        return ; 
    if (ptr->key == k) 
    { 
        min_diff_key = k; 
        return; 
    } 
  
    if (min_diff > abs(ptr->key - k)) 
    { 
        min_diff = abs(ptr->key - k); 
        min_diff_key = ptr->key; 
    } 
  
    if (k < ptr->key) 
        maxDiffUtil(ptr->left, k, min_diff, min_diff_key); 
    else
        maxDiffUtil(ptr->right, k, min_diff, min_diff_key); 
} 

int maxDiff(Tree *root, int k) 
{ 
    int min_diff = INT_MAX, min_diff_key = -1; 
    maxDiffUtil(root, k, min_diff, min_diff_key); 
  
    return min_diff_key; 
}

struct Tree *searchDelete(Tree **node, int value){
	if((*node)!=NULL){
		if(value<(*node)->key){
			search(&(*node)->left,value);
		}else if(value>(*node)->key){
			search(&(*node)->right,value);
		}else{
			root = deleteNode(root , value , (*node)->name , (*node)->desc);
		}
	}
}

int AddToArray(Tree *node, int arr[], int i)
{
     if(node == NULL)
          return i;
     arr[i] = node->key;
     i++;
     if(node->left != NULL)
          i = AddToArray(node->left, arr, i);
     if(node->right != NULL)
          i = AddToArray(node->right, arr, i);

     return i;
}

int compare(const void * a, const void * b)
{
   return ( *(int*)a - *(int*)b );
}

struct Tree *searchRange(Tree **node, int value){
	if((*node)!=NULL){
		if(value<(*node)->key){
			search(&(*node)->left,value);
		}else if(value>(*node)->key){
			search(&(*node)->right,value);
		}else{
			printf("%d %s\n", (*node)->key,(*node)->name);
		}
	}
}

int main(){
	int hitungs = 0;
	int arr[100000];	
	int choose=0;
	int pilihan,counter,suhu;
	int lebihBesar = 0;
	int perbedaan = 0;
	int hitung = 0;
	char nama[35];
	int c;
	int awal,akhir;
	
	scanf("%d",&counter);getchar();
    do{
		scanf("%d",&pilihan);getchar();
		if(pilihan==1){
			scanf("%d %s",&suhu,&nama);fflush(stdin);
			root = insertNode(root, suhu,nama);
			lebihBesar = CountGreater(root, suhu,nama);
			printf("%d\n",lebihBesar); 
		}
		else if(pilihan==3){
			scanf("%d",&suhu);fflush(stdin);
			perbedaan = maxDiff(root, suhu);
			search(&root,perbedaan);
		}
		else if(pilihan==2){
			scanf("%d",&suhu);fflush(stdin);
			searchDelete(&root,suhu);
			c = count(root);
			printf("%d\n",c);
		}
		else if(pilihan==4){
			scanf("%d %d",&awal,&akhir);fflush(stdin);
			awal  = awal - 1;
			akhir = akhir - 1;
			hitungs = count(root);
			AddToArray(root, arr, 0);
			qsort(arr,hitungs,sizeof(int),compare);
			 int size = my_sizeof(arr)/my_sizeof(arr[0]) + 1; 
			 
			 if(akhir > size){
			 	for (int i=awal; i<size; i++)
			    {
			    	if(arr[i]==NULL){
			    		continue;
					}
			    	printf("%d: ", i+1);
			    	searchRange(&root,arr[i]);
			    }
			 }else{
			 	for (int i=awal; i<=akhir; i++)
			    {
			    		if(arr[i]==NULL){
			    		continue;
					}
			        printf("%d: ", i+1);
			    	searchRange(&root,arr[i]);
			    }
			 }
			for (int i=awal; i<size; i++)
			    {
			    	arr[i] = NULL;
			    }
		}
        counter--;
	}while(counter != 0);
}
