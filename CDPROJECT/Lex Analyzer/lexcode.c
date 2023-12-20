#include <stdio.h>
#include <string.h>
#include <regex.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_TOKENS 1000

typedef enum {
    semi_colon, comma, dot, quote, left_parenthesis, right_parenthesis,
    equal, create, table, select, delete, from, where, insert, into, values,
    update, set, drop, column, add, not, null_token, primary, key, identifier,
    data_type, alter
} token_class;

typedef struct {
    int token_class;
    char *token_text;
    int line_no;
} token;

int token_ptr = 0;
int current_token_ptr = -1;
token token_list[MAX_TOKENS];
int current_line_no = 0;
int flag = 1;

void insert_token(token_class class, const char *text) {
    token_list[token_ptr].token_class = class;
    token_list[token_ptr].token_text = strdup(text);
    token_list[token_ptr].line_no = current_line_no;
    token_ptr++;
}

void tokenizefull() {
    char *a = (char *)malloc(sizeof(char) * 1000);
    while (fgets(a, 1000, stdin)) {
        current_line_no++;
        char *tok = strtok(a, " \t\n");
        while (tok != NULL) {
            int itemp = 0;
            char *temp = strdup(tok);
            while (strlen(temp) > 0) {
                while (!is_type(temp))
                    temp[strlen(temp) - 1] = '\0';
                itemp += strlen(temp);
                strcpy(temp, substring(tok, itemp, strlen(tok)));
            }
            free(temp);
            tok = strtok(NULL, " \t\n");
        }
    }
}

int yylex() {
    if (flag == 1) {
        tokenizefull();
        flag = 0;
    }
    current_token_ptr++;
    return token_list[current_token_ptr].token_class;
}

bool is_type(const char *temp) {
    if (strcmp(temp, ";") == 0) {
        insert_token(semi_colon, ";");
        return true;
    } else if (strcmp(temp, ",") == 0) {
        insert_token(comma, ",");
        return true;
    } else if (strcmp(temp, ".") == 0) {
        insert_token(dot, ".");
        return true;
    } else if (strcmp(temp, "'") == 0) {
        insert_token(quote, "'");
        return true;
    } else if (strcmp(temp, "(") == 0) {
        insert_token(left_parenthesis, "(");
        return true;
    } else if (strcmp(temp, ")") == 0) {
        insert_token(right_parenthesis, ")");
        return true;
    } else if (strcmp(temp, "=") == 0) {
        insert_token(equal, "=");
        return true;
    } else if (strcmp(temp, "create") == 0) {
        insert_token(create, "create");
        return true;
    } else if (strcmp(temp, "table") == 0) {
        insert_token(table, "table");
        return true;
    } else if (strcmp(temp, "select") == 0) {
        insert_token(select, "select");
        return true;
    } else if (strcmp(temp, "delete") == 0) {
        insert_token(delete, "delete");
        return true;
    } else if (strcmp(temp, "from") == 0) {
        insert_token(from, "from");
        return true;
    } else if (strcmp(temp, "where") == 0) {
        insert_token(where, "where");
        return true;
    } else if (strcmp(temp, "insert") == 0) {
        insert_token(insert, "insert");
        return true;
    } else if (strcmp(temp, "into") == 0) {
        insert_token(into, "into");
        return true;
    } else if (strcmp(temp, "values") == 0) {
        insert_token(values, "values");
        return true;
    } else if (strcmp(temp, "update") == 0) {
        insert_token(update, "update");
        return true;
    } else if (strcmp(temp, "set") == 0) {
        insert_token(set, "set");
        return true;
    } else if (strcmp(temp, "drop") == 0) {
        insert_token(drop, "drop");
        return true;
    } else if (strcmp(temp, "column") == 0) {
        insert_token(column, "column");
        return true;
    } else if (strcmp(temp, "add") == 0) {
        insert_token(add, "add");
        return true;
    } else if (strcmp(temp, "not") == 0) {
        insert_token(not, "not");
        return true;
    } else if (strcmp(temp, "null") == 0) {
        insert_token(null_token, "null");
        return true;
    } else if (strcmp(temp, "primary") == 0) {
        insert_token(primary, "primary");
        return true;
    } else if (strcmp(temp, "key") == 0) {
        insert_token(key, "key");
        return true;
    } else if (strcmp(temp, "*") == 0) {
        insert_token(identifier, "*");
        return true;
    } else if (strcmp(temp, "integer") == 0) {
        insert_token(data_type, "integer");
        return true;
    } else if (strcmp(temp, "alter") == 0) {
        insert_token(alter, "alter");
        return true;
    } else if (check_regex(temp)) {
        return true;
    } else {
        // printf("%s:unexpected character\n", temp);
        return false;
    }
}

int check_regex(const char *temp) {
    regex_t regex1, regex2, regex3;
    int reti1, reti2, reti3;
    char *pattern1 = "^varchar([0-9][0-9]*)$";
    char *pattern3 = "^[_a-zA-Z][_a-zA-Z0-9]*$";
    char *pattern2 = "^[0-9][0-9]*$";
    reti1 = regcomp(&regex1, pattern1, 0);
    reti2 = regcomp(&regex2, pattern2, 0);
    reti3 = regcomp(&regex3, pattern3, 0);

    if (!regexec(&regex1, temp, 0, NULL, 0)) {
        insert_token(data_type, temp);
    } else if (!regexec(&regex2, temp, 0, NULL, 0)) {
        insert_token(integer, temp);
    } else if (!regexec(&regex3, temp, 0, NULL, 0)) {
        insert_token(identifier, temp);
    } else {
        return 0;
    }
    return 1;
}

char *substring(const char *input, int start, int end) {
    return strndup(input + start, end - start);
}

const char *yytext() {
    return token_list[current_token_ptr].token_text;
}

int yylineno() {
    return token_list[current_token_ptr].line_no;
}

int main(void) {
    int ntoken = yylex();
    while (ntoken) {
        printf("%d:%s:%d\n", ntoken, yytext(), yylineno());
        ntoken = yylex();
    }
    return 0;
}

