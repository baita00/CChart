#pragma warning(disable: 4800)
#include "variant.h"
#include <memory.h>

CMyVariant::CMyVariant()
{
	type = MYVT_NONE;
	bRelease = false;;
}

CMyVariant::CMyVariant(bool e)
{
	type = MYVT_BOOL;
	vt.vt_bool = e;
}

CMyVariant::CMyVariant(char e)
{
	type = MYVT_CHAR;
	vt.vt_char = e;
}

CMyVariant::CMyVariant(unsigned char e)
{
	type = MYVT_UCHAR;
	vt.vt_uchar = e;
}

CMyVariant::CMyVariant(short e)
{
	type = MYVT_SHORT;
	vt.vt_short = e;
}

CMyVariant::CMyVariant(unsigned short e)
{
	type = MYVT_USHORT;
	vt.vt_ushort = e;
}

CMyVariant::CMyVariant(int e)
{
	type = MYVT_INT;
	vt.vt_int = e;
}

CMyVariant::CMyVariant(unsigned int e)
{
	type = MYVT_UINT;
	vt.vt_uint = e;
}

CMyVariant::CMyVariant(long e)
{
	type = MYVT_LONG;
	vt.vt_long = e;
}

CMyVariant::CMyVariant(unsigned long e)
{
	type = MYVT_ULONG;
	vt.vt_ulong = e;
}

CMyVariant::CMyVariant(float e)
{
	type = MYVT_FLOAT;
	vt.vt_float = e;
}

CMyVariant::CMyVariant(double e)
{
	type = MYVT_DOUBLE;
	vt.vt_double = e;
}

CMyVariant::CMyVariant(TCHAR* e)
{
	type = MYVT_PTCHAR;
	vt.vt_ptchar = e;
}

CMyVariant::CMyVariant(void* e)
{
	type = MYVT_LPARAM;
	vt.vt_plparam = e;
}










CMyVariant::~CMyVariant()
{
	Clear();
}

void CMyVariant::Clear()
{
	if (type == MYVT_NONE)
		return ;
	else if (type == MYVT_BOOL ||
		type == MYVT_CHAR ||
		type == MYVT_UCHAR ||
		type == MYVT_SHORT ||
		type == MYVT_USHORT ||
		type == MYVT_INT ||
		type == MYVT_UINT ||
		type == MYVT_LONG ||
		type == MYVT_ULONG ||
		type == MYVT_FLOAT ||
		type == MYVT_DOUBLE)
	{
		type = MYVT_NONE;
		return ;
	}
	else
	{
		if (bRelease)
		{
			if (type == MYVT_PTCHAR)
			{
				free(vt.vt_ptchar);
				vt.vt_ptchar = NULL;
			}
			else if (type == MYVT_LPARAM)
			{
				delete[] (unsigned char *) vt.vt_plparam;
				vt.vt_plparam = NULL;
			}
		}
		memset(&vt, 0, sizeof(variant));
		type = MYVT_NONE;
	}
}

bool CMyVariant::IsValid()
{
	return type != MYVT_NONE;
}

VT_TYPE CMyVariant::GetType()
{
	return type;
}








CMyVariant& CMyVariant::operator =(const CMyVariant& e)
{
	Clear();

	/*	if(e.type == MYVT_PTCHAR && e.bRelease) {
			bRelease = true;
			type = e.type;
			vt.vt_ptchar = _tcsdup(e.vt.vt_ptchar);
		}
		else {
			bRelease = e.bRelease;
			type = e.type;
			vt = e.vt;
		}*/
	type = e.type;
	vt = e.vt;
	bRelease = false;
	return *this;
}

bool CMyVariant::operator =(const bool& e)
{
	Clear();
	type = MYVT_BOOL;
	vt.vt_bool = e;
	return vt.vt_bool;
}

char CMyVariant::operator =(const char& e)
{
	Clear();
	type = MYVT_CHAR;
	vt.vt_char = e;
	return vt.vt_char;
}

unsigned char CMyVariant::operator =(const unsigned char& e)
{
	Clear();
	type = MYVT_UCHAR;
	vt.vt_uchar = e;
	return vt.vt_uchar;
}

short CMyVariant::operator =(const short& e)
{
	Clear();
	type = MYVT_SHORT;
	vt.vt_short = e;
	return vt.vt_short;
}

