#include <iostream>
#include <vector>
#include <queue>
#include <cassert>
#include <functional>
using namespace std;

/*
 
 Binary lifting in tree, O(N*lg N) for preprocessing, O(lg N) to query ancestor and LCA
 
 If efficient query about tree path leveraging monotinicity, this works like a charm.
 
 References:
  - https://cp-algorithms.com/graph/lca_binary_lifting.html
  - https://yukicoder.me/wiki/lowest_common_ancestor
  - https://www.slideshare.net/satanic2/ss-72500089
  - https://www.npca.jp/works/magazine/2015_5/
 
 Usage:
 
 BinaryLifting bf(0,11);
 bf.addEdge(0,1),bf.addEdge(0,2),...
 
 Used problem(s):
  - https://github.com/hiroshi-maybe/codeforces/blob/master/solutions/SplitTheTree.cpp#L117
  - https://github.com/hiroshi-maybe/codeforces/blob/master/solutions/Company.cpp#L219
   - LCA
  - https://github.com/hiroshi-maybe/codeforces/blob/master/solutions/Tree.cpp#L165
   - LCA
  - https://github.com/hiroshi-maybe/atcoder/blob/master/solutions/ColorfulTree.cpp#L67
   - LCA
 
 */
struct BinaryLifting {
public:
  vector<vector<int>> G;
  int V,root;
  int H,t=0;
  vector<vector<int>> P; // parent P[bits][vertices], bits=floor(lg N)+1
  vector<int> D; // depth for LCA query D[vertices]
  vector<int> L,R; // timestamp of Euler tree
  BinaryLifting() {}
  BinaryLifting(int root, int V) : V(V), root(root) {
    G=vector<vector<int>>(V);
  }
  void addEdge(int u, int v) {
    G[u].push_back(v);
    G[v].push_back(u);
  }
  BinaryLifting(int root, vector<vector<int>> &G) : G(G), root(root) {
    V=G.size();
  }
  void buildLiftingTable() {
    D=vector<int>(V,0),L=vector<int>(V,0),R=vector<int>(V,0);
    H=1;
    while((1<<H)<=V) ++H;
    P=vector<vector<int>>(H,vector<int>(V,-1));
    dfs(root,-1,0);
    for(int i=0; i<H-1; ++i) {
      for(int j=0; j<V; ++j) {
        if(P[i][j]!=-1) P[i+1][j]=P[i][P[i][j]];
      }
    }
  }
  // query to find an ancestor with `d` distance
  int ancestor(int u, int d) {
    int cur=u;
    for(int i=H-1; i>=0; --i) if(cur>=0&&(d>>i)&1) cur=P[i][cur];
    return cur;
  }
  // query to find lca(u,v)
  int lca(int u, int v) {
    assert(u<V&&v<V);
    if(D[u]>D[v]) swap(u,v);
    v=ancestor(v,D[v]-D[u]);
    if(u==v) return u;
    for(int i=H-1; i>=0; --i) {
      if(P[i][u]!=P[i][v]) u=P[i][u],v=P[i][v];
    }
    return P[0][u];
  }
private:
  void dfs(int u, int par, int d) {
    P[0][u]=par;
    D[u]=d,L[u]=t++;
    for(int v : G[u]) if(v!=par) dfs(v,u,d+1);
    R[u]=t++;
  }
};

void test_binarylifting() {
  // https://www.slideshare.net/satanic2/ss-72500089
  BinaryLifting bf(0,11);
  bf.addEdge(0,1);
  bf.addEdge(1,2);
  bf.addEdge(2,3),bf.addEdge(2,4);
  bf.addEdge(3,5);
  bf.addEdge(4,6);
  bf.addEdge(6,7);
  bf.addEdge(7,8),bf.addEdge(7,9);
  bf.addEdge(9,10);
  
  bf.buildLiftingTable();
  vector<vector<int>> exp={
    {-1, 0, 1, 2, 2, 3, 4, 6, 7, 7, 9},
    {-1,-1, 0, 1, 1, 2, 2, 4, 6, 6, 7},
    {-1,-1,-1,-1,-1, 0, 0, 1, 2, 2, 4},
    {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}
  };
  /*
   for(int i=0; i<bf.H; ++i) {
   for(int j=0; j<bf.V; ++j) cout<<bf.P[i][j]<<",";
   cout<<endl;
   }*/
  assert(bf.P==exp);

  ///////////////////////////
  // Ancestor
  
  // not found
  assert(bf.ancestor(0,1)==-1);
  assert(bf.ancestor(7,6)==-1);
  // found
  assert(bf.ancestor(9,3)==4);
  assert(bf.ancestor(10,7)==0);
  assert(bf.ancestor(5,1)==3);
  assert(bf.ancestor(0,0)==0);
  
  ///////////////////////////
  // LCA

  assert(bf.lca(0,0)==0);
  assert(bf.lca(0,1)==0);
  assert(bf.lca(5,7)==2);
  assert(bf.lca(10,5)==2);
  assert(bf.lca(10,8)==7);
}

/*
 
 Find center(s) of a tree, O(N+E) time
 
  - Tree should be connected
  - Edge should be undirected
  - Center of the tree is one or two nodes
 
 References:
  - https://en.wikipedia.org/wiki/Centered_tree
 
 Used problems:
  - https://github.com/hiroshi-maybe/codeforces/blob/master/solutions/Multihedgehog.cpp#L146
  - https://github.com/hiroshi-maybe/codeforces/blob/master/solutions/MinimalDiameterForest.cpp#L91
 
 */
