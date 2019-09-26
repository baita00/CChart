/*============================================================================*/
/*                                                                            */
/*                            C O P Y R I G H T                               */
/*                                                                            */
/*                          (C) Copyright 2019 by                             */
/*                              Yang Guojun                                   */
/*                           All Rights Reserved                              */
/*                                                                            */
/*      The author assumes no responsibility for the use or reliability of    */
/*      his software.                                                         */
/*                                                                            */
/*============================================================================*/
////////////////////////////////////////////////////////////////////////////////
//                                  版权申明                                  //
//                         版权所有(C)2006-2019，杨国君                       //
//                                保留全部权利                                //
////////////////////////////////////////////////////////////////////////////////
/* ############################################################################################################################## */

//#include "stdafx.h"
#include "MyString.h"
#include "sys/stat.h"
#include <windows.h>
////////////////////////////////////////////////////////////////////////////////////////////////
//
//定义一些操作字符串的函数

Declare_Namespace_CChart

void string_tolower ( tstring & str ) 
{
	transform(str.begin(),str.end(),str.begin(),tolower);
}

void string_toupper ( tstring & str ) 
{
	transform(str.begin(),str.end(),str.begin(),toupper);
}

tstring&   replace_all(tstring&   str,const   tstring&   old_value,const   tstring&   new_value)   
{   
    while(true)
	{
		tstring::size_type   pos(0);   
        if(   (pos=str.find(old_value))!=tstring::npos   )   
            str.replace(pos,old_value.length(),new_value);   
        else   break;   
    }   
    return   str;   
}

tstring&   replace_all_distinct(tstring&   str,const   tstring&   old_value,const   tstring&   new_value)   
{   
    for(tstring::size_type   pos(0);   pos!=tstring::npos;   pos+=new_value.length())
	{
        if(   (pos=str.find(old_value,pos))!=tstring::npos   )   
            str.replace(pos,old_value.length(),new_value);   
        else   break;   
    }   
    return   str;   
}

tstring::size_type find_first_nospace(tstring &str)
{
	tstring::size_type i=0;
	do
	{
		if(i==str.size()-1)
		{
			if(isspace(str[i]))
				return tstring::npos;
			else
				return str.size()-1;
		}
		if(isspace(str[i]))
		{
			i++;
			continue;
		}
		else
		{
			return i;
		}
	}while(true);
}

tstring::size_type rfind_first_nospace(tstring &str)
{
	tstring::size_type i=str.size()-1;
	do
	{
		if(i==0)
		{
			if(isspace(str[i]))
				return tstring::npos;
			else
				return 0;
		}
		if(isspace(str[i]))
		{
			i--;
			continue;
		}
		else
		{
			return i;
		}
	}while(true);
}

tstring::size_type	find_first_matchstring(tstring &str, tstring tog)
{
	int len = (int)str.length();
	int toglen = (int)tog.length();
	if(toglen == 0)return tstring::npos;
	if(len < toglen)return tstring::npos;

	tstring::size_type pos;
	tstring substr;
	
	for(pos = 0; pos < tstring::size_type(len-toglen); pos++)
	{
		substr = str.substr(pos, toglen);
		if(substr == tog)return pos;
	}
	return tstring::npos;
}

void trimleft(tstring &str)
{
	tstring::size_type idx=find_first_nospace(str);
	if(idx!=tstring::npos && idx<str.size() )str.erase(0,idx);
}

void trimright(tstring &str)
{
	tstring::size_type idx=rfind_first_nospace(str);
	if(idx!=tstring::npos && idx<str.size() )str.erase(idx+1);
}

void trim(tstring& str, const tstring drop)
{
	// trim right
	str.erase(str.find_last_not_of(drop)+1);
	// trim left
	str.erase(0,str.find_first_not_of(drop));
} 

void trimleft(tstring& str, const tstring drop)
{
	// trim left
	str.erase(0,str.find_first_not_of(drop));
} 

void trimright(tstring& str, const tstring drop)
{
	// trim right
	str.erase(str.find_last_not_of(drop)+1);
}

void	trimspace(tstring &str)
{
	tstring::size_type pos1, pos2;
	pos1 = find_first_nospace(str);
	pos2 = rfind_first_nospace(str);

	if(pos1 != tstring::npos && pos2 != tstring::npos)
		str = str.substr(pos1, pos2+1);
}

//对字符串操作，模拟读文件的一行，默认以\n为换行的符号
tstring readline(const tstring& buffer,tstring::size_type &curpos, const tstring symbol)
{
	if(curpos == tstring::npos)
	{
		return _TEXT("");
	}

	tstring ret;
	tstring::size_type pos;
	pos=buffer.find_first_of(symbol,curpos);
	if(pos == tstring::npos)
	{
		curpos=tstring::npos;
		return _TEXT("");
	}
	else
	{
		ret=buffer.substr(curpos,pos-curpos);
		curpos=pos+1;
		return ret;
	}
}

int	get_line_count(const tstring &buff, const tstring symbol/* =_TEXT */)
{
	tstring::size_type curpos;
	curpos = 0;

	int count = 0;
	tstring line;
	do 
	{
		line = readline(buff, curpos, symbol);
		trimspace(line);
		if(line.length()>0)count++;
	}while (curpos != tstring::npos);
	
	return count;
}

bool mywcs2mbs(const wstring wstr, string &buf)
{
	int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
	if(len <=0) return false;

	char *str = new char[len+1];
	int nbyte = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, str, len, NULL, NULL);
	if(nbyte <= 0)
	{
		delete []str;
		return false;
	}
	buf = str;
	delete []str;
	return true;
}

bool mymbs2wcs(const string str, wstring &buf)
{
	int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
	if(len <=0) return false;
	
	wchar_t *wstr = new wchar_t[len+1];
	int nbyte = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, wstr, len);
	if(nbyte <= 0)
	{
		delete []wstr;
		return false;
	}
	buf = wstr;
	delete []wstr;
	return true;
}

string mywcs2mbs(const wstring wstr)
{
	static string str;
	if(!mywcs2mbs(wstr, str))
		str="";
	return str;
}

wstring mymbs2wcs(const string str)
{
	static wstring wstr;
	if(!mymbs2wcs(str, wstr))
		wstr=L"";
	return wstr;
}

long get_file_size( const TCHAR* filename )
{
	struct _stat f_stat; 
	if( _tstat( filename, &f_stat ) == -1 )
	{
		return -1; 
	}
	return (long)f_stat.st_size; 
}

long get_file_size( FILE* fp)
{
	long curpos,filesize;
	curpos=ftell(fp);
	fseek(fp,0L,SEEK_END);
    filesize=ftell(fp); 
	fseek(fp,curpos,SEEK_SET);
	return filesize;
}

Declare_Namespace_End