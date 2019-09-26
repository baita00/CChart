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

#ifndef __MYSTRING_32167_H_122333444455555__
#define __MYSTRING_32167_H_122333444455555__

#pragma warning(disable:4786)

#include "CChartNameSpace.h"

#include <tchar.h>

#include <cctype>
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

Declare_Namespace_CChart

typedef basic_string<TCHAR, char_traits<TCHAR>, allocator<TCHAR> > tstring;
typedef basic_istringstream<TCHAR> tistringstream;
typedef basic_ostringstream<TCHAR> tostringstream;
typedef basic_ifstream<TCHAR, char_traits<TCHAR> > tifstream;
typedef basic_ofstream<TCHAR, char_traits<TCHAR> > tofstream;
typedef basic_ios<TCHAR, char_traits<TCHAR> > tios;

//string functions
void string_tolower ( tstring &str ); 
void string_toupper ( tstring &str );
tstring& replace_all(tstring &str,const tstring &old_value,const tstring &new_value);
tstring& replace_all_distinct(tstring &str,const tstring &old_value,const tstring &new_value);
tstring::size_type find_first_nospace(tstring &str);
tstring::size_type rfind_first_nospace(tstring &str);
tstring::size_type find_first_matchstring(tstring &str, tstring tog);
void trimleft(tstring &str);
void trimright(tstring &str);
void trim(tstring &str, const tstring drop = _TEXT(" "));
void trimleft(tstring &str, const tstring drop);
void trimright(tstring &str, const tstring drop);
void trimspace(tstring &str);
tstring readline(const tstring &buffer,tstring::size_type &curpos, const tstring symbol=_TEXT("\n"));
int	get_line_count(const tstring &buff, const tstring symbol=_TEXT("\n"));

bool mywcs2mbs(const wstring wstr, string &buf);
bool mymbs2wcs(const string str, wstring &buf);
string mywcs2mbs(const wstring wstr);
wstring mymbs2wcs(const string str);

long get_file_size( const TCHAR *filename );
long get_file_size( FILE *fp);

Declare_Namespace_End

#endif