#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXTOKEN 100
#define MAXCHR 100

enum {NAME, PARENS, BRACKETS };
void dcl(void);
void dirdcl(void);
int gettoken(void);
int tokentype; /* тип последней лексемы */
char token[MAXTOKEN]; /* текст последней лексемы */
char name[MAXTOKEN]; /* имя */
char datatype[MAXTOKEN]; /* тип = char, int и т.д. */
char out[1000]; /* выдаваемый текст */


main() /* преобразование объявления в словесное описание */
{
	while (gettoken() != EOF) { /* 1-я лексема в строке */
		strcpy(datatype, token); /* это тип данных */
		out[0] = '\0';
		dcl(); /* разбор остальной части строки */
		if (tokentype != '\n' )
			dcl();
		printf("%s; %s %s\n", name, out, datatype);
	}
	return 0;
}

int getch(void);
void ungetch(int);

int gettoken(void) /* возвращает следующую лексему */
{
	int c;
	char *p = token;
	while ((c = getch()) == ' ' || c == '\t' )
		;
	if (c == '(') {
		if ((c = getch()) == ')') {
			return tokentype = PARENS;
		} else {
			ungetch(c);
			return tokentype = '(';
		}
	} else if (c == '[') {
		for (*p++ = c; (*p++ = getch()) != ']'; )
			;
		*p = '\0' ;
		return tokentype = BRACKETS;
	} else if (isalpha(c)) {
		for (*p++ = c; isalnum(c = getch()); )
			*p++ = c;
		*p = '\0';
		ungetch(c);
		return tokentype = NAME;
	} else
		return tokentype = c;
}

/* dcl: разбор объявителя */
void dcl(void)
{
	int ns, c;
	for (ns = 0; gettoken() == '*'; ) /* подсчет звездочек */
		ns++;
	dirdcl();
	while (ns-- > 0)
		strcat(out, " указ. на");
}

/* dirdcl: разбор собственно объявителя */
void dirdcl(void)
{
	int type;
	if (tokentype == '(') { /* ( dcl ) */
		dcl();
		if (tokentype != ')')
			printf( "ошибка: пропущена )\n");
	} else if (tokentype == NAME)  
		if(strstr(TYPE, token))
		       ;
		else if(strstr(SPEC, token))
			;
		else
			strcpy(name, token);
	else
		printf("ошибка: должно быть name или (dcl)\n");
	while ((type = gettoken()) == PARENS || type == BRACKETS)
		if (type == PARENS)
			strcat(out, " функц. возвр.");
		else {
			strcat(out, " массив");
			strcat(out, token);
			strcat(out, " из");
		}
}

char buf[MAXCHR];
int bufp = 0;

int getch()
{
	return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c)
{
	if(bufp >= MAXCHR)
		printf("Error: buffer is fully\n");
	else
		buf[bufp++] = c;
}
