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

#include "MyGdiPlus.h"

#include <windows.h>
#include <math.h>
#include <time.h>

#include "PlotUtility.h"
#include "mytime.h"
#include "MyConst.h"
#include "../Accessary/Dib/dib.h"

// For function GradientFill
#pragma comment(lib, "msimg32")

using namespace NsCChart;

Declare_Namespace_CChart

// Rounds any number to any number. i.e. round 1.9
// to the nearest 0.4 yields 2.0
double			RoundToRange( double value, double range )
{
	double		half = range / 2;
	double		scale = value / range;
	double		mantissa;
	__int64		exponent;
	int			sign = value>0?1:-1;

	mantissa = scale - (__int64) scale;
	exponent = (__int64) scale;

	if( sign > 0 )
	{
		if( fabs( mantissa ) >= 0.5 )
			return range * (exponent + sign);
		else
			return range * exponent;
	}
	else
	{
		if( fabs( mantissa ) > 0.5 )
			return range * (exponent + sign);
		else
			return range * exponent;
	}
}

int				GetMagnitude( double value )
{
	if(value == 0.0)
		return 0;
	else
	{
		return (int)floor(log10(fabs(value)));
	}
}

double			RoundToInt( double value )
{
	if(value == 0)
		return 0.0;
	else if(value > 0)
		return floor(value + 0.5);
	else
		return ceil(value - 0.5);
}

double			Round( double value, int level )
{
	if(value == 0.0)return 0.0;
	int mag = GetMagnitude(value);
	double remain = value * pow(10.0, -mag + level);
	remain = RoundToInt(remain);
	return remain * pow(10.0, mag - level);
}

// Get the digit numbers of the value
int				GetRemainLength(double value)
{
	int mag = GetMagnitude(value);
	double remain = value * pow(10.0, -mag);
	int len = 0;
	while( fabs(remain-RoundToInt(remain)) >= 1e-9 )
	{
		len++;
		remain -= RoundToInt(remain);
		remain *= 10;

		if(len>=20)break;
	}
	return len;
}

double	MyRange(double *range)
{
	return range[1]-range[0];
}

bool	InRange(double val, double *range)
{
	return (val-range[0])*(val-range[1])<=0.0;
}

bool	InRange(double *data, double *xRange, double *yRange)
{
	return InRange(data[0], xRange) && InRange(data[1], yRange);
}

void	MoveRange(double *range, double dx)
{
	range[0] += dx;
	range[1] += dx;
}

bool	CompareRange(double *range1, double *range2)
{
	return range1[0]==range2[0] && range1[1]==range2[1];
}

void	ResetTicks(double *range, int &ticks)
{
	// addtional ticks modify
	double temp = log10( fabs( range[1] - range[0] ) );
	
	double mantissa = pow( 10.0, temp - (int)temp );
	int prefer = (int)Round(mantissa, 3);
	if(fabs(mantissa - prefer) <= 0.001)
	{
		if(prefer >= 3.0)
			ticks = prefer - 1;
		else if(prefer == 2.0)
			ticks = 3;
	}
	else
	{
		prefer = (int)Round(10.0*mantissa, 3);
		if(prefer==12)
			ticks = 5;
		else if(prefer==15)
			ticks = 4;
	}
}

// This algorithm comes from a paper by Dorothy Pugh (KarlG87710@aol.com)
// "A Robust Generalized Axis-Scaling Macro", presumably from the proceedings
// of the Southeast SAS Users Group Conference and was translated from
// SAS macro langauge to C by yours truly.

void	GetOptimalRange( double *inRange, double *outRange, bool closeRange,
						bool optimalExact, bool optimalZeroRepair, bool optimalSymetryRepair, bool optimalExtendL, bool optimalExtendU )
{
	int			nTicks, nMinor;

	GetOptimalRange( inRange, outRange, &nTicks, &nMinor, closeRange, optimalExact, optimalZeroRepair, optimalSymetryRepair, optimalExtendL, optimalExtendU );
}

void	GetOptimalRange( double *inRange, double *outRange,
								int *nTicks, int *nMinor, bool closeRange,
								bool optimalExact, bool optimalZeroRepair, bool optimalSymetryRepair, bool optimalExtendL, bool optimalExtendU )
{
	double		wasted = 0.8, margin = 0.01;

	if( closeRange == true ) { wasted = 0.9; margin = 0.00; }

	GetOptimalRangePrivate( inRange, outRange, nTicks, nMinor, margin, wasted, optimalExact, optimalZeroRepair, optimalSymetryRepair, optimalExtendL, optimalExtendU );
}

void	GetOptimalRangePrivate( double *inRange, double *outRange,
								int *nTicks, int *nMinor, double margin, double wasted,
								bool optimalExact, bool optimalZeroRepair, bool optimalSymetryRepair, bool optimalExtendL, bool optimalExtendU )
{
	const double	cutPoint1 = 1.0, cutPoint2 = 1.25, cutPoint3 = sqrt( 10. );
	const int		maxTick = 11, minTick = 3;

	double		mantissa, range, temp, interval, mn, mx;
	double		oldNew, tickTest, oldMax = inRange[1], oldMin = inRange[0];
	int			rexp, minorTick = 0, flag;
	long		test;

	if(fabs(inRange[1] - inRange[0])<1.0e-90)
	{
		if(fabs(inRange[0])<1.0e-90)
		{
			outRange[0] = -1.0;
			outRange[1] = 1.0;
			*nTicks = 1;
			*nMinor = 5;
			return;
		}
		else
		{
			temp = log10(fabs(inRange[0]));
			rexp = (int)ceil(temp);
			mantissa = inRange[0]/pow(10.0, (double)rexp);
			mn = floor(mantissa);
			mx = ceil(mantissa);
			if(mn == mx)mx += 1.0;
			outRange[0] = mn*pow(10.0, rexp);
			outRange[1] = mx*pow(10.0, rexp);
			*nTicks = 1;
			*nMinor = 2;
			return;
		}
	}

	temp = log10( fabs( inRange[1] - inRange[0] ) );

	mantissa = pow( 10.0, temp - (int)temp );
	rexp = (int) temp;

	// Which cut point regime do we belong in?
	if( mantissa >= cutPoint3 )
	{
		interval = pow( 10.0, rexp );
		minorTick = 9;
	}
	else if( mantissa >= cutPoint2 )
	{
		interval = 0.5 * pow( 10.0, rexp );
		minorTick = 4;
	}
	else
	{
		interval = pow( 10.0, rexp-1 );
		minorTick = 9;
	}
	mx = RoundToRange( inRange[1] + (0.5 + margin) * interval, interval );
	mn = RoundToRange( inRange[0] - (0.5 + margin) * interval, interval );

	// check tick ranges and axis spacing
	flag = 0;
	range = mx - mn;
	oldNew = (inRange[1]-inRange[0])/range;
	tickTest = RoundToRange( 1. + (mx-mn-interval)/(0.5*interval),1. );

	if( oldNew < wasted && tickTest <= maxTick && margin <= 0.01 &&
		oldMax < (mx - 0.5*interval) && oldMin > (mn + 0.5 * interval) )
	{
		mx = mx - 0.5 * interval;
		mn = mn + 0.5 * interval;
		interval = 0.5 * interval;
		flag = 1;
		if( minorTick == 9 ) minorTick = 4;
	}

	// Moderate number of tick marks
	test = (long)RoundToRange( (mx - mn) / interval, 1. );

	while( test >= maxTick && flag == 0 )
	{
		if( test % 2 != 0 ) mx = mx + interval;

		interval = 2 * interval;
		test = (long)RoundToRange((mx-mn)/interval,1.);
		if( minorTick == 4 ) minorTick = 9;
	}

	while( test <= minTick && flag == 0 )
	{
		if( test <= 3 ) interval = 0.5 * interval;

		test = (long)RoundToRange( (mx-mn) / interval, 1. );
		if( minorTick == 9 ) minorTick = 4;
	}

	// Done!
	outRange[0] = mn;
	outRange[1] = mx;

	// additional checking
	if(optimalZeroRepair)
	{
		if(inRange[0] >= 0.0 && outRange[0] < 0.0)
		{
			outRange[1] -= outRange[0];
			outRange[0] = 0.0;
		}
		else if(inRange[1] <= 0.0 && outRange[1] > 0.0)
		{
			outRange[0] -= outRange[1];
			outRange[1] = 0.0;
		}
	}
	
	if(optimalSymetryRepair)
	{
		if(fabs(inRange[0]+inRange[1])/max(fabs(inRange[0]), fabs(inRange[1]))<2.0e-2 && fabs(outRange[0]+outRange[1])>1.0e-90)
		{
			double val = max(fabs(outRange[0]), fabs(outRange[1]));
			outRange[0] = -val;
			outRange[1] = val;
		}
	}

	if(optimalExact)
	{
		if(inRange[0] - outRange[0] > 1e-90)
		{
			double dlt = (inRange[0] - outRange[0])/interval;
			if(fabs(dlt-(int)dlt)<1e-3)
				outRange[0] += ((int)dlt)*interval;
		}
		if(outRange[1] - inRange[1] > 1e-90)
		{
			double dlt = (outRange[1] - inRange[1])/interval;
			if(fabs(dlt-(int)dlt)<1e-3)
				outRange[1] -= ((int)dlt)*interval;
		}
	}
	
	if(optimalExtendL)
	{
		if( fabs(inRange[0]-outRange[0])/max(max(fabs(inRange[0]), fabs(outRange[0])), 1.0e-9)<2.0e-2 )
		{
			outRange[0] -= interval;
		}
	}
	if(optimalExtendU)
	{
		if(fabs(inRange[1]-outRange[1])/max(max(fabs(inRange[1]), fabs(outRange[1])), 1.0e-9)<2.0e-2)
		{
			outRange[1] += interval;
		}
	}

	// ticks
	*nTicks = (int)fabs(RoundToRange( (outRange[1]-outRange[0]) / interval, 1. ))-1;
	*nMinor = minorTick+1;

}

