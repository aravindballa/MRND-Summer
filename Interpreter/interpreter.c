#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<string.h>
#include<math.h>
#include<ctype.h>

#define MAX 20

int opStack[MAX], topOp = -1, posIndex = 0, m;
char postfix[MAX];

int isEmpty()
{
	if (topOp == -1)
		return 1;
	return 0;
}

int isFull()
{
	if (topOp == MAX - 1)
		return 1;
	return 0;
}

void push(int k)
{
	if (isFull())
		printf("Too many items in the stack");
	else
		opStack[++topOp] = k;

}

int pop()
{
	if (isEmpty())
		printf("Nothing to pop!");
	else
		return opStack[topOp--];
}

int  GetOpWeight(char op)
{
	switch (op)
	{
	case '^': return 5;
	case '/': return 4;
	case '*': return 3;
	case '+': return 2;
	case '-': return 1;

	}
}

int Result(int a, int b, char op)
{
	switch (op)
	{
	case '^': return pow(a, b);
	case '/': return a / b;
	case '*': return a*b;
	case '+': return a + b;
	case '-': return a - b;

	}
}

int HasMorePriority(char op1, char op2)
{
	if (GetOpWeight(op1) - GetOpWeight(op2) > 0)
		return 1;
	return 0;
}

void InfixToPost(char* input)
{
	int i, num;
	for (m = 0; input[m] != '\0'; m++)
	{

		if (isdigit(input[m]))
		{
			postfix[posIndex++] = input[m];
		}
		else
		{
			if (isEmpty())
				push(input[m]);
			else if (HasMorePriority(opStack[topOp], input[m]))
			{
				while (HasMorePriority(opStack[topOp], input[m]))
				{

					postfix[posIndex++] = pop();
				}
				push(input[m]);
			}
			else
				push(input[m]);
		}
	}

	while (topOp != -1)
	{
		postfix[posIndex++] = pop();
	}

	postfix[posIndex] = '\0';
}

int Evaluate(char* input)
{
	int i, a, b, res;

	InfixToPost(input);

	for (i = 0; i < posIndex; i++)
	{
		if (isdigit(postfix[i]))
			push(postfix[i] - '0');
		else
		{
			a = pop(); b = pop();
			push(Result(b, a, postfix[i]));
		}
	}

	res = pop();

	posIndex = 0;

	return res;
}


struct variable{
	char name[10];
	int value;
	struct variable *next;
} *head;

void PrintVariables()
{
	struct variable *p;
	p = head;

	while (p)
	{
		printf("%s = %d\n", p->name, p->value);
		p = p->next;
	}
}

int ToInt(char *buff)
{
	int i, value = 0;
	for (i = 0; buff[i] != '\0'; i++)
		value = 10 * value + (buff[i] - '0');

	return value;
}

int GetIntValue(char *varname)
{
	struct variable *p;
	p = head;

	while (p)
	{
		if (strcmp(p->name, varname) == 0)
			return p->value;
		p = p->next;
	}

	return NULL;
}

int ComputeIntValue(char *buff)
{
	int i, v = 0, fi = 0, var_value;
	char finalexpr[20] = { '\0' }, var[8] = { '\0' };
	
	for (i = 0; buff[i] != '\0'; i++)
	{
		if (buff[i] == ' ')
			continue;
		else if ((buff[i] >= 'a' && buff[i] <= 'z') || (buff[i] >= 'A' && buff[i] <= 'Z'))
		{
			//read until next nonchar
			while ((buff[i] >= 'a' && buff[i] <= 'z') || (buff[i] >= 'A' && buff[i] <= 'Z'))
				var[v++] = buff[i++];

			i--; //for single letter vars

			//get value of that variable
			var_value = GetIntValue(var);

			//copy that to final expr
			if (var_value == NULL){
				printf("variable %s undeclared...\n", var);
				exit(1);
			}

			while (var_value > 0) //can be written in separate function
			{
				finalexpr[fi++] = var_value % 10 + '0';
				var_value /= 10;
			}
		}
		else
			finalexpr[fi++] = buff[i];
	}

	//get value of the final expression, return it
	return Evaluate(finalexpr);

}

void ParseLine(char* line)
{
	struct variable *temp, *last;
	temp = (struct variable*)malloc(sizeof(struct variable));

	int i, bi = 0;
	char b[10] = { '\0' };
	for (i = 0; line[i] != '='; i++)
		if (line[i] != ' ')
			b[bi++] = line[i];
	i++; // skip = symbol
	strcpy(temp->name, b);

	memset(&b, 0, sizeof(b));
	bi = 0;
	for (; line[i] != '\0'; i++)
	{
		if (line[i] != ' ')
			b[bi++] = line[i];
	}

	temp->value = ComputeIntValue(b);

	//adding the ll
	if (head == NULL)
		temp->next = NULL;
	else
		temp->next = head;
	head = temp;

	PrintVariables();
}


void StoreInput(char *input)
{
	FILE *fp = fopen(input, "r+");
	char buff[128] = { '\0' };
	
	while (!feof(fp))
	{
		fscanf(fp, "%[^\n]\n", buff);
		ParseLine(buff);
	}

	fclose(fp);

	PrintVariables();
}

main()
{
	StoreInput("input.txt");

	_getch();
}