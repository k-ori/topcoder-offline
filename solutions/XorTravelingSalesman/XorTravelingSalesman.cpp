#include <fstream> // for greed plugin
#include <iostream>
#include <algorithm> // max,min
#include <vector>
#include <string>
#include <sstream>
#include <set>
#include <map>
#include <iostream>
#include <utility>
#include <cctype>
#include <queue>
#include <stack>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <unordered_set>
#include <unordered_map>
#include <limits.h>
#include <cstring>
#include <tuple>
#include <cassert>
#include <numeric>
using namespace std;

// type alias
typedef long long LL;

typedef vector < int > VI;
typedef unordered_map < int, int > MAPII;
typedef unordered_set < int > SETI;
typedef pair< int , int > II;
typedef tuple< int, int, int > III;

// repetition
#define FOR(i,a,b) for(int i=(a);i<(b);++i)
#define REP(i,n)  for(int i=0;i<(n);++i)
#define FORR(x,arr) for(auto& x:arr)
#define SZ(a) int((a).size())

// DP
#define MINUS(dp) memset(dp, -1, sizeof(dp))
#define ZERO(dp) memset(dp, 0, sizeof(dp))

// debug cout
#define dump(x)  cout << #x << " = " << (x) << endl;
#define dump2(x,y)  cout << #x << " = " << (x) << ", " << #y << " = " << (y) << endl;
#define dump3(x,y,z)  cout << #x << " = " << (x) << ", " << #y << " = " << (y) << ", " << #z << " = " << (z) << endl;
#define dump4(x,y,z,a)  cout << #x << " = " << (x) << ", " << #y << " = " << (y) << ", " << #z << " = " << (z) << ", " << #a << " = " << (a) << endl;
#define dumpAR(ar) FORR(x,(ar)) { cout << x << ','; } cout << endl;

/*
 
 1/7/2017
 
 10:14-10:43 pause
 12:45-13:15 give up, take a look at editorial and figure out brute-force
 13:15-13:28 implement by myself and system test passed

 Editorial:
  - https://apps.topcoder.com/wiki/display/tc/SRM+556
  - http://kmjp.hatenablog.jp/entry/2012/09/14/0930
  - http://topcoder.g.hatena.ne.jp/agw/20121109/1352445224
  - http://kg86.hatenablog.com/entry/2014/10/05/112321
  - http://d.hatena.ne.jp/komiyam/20120915/1347634886

 Bit binary manipulation never exceeds full bits.
 Possible value is very small. Just search all the states.
 
 Key:
  - X[i]<1024 => res<1024. Brute-force them.
 
 Summary:
  - I'm stupid enough that I did not come up with BRUTE-FORCE 👎👎👎👎👎👎👎👎👎👎👎👎👎👎👎👎👎👎👎👎👎👎👎👎
  - I was trying to use popular approaches of XOR problem
   - a XOR a = 0 (cancled)
   - compute each column independently
 
 */
int viz[51][1024];
class XorTravelingSalesman {
  public:
  int V;
  vector<string> G;
  VI X;
  void dfs(int u, int val) {
    if(viz[u][val]) return;
    viz[u][val]=true;
    REP(v,V) if(G[u][v]=='Y') dfs(v,val^X[v]);
  }
  
  int maxProfit(vector<int> X, vector<string> G) {
    this->V=SZ(X),this->G=G,this->X=X;
    ZERO(viz);
    dfs(0,X[0]);
    int res=0;
    REP(i,V) REP(v,1024) if(viz[i][v]) res=max(res,v);
    return res;
  }
};

// CUT begin
ifstream data("XorTravelingSalesman.sample");

string next_line() {
    string s;
    getline(data, s);
    return s;
}

template <typename T> void from_stream(T &t) {
    stringstream ss(next_line());
    ss >> t;
}

void from_stream(string &s) {
    s = next_line();
}

template <typename T> void from_stream(vector<T> &ts) {
    int len;
    from_stream(len);
    ts.clear();
    for (int i = 0; i < len; ++i) {
        T t;
        from_stream(t);
        ts.push_back(t);
    }
}

template <typename T>
string to_string(T t) {
    stringstream s;
    s << t;
    return s.str();
}

string to_string(string t) {
    return "\"" + t + "\"";
}

bool do_test(vector<int> cityValues, vector<string> roads, int __expected) {
    time_t startClock = clock();
    XorTravelingSalesman *instance = new XorTravelingSalesman();
    int __result = instance->maxProfit(cityValues, roads);
    double elapsed = (double)(clock() - startClock) / CLOCKS_PER_SEC;
    delete instance;

    if (__result == __expected) {
        cout << "PASSED!" << " (" << elapsed << " seconds)" << endl;
        return true;
    }
    else {
        cout << "FAILED!" << " (" << elapsed << " seconds)" << endl;
        cout << "           Expected: " << to_string(__expected) << endl;
        cout << "           Received: " << to_string(__result) << endl;
        return false;
    }
}

int run_test(bool mainProcess, const set<int> &case_set, const string command) {
    int cases = 0, passed = 0;
    while (true) {
        if (next_line().find("--") != 0)
            break;
        vector<int> cityValues;
        from_stream(cityValues);
        vector<string> roads;
        from_stream(roads);
        next_line();
        int __answer;
        from_stream(__answer);

        cases++;
        if (case_set.size() > 0 && case_set.find(cases - 1) == case_set.end())
            continue;

        cout << "  Testcase #" << cases - 1 << " ... ";
        if ( do_test(cityValues, roads, __answer)) {
            passed++;
        }
    }
    if (mainProcess) {
        cout << endl << "Passed : " << passed << "/" << cases << " cases" << endl;
        int T = time(NULL) - 1515348858;
        double PT = T / 60.0, TT = 75.0;
        cout << "Time   : " << T / 60 << " minutes " << T % 60 << " secs" << endl;
        cout << "Score  : " << 500 * (0.3 + (0.7 * TT * TT) / (10.0 * PT * PT + TT * TT)) << " points" << endl;
    }
    return 0;
}

int main(int argc, char *argv[]) {
    cout.setf(ios::fixed, ios::floatfield);
    cout.precision(2);
    set<int> cases;
    bool mainProcess = true;
    for (int i = 1; i < argc; ++i) {
        if ( string(argv[i]) == "-") {
            mainProcess = false;
        } else {
            cases.insert(atoi(argv[i]));
        }
    }
    if (mainProcess) {
        cout << "XorTravelingSalesman (500 Points)" << endl << endl;
    }
    return run_test(mainProcess, cases, argv[0]);
}
// CUT end
