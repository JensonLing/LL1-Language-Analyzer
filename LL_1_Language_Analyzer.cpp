# include <iostream>
# include <map>
# include <queue>
# include <string>
# include <fstream>
# include <vector>

# define PRODUCTION_PATH "./doc/production.txt"
//# define TOKEN_TABLE_PATH "./doc/tokens.txt"
//# define TOKEN_MAP_PATH "./doc/token_map.txt"

//# define EOF 0

using namespace std;

int N_num = 0;  //非终结符数量
int T_num = 0;  //终结符数量
int P_num = 0; //产生式数量
//int C_num = 0; //具有相同Follow集的Cluster的数量
map<int, string> T_Num2Str; //从非终结符编号到非终结符的映射
map<string, int> T_Str2Num;
map<int, string> N_Num2Str; //从终结符编号到终结符的映射
map<string, int> N_Str2Num;
map<int,bool> N_empty;
map<int, vector<int>> First;
map<int, vector<int>> Follow;//从N到Follow集的映射
map<int, vector<int>> Follow_Cluster;//Follow集中有相等关系的非终结符放在同一个Cluster里
//map<int, int> Get_Cluster_Num;//取得与该非终结符有相同Follow集的其他符号所在的Cluster号，若无则返回0

typedef struct Symbol
{
    int type; //0为终结符, 1为非终结符
    int no;
    struct Symbol* next;
}Symbol, *S_Pointer;

typedef struct Production
{
    //int N_No;  //左侧非终结符编号
    string N_Left;
    S_Pointer first_symbol; //指向产生式右边第一个符号
}Production, *P_Pointer;

Production Productions[100];//最多支持100个产生式

void Init_S(S_Pointer A)
{
    A->type = 0;
    A->next = NULL;
}

bool Same_Symbol(S_Pointer A, S_Pointer B)
{
    if(A->no == B->no && A->type == B->type)
        return true;
    return false;
}

bool Is_N(S_Pointer A)
{
    for(int i = 0; i <= P_num; i++)
    {
        if(A->no == N_Str2Num[Productions[i].N_Left])
            return true;
    }
    return false;
}

string Get_First_Symbol(char *A)
{
    int i = 0;
    char N[100] = {0};
    string res;
    string out;
    while(A[i])
    {
        N[i] = A[i];
        res = N;
        for(int j = 0; j < P_num; j++)
        {
            if(res == Productions[j].N_Left)
                out = res;
        }
        i++;
    }
    if(out != "")
        return out;
    
    N[1] = 0;
    res = N;
    return res;
}

void Load_Production()
{
    fstream t(PRODUCTION_PATH, ios::in);
    char temp[1024];
    string s;
    while(t.getline(temp, 1024))//第一遍先读取非终结符
    {
        int i = 0;
        char N[100] = {0};
        while(temp[i] != '-') i++;
        for(int j = 0; j < i; j++)
            N[j] = temp[j];
        
        string N_Symbol = N;
        cout<<"N_left is:"<<N_Symbol<<endl;
        if(N_Str2Num[N_Symbol])
        {
            Productions[P_num].N_Left = N_Symbol;
        }
        else
        {
            N_num ++;
            N_Num2Str[N_num] = N_Symbol;
            N_Str2Num[N_Symbol] = N_num;
            Productions[P_num].N_Left = N_Symbol;
        }
        P_num ++;
    }
    t.close();

    //cout<<"P1";

    int line = 0;
    fstream f(PRODUCTION_PATH, ios::in);
    while(f.getline(temp, 1024))//第二次读取终结符
    {
        //i += 2;//跳过->
        string t = temp;
        //cout<<"read:"<<t<<endl;
        int i = 0;
        while(temp[i] != '>') i++;
        cout<<"i:"<<i<<endl;
        i++;    //=产生式右边的第一个符号
        if(temp[i] == 0)
        {
            N_empty[N_Str2Num[Productions[line].N_Left]] = 1;
        }
        else
        {
            S_Pointer Head_Node = (S_Pointer)malloc(sizeof(Symbol));
            Init_S(Head_Node);
            //Head_Node->next = NULL;
            S_Pointer Cur_Node = Head_Node;
            S_Pointer Last_Node = Head_Node;
            //cout<< "node created"<<endl;
            while(temp[i])
            {
                Cur_Node = (S_Pointer)malloc(sizeof(Symbol));
                Init_S(Cur_Node);
                //Cur_Node ->next = NULL;
                Last_Node->next = Cur_Node;
                string Cur_Symbol = Get_First_Symbol(temp + i);
                cout<<"Cur_Symbol is:"<< Cur_Symbol<<endl;
                if(N_Str2Num[Cur_Symbol])
                {
                    Cur_Node->no = N_Str2Num[Cur_Symbol];
                    Cur_Node->type = 1;
                }
                else if(T_Str2Num[Cur_Symbol])
                {
                    Cur_Node->no = T_Str2Num[Cur_Symbol];
                }
                else
                {
                    T_num++;
                    T_Num2Str[T_num] = Cur_Symbol;
                    T_Str2Num[Cur_Symbol] = T_num;
                    Cur_Node->no = T_Str2Num[Cur_Symbol];
                }
                i += Cur_Symbol.length();
                Last_Node = Cur_Node;
            }
            Productions[line].first_symbol = Head_Node->next;
        }//end of if

        line++;
    }//end of while
    f.close();

    T_Num2Str[0] = "ε";
    T_Str2Num["ε"] = 0;
}

