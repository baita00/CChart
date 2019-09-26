#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include <windows.h>

#include "mytime.h"
 
#define TM_YEAR_BASE 1900
 
/*
 * We do not implement alternate representations. However, we always
 * check whether a given modifier is allowed for a certain conversion.
 */
#define ALT_E     0x01
#define ALT_O     0x02
#define LEGAL_ALT(x)    { if (alt_format & ~(x)) return (0); }
 
 
static  int conv_num(const char **, int *, int, int);
 
static const char *day[7] = {
  "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday",
  "Friday", "Saturday"
};
static const char *abday[7] = {
  "Sun","Mon","Tue","Wed","Thu","Fri","Sat"
};
static const char *mon[12] = {
  "January", "February", "March", "April", "May", "June", "July",
  "August", "September", "October", "November", "December"
};
static const char *abmon[12] = {
  "Jan", "Feb", "Mar", "Apr", "May", "Jun",
  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};
static const char *am_pm[2] = {
  "AM", "PM"
};
 
 
//window上自己实现strptime函数，linux已经提供strptime
//strptime函数windows平台上实现
char*	strptime(const char *buf, const char *fmt, struct tm *tm)
{
  char c;
  const char *bp;
  size_t len = 0;
  int alt_format, i, split_year = 0;
 
  bp = buf;
 
  while ((c = *fmt) != '\0') {
    /* Clear `alternate' modifier prior to new conversion. */
    alt_format = 0;
 
    /* Eat up white-space. */
    if (isspace(c)) {
      while (isspace(*bp))
        bp++;
 
      fmt++;
      continue;
    }
        
    if ((c = *fmt++) != '%')
      goto literal;
 
 
again:    switch (c = *fmt++) {
    case '%': /* "%%" is converted to "%". */
literal:
      if (c != *bp++)
        return (0);
      break;
 
    /*
     * "Alternative" modifiers. Just set the appropriate flag
     * and start over again.
     */
    case 'E': /* "%E?" alternative conversion modifier. */
      LEGAL_ALT(0);
      alt_format |= ALT_E;
      goto again;
 
    case 'O': /* "%O?" alternative conversion modifier. */
      LEGAL_ALT(0);
      alt_format |= ALT_O;
      goto again;
      
    /*
     * "Complex" conversion rules, implemented through recursion.
     */
    case 'c': /* Date and time, using the locale's format. */
      LEGAL_ALT(ALT_E);
      if (!(bp = strptime(bp, "%x %X", tm)))
        return (0);
      break;
 
    case 'D': /* The date as "%m/%d/%y". */
      LEGAL_ALT(0);
      if (!(bp = strptime(bp, "%m/%d/%y", tm)))
        return (0);
      break;
 
    case 'R': /* The time as "%H:%M". */
      LEGAL_ALT(0);
      if (!(bp = strptime(bp, "%H:%M", tm)))
        return (0);
      break;
 
    case 'r': /* The time in 12-hour clock representation. */
      LEGAL_ALT(0);
      if (!(bp = strptime(bp, "%I:%M:%S %p", tm)))
        return (0);
      break;
 
    case 'T': /* The time as "%H:%M:%S". */
      LEGAL_ALT(0);
      if (!(bp = strptime(bp, "%H:%M:%S", tm)))
        return (0);
      break;
 
    case 'X': /* The time, using the locale's format. */
      LEGAL_ALT(ALT_E);
      if (!(bp = strptime(bp, "%H:%M:%S", tm)))
        return (0);
      break;
 
    case 'x': /* The date, using the locale's format. */
      LEGAL_ALT(ALT_E);
      if (!(bp = strptime(bp, "%m/%d/%y", tm)))
        return (0);
      break;
 
    /*
     * "Elementary" conversion rules.
     */
    case 'A': /* The day of week, using the locale's form. */
    case 'a':
      LEGAL_ALT(0);
      for (i = 0; i < 7; i++) {
        /* Full name. */
        len = strlen(day[i]);
        if (strncmp(day[i], bp, len) == 0)
          break;
 
        /* Abbreviated name. */
        len = strlen(abday[i]);
        if (strncmp(abday[i], bp, len) == 0)
          break;
      }
 
      /* Nothing matched. */
      if (i == 7)
        return (0);
 
      tm->tm_wday = i;
      bp += len;
      break;
 
    case 'B': /* The month, using the locale's form. */
    case 'b':
    case 'h':
      LEGAL_ALT(0);
      for (i = 0; i < 12; i++) {
        /* Full name. */
        len = strlen(mon[i]);
        if (strncmp(mon[i], bp, len) == 0)
          break;
 
        /* Abbreviated name. */
        len = strlen(abmon[i]);
        if (strncmp(abmon[i], bp, len) == 0)
          break;
      }
 
      /* Nothing matched. */
      if (i == 12)
        return (0);
 
      tm->tm_mon = i;
      bp += len;
      break;
 
    case 'C': /* The century number. */
      LEGAL_ALT(ALT_E);
      if (!(conv_num(&bp, &i, 0, 99)))
        return (0);
 
      if (split_year) {
        tm->tm_year = (tm->tm_year % 100) + (i * 100);
      } else {
        tm->tm_year = i * 100;
        split_year = 1;
      }
      break;
 
    case 'd': /* The day of month. */
    case 'e':
      LEGAL_ALT(ALT_O);
      if (!(conv_num(&bp, &tm->tm_mday, 1, 31)))
        return (0);
      break;
 
    case 'k': /* The hour (24-hour clock representation). */
      LEGAL_ALT(0);
      /* FALLTHROUGH */
    case 'H':
      LEGAL_ALT(ALT_O);
      if (!(conv_num(&bp, &tm->tm_hour, 0, 23)))
        return (0);
      break;
 
    case 'l': /* The hour (12-hour clock representation). */
      LEGAL_ALT(0);
      /* FALLTHROUGH */
    case 'I':
      LEGAL_ALT(ALT_O);
      if (!(conv_num(&bp, &tm->tm_hour, 1, 12)))
        return (0);
      if (tm->tm_hour == 12)
        tm->tm_hour = 0;
      break;
 
    case 'j': /* The day of year. */
      LEGAL_ALT(0);
      if (!(conv_num(&bp, &i, 1, 366)))
        return (0);
      tm->tm_yday = i - 1;
      break;
 
    case 'M': /* The minute. */
      LEGAL_ALT(ALT_O);
      if (!(conv_num(&bp, &tm->tm_min, 0, 59)))
        return (0);
      break;
 
    case 'm': /* The month. */
      LEGAL_ALT(ALT_O);
      if (!(conv_num(&bp, &i, 1, 12)))
        return (0);
      tm->tm_mon = i - 1;
      break;
 
    case 'p': /* The locale's equivalent of AM/PM. */
      LEGAL_ALT(0);
      /* AM? */
      if (strcmp(am_pm[0], bp) == 0) {
        if (tm->tm_hour > 11)
          return (0);
 
        bp += strlen(am_pm[0]);
        break;
      }
      /* PM? */
      else if (strcmp(am_pm[1], bp) == 0) {
        if (tm->tm_hour > 11)
          return (0);
 
        tm->tm_hour += 12;
        bp += strlen(am_pm[1]);
        break;
      }
 
      /* Nothing matched. */
      return (0);
 
    case 'S': /* The seconds. */
      LEGAL_ALT(ALT_O);
      if (!(conv_num(&bp, &tm->tm_sec, 0, 61)))
        return (0);
      break;
 
    case 'U': /* The week of year, beginning on sunday. */
    case 'W': /* The week of year, beginning on monday. */
      LEGAL_ALT(ALT_O);
      /*
       * XXX This is bogus, as we can not assume any valid
       * information present in the tm structure at this
       * point to calculate a real value, so just check the
       * range for now.
       */
       if (!(conv_num(&bp, &i, 0, 53)))
        return (0);
       break;
 
    case 'w': /* The day of week, beginning on sunday. */
      LEGAL_ALT(ALT_O);
      if (!(conv_num(&bp, &tm->tm_wday, 0, 6)))
        return (0);
      break;
 
    case 'Y': /* The year. */
      LEGAL_ALT(ALT_E);
      if (!(conv_num(&bp, &i, 0, 9999)))
        return (0);
 
      tm->tm_year = i - TM_YEAR_BASE;
      break;
 
    case 'y': /* The year within 100 years of the epoch. */
      LEGAL_ALT(ALT_E | ALT_O);
      if (!(conv_num(&bp, &i, 0, 99)))
        return (0);
 
      if (split_year) {
        tm->tm_year = ((tm->tm_year / 100) * 100) + i;
        break;
      }
      split_year = 1;
      if (i <= 68)
        tm->tm_year = i + 2000 - TM_YEAR_BASE;
      else
        tm->tm_year = i + 1900 - TM_YEAR_BASE;
      break;
 
    /*
     * Miscellaneous conversions.
     */
    case 'n': /* Any kind of white-space. */
    case 't':
      LEGAL_ALT(0);
      while (isspace(*bp))
        bp++;
      break;
 
 
    default:  /* Unknown/unsupported conversion. */
      return (0);
    }
  }
  /* LINTED functional specification */
  return ((char *)bp);
}
 
 
static int conv_num(const char **buf, int *dest, int llim, int ulim)
{
  int result = 0;
 
  /* The limit also determines the number of valid digits. */
  int rulim = ulim;
 
  if (**buf < '0' || **buf > '9')
    return (0);
 
  do
  {
    result *= 10;
    result += *(*buf)++ - '0';
    rulim /= 10;
  } while ((result * 10 <= ulim) && rulim && **buf >= '0' && **buf <= '9');
 
  if (result < llim || result > ulim)
    return (0);
 
  *dest = result;
  return (1);
}
 
 
#define ARDRONE_DATE_MAXSIZE        32
#define ARDRONE_FILE_DATE_FORMAT    "%Y%m%d_%H%M%S"
#define ARDRONE_DEFAULT_DATE        "19700101_000000"
 
