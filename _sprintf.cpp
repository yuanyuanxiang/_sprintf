#include "_sprintf.h"
#include <stdarg.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <Strsafe.h>

#ifndef va_copy
#define va_copy(dst, src) memcpy(&(dst), &(src), sizeof(va_list))
#endif

/************************************************************************
* @brief 整数转字符串
* @param[in] 整数
* @param[out] 字符串
* @return 返回字符串长度
************************************************************************/
int _itoa(int num, char buf[32])
{
	static const char s[] = "0123456789";
	int n = num;
	char *dst = buf;
	if (n < 0) { *dst++ = '-'; n = -n; }
	if (n < 10)
	{
		*dst++ = s[n]; *dst = 0;
	}else
	{
		char tmp[32], *p = tmp;
		while (n) { *p++ = s[n % 10]; n /= 10; }
		while (--p != tmp) *dst++ = *p;
		*dst++ = *tmp; *dst = 0;
	}
	return dst-buf;
}

/************************************************************************
* @brief 浮点数转字符串
* @param[in] 浮点数
* @param[out] 字符串
* @param[in] 精度(小数位)
* @return 返回字符串长度
************************************************************************/
int _ftoa(double val, char buf[32], int eps)
{
	double f = val;
	char *p = buf;
	if (val < 0) { *p++ = '-'; f = -f; }
	int n = f;
	int len = _itoa(n, p);
	return len + __ftoa(f - n, p + len, eps);
}

/************************************************************************
* @brief 浮点数转字符串：范围(-1, 1)
* @param[in] 浮点数
* @param[out] 字符串
* @param[in] 精度(小数位)
* @return 返回字符串长度
************************************************************************/
int __ftoa(double val, char buf[32], int eps)
{
	double f = val;
	char *p = buf;
	assert(-1.0 < f && f < 1.0 && eps > 0);
	static const char s[] = "0123456789";
	if (f<0){*p++ = '-'; f = -f;}*p++ = '.';
	for (int i = eps+1, n; --i; ++p, f -= n)
		*p = s[n = f *= 10.0];
	*p = 0;
	return p-buf;
}

/************************************************************************
* @brief 替换sprintf
* @ref 可变长参数列表误区与陷阱——va_arg不可接受的类型
* http://www.cppblog.com/ownwaterloo/archive/2009/04/21/80655.aspx
************************************************************************/
int _sprintf(char *dst, const char *format, ...)
{
	char *s = dst;
	const char *f = format;
	va_list ap, another;
	va_start(ap, format);
	va_copy(another, ap);
	while (*f)
	{
		int n = 0;
		if ('%' != *f)
		{
			*s = *f;
			n = 1;
		}else{
			++f;
			switch (*f)
			{
			case 's':// 字符串
				{
					const char *p = va_arg(ap, char*);
					n = strlen(p);
					memcpy(s, p, n);
				}
				break;

			case 'd':// 整数
				{
					int i = va_arg(ap, int);
					char buf[32];
					n = _itoa(i, buf);
					memcpy(s, buf, n);
				}
				break;

			case 'f':// 浮点数
				{
					double v = va_arg(ap, double);
					char buf[32];
					n = _ftoa(v, buf);
					memcpy(s, buf, n);
				}
				break;

			case 'c':// 字符
				{
					*s = va_arg(ap, int);
					n = 1;
				}
				break;

			default:
				{
					assert(0);
					va_end(ap);
					int x = vsprintf(dst, format, another);
					va_end(another);
					return x;
				}
				break;
			}
		}
		++f;
		s += n;
	}
	*s = 0;
	va_end(ap);
	return s-dst;
}
