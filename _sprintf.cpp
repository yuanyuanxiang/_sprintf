#include "_sprintf.h"
#include <string>
#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>

#ifndef va_copy
#define va_copy(dst, src) memcpy(&(dst), &(src), sizeof(va_list))
#endif

/************************************************************************
* @brief 整数转字符串
* @param[in] num 整数
* @param[out] buf 字符串
* @return 返回字符串长度
************************************************************************/
int _itoa(int num, char buf[32])
{
	return _i2a(num, buf, 10);
}

/************************************************************************
* @brief 整数转字符串
* @param[in] num 整数
* @param[out] buf 字符串
* @param[in] radix 进位制整数
* @return 返回字符串长度
************************************************************************/
int _i2a(int num, char buf[32], int radix)
{
	static const char s[] = "0123456789abcdef";
	int n = num, R = radix;
	char *dst = buf;
	if (n < 0) { *dst++ = '-'; n = -n; }
	if (n < 10)
	{
		*dst++ = s[n]; *dst = 0;
	}else
	{
		char tmp[32], *p = tmp;
		while (n) { *p++ = s[n % R]; n /= R; }
		while (--p != tmp) *dst++ = *p;
		*dst++ = *tmp; *dst = 0;
	}
	return dst-buf;
}

/************************************************************************
* @brief 浮点数转字符串
* @param[in] val 浮点数
* @param[out] buf 字符串
* @param[in] eps 精度(小数位)
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
* @param[in] val 浮点数
* @param[out] buf 字符串
* @param[in] eps 精度(小数位)
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
		int n = 1;
		if ('%' != *f)
		{
			*s = *f;
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

			case 'd': case 'u':// 整数
				{
					char buf[32];
					n = _itoa(va_arg(ap, int), buf);
					memcpy(s, buf, n);
				}
				break;

			case 'f':// 浮点数
				{
					char buf[32];
					n = _ftoa(va_arg(ap, double), buf);
					memcpy(s, buf, n);
				}
				break;

			case 'x':// 16进制数
				{
					char buf[32];
					n = _i2a(va_arg(ap, int), buf, 16);
					memcpy(s, buf, n);
				}
				break;

			case 'c':// 字符
				{
					*s = va_arg(ap, int);
				}
				break;

			case '%':// 百分号
				{
					*s = '%';
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