void yunshouhu_time2date(time_t time, const char *format, char *date)
{
  if(date)
    {
#if defined(_MSC_VER) && (_MSC_VER>=1300)
	  struct tm tm;
	  localtime_s(&tm, &time);
	  strcpy_s(date, 32, ARDRONE_DEFAULT_DATE);
      strftime(date, ARDRONE_DATE_MAXSIZE, format ? format : ARDRONE_FILE_DATE_FORMAT, &tm);
#else
      struct tm *tm = localtime(&time);
      strcpy(date, ARDRONE_DEFAULT_DATE);
      if(tm != NULL)
        strftime(date, ARDRONE_DATE_MAXSIZE, format ? format : ARDRONE_FILE_DATE_FORMAT, tm);
#endif
    }
}
 
void yunshouhu_date2time(char *date, const char *format, time_t *time)
{
  struct tm tm;
 
  if(date != NULL)
    {
      *time = 0;
      if(strptime(date, (NULL != format) ? format : ARDRONE_FILE_DATE_FORMAT, &tm) != NULL)
        *time = mktime(&tm);
    }
}
 
char* format_date_time(time_t datetime,char *format)
{
  static char str_time[128]={0};
#if defined(_MSC_VER) && (_MSC_VER>=1300)
  struct tm temp_time;
  localtime_s(&temp_time, &datetime);
  strftime(str_time, sizeof(str_time), format, &temp_time);
#else
  struct tm *temp_time = localtime(&datetime);
  strftime(str_time, sizeof(str_time), format, temp_time);
#endif
  return str_time;
}

