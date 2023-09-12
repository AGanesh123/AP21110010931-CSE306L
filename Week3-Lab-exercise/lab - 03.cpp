#include<iostream>
#include<string>
#include<fstream>

using namespace std;

string symtable[50];
string keyword[30] = {"int", "while", "break", "for", "do", "if", "float", "char", "switch", "double", "short", "long",
    "unsigned", "sizeof", "else", "register", "extern", "static",
    "auto", "case", "break", "volatile", "enum", "typedef"};

bool check_keyword(string id){
    for(int i = 0; i < 24; i++)
        if(id == keyword[i])
            return true;
    return false;
}

void symbol_table(string id, int n){
    int flag = 0;
    for(int j = 0; j < n; j++){
        if(id == symtable[j]){
            flag = 1;
        }
    }

    if(flag == 0){
        symtable[n] = id;
        cout << symtable[n] << "\t" << &symtable[n] << endl;
    }
}

int main(){

    string id, num, rop, spcf;
    int state = 1;
    int l = 0; // Initialize l for spcf

    ifstream f1("x.txt");
    ofstream f2("y.txt");

    char c;
    int i;
    int n = 0;
    int k = 0;
    cout << "Identifiers\t Address\n";

    while(f1.get(c) && !f1.eof()){

        switch(state){
            case 1:
                i = 0;
                if(isalpha(c)){
                    state = 2;
                    id = c;
                    i++;
                }
                else if(isdigit(c)){
                    state = 3;
                    num = c;
                }
                else if(c == '!' || c == '=' || c == '<' || c == '>'){
                    state = 4;
                    rop = c;
                }
                else if(c == '/'){
                    state = 5;
                }
                else if(c == ' ' || c == '\t' || c == '\n'){
                    state = 1;
                }
                else if(c == '(' || c == ')' || c == ',' || c == '{' || c == '}' || c == ';' || c == '"'){
                    state = 1;
                    f2 << "delimiter:\t" << c << endl;
                }
                else if(c == '%' || c == '+' || c == '-' || c == '*' || c == '&'){
                    state = 8;
                    spcf = c;
                }
                else{
                    f2 << c << "\t" << endl;
                }
                break;
            case 2:
                if(isalnum(c)){
                    state = 2;
                    id += c;
                }
                else{
                    if(check_keyword(id))
                        f2 << "keyword:\t" << id << endl;
                    else{
                        f2 << "identifier:\t" << id << endl;
                        n++;
                        symbol_table(id, n);
                    }
                    state = 1;
                    i = 0;
                    f1.putback(c);
                }
                break;
            case 3:
                if(isdigit(c) || c == '.'){
                    state = 3;
                    num += c;
                }
                else{
                    f2 << "number:\t" << num << endl;
                    state = 1;
                    num = "";
                    f1.putback(c);
                }
                break;
            case 4:
                if(c == '='){
                    rop += c;
                    f2 << "relational_operator:\t" << rop << endl;
                    state = 1;
                    rop = "";
                    f1.putback(c);
                }
                else{
                    f2 << "operator:\t" << rop << endl;
                    state = 1;
                    rop = "";
                    f1.putback(c);
                }
                break;
            case 5:
                if(c == '*')
                    state = 6;
                else
                    f2 << "\n invalid lexeme";
                break;
            case 6:
                if(c == '*')
                    state = 7;
                else
                    state = 6;
                break;
            case 7:
                if(c == '*')
                    state = 7;
                else if(c == '/')
                    state = 1;
                else
                    state = 6;
                break;
            case 8:
                if(c == 'd' || c == 'f'){
                    spcf += c;
                    f2 << "format specifier:\t" << spcf << endl;
                    state = 1;
                    spcf = "";
                }
                else{
                    f2 << "operator:\t" << spcf << endl;
                    state = 1;
                    spcf = "";
                    f1.putback(c);
                }
        }
    }
    if(state == 6)
        f2 << "comment did not close";
    f1.close();
    f2.close();
    return 0;
}
