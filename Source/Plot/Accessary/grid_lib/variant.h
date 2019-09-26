#ifndef _CMYVARIANT_H
#define _CMYVARIANT_H

#include <tchar.h>
#include <stdio.h>
#include <assert.h>
#include <malloc.h>

typedef enum VT_TYPE
{
	MYVT_NONE, 
	MYVT_BOOL, 
	MYVT_CHAR, 
	MYVT_UCHAR, 
	MYVT_SHORT, 
	MYVT_USHORT, 
	MYVT_INT, 
	MYVT_UINT, 
	MYVT_LONG, 
	MYVT_ULONG, 
	MYVT_FLOAT, 
	MYVT_DOUBLE, 
	MYVT_PTCHAR, 
	MYVT_LPARAM, 
} VT_TYPE;

union variant
{
	bool			vt_bool;
	char			vt_char;
	unsigned char	vt_uchar;
	short			vt_short;
	unsigned short	vt_ushort;
	int				vt_int;
	unsigned int	vt_uint;
	long			vt_long;
	unsigned long	vt_ulong;
	float			vt_float;
	double			vt_double;
	TCHAR*			vt_ptchar;
	void*			vt_plparam;
};



class CMyVariant
{
public:
	CMyVariant();
	CMyVariant(bool e);
	CMyVariant(char e);
	CMyVariant(unsigned char e);
	CMyVariant(short e);
	CMyVariant(unsigned short e);
	CMyVariant(int e);
	CMyVariant(unsigned int e);
	CMyVariant(long e);
	CMyVariant(unsigned long e);
	CMyVariant(float e);
	CMyVariant(double e);
	CMyVariant(TCHAR* e);
	CMyVariant(void* e);
	virtual	~CMyVariant();

	void			Clear();
	bool			IsValid();
	VT_TYPE			GetType();




	CMyVariant&		operator = (const CMyVariant& e);
	bool			operator = (const bool& e);
	char			operator = (const char& e);
	unsigned char	operator = (const unsigned char& e);
	short			operator = (const short& e);
	unsigned short	operator = (const unsigned short& e);
	int				operator = (const int& e);
	unsigned int	operator = (const unsigned int& e);
	long			operator = (const long& e);
	unsigned long	operator = (const unsigned long& e);
	float			operator = (const float& e);
	double			operator = (const double& e);
	TCHAR*			operator = (TCHAR* e);
	void*			operator = (void* e);


	CMyVariant&		operator << (const CMyVariant& e);
	bool			operator << (const bool& e);
	char			operator << (const char& e);
	unsigned char	operator << (const unsigned char& e);
	short			operator << (const short& e);
	unsigned short	operator << (const unsigned short& e);
	int				operator << (const int& e);
	unsigned int	operator << (const unsigned int& e);
	long			operator << (const long& e);
	unsigned long	operator << (const unsigned long& e);
	float			operator << (const float& e);
	double			operator << (const double& e);
	TCHAR*			operator << (TCHAR* e);
	void*			operator << (void* e);


	operator bool();
	operator char();
	operator unsigned char();
	operator short();
	operator unsigned short();
	operator int();
	operator unsigned int();
	operator long();
	operator unsigned long();
	operator float();
	operator double();
	operator TCHAR*();
	operator void*();


	char			operator >> (char& e);
	unsigned char	operator >> (unsigned char& e);
	short			operator >> (short& e);
	unsigned short	operator >> (unsigned short& e);
	int				operator >> (int& e);
	unsigned int	operator >> (unsigned int& e);
	long			operator >> (long& e);
	unsigned long	operator >> (unsigned long& e);
	float			operator >> (float& e);
	double			operator >> (double& e);
	TCHAR*			operator >> (TCHAR* e);

public:
	union variant		vt;
	VT_TYPE				type;

//protected:
	bool				bRelease;


public:
	static VT_TYPE	GetType(TCHAR* psz);
};

#endif