void	GetLogValue(double val, int &ev, double &rv)
{
	if(fabs(val)<=fMinVal)
	{
		ev = 0;
		rv = 0;
	}
	ev = (int)floor(log10(fabs(val)));
	rv = val*pow(10.0, -(double)ev);
}

void	 RegulateTicks(double dMin, double dMax, int &nTicks, int &nMinorTicks, double &tickMin, double &tickMax)
{
	int eMin, eMax;
	double rMin, rMax;

	if(dMin>dMax)
	{
		std::swap(dMin, dMax);
	}

	GetLogValue(dMin, eMin, rMin);
	GetLogValue(dMax, eMax, rMax);

	double r1, r2, gap;
	double rm;
	int er, de;
	double rr;

	if(fabs(dMax-dMin)<fMinVal)
	{
		nTicks = 2;
		tickMin = floor(rMax)*pow(10.0, eMax);
		tickMax = ceil(rMax)*pow(10.0, eMax);
		if(floor(rMax)==ceil(rMax))
		{
			tickMin -= pow(10.0, eMax);
			tickMax += pow(10.0, eMax);
		}
	}
	else if(fabs(dMin)<fMinVal)
	{
		tickMin = 0;
		if(rMax >= 2.0)
		{
			nTicks = (int)floor(rMax)+1;
			tickMax	= (nTicks-1)*pow(10.0, eMax);
		}
		else
		{
			nTicks = (int)floor(rMax*5.0)+1;
			tickMax = (nTicks-1)*pow(10.0, eMax)/5.0;
		}
	}
	else if(fabs(dMax)<fMinVal)
	{
		tickMax = 0;
		if(rMin <= -2.0)
		{
			nTicks = (int)floor(fabs(rMin))+1;
			tickMin	= -(nTicks-1)*pow(10.0, eMin);
		}
		else
		{
			nTicks = (int)floor(fabs(rMax)*5.0)+1;
			tickMin = -(nTicks-1)*pow(10.0, eMin)/5.0;
		}
	}
	else if(dMax> 0.0 && dMin>0.0)
	{
		if(eMin == eMax)
		{
			r1 = ceil(rMin);
			r2 = floor(rMax);
			if(r2-r1>=1)
			{
				nTicks = (int)(r2 - r1 + 1);
				tickMin = r1*pow(10.0, eMin);
				tickMax = r2*pow(10.0, eMin);
			}
			else
			{
				rm = dMax - dMin;
				GetLogValue(rm, er, rr);
				de = eMin - er;
				
				r1 = ceil(dMin / pow(10.0, eMin - de));
				r2 = floor(dMax / pow(10.0, eMin - de));
				nTicks = (int)(r2 - r1 + 1);
				tickMin = r1 * pow(10.0, eMin - de);
				tickMax = r2 * pow(10.0, eMin - de);

				if(nTicks == 1)
				{
					r1 = ceil(dMin *2/ pow(10.0, eMin - de));
					r2 = floor(dMax *2/ pow(10.0, eMin - de));
					nTicks = (int)(r2 - r1 + 1);
					tickMin = r1 * pow(10.0, eMin - de) /2;
					tickMax = r2 * pow(10.0, eMin - de) /2;
				}
				
			}
		}
		else
		{
			r2 = floor(rMax);
			if(r2>=3)
			{
				nTicks = (int)r2;
				tickMin = pow(10.0, eMax);
				tickMax = r2 * pow(10.0, eMax);
			}
			else if(r2==2)
			{
				nTicks = (int)r2;
				tickMin = pow(10.0, eMax);
				tickMax = r2 * pow(10.0, eMax);
				if(dMin<=tickMin/2.0)
				{
					nTicks = 4;
					tickMin /= 2.0;
				}
			}
			else
			{
				if(eMax == eMin+1)
				{
					r1 = ceil(rMin);
					r2 = floor(dMax / pow(10.0, eMax-1));
					if(r1<=9 || r2 >= 11)
					{
						nTicks = (int)(r2 - r1 + 1);
						if(r2 - r1 < 3)
						{
							r1 = ceil(dMin*2.0 / pow(10.0, eMin));
							r2 = floor(dMax*2.0 / pow(10.0, eMax-1));
							nTicks = (int)(r2 - r1 + 1);
							tickMin = r1 * pow(10.0, eMin)/2.0;
							tickMax = r2 * pow(10.0, eMax-1)/2.0;

							if(nTicks == 1)
							{
								r1 = ceil(dMin*4.0 / pow(10.0, eMin));
								r2 = floor(dMax*4.0 / pow(10.0, eMax-1));
								nTicks = (int)(r2 - r1 + 1);
								tickMin = r1 * pow(10.0, eMin)/4.0;
								tickMax = r2 * pow(10.0, eMax-1)/4.0;
							}
						}
						else if(r2 - r1 < 6)
						{
							tickMin = r1 * pow(10.0, eMin);
							tickMax = r2 * pow(10.0, eMax-1);
						}
						else if(r2 - r1 < 12)
						{
							r1 = ceil(dMin/2.0 / pow(10.0, eMin));
							r2 = floor(dMax/2.0 / pow(10.0, eMax-1));
							nTicks = (int)(r2 - r1 + 1);
							tickMin = r1 * pow(10.0, eMin)*2.0;
							tickMax = r2 * pow(10.0, eMax-1)*2.0;
						}
						else
						{
							r1 = ceil(dMin/4.0 / pow(10.0, eMin));
							r2 = floor(dMax/4.0 / pow(10.0, eMax-1));
							nTicks = (int)(r2 - r1 + 1);
							tickMin = r1 * pow(10.0, eMin)*4.0;
							tickMax = r2 * pow(10.0, eMax-1)*4.0;
						}
					}
					else
					{
						rm = dMax - dMin;
						gap = pow(10.0, eMin);
						
						if(rm >= gap)
						{
							r1 = ceil(dMin*2 / pow(10.0, eMin));
							r2 = floor(dMax*2 / pow(10.0, eMax-1));
							nTicks = (int)(r2 - r1 + 1);
							tickMin = r1 * pow(10.0, eMin)/2.0;
							tickMax = r2 * pow(10.0, eMax-1)/2.0;
						}
						else
						{
							GetLogValue(rm, er, rr);
							de = eMin - er;
							r1 = ceil(dMin / pow(10.0, eMin - de));
							r2 = floor(dMax / pow(10.0, eMin - de));
							nTicks = (int)(r2 - r1 + 1);
							tickMin = r1 * pow(10.0, eMin - de);
							tickMax = r2 * pow(10.0, eMin - de);
						}
					}
				}
				else
				{
					r2 = floor(dMax / pow(10.0, eMax-1));
					if(r2 >= 11)
					{
						r2 = floor(dMax/ 2.0 / pow(10.0, eMax-1));
						nTicks = (int)r2;
						tickMin = 2 * pow(10.0, eMax-1);
						tickMax = r2 * pow(10.0, eMax-1)*2.0;
					}
					else
					{
						r1 = 1; 
						nTicks = (int)(r2 - r1 + 1);
						tickMin = r1 * pow(10.0, eMax-1);
						tickMax = r2 * pow(10.0, eMax-1);
					}
					
				}
			}
		}
	}
	else if(dMax<0.0 && dMin<0.0)
	{
		RegulateTicks(-dMax, -dMin, nTicks, nMinorTicks, tickMin, tickMax);
		nTicks+=2;//后面又要减去2
		double tmp = tickMin;
		tickMin = -tickMax;
		tickMax = -tmp;
	}
	else
	{
		r1 = ceil(rMin);
		r2 = floor(rMax);
		if(eMin == eMax)
		{
			if(r2-r1 >= 1)
			{
				if(r2-r1 <= 6)
				{
					nTicks = (int)(r2 - r1 + 1);
					tickMin = r1 * pow(10.0, eMin);
					tickMax = r2 * pow(10.0, eMax);
				}
				else if(r2-r1 <= 12)
				{
					r1 = ceil(dMin / 2.0 / pow(10.0, eMin));
					r2 = floor(dMax / 2.0 / pow(10.0, eMax));
					nTicks = (int)(r2 - r1 + 1);
					tickMin = r1 * pow(10.0, eMin)*2.0;
					tickMax = r2 * pow(10.0, eMin)*2.0;
				}
				else
				{
					r1 = ceil(dMin / 4.0 / pow(10.0, eMin));
					r2 = floor(dMax / 4.0 / pow(10.0, eMax));
					nTicks = (int)(r2 - r1 + 1);
					tickMin = r1 * pow(10.0, eMin)*4.0;
					tickMax = r2 * pow(10.0, eMin)*4.0;
				}
			}
			else
			{
				rm = dMax - dMin;
				GetLogValue(rm, er, rr);
				de = eMin - er;
				r1 = ceil(dMin / pow(10.0, eMin - de));
				r2 = floor(dMax / pow(10.0, eMin - de));
				nTicks = (int)(r2 - r1 + 1);
				tickMin = r1 * pow(10.0, eMin - de);
				tickMax = r2 * pow(10.0, eMin - de);
			}
		}
		else if(eMin < eMax)
		{
			if(r2 >= 2)
			{
				r1 = ceil(dMin / pow(10.0, eMax));
				r2 = floor(dMax / pow(10.0, eMax));
				nTicks = (int)(r2 - r1 + 1);
				tickMin = r1 * pow(10.0, eMax);
				tickMax = r2 * pow(10.0, eMax);
			}
			else
			{
				r1 = ceil(dMin*2 / pow(10.0, eMax));
				r2 = floor(dMax*2 / pow(10.0, eMax));
				nTicks = (int)(r2 - r1 + 1);
				tickMin = r1 * pow(10.0, eMax)/2;
				tickMax = r2 * pow(10.0, eMax)/2;
			}
		}
		else
		{
			if(r2 <= -2)
			{
				r1 = ceil(dMin / pow(10.0, eMin));
				r2 = floor(dMax / pow(10.0, eMin));
				nTicks = (int)(r2 - r1 + 1);
				tickMin = r1 * pow(10.0, eMin);
				tickMax = r2 * pow(10.0, eMin);
			}
			else
			{
				r1 = ceil(dMin*2 / pow(10.0, eMin));
				r2 = floor(dMax*2 / pow(10.0, eMin));
				nTicks = (int)(r2 - r1 + 1);
				tickMin = r1 * pow(10.0, eMin)/2;
				tickMax = r2 * pow(10.0, eMin)/2;
			}
		}
	}

	// modification
	//gap = (dMax - dMin)/(nTicks - 1);
	gap = (tickMax - tickMin)/(nTicks - 1);
	while(tickMin > dMin + gap)
	{
		nTicks++;
		tickMin -= gap;
	}
	while(tickMax < dMax - gap)
	{
		nTicks++;
		tickMax += gap;
	}

	nTicks-=2;// CAxis need this;
	nMinorTicks = 5;
}

