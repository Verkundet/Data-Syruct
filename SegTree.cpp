#include<iostream>
#include<algorithm>
using namespace std;
//以维护区间和为例
const int N=10;
//a[]为原始数据，d[]为维护线段树的数组（大小为原数据4倍左右）
//t[]为懒惰标记数组
int d[4*N],a[N],t[4*N];


//构建线段树
//x为当前节点，[l,r]为目标区间
void build(int p,int l,int r){
    if(l==r){
        d[p]=a[l];
        return ;
    }

    int m=l+((r-l)>>1);
    //递归建树
    build(2*p,l,m);
    build(2*p+1,m+1,r);
    d[p]=d[2*p]+d[2*p+1];
}
//查询线段树
// [l,r]为查询区间，[x,y]为当前节点所在区间，p为当前节点
int query(int l,int r,int x,int y,int p){
    //如果当前区间被查询区间完全覆盖，直接返回当前区间值
    if(l<=x&&y<=r)
        return d[p];

    int m=x+((y-x)>>1),sum=0;
    //如果查询区间与当前区间的左儿子[x,m]有交集，递归查询[x,m];
    if(l<=m){
        sum+=query(l,r,x,m,2*p);
    }
    //右儿子同理
    if(r>m){
        sum+=query(l,r,m+1,r,2*p+1);
    }
    return sum;
}

//修改与懒惰标记
//[l,r]为待修改区间，c为改变量
//[x,y]为当前区间，p为当前节点编号
void update(int l,int r,int c,int x,int y,int p){
    //当前区间为待修改区间子集时直接修改节点值，然后打上变量
    if(l<=x&&y<=r){
        d[p]+=(y-x+1)*c,t[p]+=c;
        return ;
    }

    int m=x+((y-x)>>1);
    //如果当前节点有懒惰标记，更新子节点的值和懒惰标记
    if(t[p]&&x!=y){
        d[p*2]+=t[p]*(m-x+1),d[2*p+1]=t[p]*(y-m);
        t[p*2]+=t[p],t[p*2+1]+=t[p];
        t[p]=0;
    }

    if(l<=m)update(l,r,c,x,m,2*p);
    if(r>m)update(l,r,c,m+1,y,2*p+1);
    d[p]=d[p*2]+d[p*2+1];
}

int changed_query(int l,int r,int x,int y,int p){
    if(l<=x&&y<=r)return d[p];
    int m=x+((y-x)>>1);
    if(t[p]){
        d[p*2]+=t[p]*(m-x+1),d[p*2+1]=t[p]*(y-m);
        t[p*2]+=t[p],t[p*+1]+=t[p];
        t[p]=0;
    }

    int sum=0;
    if(l<=m)sum+=changed_query(l,r,x,m,p*2);
    if(r>m)sum+=changed_query(l,r,m+1,y,p*2+1);
    return sum;
}


int main(){
    int n;
    cin>>n;
    for(int i=1;i<=n;i++)cin>>a[i];
    build(1,1,n);
    update(2,4,6,1,n,1);
    
    int c=changed_query(3,6,1,n,1);
    cout<<c;

    return 0;
}

