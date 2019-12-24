/**********************************************************
 * Author: Lunium Luk
 * Final Edit Date: 2019/12/24
 * Input Specification:
 * A Postorder expression: (EXAMPLE)
 * 23 34 45 * 5 6 + 7 + / +
 * meaning (23+34*45/(5+6+7)) in Inorder Expression
 * then the program will calculate the expression and output result
 * core algorithm: using char expr[] to read in the data
 * pop them out and push the numbers into a stach and pop two when encounter an operator
 * when converting string to integers, it took a little effort for not using existing functions
**********************************************************/
#include <stdio.h>
#include <stdlib.h>
typedef double ElementType;
#define Infinity 1e8
#define Max_Expr 30   /* max size of expression */
ElementType EvalPostfix( char *expr );
int main()//计算后序遍历表达式
{
    ElementType v;
    char expr[Max_Expr];
    gets(expr);
    v = EvalPostfix( expr );
    if ( v < Infinity )
        printf("%f\n", v);
    else
        printf("ERROR\n");
    return 0;
}
/* Your function will be put here */
ElementType EvalPostfix( char *expr ){
    ElementType num[Max_Expr];
//作为栈储存读入的数字，当碰到运算符时取出栈头的两个数字运算后放入栈头
    int i = 0, numi = 0, flag = 1, pow;
    while(expr[i] != '\0'){
        //printf("//%d//", numi);
        if(expr[i] == ' '){
            i++;
            flag = 1;
        }
        if(expr[i] == '-'){
            if(expr[i + 1] != ' ' && expr[i + 1] != '\0'){
                i++;
                flag = -1;//判断是否是负数
            }
        }
        if(expr[i] >= '0' && expr[i] <= '9'){
            num[numi] = expr[i] - '0';
            i++;
            while(expr[i] != ' ' && expr[i] != '.' && expr[i] != '\0'){
                num[numi] = num[numi] * 10 + expr[i] - '0';
                i++;
            }//处理小数点
            if(expr[i] == '.'){
                pow = 10;
                i++;
                while(expr[i] != ' '){
                    num[numi] += (double)(expr[i] - '0') / pow;
                    pow *= 10;
                    i++;
                }
            }
            num[numi] *= flag;
            numi++;
        }
        else{
            if(numi - 2 < 0){
                //printf("%d", numi);
                return Infinity;
            }
            //printf("$%c$", expr[i]);
            numi--;
            if(expr[i] == '+'){
                num[numi - 1] = num[numi - 1] + num[numi];
            }
            else if(expr[i] == '-'){
                num[numi - 1] = num[numi - 1] - num[numi];
            }
            else if(expr[i] == '*'){
                num[numi - 1] = num[numi - 1] * num[numi];
            }
            else if(expr[i] == '/'){
                if(num[numi] == 0){
                    return Infinity;
                }
                num[numi - 1] = num[numi - 1] / num[numi];
            }
            //printf("%f", num[numi - 1]);
            i++;
        }
        flag = 1;
    }
    if(numi != 1){
        return Infinity;
    }
    return num[0];
}