void	ComputeTicksWithGap(double dMin, double dMax, double gap, int &nTicks, int &nMinorTicks, double &tickMin, double &tickMax)
{
	if(gap<=0)return;

	while(gap>fabs(dMax-dMin)/2.0)
	{
		gap/=2.0;
	}
	tickMin = ceil(dMin/gap)*gap;
	tickMax = floor(dMax/gap)*gap;
	nTicks = int(floor(dMax/gap) - ceil(dMin/gap))-1;
	nMinorTicks = 5;
}

// This does a silly, but useful check for an optimal tick interval
// given a known range. It tries to find the interval set with
// the least remainder. The order is from my head, and what
// I think works best. There's no other good reason for the order.
double		GetTickOptimalTickInterval( double lower, double upper )
{
	// this is written by the original author, i modified it..
/*	double		range = fabs(upper - lower);
	double		error = fMAXVAL, minError = fMAXVAL, logi;

	const int			tries[] = { 7, 6, 8, 5, 9, 4, 10, 3, 2 };
	const int			divTries[] = {4, 5, 3, 8, 6, 7, 2, 9, 10 };

	int			minDiv = 1, i;

	if(fabs(range)<fMINVAL)return 0;

	for( i = 0; i < 9; i++ )
	{
		// Take log base i of range
		logi = log( range ) / log( (double)tries[i] );

		// See how its error compares to others
		error = fabs(logi - (long) logi);

		if( error < minError )
		{
			minError = error;
			minDiv = tries[i];
		}

		error = fabs(range / divTries[i] - RoundToRange(range / divTries[i], 1.));

		if( error < minError )
		{
			minError = error;
			minDiv = divTries[i];
		}
	}

	return range / (double)minDiv;

*/
	double		range = fabs(upper - lower);

	const int			tries[] = { 7, 6, 4, 8, 3, 5, 10, 9, 2 };

	int mag = GetMagnitude(range);
	double remain = range*pow(10.0, -mag+1);
	int rti = (int)RoundToInt(remain);
	if(remain - rti < 1.0e-2)range = rti*pow(10.0, mag-1); 

	int nlen=100, nthis, nindex;
	for(int i=0; i<sizeof(tries)/sizeof(tries[0]); i++)
	{
		nthis = GetRemainLength(range/tries[i]);
		if(nthis<nlen)
		{
			nlen = nthis;
			nindex = i;
		}
	}
	return range/(double)tries[nindex];
}

// Simply gets the number of tick marks, major and minor, given
// the exact specified range
void	GetOptimalRangeExact( double *inRange, double *outRange,
								int *nTicks, int *nMinor, int nSegsPrefer )
{
	double		temp, tempRange;

	if(nSegsPrefer<=0)temp = GetTickOptimalTickInterval( inRange[0], inRange[1] );
	else temp = (inRange[1]-inRange[0])/nSegsPrefer;

	if(fabs(temp)/fabs(inRange[1]-inRange[0])<1e-2)
	{
		*nTicks = 4;
		*nMinor = 4;
		outRange[0] = inRange[0];
		outRange[1] = inRange[1];
		return;
	}
	//the interval is not integer
	double ratio = temp/pow( 10.0, floor( log10( fabs( temp ) ) )-1 );
	if(fabs(ratio - RoundToInt(ratio))>1e-2)
	{
		*nTicks = 4;
		*nMinor = 4;
		outRange[0] = inRange[0];
		outRange[1] = inRange[1];
		return;
	}
	//
	*nTicks = (int)fabs(RoundToRange((inRange[1] - inRange[0]) / temp, 1.0 )) - 1;

	tempRange = temp;
	temp = GetTickOptimalTickInterval( inRange[0], inRange[0] + temp );
	if(fabs(temp)/fabs(inRange[1]-inRange[0])<1e-2)
	{
		*nTicks = 4;
		*nMinor = 4;
		outRange[0] = inRange[0];
		outRange[1] = inRange[1];
		return;
	}
	*nMinor = (int)fabs(RoundToRange( tempRange / temp, 1.0 ));

	// Done!
	outRange[0] = inRange[0];
	outRange[1] = inRange[1];

	// Additional
	int nOldTicks = *nTicks;
	ResetTicks(outRange, *nTicks);
	if(nOldTicks != *nTicks)
	{
		*nMinor = 5;
	}
}

void	GradientSides(HDC hDC, POINT pt1, POINT pt2, POINT pt3, POINT pt4, COLORREF crCt, COLORREF crS1, COLORREF crS2)
{
	Point pt[] = {Point(pt1.x, pt1.y), Point((pt1.x+pt2.x)/2, (pt1.y+pt2.y)/2), Point(pt2.x, pt2.y), Point(pt3.x, pt3.y), Point((pt3.x+pt4.x)/2, (pt3.y+pt4.y)/2), Point(pt4.x, pt4.y)};
	//PathGradientBrush *brush = new PathGradientBrush(pt, sizeof(pt)/sizeof(pt[0]));
	PathGradientBrush brush(pt, sizeof(pt)/sizeof(pt[0]));
	brush.SetCenterColor(Color(GetRValue(crCt), GetGValue(crCt), GetBValue(crCt)));
	Color crCent = Color(GetRValue(crCt), GetGValue(crCt), GetBValue(crCt));
	Color crSide1 = Color(GetRValue(crS1), GetGValue(crS1), GetBValue(crS1));
	Color crSide2 = Color(GetRValue(crS2), GetGValue(crS2), GetBValue(crS2));
	Color crRound[] = {crSide1, crCent, crSide2, crSide2, crCent, crSide1};
	int nCr=6;
	brush.SetSurroundColors(crRound, &nCr);
	
	Graphics graph(hDC);
		graph.FillPolygon(&brush, pt, 6);
}

void	GradientSides(HDC hDC, POINT point[6], COLORREF crCt, COLORREF crS1, COLORREF crS2)
{
	Point pt[] = {Point(point[0].x, point[0].y), Point(point[1].x, point[1].y), Point(point[2].x, point[2].y), Point(point[3].x, point[3].y), Point(point[4].x, point[4].y), Point(point[5].x, point[5].y)};
	//PathGradientBrush *brush = new PathGradientBrush(pt, sizeof(pt)/sizeof(pt[0]));
	PathGradientBrush brush(pt, sizeof(pt)/sizeof(pt[0]));
	brush.SetCenterPoint(Point( (point[1].x+point[4].x)/2, (point[1].y+point[4].y)/2 ));
	brush.SetCenterColor(Color(GetRValue(crCt), GetGValue(crCt), GetBValue(crCt)));
	Color crCent = Color(GetRValue(crCt), GetGValue(crCt), GetBValue(crCt));
	Color crSide1 = Color(GetRValue(crS1), GetGValue(crS1), GetBValue(crS1));
	Color crSide2 = Color(GetRValue(crS2), GetGValue(crS2), GetBValue(crS2));
	Color crRound[] = {crSide1, crCent, crSide2, crSide2, crCent, crSide1};
	int nCr=6;
	brush.SetSurroundColors(crRound, &nCr);
	
	Graphics graph(hDC);
		graph.FillPolygon(&brush, pt, 6);
}

