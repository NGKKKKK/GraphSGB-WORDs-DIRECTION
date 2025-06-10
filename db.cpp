#include <bits/stdc++.h>
#include <windows.h>
using namespace std;

int MAX_SIZE = 5;
int CHECK_SIZE = 4;

struct Vertex{
    string data;
    int id = 0;
    int low = 0;
    vector<Vertex*> edgeFrom;
    bool visited = false;

    Vertex(){
        data = "";
    }
    Vertex(const string& input){
        data = input;
    }
    bool isEdge(Vertex* v){
        string tmp1 = data.substr(MAX_SIZE - CHECK_SIZE,CHECK_SIZE);
        string tmp2 = v->data;
        sort(tmp1.begin(), tmp1.end());
        sort(tmp2.begin(), tmp2.end());
        int i = 0; int j = 0;
        while (i < CHECK_SIZE && j < MAX_SIZE){
            if (tmp1[i] == tmp2[j]) i++;
            j++;
        }
        return (i >= CHECK_SIZE);
    }
    void addEdge(Vertex* v){
        edgeFrom.push_back(v);
    }
};

int SCC = 0;
int cnt = 0;
stack<Vertex*> st;
vector<vector<Vertex*>> SCC_LIST;

void DFS(Vertex* v){
    v->id = v->low = ++cnt;
    st.push(v);
    for (Vertex* u : v->edgeFrom){
        if(u->visited) continue;
        if (!u->id){
            DFS(u);
            v->low = min(v->low, u->low);
        }
        else v->low = min(v->low, u->id);
    }
    if (v->low == v->id){
        vector<Vertex*> SCC_CUR;
        SCC++;
        Vertex* x;
        do{
            x = st.top();
            x->visited = true;
            st.pop();
            SCC_CUR.push_back(x);
        }while (x != v);
        SCC_LIST.push_back(SCC_CUR);
    }
}

vector<Vertex*> findSCC_Belong_Vertex(const string& word) {
    for (const auto& tmp : SCC_LIST) {
        for (Vertex* v : tmp) {
            if (v->data == word){
                //for (Vertex* llll : v->edgeFrom) cout << llll->data << endl;
                return tmp;
            }
        }
    }
    return {};
}

void resetGraph(vector<Vertex*>& Graph){
    for (Vertex* v : Graph) v->visited = false;
}

Vertex* findVertexWithWord(const string& word, const vector<Vertex*>& Graph){
    for (Vertex* v : Graph){
        if (v->data == word) return v;
    }
    return nullptr;
}

vector<Vertex*> findShortestPath(Vertex* u, Vertex* v){
    queue<Vertex*> que;
    unordered_map<Vertex*, Vertex*> trace;
    u->visited = true;
    que.push(u);
    while (!que.empty()){
        Vertex* cur = que.front();
        que.pop();
        if (cur == v){
            vector<Vertex*> res;
            Vertex* tmp = v;
            while (tmp != nullptr){
                res.push_back(tmp);
                tmp = trace[tmp];
            }
            return res;
        }
        for (Vertex* x : cur->edgeFrom){
            if (!x->visited){
                x->visited = true;
                trace[x] = cur;
                que.push(x);
            }
        }
    }
    return {};
}

int main(){
    SetConsoleOutputCP(65001);
    vector<Vertex*> Graph;
    ifstream file("LON.txt");
    if (!file.is_open()) return 0;
    string word;
    while (file >> word){
        Vertex* newVertex = new Vertex(word);
        Graph.push_back(newVertex);
        //cout << "#" << word << endl;
    }
    file.close();
    int sz = Graph.size();
    for (int i = 0; i < sz; ++i){
        for (int j = i+1; j < sz; ++j){
            if (Graph[i]->isEdge(Graph[j])) Graph[i]->addEdge(Graph[j]);
            if (Graph[j]->isEdge(Graph[i])) Graph[j]->addEdge(Graph[i]);
        }
    }
    /*for (Vertex* j : Graph){
        cout << j->data << " : ";
        for (Vertex* k : j->edgeFrom) cout << k->data << " ";
        cout << endl;
    }*/
    for (Vertex* v : Graph){
        if (!v->id) DFS(v);
    }
    cout << "a) " << SCC << endl;
    string findWord;
    cout << "b) Mời nhập từ: "; cin >> findWord;
    vector<Vertex*> findSCC = findSCC_Belong_Vertex(findWord);
    if (findSCC.empty()){
        cout << "Lỗi từ đầu vào không hợp lệ" << endl;
    }else{
    cout << "Các đỉnh cùng thành phần liên thông mạnh với " << findWord << " là: ";
    for (Vertex* k : findSCC){
        if (k->data != findWord) cout << k->data << " ";
    }
    }
    cout << endl;
    cout << "c) " << endl;
    resetGraph(Graph);
    string w1,w2;
    cout << "Từ 1 : "; cin >> w1;
    cout << "Từ 2 : "; cin >> w2;
    Vertex* u = findVertexWithWord(w1, Graph);
    Vertex* v = findVertexWithWord(w2, Graph);
    if (u == nullptr || v == nullptr){
        cout << "Lỗi đầu vào : Không tìm thấy từ hợp lệ !" << endl;
        return 0;
    }
    vector<Vertex*> ans2 = findShortestPath(u,v);
    int sz_ = ans2.size();
    if (sz_ <= 1){
        cout << "Không có đường đi ngắn nhất giữa 2 đỉnh đã cho" << endl;
        return 0;
    }
    cout << "Đường đi ngắn nhất giữa 2 đỉnh đã cho có độ dài là: " << sz_-1 << endl << "Đường đi: ";
    for (int i = sz_-1; i > 0; --i) cout << ans2[i]->data << " -> ";
    cout << ans2[0]->data;
    cout << endl;
    for (Vertex* kl : Graph) delete kl;
    return 0;
}
