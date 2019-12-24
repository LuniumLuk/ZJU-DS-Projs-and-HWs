#include <stdio.h>
#include <stdlib.h>
#define maxn 500
#define maxm 125000
#define maxdist 100000

/**
题目：删去一个点，然后求出需要增加最小代价的边集合生成连通图
思路：
prim+最小堆
1.之前图中未破坏的边必用，从而把两两之间可互达的点集合 合并成一个点
2.求出不同点集合的最短距离，用prim+最小堆求出最小生成树

kruskal
1.之前图中未破坏的边必用，全部加到图中
2.途中被破坏的边按照边权从小到大的顺序依次加入图中，直到图变为连通图

两个方法的对应一个点的最小生成树的复杂度都是nlogm，第二个方法较好写

优化：
1.未破坏的边直接加入图中
2.当有n-2条边(当前删去一个点后，图中n-1个点)在图中时，直接结束建生成树

另外：
求图的连通性：
有向 强连通
无向 并查集

当然，并查集好写很多。。。
**/

struct node
{
    long x,y,c,s;
}edge0[maxm+1],edge1[maxm+1];
long fa[maxn+1];

long max(long a,long b)
{
    if (a>b)
        return a;
    else
        return b;
}

int cmp(const void *a,const void *b)
{
    //先把未损坏的路径加入图中
    if ((*(struct node *)a).s < (*(struct node *)b).s)
        return 1;
    else if ((*(struct node *)a).s > (*(struct node *)b).s)
        return -1;
    //再对已损坏的路径进行排序，用kruskal算法,m log m
    else if ((*(struct node *)a).c < (*(struct node *)b).c)
        return 1;
    else
        return -1;
}

long getfather(long d)
{
    if (fa[d]==d)
        return d;
    else
        return getfather(fa[d]);
}

int main()
{
    long i,j,n,m,p,q,x,y,c,s,g,fx,fy,ans,cost[maxn+1];
    scanf("%ld%ld",&n,&m);
    
        p=0;
        q=0;
        for (i=0;i<m;i++)
        {
            scanf("%ld%ld%ld%ld",&x,&y,&c,&s);
            if (s==1)
            {
                p++;
                edge1[p].x=x; edge1[p].y=y; edge1[p].c=c; edge1[p].s=s;
            }
            else
            {
                q++;
                edge0[q].x=x; edge0[q].y=y; edge0[q].c=c; edge0[q].s=s;
            }
        }
        qsort(edge0+1,q,sizeof(struct node),cmp);

        ans=0;
        for (i=1;i<=n;i++)
        {
            for (j=1;j<=n;j++)
                fa[j]=j;
            g=0;
            //edge - exist
            for (j=1;j<=p;j++)
            {
                if (edge1[j].x==i || edge1[j].y==i) continue;
                fx=getfather(edge1[j].x);
                fy=getfather(edge1[j].y);
                if (fx==fy) continue;
                fa[fx]=fy;
                g++;
                if (g==n-2)
                    break;
            }
            //优化
            if (g==n-2)
            {
                cost[i]=0;
                continue;
            }

            //edge - not exist
            cost[i]=0;
            for (j=1;j<=q;j++)
            {
                if (edge0[j].x==i || edge0[j].y==i) continue;
                fx=getfather(edge0[j].x);
                fy=getfather(edge0[j].y);
                if (fx==fy) continue;
                fa[fx]=fy;
                g++;
                cost[i]+=edge0[j].c;
                //优化
                if (g==n-2)
                    break;
            }
            if (g<n-2)
                cost[i]=maxdist;
            ans=max(ans,cost[i]);
        }
        if (ans>0)
        {
            for (i=1;i<=n;i++)
                if (cost[i]==ans)
                {
                    printf("%ld",i);
                    break;
                }
            for (j=i+1;j<=n;j++)
                if (cost[j]==ans)
                    printf(" %ld",j);
            printf("\n");
        }
        else
            printf("0\n");
    
    return 0;
}