//也可以用LinearGradientBrush实现，要用SetBlendTriangularShape
void	GradientRect( HDC hDC, RECT rect, int type, COLORREF cr1, COLORREF cr2)
{
	TRIVERTEX			vert[4];
	GRADIENT_RECT		gRect;
	GRADIENT_TRIANGLE	gTri[2];
	UINT r,g,b;
	DWORD dwMode;

	switch(type)
	{
	case kGradientL2R:
	case kGradientT2B:
		r=GetRValue(cr1);
		g=GetGValue(cr1);
		b=GetBValue(cr1);

		vert [0] .x      = rect.left;
		vert [0] .y      = rect.top;
		vert [0] .Red    = r*256;
		vert [0] .Green  = g*256;
		vert [0] .Blue   = b*256;
		vert [0] .Alpha  = 0x0000;
		
		r=GetRValue(cr2);
		g=GetGValue(cr2);
		b=GetBValue(cr2);

		vert [1] .x      = rect.right;
		vert [1] .y      = rect.bottom;
		vert [1] .Red    = r*256;
		vert [1] .Green  = g*256;
		vert [1] .Blue   = b*256;
		vert [1] .Alpha  = 0x0000;

		gRect.UpperLeft  = 0;
		gRect.LowerRight = 1;

		
		if(type == kGradientL2R)
		{
			dwMode=GRADIENT_FILL_RECT_H;
		}
		else
		{
			dwMode=GRADIENT_FILL_RECT_V;
		}
		GradientFill(hDC, vert, 4, &gRect, 1, dwMode);
		return;
	case kGradientLT2RB:
		r=GetRValue(cr1);
		g=GetGValue(cr1);
		b=GetBValue(cr1);
		
		vert [0] .x      = rect.left;
		vert [0] .y      = rect.top;
		vert [0] .Red    = r*256;
		vert [0] .Green  = g*256;
		vert [0] .Blue   = b*256;
		vert [0] .Alpha  = 0x0000;

		r=( GetRValue(cr1)+GetRValue(cr2) )/2;
		g=( GetGValue(cr1)+GetGValue(cr2) )/2;
		b=( GetBValue(cr1)+GetBValue(cr2) )/2;
		
		vert [1] .x      = rect.right;
		vert [1] .y      = rect.top;
		vert [1] .Red    = r*256;
		vert [1] .Green  = g*256;
		vert [1] .Blue   = b*256;
		vert [1] .Alpha  = 0x0000;

		vert [2] .x      = rect.left;
		vert [2] .y      = rect.bottom;
		vert [2] .Red    = r*256;
		vert [2] .Green  = g*256;
		vert [2] .Blue   = b*256;
		vert [2] .Alpha  = 0x0000;

		r=GetRValue(cr2);
		g=GetGValue(cr2);
		b=GetBValue(cr2);

		vert [3] .x      = rect.right;
		vert [3] .y      = rect.bottom;
		vert [3] .Red    = r*256;
		vert [3] .Green  = g*256;
		vert [3] .Blue   = b*256;
		vert [3] .Alpha  = 0x0000;

		gTri[0].Vertex1 = 0;
		gTri[0].Vertex2 = 1;
		gTri[0].Vertex3 = 2;

		gTri[1].Vertex1 = 3;
		gTri[1].Vertex2 = 1;
		gTri[1].Vertex3 = 2;
		
		
		dwMode=GRADIENT_FILL_TRIANGLE;
		
		GradientFill(hDC, vert, 4, &gTri, 2, dwMode);
		return;

	case kGradientLB2RT:
		r=GetRValue(cr1);
		g=GetGValue(cr1);
		b=GetBValue(cr1);
		
		vert [0] .x      = rect.left;
		vert [0] .y      = rect.bottom;
		vert [0] .Red    = r*256;
		vert [0] .Green  = g*256;
		vert [0] .Blue   = b*256;
		vert [0] .Alpha  = 0x0000;
		
		r=( GetRValue(cr1)+GetRValue(cr2) )/2;
		g=( GetGValue(cr1)+GetGValue(cr2) )/2;
		b=( GetBValue(cr1)+GetBValue(cr2) )/2;
		
		vert [1] .x      = rect.right;
		vert [1] .y      = rect.bottom;
		vert [1] .Red    = r*256;
		vert [1] .Green  = g*256;
		vert [1] .Blue   = b*256;
		vert [1] .Alpha  = 0x0000;
		
		vert [2] .x      = rect.left;
		vert [2] .y      = rect.top;
		vert [2] .Red    = r*256;
		vert [2] .Green  = g*256;
		vert [2] .Blue   = b*256;
		vert [2] .Alpha  = 0x0000;

		r=GetRValue(cr2);
		g=GetGValue(cr2);
		b=GetBValue(cr2);
		
		vert [3] .x      = rect.right;
		vert [3] .y      = rect.top;
		vert [3] .Red    = r*256;
		vert [3] .Green  = g*256;
		vert [3] .Blue   = b*256;
		vert [3] .Alpha  = 0x0000;

		gTri[0].Vertex1 = 0;
		gTri[0].Vertex2 = 1;
		gTri[0].Vertex3 = 2;
		
		gTri[1].Vertex1 = 3;
		gTri[1].Vertex2 = 1;
		gTri[1].Vertex3 = 2;
		
		dwMode=GRADIENT_FILL_TRIANGLE;
		
		GradientFill(hDC, vert, 4, &gTri, 2, dwMode);
		return;

	case kGradientCt2Bd:
		{
			Point pt[] = {Point(rect.left, rect.bottom), Point(rect.right, rect.bottom), Point(rect.right, rect.top), Point(rect.left, rect.top)};
			PathGradientBrush *brush = new PathGradientBrush(pt, sizeof(pt)/sizeof(pt[0]));
			brush->SetCenterColor(Color(GetRValue(cr1), GetGValue(cr1), GetBValue(cr1)));
			Color crBd = Color(GetRValue(cr2), GetGValue(cr2), GetBValue(cr2));
			int nCr=1;
			brush->SetSurroundColors(&crBd, &nCr);

			Graphics graph(hDC);
			graph.FillRectangle(brush, rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top);
		}
		return;
	case kGradientHCt2Bd:
		POINT pt[4];
		pt[0].x = rect.left;
		pt[0].y = rect.top;
		
		pt[1].x = rect.left;
		pt[1].y = rect.bottom;
		
		pt[2].x = rect.right;
		pt[2].y = rect.bottom;
		
		pt[3].x = rect.right;
		pt[3].y = rect.top;
		GradientSides(hDC, pt[0], pt[1], pt[2], pt[3], cr1, cr2, cr2);
		return;
	case kGradientVCt2Bd:
		//POINT pt[4];
		pt[0].x = rect.left;
		pt[0].y = rect.top;
		
		pt[3].x = rect.left;
		pt[3].y = rect.bottom;
		
		pt[2].x = rect.right;
		pt[2].y = rect.bottom;
		
		pt[1].x = rect.right;
		pt[1].y = rect.top;
		GradientSides(hDC, pt[0], pt[1], pt[2], pt[3], cr1, cr2, cr2);
		return;
	case kGradientRadial:
		{
			int ctx = (rect.left+rect.right)/2;
			int cty = (rect.top+rect.bottom)/2;
			int wid	= rect.right - rect.left;
			int	hei	= rect.bottom - rect.top;
			double sqrt2 = sqrt(2.0);

			int x0 = int(ctx - wid*sqrt2/2.0);
			int y0 = int(cty - hei*sqrt2/2.0);

			GraphicsPath *path = new GraphicsPath;
			path->AddEllipse(x0, y0, int(wid*sqrt2), int(hei*sqrt2));

			PathGradientBrush *brush = new PathGradientBrush(path);
			
			brush->SetCenterColor(Color(GetRValue(cr1), GetGValue(cr1), GetBValue(cr1)));
			Color crBd = Color(GetRValue(cr2), GetGValue(cr2), GetBValue(cr2));
			int nCr=1;
			brush->SetSurroundColors(&crBd, &nCr);


			Graphics graph(hDC);

			Point polyPoints[] = {Point(rect.left, rect.top), Point(rect.right, rect.top), 
				Point(rect.right, rect.bottom), Point(rect.left, rect.bottom)};
			GraphicsPath clippath;
			clippath.AddPolygon(polyPoints, 4);
			// Construct a region based on the path.
			Region region(&clippath);
			// Set the clipping region of the Graphics object.
			graph.SetClip(&region);
			
			graph.FillEllipse(brush, x0, y0, int(wid*sqrt2), int(hei*sqrt2));
		}
		return;
	}
}

void		GradientRect( HDC hDC, RECT rect, bool bHoriz, COLORREF *pCr, long *pCord, int nCord)
{
	if(!pCr || !pCord)return;
	int i;
	for(i=0; i<nCord-1; i++)
	{
		if(bHoriz)
		{
			if(pCord[i]>=pCord[i+1])return;
		}
		else
		{
			if(pCord[i]<=pCord[i+1])return;
		}
	}
	if(bHoriz)
	{
		if(pCord[0]<=rect.left)return;
		if(pCord[nCord-1]>=rect.right)return;
	}
	else
	{
		if(pCord[0]>=rect.bottom)return;
		if(pCord[nCord-1]<=rect.top)return;
	}

	LinearGradientBrush *linGrBrush;
	if(bHoriz)
	{
		linGrBrush = new LinearGradientBrush(Point(rect.left, rect.top), Point(rect.right, rect.top), Color(255, GetRValue(pCr[0]), GetGValue(pCr[0]), GetBValue(pCr[0])), Color(255, GetRValue(pCr[nCord+1]), GetGValue(pCr[nCord+1]), GetBValue(pCr[nCord+1])));
	}
	else
	{
		linGrBrush = new LinearGradientBrush(Point(rect.left, rect.bottom), Point(rect.left, rect.top), Color(255, GetRValue(pCr[0]), GetGValue(pCr[0]), GetBValue(pCr[0])), Color(255, GetRValue(pCr[nCord+1]), GetGValue(pCr[nCord+1]), GetBValue(pCr[nCord+1])));
	}
	Color *colors = new Color[nCord+2];
	for(i=0; i<nCord+2; i++)
	{
		colors[i] = Color(255, GetRValue(pCr[i]), GetGValue(pCr[i]), GetBValue(pCr[i]));
	}
	float *ratio = new float[nCord+2];
	ratio[0] = 0.0f;
	ratio[nCord+1] = 1.0f;
	for(i=1; i<nCord+1; i++)
	{
		if(bHoriz)
		{
			ratio[i] = (pCord[i-1] - rect.left)/(float)Width(rect);
		}
		else
		{
			ratio[i] = (rect.bottom - pCord[i-1])/(float)Height(rect);
		}
	}
	linGrBrush->SetInterpolationColors(colors, ratio, nCord+2);

	Graphics graphics(hDC);
	graphics.FillRectangle(linGrBrush, rect.left, rect.top, Width(rect), Height(rect)); 

	delete linGrBrush;
}