unsigned short CMyVariant::operator =(const unsigned short& e)
{
	Clear();
	type = MYVT_USHORT;
	vt.vt_ushort = e;
	return vt.vt_ushort;
}

int CMyVariant::operator =(const int& e)
{
	Clear();
	type = MYVT_INT;
	vt.vt_int = e;
	return vt.vt_int;
}

unsigned int CMyVariant::operator =(const unsigned int& e)
{
	Clear();
	type = MYVT_UINT;
	vt.vt_uint = e;
	return vt.vt_uint;
}

long CMyVariant::operator =(const long& e)
{
	Clear();
	type = MYVT_LONG;
	vt.vt_long = e;
	return vt.vt_long;
}

unsigned long CMyVariant::operator =(const unsigned long& e)
{
	Clear();
	type = MYVT_ULONG;
	vt.vt_ulong = e;
	return vt.vt_ulong;
}

float CMyVariant::operator =(const float& e)
{
	Clear();
	type = MYVT_FLOAT;
	vt.vt_float = e;
	return vt.vt_float;
}

double CMyVariant::operator =(const double& e)
{
	Clear();
	type = MYVT_DOUBLE;
	vt.vt_double = e;
	return vt.vt_double;
}

TCHAR* CMyVariant::operator =(TCHAR* e)
{
	Clear();
	type = MYVT_PTCHAR;
	vt.vt_ptchar = e;
	return vt.vt_ptchar;
}

void* CMyVariant::operator =(void* e)
{
	Clear();
	type = MYVT_LPARAM;
	vt.vt_plparam = e;
	return vt.vt_plparam;
}










CMyVariant& CMyVariant::operator <<(const CMyVariant& e)
{
	return operator = (e);
}

bool CMyVariant::operator <<(const bool& e)
{
	return operator = (e);
}

char CMyVariant::operator <<(const char& e)
{
	return operator = (e);
}

unsigned char CMyVariant::operator <<(const unsigned char& e)
{
	return operator = (e);
}

short CMyVariant::operator <<(const short& e)
{
	return operator = (e);
}

unsigned short CMyVariant::operator <<(const unsigned short& e)
{
	return operator = (e);
}

int CMyVariant::operator <<(const int& e)
{
	return operator = (e);
}

unsigned int CMyVariant::operator <<(const unsigned int& e)
{
	return operator = (e);
}

long CMyVariant::operator <<(const long& e)
{
	return operator = (e);
}

unsigned long CMyVariant::operator <<(const unsigned long& e)
{
	return operator = (e);
}

float CMyVariant::operator <<(const float& e)
{
	return operator = (e);
}

double CMyVariant::operator <<(const double& e)
{
	return operator = (e);
}

TCHAR* CMyVariant::operator <<(TCHAR* e)
{
	Clear();
	int n = (int)_tcslen(e);
	type = MYVT_PTCHAR;
	vt.vt_ptchar = _tcsdup(e);
	bRelease = true;
	return vt.vt_ptchar;
}

void* CMyVariant::operator <<(void* e)
{
	return operator = (e);
}



























CMyVariant::operator bool()
{
	if (type == MYVT_BOOL)
		return vt.vt_bool;
	else if (type == MYVT_CHAR)
		return (bool) vt.vt_char;
	else if (type == MYVT_UCHAR)
		return (bool) vt.vt_uchar;
	else if (type == MYVT_SHORT)
		return (bool) vt.vt_short;
	else if (type == MYVT_USHORT)
		return (bool) vt.vt_ushort;
	else if (type == MYVT_INT)
		return (bool) vt.vt_int;
	else if (type == MYVT_UINT)
		return (bool) vt.vt_uint;
	else if (type == MYVT_LONG)
		return (bool) vt.vt_long;
	else if (type == MYVT_ULONG)
		return (bool) vt.vt_ulong;
	else if (type == MYVT_FLOAT)
		return (bool) vt.vt_float;
	else if (type == MYVT_DOUBLE)
		return (bool) vt.vt_double;
	else
		return 0;
}

