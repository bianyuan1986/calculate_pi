#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PRECISION_COUNT 20000
#define ITERATION_COUNT 100000

unsigned char x[PRECISION_COUNT];
unsigned char z[PRECISION_COUNT];

void printPai(unsigned char *pai, int precision)
{
	int i = 0;

	printf("%u%u.", pai[0], pai[1]);
	for( i = 2; i < precision; i++)
	{
		printf("%u", pai[i]);
	}
	printf("\n");
}

int search(unsigned char *pai, int len, int paiLen)
{
	int i = 0;
	int j = 0;
	unsigned char *pos = pai;
	unsigned char *end = pai + len - paiLen;
	int find = 1;

	while( pos <= end )
	{
		find = 1;
		for( i = 0,j = paiLen-1; i < j; i++,j-- )
		{
			if( pos[i] != pos[j] )
			{
				find = 0;
				break;
			}
		}

		if( find )
		{
			return pos-pai;
		}
		pos++;
	}

	return -1;
}

/* pai/2 = 1 + 1/3 + (1/3)*(2/5)*(3/7) + (1/3)*(2/5)*(3/7)*(4/9) + (1/3)*(2/5)*...*(n/(3+2*(n-1))) 
   pai = 2 + 2/3 + (2/3)*(2/5)*(3/7) + (2/3)*(2/5)*(3/7)*(4/9) + (2/3)*(2/5)*...*(n/(3+2*(n-1))) */
void pai(int precision)
{
	int a = 1;
	int b = 3;
	int c = 0;
	int d = 0;
	int cnt = 0;
	int i = 0;
	unsigned int run = 1;

	memset(x, 0x00, sizeof(x));
	memset(z, 0x00, sizeof(z));

	//x用来存储最终计算结果，每次叠加
	x[1] = 2;
	//z中用来计算上述表达书中的每个乘除法部分，保留上次乘除的计算结果用于当前计算，避免重复计算
	z[1] = 2;

	/*
	 * 1、z中值计算过程为上次计算的值为valueLast, valueCurrent = valueLast*a/b，即valueLast*n/(3+2*(n-1))
	 */
	while( run && (cnt++ < ITERATION_COUNT) )
	{
		//z *= a
		d = 0;
		for(i = PRECISION_COUNT-1; i > 0; i--)
		{
			//当前位乘以a加上右边位的进位
			c = z[i]*a + d;
			//计算当前位的值
			z[i] = c % 10;
			//计算进位数
			d = c / 10;
		}

		//z /= b
		d = 0;
		for( i = 0; i < PRECISION_COUNT; i++)
		{
			//上一位的余数累加到当前位
			c = z[i] + d*10;
			//当前位的商值
			z[i] = c / b;
			//当前位余数，用于累计到下一位的除法计算中
			d = c % b;
		}

		//x += z
		run = 0;
		for( i = PRECISION_COUNT-1; i > 0; i--)
		{
			//进行累加计算，计算当前位的和
			c = x[i] + z[i];
			//求当前位的值
			x[i] = c % 10;
			//如果需要进位则向前一位进位
			x[i-1] += c / 10;
			//避免不必要的迭代，如果计算出的z全为0则可以停止迭代
			run |= z[i];
		}

		a++;
		b += 2;
	}
}

int main(int argc, char *argv[])
{
	int pos = 0;
	struct timeval t1;
	struct timeval t2;

	gettimeofday(&t1, NULL);
	pai(PRECISION_COUNT);
	gettimeofday(&t2, NULL);
	printf("Calculate pai consumed:%lds\n", t2.tv_sec-t1.tv_sec);

	printPai(x, PRECISION_COUNT);
	pos = search(x, PRECISION_COUNT, 10);
	if( pos >= 0 )
	{
		printf("Find in pos:%d\n", pos-2+1);
	}

	return 0;
}

