
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINES 2000
#define MAX_VARS 200
#define MAX_FUNCS 100

typedef struct {
    char name[50];
    char value[256];
} Var;

typedef struct {
    char name[50];
    int startLine;
} Function;

Var vars[MAX_VARS];
int varCount = 0;

Function funcs[MAX_FUNCS];
int funcCount = 0;

char *lines[MAX_LINES];
int lineCount = 0;

char *trim(char *str) {
    while (isspace(*str)) str++;
    char *end = str + strlen(str) - 1;
    while (end > str && isspace(*end)) *end-- = '\0';
    return str;
}

char* getVar(char *name) {
    for (int i = 0; i < varCount; i++)
        if (strcmp(vars[i].name, name) == 0)
            return vars[i].value;
    return "";
}

void setVar(char *name, char *value) {
    for (int i = 0; i < varCount; i++) {
        if (strcmp(vars[i].name, name) == 0) {
            strcpy(vars[i].value, value);
            return;
        }
    }
    strcpy(vars[varCount].name, name);
    strcpy(vars[varCount].value, value);
    varCount++;
}

int evaluateMath(char *expr) {
    int a = 0, b = 0;
    char op;
    sscanf(expr, "%d %c %d", &a, &op, &b);
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return b != 0 ? a / b : 0;
        default: return atoi(expr);
    }
}

int evaluateCondition(char *expr) {
    int a = 0, b = 0;
    char op[3];
    sscanf(expr, "%d %2s %d", &a, op, &b);
    if (strcmp(op, "==") == 0) return a == b;
    if (strcmp(op, "!=") == 0) return a != b;
    if (strcmp(op, ">") == 0) return a > b;
    if (strcmp(op, "<") == 0) return a < b;
    if (strcmp(op, ">=") == 0) return a >= b;
    if (strcmp(op, "<=") == 0) return a <= b;
    return 0;
}

int executeBlock(int start);

void loadFile(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return;
    char buffer[512];
    while (fgets(buffer, sizeof(buffer), fp)) {
        lines[lineCount++] = strdup(buffer);
    }
    fclose(fp);
}

int executeBlock(int start) {
    for (int i = start + 1; i < lineCount; i++) {
        char *line = trim(lines[i]);
        if (strcmp(line, "}") == 0) return i;
        if (line[0] == '#') continue;

        if (strncmp(line, "import", 6) == 0) {
            char fname[100];
            sscanf(line, "import "%[^"]"", fname);
            loadFile(fname);
            continue;
        }

        if (strncmp(line, "say:", 4) == 0) {
            char *text = trim(line + 4);
            if (text[0] == '"') {
                printf("%s\n", text + 1);
            } else {
                printf("%s\n", getVar(text));
            }
        } else if (strstr(line, "<-") != NULL) {
            char *name = strtok(line, "<-");
            char *val = strtok(NULL, "<-");
            name = trim(name);
            val = trim(val);
            if (strcmp(val, "ask") == 0) {
                char input[256];
                fgets(input, 256, stdin);
                input[strcspn(input, "\n")] = 0;
                setVar(name, input);
            } else if (isdigit(val[0]) || strchr(val, '+') || strchr(val, '-')) {
                int result = evaluateMath(val);
                char buf[50];
                sprintf(buf, "%d", result);
                setVar(name, buf);
            } else {
                setVar(name, val);
            }
        } else if (strncmp(line, "loop", 4) == 0) {
            int times;
            sscanf(line, "loop %d times {", &times);
            int loopEnd = executeBlock(i);
            for (int j = 0; j < times; j++)
                executeBlock(i);
            i = loopEnd;
        } else if (strncmp(line, "when", 4) == 0) {
            char cond[100];
            sscanf(line, "when %[^\{]", cond);
            if (evaluateCondition(trim(cond))) {
                i = executeBlock(i);
            } else {
                while (i < lineCount && strstr(lines[i], "else") == NULL) i++;
                if (i < lineCount) i = executeBlock(i);
            }
        } else if (strncmp(line, "while", 5) == 0) {
            char cond[100];
            sscanf(line, "while %[^\{]", cond);
            int loopStart = i;
            int loopEnd = executeBlock(i);
            while (evaluateCondition(trim(cond))) {
                executeBlock(loopStart);
            }
            i = loopEnd;
        } else if (strncmp(line, "fn", 2) == 0) {
            char fname[50];
            sscanf(line, "fn %s {", fname);
            strcpy(funcs[funcCount].name, fname);
            funcs[funcCount].startLine = i;
            funcCount++;
            i = executeBlock(i);
        } else if (strncmp(line, "call", 4) == 0) {
            char fname[50];
            sscanf(line, "call %s", fname);
            for (int f = 0; f < funcCount; f++)
                if (strcmp(funcs[f].name, fname) == 0)
                    executeBlock(funcs[f].startLine);
        }
    }
    return lineCount;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: veko <file.vk>\n");
        return 1;
    }
    loadFile(argv[1]);
    executeBlock(-1);
    return 0;
}