void		MyGetTextExtentPoint(HDC hDC, const LOGFONT *logFont, tstring str, UINT uFormat, LPSIZE lpSize)
{
	HFONT hFont, hOldFont;
	if(logFont!=0)
	{
		hFont = CreateFontIndirect(logFont);
	}
	else
	{
		LOGFONT lf = { 0 };
		::GetObject(::GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
		hFont = CreateFontIndirect(&lf);
	}
	hOldFont = (HFONT)SelectObject(hDC, hFont);

	RECT rect = {0, 0, 0, 0};
	DrawText(hDC, str.c_str(), (int)str.length(), &rect, uFormat | DT_CALCRECT);
	lpSize->cx = rect.right - rect.left;
	lpSize->cy = rect.bottom - rect.top;

	SelectObject(hDC, hOldFont);
	DeleteObject(hFont);
}

void		EraseRect( HDC hDC, RECT rect, COLORREF color)
{
	HBRUSH hBrush, hOldBrush;
	hBrush = CreateSolidBrush(color);
	hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
	
	HPEN hPen, hOldPen;
	hPen = (HPEN)GetStockObject(NULL_PEN);
	hOldPen = (HPEN)SelectObject(hDC, hPen);
	
	Rectangle(hDC, rect.left, rect.top, rect.right, rect.bottom);
	
	SelectObject(hDC, hOldPen);
	DeleteObject(hPen);
	SelectObject(hDC, hOldBrush);
	DeleteObject(hBrush);
}

void		EraseRectGap(HDC hDC, RECT outerRect, RECT innerRect, COLORREF color)
{
	RECT tempRect;
	// Left top
	SetRect(&tempRect, outerRect.left, outerRect.top, innerRect.left, innerRect.top);
	InflateRect(&tempRect, 1, 1);
	EraseRect(hDC, tempRect, color);
	// Top
	SetRect(&tempRect, innerRect.left, outerRect.top, innerRect.right, innerRect.top);
	InflateRect(&tempRect, 1, 1);
	EraseRect(hDC, tempRect, color);
	// Left right
	SetRect(&tempRect, innerRect.right, outerRect.top, outerRect.right, innerRect.top);
	InflateRect(&tempRect, 1, 1);
	EraseRect(hDC, tempRect, color);
	// Left
	SetRect(&tempRect, outerRect.left, innerRect.top, innerRect.left, innerRect.bottom);
	InflateRect(&tempRect, 1, 1);
	EraseRect(hDC, tempRect, color);
	// Right
	SetRect(&tempRect, innerRect.right, innerRect.top, outerRect.right, innerRect.bottom);
	InflateRect(&tempRect, 1, 1);
	EraseRect(hDC, tempRect, color);
	// bottom top
	SetRect(&tempRect, outerRect.left, innerRect.bottom, innerRect.left, outerRect.bottom);
	InflateRect(&tempRect, 1, 1);
	EraseRect(hDC, tempRect, color);
	// bottom
	SetRect(&tempRect, innerRect.left, innerRect.bottom, innerRect.right, outerRect.bottom);
	InflateRect(&tempRect, 1, 1);
	EraseRect(hDC, tempRect, color);
	// bottom right
	SetRect(&tempRect, innerRect.right, innerRect.bottom, outerRect.right, outerRect.bottom);
	InflateRect(&tempRect, 1, 1);
	EraseRect(hDC, tempRect, color);
}

void		EraseEllipse( HDC hDC, RECT rect, COLORREF color)
{
	HBRUSH hBrush, hOldBrush;
	hBrush = CreateSolidBrush(color);
	hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
	
	HPEN hPen, hOldPen;
	hPen = (HPEN)GetStockObject(NULL_PEN);
	hOldPen = (HPEN)SelectObject(hDC, hPen);
	
	Ellipse(hDC, rect.left, rect.top, rect.right, rect.bottom);
	
	SelectObject(hDC, hOldPen);
	DeleteObject(hPen);
	SelectObject(hDC, hOldBrush);
	DeleteObject(hBrush);
}

HBITMAP HBitmapFromFile(tstring fileName)
{
	wstring name;
#if defined(_UNICODE) || defined(UNICODE)
	name = fileName;
#else
	if(!mymbs2wcs(fileName, name))return 0;
#endif
	Bitmap bitmap(name.c_str());
	if(bitmap.GetLastStatus() != Ok)return NULL;

	HBITMAP hBitmap;
	Color bgColor(255, 255, 255, 255);
	
	bitmap.GetHBITMAP(bgColor, &hBitmap);
	return hBitmap;
	
/*	string name;
#if defined(_UNICODE) || defined(UNICODE)
	name = mywcs2mbs(fileName);
#else
	name = fileName;
#endif
	HANDLE hDIB = OpenDIB(name.c_str());
	HPALETTE hPal = CreateDIBPalette(hDIB);
	HBITMAP hBitmap = BitmapFromDIB(hDIB, hPal);
	DeleteObject(hPal);
	DeleteObject(hDIB);

	return hBitmap;*/
}

HBITMAP HBitmapFromWnd(HWND hWnd)
{
	if(!IsWindow(hWnd))
		return 0;
	HDC hDC = GetDC(hWnd);
	HBITMAP hBm = HBitmapFromDC(hDC);
	ReleaseDC(hWnd, hDC);
	return hBm;
}

HBITMAP HBitmapFromDC(HDC hDC)
{
	HDC hMemDC;
	hMemDC = CreateCompatibleDC(hDC);
	
	HBITMAP hBm, hOldBm;
	RECT rt;
	GetClientRect(WindowFromDC(hDC), &rt);
	hBm = CreateCompatibleBitmap(hDC, rt.right-rt.left, rt.bottom-rt.top);
	hOldBm = (HBITMAP)SelectObject(hMemDC, hBm);
	BitBlt(hMemDC, rt.left, rt.top, rt.right-rt.left, rt.bottom-rt.top, hDC, rt.left, rt.top, SRCCOPY);
	hBm = (HBITMAP)SelectObject(hMemDC, hOldBm);
	
	DeleteDC(hMemDC);
	
	return hBm;
}

HBITMAP	CopyHBitmap(HBITMAP hSourceBitmap)
{
	if(!hSourceBitmap)
		return 0;

	HBITMAP hBitmapRet = (HBITMAP)CopyImage( hSourceBitmap, IMAGE_BITMAP, 0, 0, LR_COPYRETURNORG );
	return hBitmapRet;
}

SIZE	GetBitmapSize(HBITMAP hBitmap)
{
	SIZE bitmapSize = {0, 0};
	
	if(!hBitmap)return bitmapSize;
	
	BITMAP bm;
	GetObject(hBitmap, sizeof(BITMAP), &bm);
	bitmapSize.cx = bm.bmWidth;
	bitmapSize.cy = bm.bmHeight;
	
	return bitmapSize;
}

bool	CopyImageToClipBrd(HWND hWnd)
{
	HBITMAP hBm = HBitmapFromWnd(hWnd);
	if(!hBm)
		return false;

	BITMAP bm;
	GetObject(hBm, sizeof(BITMAP), &bm);

	HANDLE hDib;
	hDib = DIBFromBitmap(hBm, 0, bm.bmBitsPixel, 0);
	if(!hDib)
	{
		DeleteObject(hBm);
		return false;
	}
	
	if (OpenClipboard(hWnd))
	{
		//BeginWaitCursor();
		EmptyClipboard();
		SetClipboardData (CF_DIB, hDib );
		CloseClipboard();
		//EndWaitCursor();

		DeleteObject(hBm);
		DeleteObject(hDib);
		return true;
	}
	DeleteObject(hBm);
	DeleteObject(hDib);
	return false;
}

bool	SaveImageToFile(HWND hWnd, tstring fileName, tstring fileext)
{
	Sleep(100);
	
	HBITMAP hBm = HBitmapFromWnd(hWnd);
	if(!hBm)
		return false;

	SaveHBitmapToFile(hBm, fileName, fileext);
	
	DeleteObject(hBm);
	
	return true;
}

bool	SaveHBitmapToFile(HBITMAP hBm, tstring fileName, tstring fileext)
{
	if(!hBm)return false;
	
	BITMAP bm;
	GetObject(hBm, sizeof(BITMAP), &bm);
	
	HANDLE hDib;
	hDib = DIBFromBitmap(hBm, 0, bm.bmBitsPixel, 0);
	if(!hDib)
	{
		DeleteObject(hBm);
		return false;
	}

	// The saving action is replaced by GDI+ now
	string_toupper(fileext);
	if (fileext == _TEXT("JPG"))fileext = _TEXT("JPEG");
	else if (fileext == _TEXT("TIF"))fileext = _TEXT("TIFF");
	
	CLSID clsid;
	if (GetEncoderClassID(fileext.c_str(), &clsid))// function declared myself
	{
		HPALETTE hPal = CreateDIBPalette(hDib);
		Bitmap *bitmap = Bitmap::FromHBITMAP(hBm, hPal);

#if defined(_UNICODE) || defined(UNICODE)
		bitmap->Save(fileName.c_str(), &clsid, NULL);
#else
		wstring name;
		if(!mymbs2wcs(fileName, name))return false;
		bitmap->Save(name.c_str(), &clsid, NULL);
#endif

		if (bitmap->GetLastStatus() != Ok) 
			MessageBox(NULL, _TEXT("Error occur when saving image!"), _TEXT("Error"), MB_OK);
	}
	else
	{
		MessageBox(NULL, _TEXT("Decoder not found!"), _TEXT("Error"), MB_OK);
	}

/*
	// The following is the old GDI code, now commented
	string name;
#if defined(_UNICODE) || defined(UNICODE)
	name = mywcs2mbs(fileName);
#else
	name = fileName;
#endif
	if(!WriteDIB(name.c_str(), hDib))
	{
		DeleteObject(hBm);
		DeleteObject(hDib);
		return false;
	}
*/

	DeleteObject(hDib);

	return true;
}

BOOL			MyClientToScreen(HWND hWnd, LPRECT lpRect)
{
	if(!ClientToScreen(hWnd, (LPPOINT)lpRect))return FALSE;
	if(!ClientToScreen(hWnd, (LPPOINT)lpRect+1))return FALSE;
	return TRUE;
}
BOOL			MyScreenToClient(HWND hWnd, LPRECT lpRect)
{
	if(!ScreenToClient(hWnd, (LPPOINT)lpRect))return FALSE;
	if(!ScreenToClient(hWnd, (LPPOINT)lpRect+1))return FALSE;
	return TRUE;
}

long	Width(RECT rect)
{
	return abs(rect.right - rect.left);
}

long	Height(RECT rect)
{
	return abs(rect.bottom - rect.top);
}

double	GetCenterX(RECT rect)
{
	return (rect.left + rect.right)/2.0;
}

double	GetCenterY(RECT rect)
{
	return (rect.top + rect.bottom)/2.0;
}

bool	SquareRect(RECT &rect)
{
	if( abs( rect.right - rect.left ) > abs( rect.bottom - rect.top ) )
	{
		rect.right = rect.left + abs( rect.bottom - rect.top );
	}
	else
	{
		rect.bottom = rect.top + abs( rect.right - rect.left );
	}
	return true;
}

bool	ScaleRect(RECT &rect, double xlow, double xhigh, double ylow, double yhigh)
{
	if(fabs(xlow - xhigh)<fMinVal || fabs(ylow-yhigh)<fMinVal)return false;

	double dataRatio = fabs(yhigh - ylow)/fabs(xhigh - xlow);
	double rectRatio = Height(rect)/(double)Width(rect);

	if(dataRatio > rectRatio)
	{
		rect.right = long(rect.left + Height(rect) / dataRatio);
	}
	else
	{
		rect.bottom = long(rect.top + Width(rect) * dataRatio);
	}
	return true;
}

void	NormalizeRect(RECT &rect)
{
	if(rect.left > rect.right) std::swap(rect.left, rect.right);
	if(rect.top > rect.bottom) std::swap(rect.top, rect.bottom);
}

RECT	GetTextRectAroundEllipse(HDC hDC, RECT ellipseRect, RECT *outerRect, double tht, double scalex, double scaley, tstring str, LOGFONT logFont, int innerGap/* = 4*/, bool outer/* = true*/)
{
	RECT textRect = {0, 0, 0, 0};
	
	SIZE tmSize;
	int cx, cy;
	MyGetTextExtentPoint(hDC, &logFont, str, DT_LEFT, &tmSize);
	cx = tmSize.cx;
	cy = tmSize.cy;
	
	double ctX, ctY;
	ctX = (ellipseRect.left + ellipseRect.right)/2.0;
	ctY = (ellipseRect.bottom + ellipseRect.top)/2.0;
	double a, b;
	a = Width(ellipseRect)/2.0;
	b = Height(ellipseRect)/2.0;
	
	int x0, y0;
	x0 = int(ctX + a * cos(tht) * scalex);
	y0 = int(ctY - b * sin(tht) * scaley);

	int delta;
	
	textRect.left = x0;
	textRect.top = y0;
	textRect.right = x0 + cx;
	textRect.bottom = y0 + cy;
	
	double rng = tht/3.14159265358979323846264336328*2.0;
	double rmd = rng - 4 * (floor(rng/4.0));
	
	double precise = 0.05;

	double x, y, dx, dy;
	x = fabs(a * cos(tht) * scalex);
	y = fabs(b * sin(tht) * scalex);
	if(outer)
	{
		if(fabs(rmd) < precise)
		{
			dx = -innerGap;
			dy = cy/2.0;
		}
		else if(fabs(rmd - 1.0) < precise)
		{
			dx = cx/2.0;
			dy = (cy+innerGap);
		}
		else if(fabs(rmd - 2.0) < precise)
		{
			dx = (cx+innerGap);
			dy = cy/2.0;
		}
		else if(fabs(rmd - 3.0) < precise)
		{
			dx = cx/2.0;
			dy = -innerGap;
		}
		else if( rmd > 0.0 && rmd < 1.0 )
		{
			dx = ( (a - x)*cx/2.0 + (x - 0)*(-innerGap) )/a;
			dy = ( (y - 0)*(cy+innerGap) + (b - y)*cy/2.0 )/b;
		}
		else if( rmd > 1.0 && rmd < 2.0)
		{
			dx = ( (x - 0)*(cx+innerGap) + (a - x)*cx/2.0 )/a;
			dy = ( (b - y)*cy/2.0 + (y - 0)*(cy+innerGap) )/b;
		}
		else if( rmd > 2.0 && rmd < 3.0)
		{
			dx = ( (a - x)*cx/2.0 + (x - 0)*(cx+innerGap))/a;
			dy = ( (y - 0)*(-innerGap) + (b - y)*cy/2.0 )/b;
		}
		else if( rmd > 3.0 && rmd < 4.0)
		{
			dx = ( (x - 0)*(-innerGap) + (a - x)*cx/2.0 )/a;
			dy = ( (b - y)*cy/2.0 + (y - 0)*(-innerGap) )/b;
		}
		textRect.left -= (long)dx;
		textRect.right -= (long)dx;
		textRect.top -= (long)dy;
		textRect.bottom -= (long)dy;
		
		if(outerRect)
		{
			if(textRect.left < outerRect->left)
			{
				delta = outerRect->left - textRect.left;
				textRect.left += delta;
				textRect.right += delta;
			}
			if(textRect.right > outerRect->right)
			{
				delta = textRect.right - outerRect->right;
				textRect.left -= delta;
				textRect.right -= delta;
			}
			if(textRect.top < outerRect->top)
			{
				delta = outerRect->top - textRect.top;
				textRect.bottom += delta;
				textRect.top += delta;
			}
			if(textRect.bottom > outerRect->bottom)
			{
				delta = textRect.bottom - outerRect->bottom;
				textRect.bottom -= delta;
				textRect.top -= delta;
			}
		}
	}
	else
	{
		if(fabs(rmd) < precise)
		{
			dx = (cx+innerGap);
			dy = cy/2.0;
		}
		else if(fabs(rmd - 1.0) < precise)
		{
			dx = cx/2.0;
			dy = -innerGap;
		}
		else if(fabs(rmd - 2.0) < precise)
		{
			dx = - innerGap;
			dy = cy/2.0;
		}
		else if(fabs(rmd - 3.0) < precise)
		{
			dx = cx/2.0;
			dy = (cy+innerGap);
		}
		else if( rmd > 0.0 && rmd < 1.0 )
		{
			dx = ( (a - x)*cx/2.0 + (x - 0)*(cx + innerGap) )/a;
			dy = ( (y - 0)*(-innerGap) + (b - y)*cy/2.0 )/b;
		}
		else if( rmd > 1.0 && rmd < 2.0)
		{
			dx = ( (x - 0)*(-innerGap) + (a - x)*cx/2.0 )/a;
			dy = ( (b - y)*cy/2.0 + (y - 0)*(-innerGap) )/b;
		}
		else if( rmd > 2.0 && rmd < 3.0)
		{
			dx = ( (a - x)*cx/2.0 + (x - 0)*(-innerGap) )/a;
			dy = ( (y - 0)*(cy+innerGap)+ (b - y)*cy/2.0 )/b;
		}
		else if( rmd > 3.0 && rmd < 4.0)
		{
			dx = ( (x - 0)*(cx+innerGap) + (a - x)*cx/2.0 )/a;
			dy = ( (b - y)*cy/2.0 + (y - 0)*(cy+innerGap) )/b;
		}

		textRect.left -= (long)dx;
		textRect.right -= (long)dx;
		textRect.top -= (long)dy;
		textRect.bottom -= (long)dy;
	}	
	return textRect;
}

void	DrawCircleEdge(HDC hDC, double ctx, double cty, double radius, bool toInner, double thick, double angle1, COLORREF cr1, COLORREF cr2)
{
	Graphics graph(hDC);	
	graph.SetSmoothingMode(SmoothingModeHighQuality);

	Color color1(GetRValue(cr1), GetGValue(cr1), GetBValue(cr1));
	Color color2(GetRValue(cr2), GetGValue(cr2), GetBValue(cr2));

	if(!toInner)radius += thick;

	double myPi = 3.14159265358979323846264338328;
	PointF pt1(float(ctx + radius*cos(angle1*myPi/180.0)), float(cty - radius*sin(angle1*myPi/180.0)));
	PointF pt2(float(ctx - radius*cos(angle1*myPi/180.0)), float(cty + radius*sin(angle1*myPi/180.0)));

	LinearGradientBrush brush(pt1, pt2, color1, color2);
	
	Pen pen(&brush, (float)thick);
	pen.SetAlignment(PenAlignmentInset);
	graph.DrawEllipse(&pen, float(ctx - radius), float(cty - radius), float(2.0 * radius), float(2.0 * radius));
}

void	DrawArrowInCircle(HDC hDC, POINT point, int nRadius, double angle, COLORREF fillColor, COLORREF arrowColor)
{
	Graphics g(hDC);

	Color crf(255, GetRValue(fillColor), GetGValue(fillColor), GetBValue(fillColor));
	SolidBrush brush(crf) ;

	g.FillEllipse(&brush, point.x-nRadius, point.y-nRadius, 2*nRadius, 2*nRadius);
	
	g.SetSmoothingMode(SmoothingModeHighQuality);
	Color cra(255, GetRValue(arrowColor), GetGValue(arrowColor), GetBValue(arrowColor));
	Pen pen(cra, (Gdiplus::REAL)nRadius/3);
	//pen.SetEndCap(LineCapArrowAnchor);   //{终点线帽，这里是圆形锚头帽}
	AdjustableArrowCap cap(4, 3, true);
	pen.SetCustomEndCap(&cap);

	double ang = angle*myDeg;
	
	g.DrawLine(&pen, int(point.x-(nRadius-1)*cos(ang)), int(point.y+(nRadius-1)*sin(ang)), int(point.x+nRadius*cos(ang)), int(point.y-nRadius*sin(ang)));
}

void	FillEllipse(HDC hDC, POINT point, int a, int b, COLORREF fillColor)
{
	Graphics g(hDC);
	
	Color crf(255, GetRValue(fillColor), GetGValue(fillColor), GetBValue(fillColor));
	SolidBrush brush(crf) ;
	
	g.FillEllipse(&brush, point.x-a, point.y-b, 2*a, 2*b);
}

void	DrawLine(HDC hDC, RECT plotRect, double *pt1, double *pt2, double *xRange, double *yRange, COLORREF cr, int nLineWidth, int nLineStyle)
{
	DrawLine(hDC, plotRect, pt1[0], pt1[1], pt2[0], pt2[1], xRange, yRange, cr, nLineWidth, nLineStyle);
}

void	DrawLine(HDC hDC, RECT plotRect, double x1, double y1, double x2, double y2, double *xRange, double *yRange, COLORREF cr, int nLineWidth, int nLineStyle)
{
	POINT point1, point2;
	point1.x = long(plotRect.left + (x1-xRange[0])*Width(plotRect)/(xRange[1]-xRange[0]));
	point1.y = long(plotRect.bottom - (y1-yRange[0])*Height(plotRect)/(yRange[1]-yRange[0]));
	point2.x = long(plotRect.left + (x2-xRange[0])*Width(plotRect)/(xRange[1]-xRange[0]));
	point2.y = long(plotRect.bottom - (y2-yRange[0])*Height(plotRect)/(yRange[1]-yRange[0]));
	
	HPEN hPen = CreatePen(nLineStyle, nLineWidth, cr);
	HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);
	MoveToEx(hDC, point1.x, point1.y, NULL);
	LineTo(hDC, point2.x, point2.y);
	SelectObject(hDC, hOldPen);
	DeleteObject(hPen);
}

