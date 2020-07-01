/**********************************/
/*****    계산기 프로그램    ******/
/*****       강 국 진	     ******/
/**********************************/


#include <stdio.h>
#include <Stdlib.h> 
#include <string.h>
#include <math.h>

#define PI 3.14159				//PI 매크로 설정
#define MAX_STACK_SIZE 100
#define MAX_EXPR_SIZE 100
typedef enum {lparen, rparen, nsin ,ncos, ntimes, plus, minus, times, divide, mod, eos, operand} precedence;
//-시 /보다 더 높은 우선순위 처리를 위하여 ntime 생성

char expr[MAX_EXPR_SIZE];			//입력 받은 수식을 저장할 배열
char post_expr[MAX_EXPR_SIZE];		//후위연산 수식을 저장할 배열
double number[MAX_STACK_SIZE];		//입력받은 수를 저장할 배열
double num_stack[MAX_STACK_SIZE];	//연산을 위한 수를 저장할 배열
precedence stack[MAX_STACK_SIZE];	//후위연산을 만들기 위해 쓰이는 배열


static int isp[]={0, 19, 18, 18, 14, 12, 12, 13, 13, 13, 0};	//스택에서 우선 순위 
static int icp[]={20, 19, 18, 18, 14, 12, 12, 13, 13, 13, 0};	//입력 받을 때 우선 순위
//sin과 cos경우 우선 순위가 괄호 다음
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

	inp();			//수식 입력
	postfix();		//후위연산수식으로 변경

	printf("\n\n= %lf\n",eval());	//결과 출력

	return 0;
}

