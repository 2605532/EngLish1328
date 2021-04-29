#ifndef TOKEN_H_INCLUDED
#define TOKEN_H_INCLUDED

#include<iomanip>
#include <stdio.h>
#include <string.h>
using namespace std;

#define RESERVED_TABLE_LENGTH 44     //�����ֱ���
#define Special_STABLE_LENGTH 19     //������ű���
#define CODE_LENGHT 300             //������󳤶�
#define WORD_COUNT 100               //����������


class Token_Snl{
public:
    Token_Snl(){
        for(int i=0;i<RESERVED_TABLE_LENGTH;i++){
            Reserved_ID[i] = i + 1;
        }
        for(int i=0;i<Special_STABLE_LENGTH;i++){
            Special_ID[i] = RESERVED_TABLE_LENGTH + i + 1;
        }
        for(int i=0;i<WORD_COUNT;i++){
            StrID[i] = 0;
        }
        countStr = 0;
    }
    void readSnlFile(char filename[]){
        FILE * fp = NULL;
        codes_length = 0;
        if((fp = fopen(filename,"r")) == NULL)
        {
            cout<<"File open error!";
        }

        char ch;
        ch=fgetc(fp);
        while(ch != EOF)
        {
            codes[codes_length] = ch;
            ch=fgetc(fp);
            codes_length++;
        }
        fclose(fp);
    }
    void printSnlFile(){
        cout<<"\n=====   program   ======\n";
        for(int i=0;i<codes_length;i++){
            cout<<codes[i];
        }
        cout<<"\n===== program end ======\n";
    }

    void tokenAnalyze(){
        char ch = '#';
        int wordB = 0,wordE = 0;
        countStr = 0;
        int line = 1;
        int countdian = 0;
        for(int i=0;i<=codes_length;i++){
            ch = codes[i];
            if(ch == '#')
            {
                cout<<"token NULL\n";
            }
            else if(ch == '.'){
                countdian++;
                 if(wordB != wordE)
                {
                    tokenStr[countStr] = separate(codes,wordB,wordE-1);
                    tokenStrLine[countStr] = line;
                    countStr++;
                }
                wordB = wordE+1;
                wordE = wordB;
                if(codes[i+1] != '.'){
                    //����..���
                    if(countdian > 2){
                        tokenStr[countStr] = "...";
                    }else if(countdian == 2){
                        tokenStr[countStr] = "..";
                    }else
                        tokenStr[countStr] = ".";
                    tokenStrLine[countStr] = line;
                    countStr++;
                    countdian = 0;
                }
            }
            else if(ch == '['||ch == ']'||ch == '('||ch == ')'){
                 if(wordB != wordE)
                {
                    tokenStr[countStr] = separate(codes,wordB,wordE-1);
                    tokenStrLine[countStr] = line;
                    countStr++;
                }
                wordB = wordE+1;
                wordE = wordB;
                tokenStr[countStr] = ch;
                tokenStrLine[countStr] = line;
                countStr++;
            }
            else if(ch == ' '||ch == '\n'||ch == '\t'){
                if(wordB != wordE)
                {
                    tokenStr[countStr] = separate(codes,wordB,wordE-1);
                    tokenStrLine[countStr] = line;
                    countStr++;
                }
                wordB = wordE+1;
                wordE = wordB;
            }
            else if(ch == ';'){
                if(wordB != wordE)
                {
                    tokenStr[countStr] = separate(codes,wordB,wordE-1);
                    tokenStrLine[countStr] = line;
                    countStr++;
                }
                wordB = wordE+1;
                wordE = wordB;
                tokenStr[countStr] = ";";
                tokenStrLine[countStr] = line;
                countStr++;
            }
            else if(i == codes_length){
                if(wordB!=wordE)
                {
                    tokenStr[countStr] = separate(codes,wordB,wordE-1);
                    tokenStrLine[countStr] = line;
                    countStr++;
                }
            }
            else{
                wordE++;
            }

            if(ch == '\n')
                line++;
        }//for:str
        str_analyze();
    }
    void printToken(){
        cout<<"NO   ID   WORD     TYPE"<<endl;
        for(int i=0;i<countStr;i++){
            cout<<setw(5)<<left<<tokenStrLine[i];                //����
            cout<<setw(5)<<left<<StrID[i];              //���ű���
            cout<<setw(10)<<left<<tokenStr[i];          //�ʷ���Ϣ
            cout<<setw(10)<<left<<tokenType[i]<<endl;   //������Ϣ
        }
        printError();
    }
    void printError(){
        cout<<endl;
        int errorCount = 0 ;
        for(int i=0;i<countStr;i++){
            if(tokenType[i] == "#error#")
            {
                cout<<"error: line["<<tokenStrLine[i]<<"]"<<tokenStr[i]<<endl;
                errorCount++;
            }
        }
        cout<<errorCount<<" error(s)"<<endl;
    }
private:
    /*���ո񡢻س����Ʊ����';'�����ַ���*/
    string separate(char str[],int wordB,int wordE){
        string s;
        for(int i=wordB;i<=wordE;i++){
            s=s+str[i];
        }
        return s;
    }
    /*��ʶ��ʶ��*/
    bool id_word(char s[],int s_length){
        bool flag = true;
        char ch = s[0];
        if(ch>='a'&&ch<='z')
            ;
        else if(ch == '_')
            ;
        else
            flag = false;
        for(int i=1;i<s_length;i++)
        {
            ch = s[i];
            if(ch>='a'&&ch<='z')
                ;
            else if(ch == '_')
                ;
            else if(ch>='0'&&ch<='9')
                ;
            else
                flag = false;
        }
        return flag;
    }
    /*������ʶ��*/
    bool reserved_word(string s,int no){
        for(int i=0;i<RESERVED_TABLE_LENGTH;i++){
            if(Reserved_TABLE[i]==s)
            {
                StrID[no] = Reserved_ID[i];
                return true;
            }
        }
        return false;
    }
    /*�������ʶ��*/
    bool special_word(string s,int no){
        for(int i=0;i<Special_STABLE_LENGTH;i++){
            if(Special_STABLE[i]==s)
            {
                StrID[no] = Special_ID[i];
                return true;
            }
        }
        return false;
    }
    /*����ʶ��*/
    bool const_num(char s[],int s_length){
        bool flag = true;
        char ch = s[0];
        if(ch == '0'&&s_length == 1)
            return true;
        if(ch>='1'&&ch<='9')
            ;
        else
            flag = false;
        for(int i=1;i<s_length;i++)
        {
            ch = s[i];
            if(ch>='0'&&ch<='9')
                ;
            else
                flag = false;
        }
        return flag;
    }
    /*���ʷ���*/
    void str_analyze()
    {
        char  s[30];
        for(int i=0;i<countStr;i++){
            //cout<<tokenStr[i]<<"\n";
            strcpy(s,tokenStr[i].c_str());
            if(id_word(s,tokenStr[i].length())){
                if(reserved_word(s,i))
                    tokenType[i] = "KEY_WORD";
                else
                    tokenType[i] = "ID";
            }
            else if(special_word(s,i)){
                tokenType[i] = tokenStr[i];
            }
            else if(const_num(s,tokenStr[i].length())){
                tokenType[i] = "NUMBER";
            }
            else{
                tokenType[i] = "#error#";
            }
        }
    }

