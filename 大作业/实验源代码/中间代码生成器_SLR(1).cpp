#include<bits/stdc++.h>
using namespace std;

/**
*R�Ǿ��ʶ�����������Զ���������ʽ
*0 1 2 3 4 5 6 7 8 9 : ;   �����ƽ�״̬
*A B C D E F �����Լ״̬ �ֱ���� E -> E ��0 T , E -> T , T -> T ��1 F , T -> F , F -> i , F -> (E)
*Y ����ʶ��ɹ�
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

stack<char> SYN; //���ջ
stack<char> SEM; //����ջ
vector<string> quats; //��������Ԫʽ
stack<char> analyse_stack; //����ջ
char var;//������ʱ����

int column(char);//���������ַ���Ӧ��״̬�����б��� �� a-z=>0 , ( => 1 , ) => 2 , +- => 3 , */ => 4 , # => 5 ...
char state_transfer(char,char); //�����ھ��ʶ�����н���״̬ת���������ر任���״̬����
void reduce(char); //��Լ
bool build(string&); // ���𲻶϶�ȡ����
void generate_quat(); //����SYN��SEM��������Ԫʽ

void init(){
    var = 'A';
    while(!SYN.empty()) SYN.pop();
    while(!SEM.empty()) SEM.pop();
    quats.clear();
    while(!analyse_stack.empty()) analyse_stack.pop();
}

int main(){
    string s; // ����ı��ʽ
    getline(cin,s);
    while(s!="EXIT"){
        init();
        cout<<"������ı��ʽΪ��";
        cout<<s<<endl;
        s=s+'#';
        analyse_stack.push('#');
        analyse_stack.push('0');

        bool res = build(s);
        if(res)
        {
            cout<<"������ı��ʽ��ȷ,��Ԫʽ�������£�"<<endl;
            for(auto s:quats) cout<<s<<endl;
        }else
        {
            cout<<"������ı��ʽ����"<<endl;
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
        //�ƽ�״̬������ǰ���ʺ�״̬���붼ѹ��ջ,����ȡ��һ������
        else if(after_change>='0' && after_change<=';')
        {
            analyse_stack.push(cur_word);
            analyse_stack.push(after_change);
            i++;
        }
        //��Լ״̬��������Լ�ı��ʽ��Լ������ȡ����
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
        analyse_stack.pop();//��ջ T
        analyse_stack.pop();
        ch_SYN = analyse_stack.top();
        SYN.push(ch_SYN);
        if(SEM.size()>=2 && SYN.size()>=1) generate_quat();
        analyse_stack.pop(); //��ջw0
        analyse_stack.pop();
        analyse_stack.pop(); //��ջE
        next_state = state_transfer(analyse_stack.top(),'E');
        analyse_stack.push('E');
        analyse_stack.push(next_state); // �� ����ʽ�Ҳ� ������ջ
        break;
    case 'B':
        analyse_stack.pop();
        analyse_stack.pop(); //��ջ T
        next_state = state_transfer(analyse_stack.top(),'E');
        analyse_stack.push('E');
        analyse_stack.push(next_state);//��ջE
        break;
    case 'C':
        analyse_stack.pop();
        analyse_stack.pop(); //��ջF
        analyse_stack.pop();
        ch_SYN = analyse_stack.top();
        SYN.push(ch_SYN);
        if(SEM.size()>=2 && SYN.size()>=1) generate_quat();
        analyse_stack.pop(); //��ջ w1
        analyse_stack.pop();
        analyse_stack.pop(); //��ջ T
        next_state = state_transfer(analyse_stack.top(),'T');
        analyse_stack.push('T');
        analyse_stack.push(next_state); //��ջT
        break;
    case 'D':
        analyse_stack.pop();
        analyse_stack.pop(); //��ջ F
        next_state = state_transfer(analyse_stack.top(),'T');
        analyse_stack.push('T');
        analyse_stack.push(next_state); //��ջT
        break;
    case 'E':
        analyse_stack.pop();
        ch_SEM = analyse_stack.top();
        SEM.push(ch_SEM);
        if(SEM.size()>=2 && SYN.size()>=1) generate_quat();
        analyse_stack.pop(); //��ջ i
        next_state = state_transfer(analyse_stack.top(),'F');
        analyse_stack.push('F');
        analyse_stack.push(next_state); //��ջF
        break;
    case 'F':
        analyse_stack.pop();
        analyse_stack.pop(); //��ջ ��
        analyse_stack.pop();
        analyse_stack.pop(); //��ջE
        analyse_stack.pop();
        analyse_stack.pop(); //��ջ��
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
    SEM.push(var); //�������ı����Ž�����ջ
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