void inp(void)
{
	int i=0, j=0, k, num_i=0; //각각 i는 버퍼, j는 expr, num_i는 수를 저장할 배열의 인덱스
	int paren_cnt=0;		//괄호를 카운트하는 변수
	char c;						//입력 문자를 저장
	char buffer[10];			//임시 버퍼

	while(1)
	{
		c=getchar();			//문자 입력

		if(c=='(')
			paren_cnt++;
		else if(c==')')
			paren_cnt--;

		if(c>='0' && c<='9' || c=='.') //숫자거나 '.'이면 버퍼에 저장 
		{
			if(expr[j-1]==')')			//이전 수식이 )면 *를 넣어줌 (3+2)2 => (3+2)*2
					expr[j++]='*';

			buffer[i++]=c;				//일반 숫자나 .이면 버퍼에 저장
		}
		else if(c=='\n')				//수식의 끝일때
		{
			if(k=strcmp("",buffer))		//버퍼에 값(숫자)이 있으면 수식에 저장 
			{
				buffer[i]='\0';
				number[num_i++]=atof(buffer);
				expr[j++]='n';
			}
			break;						//입력 종료
		}
		else if(c=='-')					//'-'가 들어오면
		{
			if(k=strcmp("",buffer))		//버퍼가 있으면 수식에 저장
			{
				buffer[i]='\0';			//널을 넣어 문자열 완성
				number[num_i++]=atof(buffer); //숫자로 변환하여 숫자배열에 입력
				strcpy(buffer, "");		//버퍼를 비워줌
				expr[j++]='n';			//숫자 자리에 n(number)을 치환

				expr[j++]='+';			//그 버퍼에 +를 해줌 3-2 => 버퍼의 3을 저장 후
				i=0; //버퍼인덱스 초기화//3+ 시켜줌. 만약 버퍼가 없으면 0+
			}

			number[num_i++]=-1.0;		//-1을 곱해준다.  -3 => -1*3
			
			expr[j++]='n';
			expr[j++]='#';				//-1을 곱해줄때 다른 연산보다 높은 우선 순위를 요구하므로 우선순위 높은 *연산인 #을 설정 
			//예) 2-3 => 2 + -1*3	
		}
		else if(c>='a' && c<='z')
		{
			if(i>=1 && buffer[i-1]>='0' && buffer[i-1]<='9')	//앞에 수가 있으면 버퍼를 저장
			{
				buffer[i]='\0';
				if(strlen(buffer)==0)
					number[num_i++]=1;
				else
					number[num_i++]=atof(buffer);
				strcpy(buffer, "");

				expr[j++]='n';
				expr[j++]='*';			//버퍼의 수와 sin, cos 사이에 *를 저장
				i=0;
			}
			
			buffer[i++]=c;		//버퍼에 위 문자를 저장
			
			if(i==3)
			{
				buffer[i]='\0';	//3글자가 채워지면 널값을 넣어 문자열을 만든다.
			
				if(!(k=strcmp(buffer, "cos")))	//cos이면 c를 expr에 넣는다.
				{
					expr[j++]='c';
					strcpy(buffer, "");
					i=0;
				}
				else if(!(k=strcmp(buffer, "sin"))) //sin이면 s를 expr에 넣는다.
				{
					expr[j++]='s';
					strcpy(buffer, "");
					i=0;
				}
				else
				{
					printf("잘못된 연산기호입니다!\n");
					exit(1);
				}
			}
		}
		else
		{
			if( (c=='(' || c==')') && !(k=strcmp("",buffer)))	
			//(와 )가 왔을때 버퍼가 비어있으면 (나 )면 입력
			{
				expr[j++]=c;
			}
			else
			{
				if(c=='/' && expr[j-1]==')')	//'/'가 입력 되었을 때 이전 값이 ')'이면 *를 입력
					expr[j++]='*';

				buffer[i]='\0';			//버퍼값 저장
				if(strlen(buffer)==0)	//버퍼가 빈경우 1을 치환 : (2+3) => '1'*(2+3)을 위해
					number[num_i++]=1;
				else					//버퍼에 값이 있을 경우 그 값을 치환
					number[num_i++]=atof(buffer);
				strcpy(buffer, "");
				expr[j++]='n';
					
				if(c=='(' && expr[j-1]=='n')	//'('가 입력 되었을 때 이전 값이 숫자이면 *를 입력 
					expr[j++]='*';				//2(2+3) => 2'*'(2+3)을 위해

				
				expr[j++]=c;
				i=0;
			}
		}
	}

	if(paren_cnt!=0 || (expr[j-1]!='n' && expr[j-1]!=')') )	// 괄호의 갯수가 맞지 않거나 수 또는 닫는 괄호가 아닌 수로 끝나면 에러
	{
		printf("Fault Expressin!!!\n");
		exit(1);
	}

	printf("\n타입 : ");						//입력 받은 수식의 치환 타입 출력
	for(i=0;i<=j;i++)
		printf("%c ",expr[i]);

	
	printf("\n\nn의 숫자 :\n");					//치환된 숫자를 출력
	for(i=0;i<num_i;i++)
		printf("number[%d] : %lf\n",i+1, number[i]);
}


void postfix(void)
//후위연산으로 변경하는 함수
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
			post_expr[pi++]=symbol;		//숫자면 바로 저장
		}
  
		else if(token==rparen)			//우괄호를 만났을때 
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

	printf("\nPostFix한 수식 : ");		//postFix한 후의 수식 출력
	for(i=0;i<pi;i++)
		printf("%c ", post_expr[i]);
}


precedence get_token(char* symbol, int* n, int s)
//토큰을 취하기 위한 함수
{
	if(s==0)	//후위연산을 할때
		*symbol=expr[(*n)++];
	else		//후위연산을 한 후 결과를 낼떄
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
//토큰을 출력(반환)하기 위한 함수
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
//연산을 위함 함수
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
			add_number(&top, number[i++]);	//치환된 수를 저장한 배열을 계산 스택에 저장
		else if(token==ncos || token==nsin)	//sin, cos일때의 처리
		{
			op1=delete_number(&top);		//스택의 제일 윗자리를 받아둔다
			switch(token)
			{
				case nsin: add_number(&top, sin(op1*PI/180)); break; //그 값을 라디안 연산하여 저장
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
				//모듈 연산시 정수형으로 형변환하여 처리해줌
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