vector<int> findCenter(vector<vector<int>> &G) {
  int N=G.size();
  vector<int> deg(N,0);
  queue<int> Q;
  for(int u=0; u<N; ++u) {
    int x=G[u].size();
    deg[u]=x;
    if(x<=1) Q.emplace(u);
  }
  int d=0;
  vector<int> res;
  while(Q.size()) {
    int L=Q.size();
    res=vector<int>();
    while(L--) {
      int u=Q.front(); Q.pop();
      res.emplace_back(u);
      --deg[u];
      for(auto v : G[u]) {
        --deg[v];
        if(deg[v]==1) Q.emplace(v);
      }
    }
    ++d;
  }
  assert(1<=res.size()&&res.size()<=2);
  return res;
}

void test_findCenter() {
  // 0
  vector<vector<int>> G1={{}};
  vector<int> exp1={0};
  assert(findCenter(G1)==exp1);

  // 0-1
  vector<vector<int>> G2={{1},{0}};
  vector<int> exp2={0,1};
  assert(findCenter(G2)==exp2);

  vector<vector<int>> G_even={
    {1,2},
    {0,3,4,6,8},
    {0,5},
    {1},
    {1},
    {2,7},
    {1},
    {5},
    {1}
  };
  vector<int> exp_even={0,2};
  vector<int> ac_even=findCenter(G_even);
  sort(ac_even.begin(),ac_even.end());
  assert(exp_even==ac_even);

  vector<vector<int>> G_odd={
    {1},
    {0,2,3},
    {1,4},
    {1,5},
    {2,6,7,9},
    {3,8},
    {4},
    {4},
    {5},
    {4}
  };
  vector<int> exp_odd={1};
  vector<int> ac_odd=findCenter(G_odd);
  assert(ac_odd==exp_odd);
}

/*
 
 Compute diameter of a tree, O(N+E) time
 
  - It's possible to solve by two dfs
  - Tree should be connected
  - Edge should be undirected
  - diameter is odd if center is an edge
  - diameter is even if center is a vertex
 
 References:
  - https://cs.stackexchange.com/questions/22855/algorithm-to-find-diameter-of-a-tree-using-bfs-dfs-why-does-it-work
  - https://www.slideshare.net/chokudai/arc022
 
 Used problems:
  - https://github.com/hiroshi-maybe/codeforces/blob/master/solutions/MinimalDiameterForest.cpp#L71
  - https://github.com/hiroshi-maybe/atcoder/blob/master/solutions/BonsaiGrafting.cpp#L97
 
 */
int treeDiameter(vector<vector<int>> &G, int u=0) {
  int N=G.size();
  vector<int> D(N,0);
  
  function<void(int,int,int)> dfs=[&](int u, int pre, int d) -> void {
    D[u]=d;
    for(auto v: G[u]) if(v!=pre) dfs(v,u,d+1);
  };
  dfs(u,-1,0);
  int a=max_element(D.begin(),D.end())-D.begin();
  
  D=vector<int>(N,0);
  dfs(a,-1,0);
  int b=max_element(D.begin(),D.end())-D.begin();;
  
  // (a,b) is pair of vertices which form diameter
  //  printf("%d-%d: %d\n",a,b,D[b]);
  return D[b];
}

void test_diameter() {
  // 0
  vector<vector<int>> G1={{}};
  assert(treeDiameter(G1)==0);
  
  // 0-1
  vector<vector<int>> G2={{1},{0}};
  assert(treeDiameter(G2)==1);
  
  vector<vector<int>> G_even={
    {1,2},
    {0,3,4,6,8},
    {0,5},
    {1},
    {1},
    {2,7},
    {1},
    {5},
    {1}
  };
  assert(treeDiameter(G_even)==5);
  
  vector<vector<int>> G_odd={
    {1},
    {0,2,3},
    {1,4},
    {1,5},
    {2,6,7,9},
    {3,8},
    {4},
    {4},
    {5},
    {4}
  };
  vector<int> exp_odd={1};
  assert(treeDiameter(G_odd)==6);
}

/*
 
 Random tree generator
 
 */
#include <chrono>
#include <random>
// mt19937_64 for 64 bit unsigned integer
//mt19937 rnd(time(nullptr));
mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());
int genRandNum(int lb, int ub) {
  // Helper to generate random integer in range [lb, ub)
  int x = rnd() % (int)(ub - lb);
  return x + lb;
}
vector<int> gen_tree(int N) {
  vector<int> P(N-1,0);
  printf("%d\n",N);
  for(int u=1; u<=N-1; ++u) {
    P[u-1]=genRandNum(0,u);
    printf("%d %d\n",u+1,P[u-1]+1);
  }
  // parent of u+1 = P[u] (P[u]<u+1)
  return P;
}

// make adjacent list representation of a tree
vector<vector<int>> packtree(vector<int> P) {
  int N=P.size()+1;
  vector<vector<int>> G(N);
  for(int i=0; i<N-1; ++i) {
    int u=i+1,v=P[u];
    G[v].push_back(u),G[u].push_back(v);
  }
  return G;
}

// main

int main(int argc, char const *argv[]) {
  test_binarylifting();
  test_findCenter();
  test_diameter();
  
  gen_tree(100000);
}

// g++ -std=c++14 -Wall -O2 -D_GLIBCXX_DEBUG -fsanitize=address tree.cpp && ./a.out
