#include "_sprintf.h"
#include <windows.h>
#include <stdio.h>
#include <time.h>

#ifdef _DEBUG
#define TIMES 100
#define N 128
#else
#define TIMES 20000
#define N 1024
#endif

typedef char str64[100];

int main()
{
	char buf[32] = {0};
	int len = _itoa(0, buf);
	len = _itoa(-1, buf);
	len = _itoa(123456789, buf);
	len = _itoa(-123456789, buf);

	len = _ftoa(987654321.123456789, buf);
	len = _ftoa(-0.123456789, buf);
	len = _ftoa(.1, buf);

	// 生成一组随机数
	srand(time(NULL));
	float arr[N];
	for (int i = 0; i < N; ++i)
	{
		arr[i] = 1e2*rand()/RAND_MAX;
	}

	str64 _p[N], p[N];
	// _sprintf
	clock_t t = clock();
	for (int j = 0; j < TIMES; ++j)
	{
		for (int i = 0; i < N; ++i)
		{
			_sprintf(_p[i], "string= %s, int= %d, float= %f, char= %c.\n", 
				"字符串", int(arr[i]), arr[i], 'c');
		}
	}
	int t1 = clock() - t;
	printf("_sprintf using: %d ms\n", t1);
	
	// sprintf
	t = clock();
	for (int j = 0; j < TIMES; ++j)
	{
		for (int i = 0; i < N; ++i)
		{
			sprintf(p[i], "string= %s, int= %d, float= %.6f, char= %c.\n", 
				"字符串", int(arr[i]), arr[i], 'c');
		}
	}
	int t2 = clock() - t;
	printf("sprintf_s using: %d ms\n\n", t2);

	int n = rand() % N;
	printf("_p[%d]: %s\n", n, _p[n]);
	printf("speed up: %f\n", 1.0*t2/t1);
	// compare
	for (int i = 0; i < N; ++i)
	{
		if (strcmp(_p[i], p[i]))
		{
			printf(_p[i]);
			printf("\n");
			printf(p[i]);
			printf("\n\n");
		}
	}

	system("PAUSE");

	return 0;
}