void Print_Production(int n)
{
    cout << Productions[n].N_Left << "->";
    S_Pointer Cur_Node = Productions[n].first_symbol;
    //cout<<"first_symbol type is:"<<Cur_Node->type<<endl;
    while(Cur_Node != NULL)
    {
        if(Cur_Node->type == 0)
            cout<< T_Num2Str[Cur_Node->no];
        else
        {
            cout<< N_Num2Str[Cur_Node->no];
        }
        cout<<"("<<Cur_Node->type<<")";
        Cur_Node = Cur_Node->next;
    }
    cout<<endl;
}

void Print_Statistics()
{
    cout<<"Terminal Symbols ("<<T_num<<") :"<<endl;
    for(int i = 1; i <= T_num; i++)
    {
        cout<< T_Num2Str[i]<<" ";
    }
    cout<<endl<<endl;
    cout<<"Non-terminal Symbols ("<<N_num<<") :"<<endl;
    for(int i = 1; i <= N_num; i++)
    {
        cout<< N_Num2Str[i]<<" ";
    }
    cout<<endl<<endl;
    cout<<"Productions ("<<P_num<<") :"<<endl;
    for(int i = 0; i < P_num; i++)
    {
        Print_Production(i);
    }
    cout<<endl<<endl;
}

bool T_Exists(int flag, int n, int key)//查找某一元素是否出现在First或Follow集中，flag为0则在First集中查找
{
    vector<int> temp;
    if(flag == 0)
        temp.assign(First[n].begin(), First[n].end());
    else
        temp.assign(Follow[n].begin(), Follow[n].end());
    int size = temp.size();
    //cout<<"size:"<<size<<endl;
    for(int i= 0; i < size; i++)
    {
        if(temp[i] == key)
            return true;
    }
    return false;
}

void Print_First_And_Follow()
{
    cout<<"[First]"<<endl;
    for(int i = 1; i <= N_num; i++)
    {
        cout<< N_Num2Str[i]<<": ";
        vector<int> temp;
        temp.assign(First[i].begin(), First[i].end());
        for(int j = 0; j < temp.size(); j++)
        {
            cout<<T_Num2Str[temp[j]]<<" ";
        }
        cout<<endl;
    }

    cout<<"[Follow]"<<endl;
    for(int i = 1; i <= N_num; i++)
    {
        cout<< N_Num2Str[i]<<": ";
        vector<int> temp;
        temp.assign(Follow[i].begin(), Follow[i].end());
        for(int j = 0; j < temp.size(); j++)
        {
            cout<<T_Num2Str[temp[j]]<<" ";
        }
        cout<<endl;
    }
}

bool Calc_First(int i)//返回empty_flag, i是产生式序号
{
    bool empty_flag = 1;
    int Cur_N = N_Str2Num[Productions[i].N_Left];
    S_Pointer Cur_Node = Productions[i].first_symbol;
    while(empty_flag && Cur_Node)
    {
        if(Cur_Node->type == 0)
        {
            if(!T_Exists(0, Cur_N, Cur_Node->no))
            {
                First[Cur_N].push_back(Cur_Node->no);
            }
            empty_flag = 0;
        }
        else
        {
            empty_flag = 0;
            for(int j = 0; j < P_num; j++)
            {
                if(N_Str2Num[Productions[j].N_Left] == Cur_Node->no)
                {
                    int temp_flag = Calc_First(j);
                    empty_flag |= temp_flag;
                    //把产生式右侧N的所有非空First元素加入到当前元素First集下
                    vector<int> temp;
                    temp.assign(First[Cur_Node->no].begin(), First[Cur_Node->no].end());
                    for(int k = 0; k < temp.size(); k++)
                    {
                        if(!T_Exists(0, Cur_N, temp[k]) && temp[k] != 0)
                        {
                            First[Cur_N].push_back(temp[k]);
                        }
                    }
                }
            }
        }
        Cur_Node = Cur_Node->next;
    }
    if(empty_flag)
    {
        if(!T_Exists(0, Cur_N, 0))
        {
            First[Cur_N].push_back(0);
        }
    }
    return empty_flag;
}

