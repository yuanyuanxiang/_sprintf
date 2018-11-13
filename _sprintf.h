#pragma once

// 整数转字符串
inline int _itoa(int num, char buf[32]);

int _i2a(int num, char buf[32], int radix= 10);

// 浮点转字符串
int _ftoa(double val, char buf[32], int eps= 6);

// 浮点转字符串，范围（-1，1）
int __ftoa(double val, char buf[32], int eps= 6);

// 替代sprintf
int _sprintf(char *dst, const char *format, ...);
