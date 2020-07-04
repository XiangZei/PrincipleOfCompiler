#include<bits/stdc++.h>
using namespace std;

/**
*R是句柄识别器的有限自动机矩阵形式
*0 1 2 3 4 5 6 7 8 9 : ;   代表移进状态
*A B C D E F 代表归约状态 分别代表 E -> E ω0 T , E -> T , T -> T ω1 F , T -> F , F -> i , F -> (E)
*Y 代表识别成功
**/
char R[13][9] ={{'1','9',' ',' ',' ',' ','3','2','6'},
                {'E','E','E','E','E','E',' ',' ',' '},
                {'D','D','D','D','D','D',' ',' ',' '},
                {' ',' ',' ','4',' ','Y',' ',' ',' '},
                {'1','9',' ',' ',' ',' ',' ','2','5'},
                {' ',' ','A','A','7','A',' ',' ',' '},
                {' ',' ','B','B','7','B',' ',' ',' '},
                {'1','9',' ',' ',' ',' ',' ','8',' '},
                {'C','C','C','C','C','C',' ',' ',' '},
                {'1','9',' ',' ',' ',' ',':','2','6'},
                {' ',' ',';','4',' ',' ',' ',' ',' '},
                {'F','F','F','F','F','F',' ',' ',' '}};

stack<char> SYN; //算符栈
stack<char> SEM; //语意栈
vector<string> quats; //产生的四元式
stack<char> analyse_stack; //分析栈
char var;//生成临时变量

int column(char);//查找输入字符对应的状态机的列编码 如 a-z=>0 , ( => 1 , ) => 2 , +- => 3 , */ => 4 , # => 5 ...
char state_transfer(char,char); //用于在句柄识别器中进行状态转换，并返回变换后的状态编码
void reduce(char); //归约
bool build(string&); // 负责不断读取单词
void generate_quat(); //根据SYN，SEM，产生四元式

void init(){
    var = 'A';
    while(!SYN.empty()) SYN.pop();
    while(!SEM.empty()) SEM.pop();
    quats.clear();
    while(!analyse_stack.empty()) analyse_stack.pop();
}

int main(){
    string s; // 输入的表达式
    getline(cin,s);
    while(s!="EXIT"){
        init();
        cout<<"您输入的表达式为：";
        cout<<s<<endl;
        s=s+'#';
        analyse_stack.push('#');
        analyse_stack.push('0');

        bool res = build(s);
        if(res)
        {
            cout<<"您输入的表达式正确,四元式序列如下："<<endl;
            for(auto s:quats) cout<<s<<endl;
        }else
        {
            cout<<"您输入的表达式有误"<<endl;
        }
        cout<<endl;
        getline(cin,s);
    }
    return 0;
}

bool build(string &s)
{
    int i;
    for(i =0; i < s.size();)
    {
        char cur_word = s[i];
        char after_change = state_transfer(analyse_stack.top(),cur_word);
        if(after_change=='Y') return true;
        if(after_change==' '){ return false; }
        //移进状态，将当前单词和状态编码都压进栈,并读取下一个单词
        else if(after_change>='0' && after_change<=';')
        {
            analyse_stack.push(cur_word);
            analyse_stack.push(after_change);
            i++;
        }
        //规约状态，将待规约的表达式归约，不读取单词
        else if(after_change>='A' && after_change<='F')
        {
            reduce(after_change);
        }
    }
    return false;
}
void reduce(char expression_num)
{
    char ch_SYN;
    char ch_SEM;
    char next_state;
    switch (expression_num){
    case 'A':
        analyse_stack.pop();
        analyse_stack.pop();//出栈 T
        analyse_stack.pop();
        ch_SYN = analyse_stack.top();
        SYN.push(ch_SYN);
        if(SEM.size()>=2 && SYN.size()>=1) generate_quat();
        analyse_stack.pop(); //出栈w0
        analyse_stack.pop();
        analyse_stack.pop(); //出栈E
        next_state = state_transfer(analyse_stack.top(),'E');
        analyse_stack.push('E');
        analyse_stack.push(next_state); // 将 产生式右侧 符号入栈
        break;
    case 'B':
        analyse_stack.pop();
        analyse_stack.pop(); //出栈 T
        next_state = state_transfer(analyse_stack.top(),'E');
        analyse_stack.push('E');
        analyse_stack.push(next_state);//进栈E
        break;
    case 'C':
        analyse_stack.pop();
        analyse_stack.pop(); //出栈F
        analyse_stack.pop();
        ch_SYN = analyse_stack.top();
        SYN.push(ch_SYN);
        if(SEM.size()>=2 && SYN.size()>=1) generate_quat();
        analyse_stack.pop(); //出栈 w1
        analyse_stack.pop();
        analyse_stack.pop(); //出栈 T
        next_state = state_transfer(analyse_stack.top(),'T');
        analyse_stack.push('T');
        analyse_stack.push(next_state); //进栈T
        break;
    case 'D':
        analyse_stack.pop();
        analyse_stack.pop(); //出栈 F
        next_state = state_transfer(analyse_stack.top(),'T');
        analyse_stack.push('T');
        analyse_stack.push(next_state); //进栈T
        break;
    case 'E':
        analyse_stack.pop();
        ch_SEM = analyse_stack.top();
        SEM.push(ch_SEM);
        if(SEM.size()>=2 && SYN.size()>=1) generate_quat();
        analyse_stack.pop(); //出栈 i
        next_state = state_transfer(analyse_stack.top(),'F');
        analyse_stack.push('F');
        analyse_stack.push(next_state); //进栈F
        break;
    case 'F':
        analyse_stack.pop();
        analyse_stack.pop(); //出栈 ）
        analyse_stack.pop();
        analyse_stack.pop(); //出栈E
        analyse_stack.pop();
        analyse_stack.pop(); //出栈（
        next_state = state_transfer(analyse_stack.top(),'F');
        analyse_stack.push('F');
        analyse_stack.push(next_state);
        break;
    }
}

void generate_quat()
{
    char ch_SYN = SYN.top(); SYN.pop();
    char ch_SEM2 = SEM.top(); SEM.pop();
    char ch_SEM1 = SEM.top(); SEM.pop();
    stringstream ss;
    ss<<"("<<ch_SYN<<" "<<ch_SEM1<<" "<<ch_SEM2<<" "<<var<<")";
    string quat = ss.str();
    SEM.push(var); //将产生的变量放进语意栈
    quats.push_back(quat);
    var=var+1;

}

char state_transfer(char r,char c)
{
    int row = r-'0';
    int col = column(c);
    return R[row][col];
}

int column(char c)
{
    if(c>='a'&&c<='z')return 0;
    else if(c=='(') return 1;
    else if(c==')') return 2;
    else if(c=='+'||c=='-') return 3;
    else if(c=='*'||c=='/') return 4;
    else if(c=='#') return 5;
    else if(c=='E') return 6;
    else if(c=='F') return 7;
    else if(c=='T') return 8;
    return -1;
}
