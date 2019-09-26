#pragma once
#include <time.h>
#include <tchar.h>
 
//window上自己实现strptime函数，linux已经提供strptime
//strptime函数windows平台上实现
char*	strptime(const char *buf, const char *fmt, struct tm *tm); 
void	yunshouhu_time2date(time_t time, const char *format, char *date);
void	yunshouhu_date2time(char *date, const char *format, time_t *time);
char*	format_date_time(time_t datetime,char *format);

TCHAR*	_tcsptime(const TCHAR *buf, const TCHAR *fmt, struct tm *tm);