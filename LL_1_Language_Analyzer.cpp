# include <iostream>
# include <map>
# include <queue>
# include <string>
# include <fstream>

# define PRODUCTION_PATH "./doc/production.txt"
//# define TOKEN_TABLE_PATH "./doc/tokens.txt"
//# define TOKEN_MAP_PATH "./doc/token_map.txt"

//# define EOF 0

using namespace std;

int N_num = 0;  //非终结符数量
int T_num = 0;  //终结符数量
int P_num = 0; //产生式数量
map<int, string> T_Num2Str; //从非终结符编号到非终结符的映射
map<string, int> T_Str2Num;
map<int, string> N_Num2Str; //从终结符编号到终结符的映射
map<string, int> N_Str2Num;
map<int,bool> N_empty;

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

int main()
{
    Load_Production();
    cout<<"Load OK"<<endl;
    //Print_Production(0);
    Print_Statistics();
    cout<< N_empty[N_Str2Num["A"]];



}