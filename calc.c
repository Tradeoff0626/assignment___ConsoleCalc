/**********************************/
/*****    ���� ���α׷�    ******/
/*****       �� �� ��	     ******/
/**********************************/


#include <stdio.h>
#include <Stdlib.h> 
#include <string.h>
#include <math.h>

#define PI 3.14159				//PI ��ũ�� ����
#define MAX_STACK_SIZE 100
#define MAX_EXPR_SIZE 100
typedef enum {lparen, rparen, nsin ,ncos, ntimes, plus, minus, times, divide, mod, eos, operand} precedence;
//-�� /���� �� ���� �켱���� ó���� ���Ͽ� ntime ����

char expr[MAX_EXPR_SIZE];			//�Է� ���� ������ ������ �迭
char post_expr[MAX_EXPR_SIZE];		//�������� ������ ������ �迭
double number[MAX_STACK_SIZE];		//�Է¹��� ���� ������ �迭
double num_stack[MAX_STACK_SIZE];	//������ ���� ���� ������ �迭
precedence stack[MAX_STACK_SIZE];	//���������� ����� ���� ���̴� �迭


static int isp[]={0, 19, 18, 18, 14, 12, 12, 13, 13, 13, 0};	//���ÿ��� �켱 ���� 
static int icp[]={20, 19, 18, 18, 14, 12, 12, 13, 13, 13, 0};	//�Է� ���� �� �켱 ����
//sin�� cos��� �켱 ������ ��ȣ ����
//sin(10+80) => sin(90),  sin10+80 =>sin(10)+80


void inp(void);
void postfix(void);
precedence get_token(char*, int*,int);
char print_token(precedence);
precedence delete_stack(int*);
void add(int*, precedence);
double eval(void);
double delete_number(int*);
void add_number(int*, double);


int main()
{
	printf("Expression>> ");

	inp();			//���� �Է�
	postfix();		//��������������� ����

	printf("\n\n= %lf\n",eval());	//��� ���

	return 0;
}

void inp(void)
{
	int i=0, j=0, k, num_i=0; //���� i�� ����, j�� expr, num_i�� ���� ������ �迭�� �ε���
	int paren_cnt=0;		//��ȣ�� ī��Ʈ�ϴ� ����
	char c;						//�Է� ���ڸ� ����
	char buffer[10];			//�ӽ� ����

	while(1)
	{
		c=getchar();			//���� �Է�

		if(c=='(')
			paren_cnt++;
		else if(c==')')
			paren_cnt--;

		if(c>='0' && c<='9' || c=='.') //���ڰų� '.'�̸� ���ۿ� ���� 
		{
			if(expr[j-1]==')')			//���� ������ )�� *�� �־��� (3+2)2 => (3+2)*2
					expr[j++]='*';

			buffer[i++]=c;				//�Ϲ� ���ڳ� .�̸� ���ۿ� ����
		}
		else if(c=='\n')				//������ ���϶�
		{
			if(k=strcmp("",buffer))		//���ۿ� ��(����)�� ������ ���Ŀ� ���� 
			{
				buffer[i]='\0';
				number[num_i++]=atof(buffer);
				expr[j++]='n';
			}
			break;						//�Է� ����
		}
		else if(c=='-')					//'-'�� ������
		{
			if(k=strcmp("",buffer))		//���۰� ������ ���Ŀ� ����
			{
				buffer[i]='\0';			//���� �־� ���ڿ� �ϼ�
				number[num_i++]=atof(buffer); //���ڷ� ��ȯ�Ͽ� ���ڹ迭�� �Է�
				strcpy(buffer, "");		//���۸� �����
				expr[j++]='n';			//���� �ڸ��� n(number)�� ġȯ

				expr[j++]='+';			//�� ���ۿ� +�� ���� 3-2 => ������ 3�� ���� ��
				i=0; //�����ε��� �ʱ�ȭ//3+ ������. ���� ���۰� ������ 0+
			}

			number[num_i++]=-1.0;		//-1�� �����ش�.  -3 => -1*3
			
			expr[j++]='n';
			expr[j++]='#';				//-1�� �����ٶ� �ٸ� ���꺸�� ���� �켱 ������ �䱸�ϹǷ� �켱���� ���� *������ #�� ���� 
			//��) 2-3 => 2 + -1*3	
		}
		else if(c>='a' && c<='z')
		{
			if(i>=1 && buffer[i-1]>='0' && buffer[i-1]<='9')	//�տ� ���� ������ ���۸� ����
			{
				buffer[i]='\0';
				if(strlen(buffer)==0)
					number[num_i++]=1;
				else
					number[num_i++]=atof(buffer);
				strcpy(buffer, "");

				expr[j++]='n';
				expr[j++]='*';			//������ ���� sin, cos ���̿� *�� ����
				i=0;
			}
			
			buffer[i++]=c;		//���ۿ� �� ���ڸ� ����
			
			if(i==3)
			{
				buffer[i]='\0';	//3���ڰ� ä������ �ΰ��� �־� ���ڿ��� �����.
			
				if(!(k=strcmp(buffer, "cos")))	//cos�̸� c�� expr�� �ִ´�.
				{
					expr[j++]='c';
					strcpy(buffer, "");
					i=0;
				}
				else if(!(k=strcmp(buffer, "sin"))) //sin�̸� s�� expr�� �ִ´�.
				{
					expr[j++]='s';
					strcpy(buffer, "");
					i=0;
				}
				else
				{
					printf("�߸��� �����ȣ�Դϴ�!\n");
					exit(1);
				}
			}
		}
		else
		{
			if( (c=='(' || c==')') && !(k=strcmp("",buffer)))	
			//(�� )�� ������ ���۰� ��������� (�� )�� �Է�
			{
				expr[j++]=c;
			}
			else
			{
				if(c=='/' && expr[j-1]==')')	//'/'�� �Է� �Ǿ��� �� ���� ���� ')'�̸� *�� �Է�
					expr[j++]='*';

				buffer[i]='\0';			//���۰� ����
				if(strlen(buffer)==0)	//���۰� ���� 1�� ġȯ : (2+3) => '1'*(2+3)�� ����
					number[num_i++]=1;
				else					//���ۿ� ���� ���� ��� �� ���� ġȯ
					number[num_i++]=atof(buffer);
				strcpy(buffer, "");
				expr[j++]='n';
					
				if(c=='(' && expr[j-1]=='n')	//'('�� �Է� �Ǿ��� �� ���� ���� �����̸� *�� �Է� 
					expr[j++]='*';				//2(2+3) => 2'*'(2+3)�� ����

				
				expr[j++]=c;
				i=0;
			}
		}
	}

	if(paren_cnt!=0 || (expr[j-1]!='n' && expr[j-1]!=')') )	// ��ȣ�� ������ ���� �ʰų� �� �Ǵ� �ݴ� ��ȣ�� �ƴ� ���� ������ ����
	{
		printf("Fault Expressin!!!\n");
		exit(1);
	}

	printf("\nŸ�� : ");						//�Է� ���� ������ ġȯ Ÿ�� ���
	for(i=0;i<=j;i++)
		printf("%c ",expr[i]);

	
	printf("\n\nn�� ���� :\n");					//ġȯ�� ���ڸ� ���
	for(i=0;i<num_i;i++)
		printf("number[%d] : %lf\n",i+1, number[i]);
}