bool	ConvertWC2MB(const wchar_t *wstr, char *buf)
{
	if(!wstr || !buf)return false;

	int len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
	if(len<=0)return false;
	
	int nbyte = WideCharToMultiByte(CP_ACP, 0, wstr, -1, buf, len, NULL, NULL);
	if(nbyte<=0)return false;

	return true;
}

bool	ConvertMB2WC(const char *str, wchar_t *buf)
{
	if(!str || !buf)return false;

	int len = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
	if(len<=0)return false;
	
	int nbyte = MultiByteToWideChar(CP_ACP, 0, str, -1, buf, len);
	if(nbyte<=0)return false;

	return true;
}

TCHAR*	_tcsptime(const TCHAR *buf, const TCHAR *fmt, struct tm *tm)
{
#if defined(_UNICODE) || defined(UNICODE)
	//when widechar, convert to multibyte
	char *buf1, *fmt1;
	int len;
	len = (int)wcslen(buf);
	if(len<=0)
	{
		return 0;
	}
	buf1 = new char[len+1];
	memset(buf1, 0, (len+1)*sizeof(char));
	if(!ConvertWC2MB(buf, buf1))
	{
		delete []buf1;
		return 0;
	}

	if(!fmt)
	{
		len=(int)strlen(ARDRONE_FILE_DATE_FORMAT);
	}
	else
	{
		len=(int)wcslen(fmt);
	}
	fmt1 = new char[len+1];
	memset(fmt1, 0, (len+1)*sizeof(char));
	if(!fmt)
	{
		memcpy(fmt1, ARDRONE_FILE_DATE_FORMAT, len);
	}
	else
	{
		if(!ConvertWC2MB(fmt, fmt1))
		{
			delete []buf1;
			delete []fmt1;
			return 0;
		}
	}
	char *ret = strptime(buf1, fmt1, tm);

	if(!ret)
	{
		delete []buf1;
		delete []fmt1;
		return 0;
	}

	int offset = int(ret - buf1);
	delete []buf1;
	delete []fmt1;
	return (wchar_t*)(buf + offset);
#else
	return strptime(buf, fmt, tm);
#endif
}