bool N_exists_in_Cluster(int N, int Cluster)
{
    //cout<<"------------------"<<endl;
    //bool flag = false;
    vector<int> Cur_Vec = Follow_Cluster[Cluster];
    for(int i = 0; i < Cur_Vec.size(); i++)
    {
        //cout<<"Current_Num:"<<Cur_Vec[i]<<endl;
        if(Cur_Vec[i] == N)
            return true; 
    }
    return false;
}

void Generate_Cluster()
{
    for(int i = 0; i < P_num; i++)//先检查所有具有相同Follow集的非终结符,把它们放到同一个Cluster里
    {
        S_Pointer Cur_Node = Productions[i].first_symbol;
        int Cur_left = N_Str2Num[Productions[i].N_Left];
        while(Cur_Node)
        {
            S_Pointer temp_node = Cur_Node->next;
            if(Cur_Node->type)
            {
                bool ept_flag = true;//检查该非终结符右侧是否全为空或可推导出空
                while(temp_node)
                {
                    if(temp_node->type == 0 || N_empty[temp_node->no] == 0)
                        ept_flag = false;
                    temp_node = temp_node->next;
                }

                if(ept_flag)//把该非终结符和产生式左侧的非终结符加到一个Cluster中
                {
                    int Cur_Cluster = Cur_left;
                    if(!N_exists_in_Cluster(Cur_Node->no, Cur_Cluster))
                        Follow_Cluster[Cur_Cluster].push_back(Cur_Node->no);
                }
            }

            Cur_Node = Cur_Node->next;
        }
    }
}

void Cluster_All_Add(int N, int key)
{
    if(!T_Exists(1, N, key))
        Follow[N].push_back(key);
    if(Follow_Cluster[N].size())
    {
        vector<int> Cur_Cluster = Follow_Cluster[N];
        for(int i = 0; i < Cur_Cluster.size(); i++)
        {
            int Cur_N = Cur_Cluster[i];
            if(!T_Exists(1, Cur_N, key))
                Cluster_All_Add(Cur_N, key);
        }
    }
}

void Calc_Follow()
{
    T_Num2Str[-1] = "$";
    T_Str2Num["$"] = -1;
    Cluster_All_Add(N_Str2Num[Productions[0].N_Left], -1);

    for(int i = 0; i < P_num; i++)
    {
        int Cur_Left = N_Str2Num[Productions[i].N_Left];
        S_Pointer Cur_Node = Productions[i].first_symbol;

        while(Cur_Node)
        {
            if(Cur_Node->type)
            {
                //int Cur_Cluster = Get_Cluster_Num[Cur_Node->no];
                bool temp_flag = true;  //用以判断上个非终结符是否可推空
                S_Pointer temp_node = Cur_Node->next;
                while(temp_node && temp_flag)
                {
                    if(temp_node->type)
                    {
                        int temp = temp_node->no;
                        vector<int> temp_first = First[temp];
                        //把右侧所有符合条件的非终结符的First集中除空外的所有内容加到当前的follow集中
                        for(int i = 0; i < temp_first.size(); i++)
                        {
                            if(!T_Exists(1, Cur_Node->no, temp_first[i]) && temp_first[i] != 0)
                            {
                                Cluster_All_Add(Cur_Node->no, temp_first[i]);
                            }
                        }

                        temp_flag = N_empty[temp];
                    }
                    else
                    {
                        Cluster_All_Add(Cur_Node->no, temp_node->no);
                        temp_flag = false;
                    }
                    temp_node = temp_node->next;
                }//end of temp_node
            }//end of if

            Cur_Node = Cur_Node->next;
        }//end of Cur_Node
    }//end of for
}

int main()
{
    
    Load_Production();
    cout<<"Load OK"<<endl;
    //Print_Production(0);
    Print_Statistics();
    //cout<< N_empty[N_Str2Num["A"]];
    for(int i = 0; i < P_num; i++)
        Calc_First(i);
    Generate_Cluster();
    Calc_Follow();
    Print_First_And_Follow();

    //cout<<"Cluster_Num:"<<C_num<<endl;
    for(int i = 1; i <= N_num; i++)
    {
        vector<int> Cur_Vec = Follow_Cluster[i];
        cout<<"Cluster "<<i<<": ";
        for(int j = 0; j < Cur_Vec.size(); j++)
        {
            cout<<Cur_Vec[j]<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
   /*
   First[0].push_back(4);
   First[0].push_back(6);
   cout<<T_Exists(0, 0, 5);
   */


}