void postfix(void)
//������������ �����ϴ� �Լ�
{
	char symbol;
	precedence token;
	int n=0, i=0, pi=0;
	int top=0;
	stack[0]=eos;

	for(token = get_token(&symbol, &n, 0); token!=eos; token=get_token(&symbol, &n, 0))
	{
		if(token==operand)
		{
			post_expr[pi++]=symbol;		//���ڸ� �ٷ� ����
		}
  
		else if(token==rparen)			//���ȣ�� �������� 
		{
			while(stack[top]!=lparen)
				post_expr[pi++]=print_token(delete_stack(&top));
			delete_stack(&top);
		}
		else
		{
			while (isp[stack[top]]>=icp[token])
				post_expr[pi++]=print_token(delete_stack(&top));
			add(&top, token);
		}
	}

	while((token=delete_stack(&top))!=eos)
	{
		printf(" ");
		post_expr[pi++]=print_token(token);
	}

	printf("\nPostFix�� ���� : ");		//postFix�� ���� ���� ���
	for(i=0;i<pi;i++)
		printf("%c ", post_expr[i]);
}


precedence get_token(char* symbol, int* n, int s)
//��ū�� ���ϱ� ���� �Լ�
{
	if(s==0)	//���������� �Ҷ�
		*symbol=expr[(*n)++];
	else		//���������� �� �� ����� ����
		*symbol=post_expr[(*n)++];

	switch(*symbol)
	{
		case '(' : return lparen;
		case ')' : return rparen;
		case '+' : return plus;
		case '-' : return minus;
		case '/' : return divide;
		case '*' : return times;
		case '%' : return mod;
		case '\0' : return eos;
		case 's' : return nsin;
		case 'c' : return ncos;
		case '#' : return ntimes; 
		default : return operand;
	 }
}


char print_token(precedence symbol)
//��ū�� ���(��ȯ)�ϱ� ���� �Լ�
{
	switch(symbol)
	{
		case lparen : 
		case rparen : break;
		case plus : return '+';
		case minus : return '-';
		case divide : return '/';
		case times : return '*';
		case mod : return '%';
		case nsin : return 's';
		case ncos : return 'c';
		case ntimes : return '#';
		case eos : break;
	}

	return 0;
}


precedence delete_stack(int* n)
{
	return stack[(*n)--];
}

void add(int *n, precedence symbol)
{
	stack[++(*n)]=symbol; 
}


double eval(void)
//������ ���� �Լ�
{
	precedence token;
	char symbol;
	double op1, op2;
	int n=0, i=0;
	int top=-1;
	token = get_token(&symbol, &n, 1);
	while(token!=eos)
	{
		if(token==operand)
			add_number(&top, number[i++]);	//ġȯ�� ���� ������ �迭�� ��� ���ÿ� ����
		else if(token==ncos || token==nsin)	//sin, cos�϶��� ó��
		{
			op1=delete_number(&top);		//������ ���� ���ڸ��� �޾Ƶд�
			switch(token)
			{
				case nsin: add_number(&top, sin(op1*PI/180)); break; //�� ���� ���� �����Ͽ� ����
				case ncos: add_number(&top, cos(op1*PI/180)); break;
			}
		}
		else
		{
			op2=delete_number(&top);
			op1=delete_number(&top);
			switch(token)
			{
				case plus: add_number(&top, op1+op2); break;
				case minus: add_number(&top, op1-op2); break;
				case times: add_number(&top, op1*op2); break;
				case divide: add_number(&top, op1/op2); break;
				case ntimes: add_number(&top, op1*op2); break;
				case mod: add_number(&top, (double)((int)op1%(int)op2)); break;
				//��� ����� ���������� ����ȯ�Ͽ� ó������
			}
		}
		token=get_token(&symbol,&n, 1);
	}

	return delete_number(&top);
}

double delete_number(int* n)
{
	return num_stack[(*n)--];
}

void add_number(int *n, double number)
{
	num_stack[++(*n)]=number; 
}