void	DrawLine(HDC hDC, POINT pt1, POINT pt2, int style, int width, COLORREF cr)
{
	Graphics g(hDC);
	
	g.SetSmoothingMode(SmoothingModeHighQuality);

	Color color(255, GetRValue(cr), GetGValue(cr), GetBValue(cr));
	Pen pen(color, (Gdiplus::REAL)width);
	switch(style)
	{
	case PS_SOLID:
		pen.SetDashStyle(DashStyleSolid);
		break;
	case PS_DASH:
		pen.SetDashStyle(DashStyleDash);
		break;
	case PS_DOT:
		pen.SetDashStyle(DashStyleDot);
		break;
	case PS_DASHDOT:
		pen.SetDashStyle(DashStyleDashDot);
		break;
	case PS_DASHDOTDOT:
		pen.SetDashStyle(DashStyleDashDotDot);
		break;
	}
	g.DrawLine(&pen, pt1.x, pt1.y, pt2.x, pt2.y);
}

void	DrawArrowLine(HDC hDC, POINT pt1, POINT pt2, int alpha, COLORREF cr, int nLineWidth, int nLineStyle)
{
	Graphics g(hDC);
	
	g.SetSmoothingMode(SmoothingModeHighQuality);
	
	Color color(alpha, GetRValue(cr), GetGValue(cr), GetBValue(cr));
	Pen pen(color, (Gdiplus::REAL)nLineWidth);
	
	//设置箭头大小
	AdjustableArrowCap cap(4, 3, true);
	pen.SetCustomEndCap(&cap);
	pen.SetStartCap(LineCapRoundAnchor); //{起点线帽，这里是箭头状锚头帽}
	//pen.SetEndCap(LineCapArrowAnchor);   //{终点线帽，这里是圆形锚头帽}
		
	g.DrawLine(&pen, pt1.x, pt1.y, pt2.x, pt2.y);
}

