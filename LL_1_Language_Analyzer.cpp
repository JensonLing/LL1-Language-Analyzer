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
    int type = 0; //0为终结符, 1为非终结符
    int no;
    S_Pointer next;
}Symbol, *S_Pointer;

typedef struct Production
{
    //int N_No;  //左侧非终结符编号
    string N_Left;
    S_Pointer first_symbol; //指向产生式右边第一个符号
}Production, *P_Pointer;

Production Productions[100];//最多支持100个产生式

bool Same_Symbol(S_Pointer A, S_Pointer B)
{
    if(A->no == B->no && A->type == B->type)
        return true;
    return false;
}

void Load_Production()
{
    fstream t(PRODUCTION_PATH, ios::in);
    char temp[1024];
    string s;
    while(t.getline(temp, 1024))
    {
        int i = 0;
        char N[100];
        while(temp[i] != '-') i++;
        for(int j = 0; j < i; j++)
            N[j] = temp[i];
        
        string N_Symbol = N;
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

        i += 2;

        if(temp[i] == 0)
        {
            N_empty[N_Str2Num[N_Symbol]] = 1;
        }
        else
        {
            
            while(temp[i])
            {

            }
        }
        



        
    }
}

void Print_Production(int n)
{

}

int main()
{
    
}