/*
头文件?time.h

1、struct tm变量转换为time_t变量

     time_t time = mktime(struct tm *tm_p);

2、time_t变量转换为struct tm变量

     struct tm *tm_p = localtime(const time_t *timer);

     struct tm *tm_p = gmtime(const time_t *timer);

      区别：localtime返回当前时区的日期时间，而gmtime返回的未经时区转换。

3、ctime将time_t类型变量转换为对应的日期时间字符串

     char *ctime(const time_t *seconds );
*/

/*
int main(int argc, char const *argv[])
{
	time_t nowTime = time(NULL);
	char line[64]={0};
	char* format="%Y-%m-%d %H:%M:%S";
	yunshouhu_time2date(nowTime,format,line);
	printf("%s\n", line);
	
	time_t secondTime;
	strcpy(line,"2018-06-29 23:20:30");
	yunshouhu_date2time(line,format,&secondTime);
	
	printf("ctime is %s\n",ctime(&secondTime));//得到日历时间
	
	yunshouhu_time2date(nowTime,NULL,line);
	printf("%s\n", line);
	
	char* data=format_date_time(time(NULL),"%Y-%m-%d %H:%M:%S");
	printf("%s\n", data);
	
	char *format2="%Y year %m month %d date %H hour %M minute %S seconds ";//还不支持中文 要使用unicode编码的中文
	data=format_date_time(time(NULL),format2);
	printf("%s\n", data);
	
	format2="%Y\u5e74%m\u6708%d\u65e5 %H\u65f6%M\u5206%S\u79d2";//还不支持中文 要使用unicode编码的中文
	data=format_date_time(time(NULL),format2);
	printf("%s\n", data);
	
	
	return 0;
}
*/