void	DrawRect(HDC hDC, RECT plotRect, double *lt, double *rb, double *xRange, double *yRange, COLORREF cr, int nLineSize, int nLineStyle, bool bFill, COLORREF fillColor)
{
	DrawRect(hDC, plotRect, lt[0], lt[1], rb[0], rb[1], xRange, yRange, cr, nLineSize, nLineStyle, bFill, fillColor);
}

void	DrawRect(HDC hDC, RECT plotRect, double left, double top, double right, double bottom, double *xRange, double *yRange, COLORREF cr, int nLineSize, int nLineStyle, bool bFill, COLORREF fillColor)
{
	HPEN hOldPen, hPen;
	hPen = CreatePen(nLineStyle, nLineSize, cr);
	HBRUSH hOldBrush, hBrush;
	if(bFill)
		hBrush = CreateSolidBrush(fillColor);
	else
		hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);

	hOldPen = (HPEN)SelectObject(hDC, hPen);
	hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);

	long l, r, t, b;

	l = long(plotRect.left + (left - xRange[0])*Width(plotRect)/(xRange[1] - xRange[0])); 
	r = long(plotRect.left + (right - xRange[0])*Width(plotRect)/(xRange[1] - xRange[0])); 
	t = long(plotRect.bottom - (top - yRange[0])*Height(plotRect)/(yRange[1] - yRange[0])); 
	b = long(plotRect.bottom - (bottom - yRange[0])*Height(plotRect)/(yRange[1] - yRange[0])); 

	Rectangle(hDC, l, t, r, b);

	SelectObject(hDC, hOldPen);
	SelectObject(hDC, hOldBrush);

	DeleteObject(hPen);
	if(bFill)
		DeleteObject(hBrush);
}