CMyVariant::operator char()
{
	if (type == MYVT_BOOL)
		return (char) vt.vt_bool;
	else if (type == MYVT_CHAR)
		return vt.vt_char;
	else if (type == MYVT_UCHAR)
		return (char) vt.vt_uchar;
	else if (type == MYVT_SHORT)
		return (char) vt.vt_short;
	else if (type == MYVT_USHORT)
		return (char) vt.vt_ushort;
	else if (type == MYVT_INT)
		return (char) vt.vt_int;
	else if (type == MYVT_UINT)
		return (char) vt.vt_uint;
	else if (type == MYVT_LONG)
		return (char) vt.vt_long;
	else if (type == MYVT_ULONG)
		return (char) vt.vt_ulong;
	else if (type == MYVT_FLOAT)
		return (char) vt.vt_float;
	else if (type == MYVT_DOUBLE)
		return (char) vt.vt_double;
	else
		return 0;
}

CMyVariant::operator unsigned char()
{
	if (type == MYVT_BOOL)
		return (unsigned char) vt.vt_bool;
	else if (type == MYVT_CHAR)
		return (unsigned char) vt.vt_char;
	else if (type == MYVT_UCHAR)
		return vt.vt_uchar;
	else if (type == MYVT_SHORT)
		return (unsigned char) vt.vt_short;
	else if (type == MYVT_USHORT)
		return (unsigned char) vt.vt_ushort;
	else if (type == MYVT_INT)
		return (unsigned char) vt.vt_int;
	else if (type == MYVT_UINT)
		return (unsigned char) vt.vt_uint;
	else if (type == MYVT_LONG)
		return (unsigned char) vt.vt_long;
	else if (type == MYVT_ULONG)
		return (unsigned char) vt.vt_ulong;
	else if (type == MYVT_FLOAT)
		return (unsigned char) vt.vt_float;
	else if (type == MYVT_DOUBLE)
		return (unsigned char) vt.vt_double;
	else
		return 0;
}

CMyVariant::operator short()
{
	if (type == MYVT_BOOL)
		return (short) vt.vt_bool;
	else if (type == MYVT_CHAR)
		return (short) vt.vt_char;
	else if (type == MYVT_UCHAR)
		return (short) vt.vt_uchar;
	else if (type == MYVT_SHORT)
		return vt.vt_short;
	else if (type == MYVT_USHORT)
		return (short) vt.vt_ushort;
	else if (type == MYVT_INT)
		return (short) vt.vt_int;
	else if (type == MYVT_UINT)
		return (short) vt.vt_uint;
	else if (type == MYVT_LONG)
		return (short) vt.vt_long;
	else if (type == MYVT_ULONG)
		return (short) vt.vt_ulong;
	else if (type == MYVT_FLOAT)
		return (short) vt.vt_float;
	else if (type == MYVT_DOUBLE)
		return (short) vt.vt_double;
	else
		return 0;
}

CMyVariant::operator unsigned short()
{
	if (type == MYVT_BOOL)
		return (unsigned short) vt.vt_bool;
	else if (type == MYVT_CHAR)
		return (unsigned short) vt.vt_char;
	else if (type == MYVT_UCHAR)
		return (unsigned short) vt.vt_uchar;
	else if (type == MYVT_SHORT)
		return (unsigned short) vt.vt_short;
	else if (type == MYVT_USHORT)
		return vt.vt_ushort;
	else if (type == MYVT_INT)
		return (unsigned short) vt.vt_int;
	else if (type == MYVT_UINT)
		return (unsigned short) vt.vt_uint;
	else if (type == MYVT_LONG)
		return (unsigned short) vt.vt_long;
	else if (type == MYVT_ULONG)
		return (unsigned short) vt.vt_ulong;
	else if (type == MYVT_FLOAT)
		return (unsigned short) vt.vt_float;
	else if (type == MYVT_DOUBLE)
		return (unsigned short) vt.vt_double;
	else
		return 0;
}