    char codes[CODE_LENGHT];    //snl����
    int codes_length;           //snl���볤��
    string tokenStr[WORD_COUNT];        //��������
    string tokenType[WORD_COUNT];       //�������
    int tokenStrLine[WORD_COUNT]; //���������к�
    int StrID[WORD_COUNT];
    int countStr;           //���ʸ���

    //�����ֱ�
    //����µı���������Ӧ���޸�define����������
    string Reserved_TABLE[RESERVED_TABLE_LENGTH] = {
    /**�ؼ���(20)**/
	"program","procedure","type","var","if","then","else","fi","while","do",
	"endwh","begin","end","read","write","array","of","record","return","repeat",
	/**���͹ؼ���(2)**/
	"integer","char1",
	/**��������,�ַ�����(2)**/
	"intc","charc",
	/**��˫�ֽ��,�����±��޽��(16)**/
	"assign","eq","lt","add","sub",
	"mul","div","lparen","rparen","dot",
	"colon","semi","comma","lmidparen","rmidparen",
	"underange",
	/**�ļ�������,�ո��,������(4)**/
	"eof1","space","error1","undiv"
    };
    int Reserved_ID[RESERVED_TABLE_LENGTH];
    //������ű�
    string Special_STABLE[Special_STABLE_LENGTH] = {
    /**�������(8)**/
    ":=","&","|","=","<",">","<=",">=",
    /**�ָ���(7)**/
    ";",",","[","]","(",")","..",
    /**�����(4)**/
    "+","-","*","/"
    };
    int Special_ID[Special_STABLE_LENGTH];
};



#endif // TOKEN_H_INCLUDED
