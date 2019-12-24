#include <stdio.h>
#include <stdlib.h>

typedef int WeightType;
typedef struct Node *PtrToNode;
typedef PtrToNode TreeNode;
struct Node {
    int nodeID;
    WeightType weight;
    TreeNode parent, firstchild, nextsibiling;
};

typedef struct TREE {
    int Num_Node, Num_nonLeafNode;
    WeightType Given_Weight;
    TreeNode TreeRoot;
}TreeInfo;

WeightType** Paths;
int Num_Paths;

TreeInfo Construct_Tree();
void FindLeaf(TreeNode N, WeightType Sum, WeightType Given);
void Sort();
int Compare(WeightType* a, WeightType* b);

int main() {
    Paths = NULL;
    Num_Paths = 0;
    TreeInfo T = Construct_Tree();
    FindLeaf(T.TreeRoot, 0, T.Given_Weight);
    Sort();
    for(int i = 0; i < Num_Paths; i++) {
        printf("%d", Paths[i][0]);
        int j = 1;
        while(Paths[i][j] != -1) {
            printf(" %d", Paths[i][j]);
            j++;
        }
        printf("\n");
    }
}

int Compare(WeightType* a, WeightType* b) {
    int i = 0;
    while(a[i] !=-1 && b[i] != -1) {
        if(a[i] > b[i]) {
            return 1;
        }
        else if(a[i] < b[i]) {
            return -1;
        }
        i++;
    }
    return 0;
}

void Sort() {
    WeightType* temp;
    for(int i = 0; i < Num_Paths-1; i++) {
        for(int j = 0; j < Num_Paths - i - 1; j++) {
            if(Compare(Paths[j], Paths[j+1]) < 0) {
                temp = Paths[j];
                Paths[j] = Paths[j+1];
                Paths[j+1] = temp;
            }
        }
    }
}

void FindLeaf(TreeNode N, WeightType Sum, WeightType Given) {
    TreeNode S;
    if(N->firstchild != NULL) {
        FindLeaf(N->firstchild, Sum + N->weight, Given);
        S = N->firstchild;
        while(S->nextsibiling) {
            FindLeaf(S->nextsibiling, Sum + N->weight, Given);
            S = S->nextsibiling;
        }
    }
    else{
        if(Sum + N->weight == Given) {
            int count = 0;
            S = N;
            while(S->parent) {
                S = S->parent;
                count++;
            }
            WeightType *path = (WeightType*)calloc(count + 2, sizeof(WeightType));
            path[count+1] = -1;
            S = N;
            for(int i = count; i >= 0; i--) {
                path[i] = S->weight;
                S = S->parent;
            }
            if(Paths == NULL){
                Num_Paths = 1;
                Paths = (WeightType**)malloc(sizeof(WeightType*));
                Paths[0] = path;
            }
            else{
                Num_Paths++;
                Paths = (WeightType**)realloc(Paths, Num_Paths*sizeof(WeightType*));
                Paths[Num_Paths-1] = path;
            }
        }
        else{
            return;
        }
    }
    return;
}

TreeInfo Construct_Tree() {
    TreeInfo T;
    scanf("%d %d %d", &T.Num_Node, &T.Num_nonLeafNode, &T.Given_Weight);
    TreeNode *Nodes = (TreeNode*)calloc(T.Num_Node, sizeof(TreeNode));
    for(int i = 0; i < T.Num_Node; i++) {
        Nodes[i] = (TreeNode)malloc(sizeof(struct Node));
        Nodes[i]->nodeID = i;
        scanf("%d", &Nodes[i]->weight);
        Nodes[i]->firstchild = Nodes[i]->nextsibiling = Nodes[i]->parent = NULL;
    }
    int NodeId, Num_Child, FirstChildId, Current_SibilingId, Next_SibilingId;
    for(int i = 0; i < T.Num_nonLeafNode; i++) {
        scanf("%d %d %d", &NodeId, &Num_Child, &FirstChildId);
        Nodes[NodeId]->firstchild = Nodes[FirstChildId];
        Nodes[FirstChildId]->parent = Nodes[NodeId];
        Current_SibilingId = FirstChildId;
        for(int j = 1; j < Num_Child; j++) {
            scanf("%d", &Next_SibilingId);
            Nodes[Current_SibilingId]->nextsibiling = Nodes[Next_SibilingId];
            Nodes[Next_SibilingId]->parent = Nodes[NodeId];
            Current_SibilingId = Next_SibilingId;
        }   
    }
    T.TreeRoot = Nodes[0];
    return T;
}