CMyVariant::operator int()
{
	if (type == MYVT_BOOL)
		return (int) vt.vt_bool;
	else if (type == MYVT_CHAR)
		return (int) vt.vt_char;
	else if (type == MYVT_UCHAR)
		return (int) vt.vt_uchar;
	else if (type == MYVT_SHORT)
		return (int) vt.vt_short;
	else if (type == MYVT_USHORT)
		return (int) vt.vt_ushort;
	else if (type == MYVT_INT)
		return vt.vt_int;
	else if (type == MYVT_UINT)
		return (int) vt.vt_uint;
	else if (type == MYVT_LONG)
		return (int) vt.vt_long;
	else if (type == MYVT_ULONG)
		return (int) vt.vt_ulong;
	else if (type == MYVT_FLOAT)
		return (int) vt.vt_float;
	else if (type == MYVT_DOUBLE)
		return (int) vt.vt_double;
	else
		return 0;
}

CMyVariant::operator unsigned int()
{
	if (type == MYVT_BOOL)
		return (unsigned int) vt.vt_bool;
	else if (type == MYVT_CHAR)
		return (unsigned int) vt.vt_char;
	else if (type == MYVT_UCHAR)
		return (unsigned int) vt.vt_uchar;
	else if (type == MYVT_SHORT)
		return (unsigned int) vt.vt_short;
	else if (type == MYVT_USHORT)
		return (unsigned int) vt.vt_ushort;
	else if (type == MYVT_INT)
		return (unsigned int) vt.vt_int;
	else if (type == MYVT_UINT)
		return vt.vt_uint;
	else if (type == MYVT_LONG)
		return (unsigned int) vt.vt_long;
	else if (type == MYVT_ULONG)
		return (unsigned int) vt.vt_ulong;
	else if (type == MYVT_FLOAT)
		return (unsigned int) vt.vt_float;
	else if (type == MYVT_DOUBLE)
		return (unsigned int) vt.vt_double;
	else
		return 0;
}

CMyVariant::operator long()
{
	if (type == MYVT_BOOL)
		return (long) vt.vt_bool;
	else if (type == MYVT_CHAR)
		return (long) vt.vt_char;
	else if (type == MYVT_UCHAR)
		return (long) vt.vt_uchar;
	else if (type == MYVT_SHORT)
		return (long) vt.vt_short;
	else if (type == MYVT_USHORT)
		return (long) vt.vt_ushort;
	else if (type == MYVT_INT)
		return (long) vt.vt_int;
	else if (type == MYVT_UINT)
		return (long) vt.vt_uint;
	else if (type == MYVT_LONG)
		return vt.vt_long;
	else if (type == MYVT_ULONG)
		return (long) vt.vt_ulong;
	else if (type == MYVT_FLOAT)
		return (long) vt.vt_float;
	else if (type == MYVT_DOUBLE)
		return (long) vt.vt_double;
	else
		return 0;
}

CMyVariant::operator unsigned long()
{
	if (type == MYVT_BOOL)
		return (unsigned long) vt.vt_bool;
	else if (type == MYVT_CHAR)
		return (unsigned long) vt.vt_char;
	else if (type == MYVT_UCHAR)
		return (unsigned long) vt.vt_uchar;
	else if (type == MYVT_SHORT)
		return (unsigned long) vt.vt_short;
	else if (type == MYVT_USHORT)
		return (unsigned long) vt.vt_ushort;
	else if (type == MYVT_INT)
		return (unsigned long) vt.vt_int;
	else if (type == MYVT_UINT)
		return (unsigned long) vt.vt_uint;
	else if (type == MYVT_LONG)
		return (unsigned long) vt.vt_long;
	else if (type == MYVT_ULONG)
		return vt.vt_ulong;
	else if (type == MYVT_FLOAT)
		return (unsigned long) vt.vt_float;
	else if (type == MYVT_DOUBLE)
		return (unsigned long) vt.vt_double;
	else
		return 0;
}

CMyVariant::operator float()
{
	if (type == MYVT_BOOL)
		return (float) vt.vt_bool;
	else if (type == MYVT_CHAR)
		return (float) vt.vt_char;
	else if (type == MYVT_UCHAR)
		return (float) vt.vt_uchar;
	else if (type == MYVT_SHORT)
		return (float) vt.vt_short;
	else if (type == MYVT_USHORT)
		return (float) vt.vt_ushort;
	else if (type == MYVT_INT)
		return (float) vt.vt_int;
	else if (type == MYVT_UINT)
		return (float) vt.vt_uint;
	else if (type == MYVT_LONG)
		return (float) vt.vt_long;
	else if (type == MYVT_ULONG)
		return (float) vt.vt_ulong;
	else if (type == MYVT_FLOAT)
		return vt.vt_float;
	else if (type == MYVT_DOUBLE)
		return (float) vt.vt_double;
	else
		return 0;
}