void	DrawTextInRect(HDC hDC, RECT rect, tstring text, const LOGFONT *logFont, int backmode, COLORREF color, UINT format, LOGFONT *lfReal)
{
	if( text == _TEXT("") )return;
	
	HFONT		hOldFont, hFont;
	bool		keepGoing = true;
	SIZE		tmSize;
	COLORREF	oldColor;
	int			oldMode;

	LOGFONT		lf;
	if(logFont)
	{
		lf = *logFont;
	}
	else
	{
		memset(&lf, 0, sizeof(LOGFONT));
		::GetObject(::GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
	}
	
	// Auto size font. Changes the title font size
	// to smaller if there is not enough space on
	// the plot. Perhaps add a user flag for this?
	while( keepGoing == true )
	{
		MyGetTextExtentPoint(hDC, &lf, text, DT_LEFT, &tmSize);
		if( tmSize.cx > abs(rect.right - rect.left) || 
			tmSize.cy > abs(rect.bottom - rect.top) )
			lf.lfHeight--;
		else
			keepGoing = false;
		
		if( lf.lfHeight <= 4 ) keepGoing = false;
	}

	if(lfReal)(*lfReal) = lf;
	
	// Draw the text. There should be a background transparent mode
	oldMode = SetBkMode( hDC, backmode );
	oldColor = SetTextColor( hDC, color );
	hFont = CreateFontIndirect(&lf);
	hOldFont = (HFONT)SelectObject(hDC, hFont);
	
	DrawText(hDC, text.c_str(), (int)text.length(), &rect, format);
	
	SetBkMode(hDC, oldMode);
	SetTextColor(hDC, oldColor);
	SelectObject( hDC, hOldFont );
	DeleteObject( hFont );
}

RECT	GetTextRect(HDC hDC, POINT ptLT, tstring text, LOGFONT *logFont, bool bConfine, RECT *lpRect, bool bLT)
{
	RECT textRect={0, 0, 0, 0};
	SIZE textSize={0, 0};
	MyGetTextExtentPoint(hDC, logFont, text, DT_LEFT, &textSize);
	textRect.left = ptLT.x;
	textRect.top = ptLT.y;
	textRect.right = textRect.left + textSize.cx;
	textRect.bottom = textRect.top + textSize.cy;
	if(!bLT)
	{
		OffsetRect(&textRect, 0, -textSize.cy);
	}

	if(bConfine && lpRect)
	{
		if(textRect.left<lpRect->left)
		{
			OffsetRect(&textRect, lpRect->left-textRect.left,0);
		}
		if(textRect.right>lpRect->right)
		{
			OffsetRect(&textRect, -(textRect.right-lpRect->right), 0);
		}
		if(textRect.top<lpRect->top)
		{
			OffsetRect(&textRect, 0, lpRect->top-textRect.top);
		}
		if(textRect.bottom>lpRect->bottom)
		{
			OffsetRect(&textRect, 0, -(textRect.bottom-lpRect->bottom));
		}
	}
	return textRect;
}

/*
time_t StringtoTime(TCHAR *str)
{
	struct tm t;
	int year,month,day, hour,minite,second;

	_stscanf_s(str,_T("%d/%d/%d %d:%d:%d"),&month,&day,&year,&hour,&minite,&second);//格式跟输入的字符串格式要统一,这里与%c对应
	t.tm_sec = second;
	t.tm_min = minite;
	t.tm_hour = hour;
	t.tm_mday = day;
	t.tm_mon = month-1;
	t.tm_year = year+2000-1900;//因为vs2010的标准时间日期字符串%c的年是两位，要注意
	t.tm_isdst = 0;

	time_t timeData;
	timeData=mktime(&t);

	return timeData;
}
*/
bool	MyStr2Time(tstring strTime, tstring fmt, time_t &tmTime)
{
	time_t myTime;
	struct tm myTm = {0};
	
	tstring dest;

	if(!_tcsptime(strTime.c_str(), fmt.c_str(), &myTm))return false;
/*	
	int year, month, day, hour, minite, second;

	if(_tcslen(strTime.c_str()) < 14)return false;

#if defined(_MSC_VER) && (_MSC_VER>=1300)
	_stscanf_s(strTime.c_str(), _T("%4d%2d%2d%2d%2d%2d"), &year, &month, &day, &hour, &minite, &second);
#else
	_stscanf(strTime.c_str(), _T("%4d%2d%2d%2d%2d%2d"), &year, &month, &day, &hour, &minite, &second);
#endif
	myTm.tm_year = year-1900;
	myTm.tm_mon = month-1;
	myTm.tm_mday = day;
	myTm.tm_hour = hour;
	myTm.tm_min = minite;
	myTm.tm_sec = second;
	myTm.tm_isdst = 0;
*/	
	myTime = mktime(&myTm);
	tmTime = myTime;
	
	return true;
}

bool	MyStr2Time(tstring strTime, tstring fmt, double &fTime)
{
	time_t tmTime;
	if(!MyStr2Time(strTime, fmt, tmTime))return false;

	fTime = (double)tmTime;
	return true;
}

bool	MyTime2Str(time_t tmTime, tstring &strTime, tstring fmt)
{
	TCHAR strLabel[32];
	struct tm *myTm;
	
#if defined(_MSC_VER) && (_MSC_VER>=1300)
	myTm = new struct tm;
	localtime_s(myTm, &tmTime);
#else
	myTm = localtime(&tmTime);
#endif
	
	memset(strLabel, 0, sizeof(TCHAR)*32);
	_tcsftime(strLabel, 31, fmt.c_str(), myTm);
	
#if defined(_MSC_VER) && (_MSC_VER>=1300)
	delete myTm;
#endif

	strTime = strLabel;
	return true;
}

bool	ValInInterval(double val, double low, double high)
{
	return (val - low) * (high - val) >= 0.0;
}

void	SetMyCapture(HWND hWnd)
{
	SetCapture(hWnd);
	RECT clientRect;
	GetClientRect(hWnd, &clientRect);
	ClientToScreen(hWnd,(LPPOINT)&clientRect);
	ClientToScreen(hWnd,(LPPOINT)&clientRect+1);
	ClipCursor(&clientRect);
}

bool	IsHandle(HANDLE handle)
{
	DWORD dwFlags;
	return GetHandleInformation(handle, &dwFlags) ? true : false;
}

void	DrawVectorText(HDC hDC, tstring strText, POINT ptPos)
{
	if(strText.empty())
        return;
 
	if(GetMapMode(hDC) != MM_TEXT)
	{
		LPtoDP(hDC, &ptPos, 1);
	}
 
	DWORD dwSize;
 
	MAT2 stM2;
	GLYPHMETRICS stGM;        
	TEXTMETRIC stTM;
 
    // 创建字体
	LOGFONT logFont;
	HFONT hOldFont, hFont = (HFONT)GetCurrentObject(hDC, OBJ_FONT);//CreateFontIndirect(&logFont);
	::GetObject(hFont, sizeof(LOGFONT), &logFont);
	bool bCreate = false;
	if(_tcscmp(logFont.lfFaceName , _TEXT("System"))==0)
	{
#if defined(_MSC_VER) && (_MSC_VER>=1300)
		_tcscpy_s((TCHAR*) logFont.lfFaceName , 16, _TEXT("Courier New"));
#else
		_tcscpy((TCHAR*) logFont.lfFaceName , _TEXT("Courier New"));
#endif
		
		hFont = CreateFontIndirect(&logFont);
		hOldFont = (HFONT)SelectObject(hDC, hFont);
		bCreate = true;
	}
	//HFONT hOldFont = (HFONT)SelectObject(hDC, hFont);
	GetTextMetrics(hDC, &stTM );
 
	// 坐标变换矩阵 , 但这种转换时 , 由于精度舍入的原因 , 取得的字体点阵的质量很差 ,
	// 给 stFont.lfEscapement 变量赋值 , 似乎微软做过优化 , 取得字体点阵勉强能说的过去 ,
	double nAngle = logFont.lfEscapement/10.0;
	double nEscapement = nAngle * 3.1415926 / 180.0;
    stM2.eM11 = FloatToFixed(cos(nEscapement));
	stM2.eM12 = FloatToFixed(sin(nEscapement));
	stM2.eM21 = FloatToFixed(-sin(nEscapement));
	stM2.eM22 = FloatToFixed(cos(nEscapement));
 
 /*		stM2 . eM11 = FloatToFixed (1.0);
	stM2 . eM12 = FloatToFixed (0.0);
	stM2 . eM21 = FloatToFixed (0.0);
	stM2 . eM22 = FloatToFixed (1.0);*/
 
    int nChar = 0; 
    int nSx = ptPos.x;
    int nSy = ptPos.y;
 
	int nFontSpace = 0;
 
	int nCx = 0;
    int nCy = 0;

	COLORREF textColor = GetTextColor(hDC);
 
    for(int i = 0; i < (int)strText.length(); i++)
    {
		//Get character
#if defined(_UNICODE) || defined(UNICODE)
		nChar = strText.at(i);
#else
        if ( strText.at(i) >= 0)
		{
			nChar = strText.at(i);
		}
        else
        {
			// 宽字节
            int th = strText.at(i);
            int tl = strText.at(i+1);
            nChar = (( th & 0x00ff)<<8) + ( tl & 0x00ff);
            i++;
        }
#endif
 
		// 得到字体轮廓信息的尺寸
		dwSize = GetGlyphOutline(hDC, nChar, GGO_BITMAP, &stGM, 0L, NULL, &stM2 );
		// 定义缓冲区
		BYTE * pBuffer = new BYTE[dwSize];
		memset ( pBuffer, 0, dwSize );
		// 取得字体轮廓
		GetGlyphOutline(hDC, nChar, GGO_BITMAP, &stGM, dwSize, pBuffer, &stM2 );
		int nStride = dwSize / stGM.gmBlackBoxY;
		
		// 轮廓数据
		OUTLINETEXTMETRIC    stOtm;
		memset( &stOtm , 0, sizeof(stOtm) ); 
		stOtm.otmSize = sizeof(stOtm ); 
		GetOutlineTextMetrics(hDC, sizeof(stOtm ), &stOtm );
 
		//x 的偏移量
		int nXOffset = stGM.gmptGlyphOrigin.x;
		//y 的偏移量    字的顶部 - y 方向原点  都是相对于 baseline 
		int nYOffset = stOtm.otmAscent - stGM.gmptGlyphOrigin.y;

		int x, y, k;
		for (y = 0; y < (int)stGM.gmBlackBoxY; ++y )
		{
			for (x = 0; x < nStride; ++x )
			{
				for (k =0; k < 8; ++k )
				{
					if ( ( pBuffer[y * nStride + x] >> ( 7- k ) ) & 1 )
					{
						int nX = nCx + 8 * x + k + nXOffset;
						int nY = nCy + y + nYOffset;
						
						SetPixel(hDC, nSx + nX , nSy + nY , textColor);
					}
				}
			}
		}
 
		// 设置下一个字符的位置
		nCx += stGM.gmCellIncX ;
        nCx += nFontSpace ;
        nCy += stGM.gmCellIncY ;
		delete []pBuffer ;
    }

    if(bCreate)
	{
		SelectObject(hDC, hOldFont);
		DeleteObject(hFont);
	}
}

FIXED	FloatToFixed( double fval )
{
	long l;
	l = (long)(fval * 65536L);
	return *(FIXED *)&l;
}

COLORREF	MixColor(COLORREF cr1, COLORREF cr2, double rat)
{
	COLORREF cr = RGB( (GetRValue(cr1)+rat*GetRValue(cr2))/(1.0+rat), (GetGValue(cr1)+rat*GetGValue(cr2))/(1.0+rat), (GetBValue(cr1)+rat*GetBValue(cr2))/(1.0+rat));
	return cr;
}

Declare_Namespace_End