CMyVariant::operator double()
{
	if (type == MYVT_BOOL)
		return (double) vt.vt_bool;
	else if (type == MYVT_CHAR)
		return (double) vt.vt_char;
	else if (type == MYVT_UCHAR)
		return (double) vt.vt_uchar;
	else if (type == MYVT_SHORT)
		return (double) vt.vt_short;
	else if (type == MYVT_USHORT)
		return (double) vt.vt_ushort;
	else if (type == MYVT_INT)
		return (double) vt.vt_int;
	else if (type == MYVT_UINT)
		return (double) vt.vt_uint;
	else if (type == MYVT_LONG)
		return (double) vt.vt_long;
	else if (type == MYVT_ULONG)
		return (double) vt.vt_ulong;
	else if (type == MYVT_FLOAT)
		return (double) vt.vt_float;
	else if (type == MYVT_DOUBLE)
		return vt.vt_double;
	else
		return 0;
}

CMyVariant::operator TCHAR*()
{
	if (type == MYVT_PTCHAR)
		return vt.vt_ptchar;
	return NULL;
}

CMyVariant::operator void*()
{
	if (type == MYVT_LPARAM)
		return vt.vt_plparam;
	return NULL;
}






char CMyVariant::operator >>(char& e)
{
	e = operator char();
	return e;
}

unsigned char CMyVariant::operator >>(unsigned char& e)
{
	e = operator unsigned char();
	return e;
}

short CMyVariant::operator >>(short& e)
{
	e = operator short();
	return e;
}

unsigned short CMyVariant::operator >>(unsigned short& e)
{
	e = operator unsigned short();
	return e;
}

int CMyVariant::operator >>(int& e)
{
	e = operator int();
	return e;
}

unsigned int CMyVariant::operator >>(unsigned int& e)
{
	e = operator unsigned int();
	return e;
}

long CMyVariant::operator >>(long& e)
{
	e = operator long();
	return e;
}

unsigned long CMyVariant::operator >>(unsigned long& e)
{
	e = operator unsigned long();
	return e;
}

float CMyVariant::operator >>(float& e)
{
	e = operator float();
	return e;
}

double CMyVariant::operator >>(double& e)
{
	e = operator double();
	return e;
}

TCHAR* CMyVariant::operator >>(TCHAR* e)
{
	if (type == MYVT_PTCHAR)
	{
		memcpy(e, vt.vt_ptchar, _tcslen(vt.vt_ptchar)*sizeof(TCHAR));
		//_tcscpy(e, vt.vt_ptchar);
		return vt.vt_ptchar;
	}
	return NULL;
}

// only can return MYVT_NONE, MYVT_ULONG, MYVT_LONG, MYVT_DOUBLE, MYVT_PTCHAR
VT_TYPE CMyVariant::GetType(TCHAR* psz)
{
	int n = (int)_tcslen(psz);
	if (n == 0)
		return MYVT_NONE;

	char num_first = '0';
	char num_last = '9';
	char dot = '.';
	char plus = '+';
	char sub = '-';

	if ((psz[0] < num_first || psz[0] > num_last) &&
		psz[0] != dot &&
		psz[0] != plus &&
		psz[0] != sub)
		return MYVT_PTCHAR;
	else
	{
		bool bDot = false;
		int nPos = 0;
		bool bUnsigned = true;
		if (psz[0] == plus || psz[0] == sub)
		{
			bUnsigned = false;
			nPos = 1;
		}

		if (psz[nPos] == dot)
		{
			++ nPos;
			dot = true;
		}

		for (int i = nPos; i < n; ++ i)
		{
			if (((psz[i] < num_first || psz[i] > num_last) && psz[i] != dot) ||
				(bDot && psz[i] == dot))
				return MYVT_PTCHAR;
			if (psz[i] == dot)
				bDot = true;
		}

		if (bDot)
		{
			return MYVT_DOUBLE;
		}
		else
		{
			if (bUnsigned)
			{
				return MYVT_ULONG;
			}
			else
			{
				return MYVT_LONG;
			}
		}
	}

	return MYVT_NONE;
}
