/* powell.f -- translated by f2c (version of 23 April 1993  18:34:30).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif
#include "f2c.h"

/* Table of constant values */

static integer c__1 = 1;

/* Subroutine */ int altmov_(integer *n, integer *npt, doublereal *xpt, 
	doublereal *xopt, doublereal *bmat, doublereal *zmat, integer *ndim, 
	doublereal *sl, doublereal *su, integer *kopt, integer *knew, 
	doublereal *adelt, doublereal *xnew, doublereal *xalt, doublereal *
	alpha, doublereal *cauchy, doublereal *glag, doublereal *hcol, 
	doublereal *w)
{
    /* System generated locals */
    integer xpt_dim1, xpt_offset, bmat_dim1, bmat_offset, zmat_dim1, 
	    zmat_offset, i__1, i__2;
    doublereal d__1, d__2, d__3, d__4;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    static doublereal diff, half;
    static integer ilbd, isbd;
    static doublereal slbd;
    static integer iubd;
    static doublereal vlag, subd, temp;
    static integer ksav;
    static doublereal step, zero, curv;
    static integer i, j, k, iflag;
    static doublereal scale, csave, tempa, tempb, tempd, const_, sumin, ha, 
	    gw, ggfree;
    static integer ibdsav;
    static doublereal dderiv, bigstp, predsq, presav, distsq, stpsav, wfixsq, 
	    wsqsav, one;


/*    The arguments N, NPT, XPT, XOPT, BMAT, ZMAT, NDIM, SL and SU all hav
e*/
/*       the same meanings as the corresponding arguments of BOBYQB. */
/*     KOPT is the index of the optimal interpolation point. */
/*    KNEW is the index of the interpolation point that is going to be mov
ed.*/
/*     ADELT is the current trust region bound. */
/*    XNEW will be set to a suitable new position for the interpolation po
int*/
/*      XPT(KNEW,.). Specifically, it satisfies the SL, SU and trust regio
n*/
/*      bounds and it should provide a large denominator in the next call 
of*/
/*      UPDATE. The step XNEW-XOPT from XOPT is restricted to moves along 
the*/
/*       straight lines through XOPT and another interpolation point. */
/*    XALT also provides a large value of the modulus of the KNEW-th Lagra
nge*/
/*      function subject to the constraints that have been mentioned, its 
main*/
/*      difference from XNEW being that XALT-XOPT is a constrained version
 of*/
/*      the Cauchy step within the trust region. An exception is that XALT
 is*/
/*       not calculated if all components of GLAG (see below) are zero. */
/*     ALPHA will be set to the KNEW-th diagonal element of the H matrix. 
*/
/*    CAUCHY will be set to the square of the KNEW-th Lagrange function at
*/
/*      the step XALT-XOPT from XOPT for the vector XALT that is returned,
*/
/*       except that CAUCHY is set to zero if XALT is not calculated. */
/*     GLAG is a working space vector of length N for the gradient of the 
*/
/*       KNEW-th Lagrange function at XOPT. */
/*    HCOL is a working space vector of length NPT for the second derivati
ve*/
/*       coefficients of the KNEW-th Lagrange function. */
/*     W is a working space vector of length 2N that is going to hold the 
*/
/*      constrained Cauchy step from XOPT of the Lagrange function, follow
ed*/
/*      by the downhill version of XALT when the uphill step is calculated
.*/

/*     Set the first NPT components of W to the leading elements of the */
/*     KNEW-th column of the H matrix. */

    /* Parameter adjustments */
    --w;
    --hcol;
    --glag;
    --xalt;
    --xnew;
    --su;
    --sl;
    zmat_dim1 = *npt;
    zmat_offset = zmat_dim1 + 1;
    zmat -= zmat_offset;
    bmat_dim1 = *ndim;
    bmat_offset = bmat_dim1 + 1;
    bmat -= bmat_offset;
    --xopt;
    xpt_dim1 = *npt;
    xpt_offset = xpt_dim1 + 1;
    xpt -= xpt_offset;

    /* Function Body */
    half = .5;
    one = 1.;
    zero = 0.;
    const_ = one + sqrt(2.);
    i__1 = *npt;
    for (k = 1; k <= i__1; ++k) {
/* L10: */
	hcol[k] = zero;
    }
    i__1 = *npt - *n - 1;
    for (j = 1; j <= i__1; ++j) {
	temp = zmat[*knew + j * zmat_dim1];
	i__2 = *npt;
	for (k = 1; k <= i__2; ++k) {
/* L20: */
	    hcol[k] += temp * zmat[k + j * zmat_dim1];
	}
    }
    *alpha = hcol[*knew];
    ha = half * *alpha;

/*     Calculate the gradient of the KNEW-th Lagrange function at XOPT. */

    i__2 = *n;
    for (i = 1; i <= i__2; ++i) {
/* L30: */
	glag[i] = bmat[*knew + i * bmat_dim1];
    }
    i__2 = *npt;
    for (k = 1; k <= i__2; ++k) {
	temp = zero;
	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
/* L40: */
	    temp += xpt[k + j * xpt_dim1] * xopt[j];
	}
	temp = hcol[k] * temp;
	i__1 = *n;
	for (i = 1; i <= i__1; ++i) {
/* L50: */
	    glag[i] += temp * xpt[k + i * xpt_dim1];
	}
    }

/*    Search for a large denominator along the straight lines through XOPT
*/
/*    and another interpolation point. SLBD and SUBD will be lower and upp
er*/
/*    bounds on the step along each of these lines in turn. PREDSQ will be
*/
/*    set to the square of the predicted denominator for each line. PRESAV
*/
/*     will be set to the largest admissible value of PREDSQ that occurs. 
*/

    presav = zero;
    i__1 = *npt;
    for (k = 1; k <= i__1; ++k) {
	if (k == *kopt) {
	    goto L80;
	}
	dderiv = zero;
	distsq = zero;
	i__2 = *n;
	for (i = 1; i <= i__2; ++i) {
	    temp = xpt[k + i * xpt_dim1] - xopt[i];
	    dderiv += glag[i] * temp;
/* L60: */
	    distsq += temp * temp;
	}
	subd = *adelt / sqrt(distsq);
	slbd = -subd;
	ilbd = 0;
	iubd = 0;
	sumin = min(one,subd);

/*    Revise SLBD and SUBD if necessary because of the bounds in SL an
d SU.*/

	i__2 = *n;
	for (i = 1; i <= i__2; ++i) {
	    temp = xpt[k + i * xpt_dim1] - xopt[i];
	    if (temp > zero) {
		if (slbd * temp < sl[i] - xopt[i]) {
		    slbd = (sl[i] - xopt[i]) / temp;
		    ilbd = -i;
		}
		if (subd * temp > su[i] - xopt[i]) {
/* Computing MAX */
		    d__1 = sumin, d__2 = (su[i] - xopt[i]) / temp;
		    subd = max(d__1,d__2);
		    iubd = i;
		}
	    } else if (temp < zero) {
		if (slbd * temp > su[i] - xopt[i]) {
		    slbd = (su[i] - xopt[i]) / temp;
		    ilbd = i;
		}
		if (subd * temp < sl[i] - xopt[i]) {
/* Computing MAX */
		    d__1 = sumin, d__2 = (sl[i] - xopt[i]) / temp;
		    subd = max(d__1,d__2);
		    iubd = -i;
		}
	    }
/* L70: */
	}

/*    Seek a large modulus of the KNEW-th Lagrange function when the i
ndex*/
/*     of the other interpolation point on the line through XOPT is KN
EW. */

	if (k == *knew) {
	    diff = dderiv - one;
	    step = slbd;
	    vlag = slbd * (dderiv - slbd * diff);
	    isbd = ilbd;
	    temp = subd * (dderiv - subd * diff);
	    if (abs(temp) > abs(vlag)) {
		step = subd;
		vlag = temp;
		isbd = iubd;
	    }
	    tempd = half * dderiv;
	    tempa = tempd - diff * slbd;
	    tempb = tempd - diff * subd;
	    if (tempa * tempb < zero) {
		temp = tempd * tempd / diff;
		if (abs(temp) > abs(vlag)) {
		    step = tempd / diff;
		    vlag = temp;
		    isbd = 0;
		}
	    }

/*    Search along each of the other lines through XOPT and anothe
r point.*/

	} else {
	    step = slbd;
	    vlag = slbd * (one - slbd);
	    isbd = ilbd;
	    temp = subd * (one - subd);
	    if (abs(temp) > abs(vlag)) {
		step = subd;
		vlag = temp;
		isbd = iubd;
	    }
	    if (subd > half) {
		if (abs(vlag) < .25) {
		    step = half;
		    vlag = .25;
		    isbd = 0;
		}
	    }
	    vlag *= dderiv;
	}

/*     Calculate PREDSQ for the current line search and maintain PRESA
V. */

	temp = step * (one - step) * distsq;
	predsq = vlag * vlag * (vlag * vlag + ha * temp * temp);
	if (predsq > presav) {
	    presav = predsq;
	    ksav = k;
	    stpsav = step;
	    ibdsav = isbd;
	}
L80:
	;
    }

/*    Construct XNEW in a way that satisfies the bound constraints exactly
.*/

    i__1 = *n;
    for (i = 1; i <= i__1; ++i) {
	temp = xopt[i] + stpsav * (xpt[ksav + i * xpt_dim1] - xopt[i]);
/* L90: */
/* Computing MAX */
/* Computing MIN */
	d__3 = su[i];
	d__1 = sl[i], d__2 = min(d__3,temp);
	xnew[i] = max(d__1,d__2);
    }
    if (ibdsav < 0) {
	xnew[-ibdsav] = sl[-ibdsav];
    }
    if (ibdsav > 0) {
	xnew[ibdsav] = su[ibdsav];
    }

/*    Prepare for the iterative method that assembles the constrained Cauc
hy*/
/*    step in W. The sum of squares of the fixed components of W is formed
 in*/
/*     WFIXSQ, and the free components of W are set to BIGSTP. */

    bigstp = *adelt + *adelt;
    iflag = 0;
L100:
    wfixsq = zero;
    ggfree = zero;
    i__1 = *n;
    for (i = 1; i <= i__1; ++i) {
	w[i] = zero;
/* Computing MIN */
	d__1 = xopt[i] - sl[i], d__2 = glag[i];
	tempa = min(d__1,d__2);
/* Computing MAX */
	d__1 = xopt[i] - su[i], d__2 = glag[i];
	tempb = max(d__1,d__2);
	if (tempa > zero || tempb < zero) {
	    w[i] = bigstp;
/* Computing 2nd power */
	    d__1 = glag[i];
	    ggfree += d__1 * d__1;
	}
/* L110: */
    }
    if (ggfree == zero) {
	*cauchy = zero;
	goto L200;
    }

/*     Investigate whether more components of W can be fixed. */

L120:
    temp = *adelt * *adelt - wfixsq;
    if (temp > zero) {
	wsqsav = wfixsq;
	step = sqrt(temp / ggfree);
	ggfree = zero;
	i__1 = *n;
	for (i = 1; i <= i__1; ++i) {
	    if (w[i] == bigstp) {
		temp = xopt[i] - step * glag[i];
		if (temp <= sl[i]) {
		    w[i] = sl[i] - xopt[i];
/* Computing 2nd power */
		    d__1 = w[i];
		    wfixsq += d__1 * d__1;
		} else if (temp >= su[i]) {
		    w[i] = su[i] - xopt[i];
/* Computing 2nd power */
		    d__1 = w[i];
		    wfixsq += d__1 * d__1;
		} else {
/* Computing 2nd power */
		    d__1 = glag[i];
		    ggfree += d__1 * d__1;
		}
	    }
/* L130: */
	}
	if (wfixsq > wsqsav && ggfree > zero) {
	    goto L120;
	}
    }

/*     Set the remaining free components of W and all components of XALT, 
*/
/*     except that W may be scaled later. */

    gw = zero;
    i__1 = *n;
    for (i = 1; i <= i__1; ++i) {
	if (w[i] == bigstp) {
	    w[i] = -step * glag[i];
/* Computing MAX */
/* Computing MIN */
	    d__3 = su[i], d__4 = xopt[i] + w[i];
	    d__1 = sl[i], d__2 = min(d__3,d__4);
	    xalt[i] = max(d__1,d__2);
	} else if (w[i] == zero) {
	    xalt[i] = xopt[i];
	} else if (glag[i] > zero) {
	    xalt[i] = sl[i];
	} else {
	    xalt[i] = su[i];
	}
/* L140: */
	gw += glag[i] * w[i];
    }

/*     Set CURV to the curvature of the KNEW-th Lagrange function along W.
 */
/*     Scale W by a factor less than one if that can reduce the modulus of
 */
/*     the Lagrange function at XOPT+W. Set CAUCHY to the final value of 
*/
/*     the square of this function. */

    curv = zero;
    i__1 = *npt;
    for (k = 1; k <= i__1; ++k) {
	temp = zero;
	i__2 = *n;
	for (j = 1; j <= i__2; ++j) {
/* L150: */
	    temp += xpt[k + j * xpt_dim1] * w[j];
	}
/* L160: */
	curv += hcol[k] * temp * temp;
    }
    if (iflag == 1) {
	curv = -curv;
    }
    if (curv > -gw && curv < -const_ * gw) {
	scale = -gw / curv;
	i__1 = *n;
	for (i = 1; i <= i__1; ++i) {
	    temp = xopt[i] + scale * w[i];
/* L170: */
/* Computing MAX */
/* Computing MIN */
	    d__3 = su[i];
	    d__1 = sl[i], d__2 = min(d__3,temp);
	    xalt[i] = max(d__1,d__2);
	}
/* Computing 2nd power */
	d__1 = half * gw * scale;
	*cauchy = d__1 * d__1;
    } else {
/* Computing 2nd power */
	d__1 = gw + half * curv;
	*cauchy = d__1 * d__1;
    }

/*     If IFLAG is zero, then XALT is calculated as before after reversing
 */
/*    the sign of GLAG. Thus two XALT vectors become available. The one th
at*/
/*     is chosen is the one that gives the larger value of CAUCHY. */

    if (iflag == 0) {
	i__1 = *n;
	for (i = 1; i <= i__1; ++i) {
	    glag[i] = -glag[i];
/* L180: */
	    w[*n + i] = xalt[i];
	}
	csave = *cauchy;
	iflag = 1;
	goto L100;
    }
    if (csave > *cauchy) {
	i__1 = *n;
	for (i = 1; i <= i__1; ++i) {
/* L190: */
	    xalt[i] = w[*n + i];
	}
	*cauchy = csave;
    }
L200:
    return 0;
} /* altmov_ */

/* Subroutine */ int bobyqa_(U_fp fcn, integer *n, integer *npt, doublereal *
	x, doublereal *xl, doublereal *xu, doublereal *rhobeg, doublereal *
	rhoend, integer *iprint, integer *maxfun, doublereal *w, integer *
	iflag)
{
    /* Format strings */
    static char fmt_10[] = "(/4x,\002Return from BOBYQA because NPT is not\
 in\002,\002 the required interval\002)";
    static char fmt_20[] = "(/4x,\002Return from BOBYQA because one of th\
e\002,\002 differences XU(I)-XL(I)\002/6x,\002 is less than 2*RHOBEG.\002)";

    /* System generated locals */
    integer i__1;
    doublereal d__1, d__2;

    /* Builtin functions */
    integer s_wsfe(cilist *), e_wsfe();

    /* Local variables */
    static integer ndim;
    static doublereal temp, zero;
    static integer j, ibmat, izmat, id, np, iw;
    extern /* Subroutine */ int bobyqb_(U_fp, integer *, integer *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, integer *, integer *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, integer *, doublereal *,
	     doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, integer *);
    static integer igo, ihq, ixb, ixa, ifv, isl, jsl, ipq, ivl, ixn, ixo, ixp,
	     isu, jsu;

    /* Fortran I/O blocks */
    static cilist io___39 = { 0, 6, 0, fmt_10, 0 };
    static cilist io___60 = { 0, 6, 0, fmt_20, 0 };



/*    This subroutine seeks the least value of a function of many variable
s,*/
/*     by applying a trust region method that forms quadratic models by */
/*     interpolation. There is usually some freedom in the interpolation 
*/
/*     conditions, which is taken up by minimizing the Frobenius norm of 
*/
/*    the change to the second derivative of the model, beginning with the
*/
/*    zero matrix. The values of the variables are constrained by upper an
d*/
/*     lower bounds. The arguments of the subroutine are as follows. */

/*     N must be set to the number of variables and must be at least two. 
*/
/*     NPT is the number of interpolation conditions. Its value must be in
 */
/*      the interval [N+2,(N+1)(N+2)/2]. Choices that exceed 2*N+1 are not
*/
/*       recommended. */
/*    Initial values of the variables must be set in X(1),X(2),...,X(N). T
hey*/
/*       will be changed to the values that give the least calculated F. 
*/
/*     For I=1,2,...,N, XL(I) and XU(I) must provide the lower and upper 
*/
/*      bounds, respectively, on X(I). The construction of quadratic model
s*/
/*      requires XL(I) to be strictly less than XU(I) for each I. Further,
*/
/*       the contribution to a model from changes to the I-th variable is 
*/
/*       damaged severely by rounding errors if XU(I)-XL(I) is too small. 
*/
/*    RHOBEG and RHOEND must be set to the initial and final values of a t
rust*/
/*      region radius, so both must be positive with RHOEND no greater tha
n*/
/*      RHOBEG. Typically, RHOBEG should be about one tenth of the greates
t*/
/*       expected change to a variable, while RHOEND should indicate the 
*/
/*      accuracy that is required in the final values of the variables. An
*/
/*      error return occurs if any of the differences XU(I)-XL(I), I=1,...
,N,*/
/*       is less than 2*RHOBEG. */
/*    The value of IPRINT should be set to 0, 1, 2 or 3, which controls th
e*/
/*      amount of printing. Specifically, there is no output if IPRINT=0 a
nd*/
/*      there is output only at the return if IPRINT=1. Otherwise, each ne
w*/
/*      value of RHO is printed, with the best vector of variables so far 
and*/
/*      the corresponding value of the objective function. Further, each n
ew*/
/*       value of F with its variables are output if IPRINT=3. */
/*    MAXFUN must be set to an upper bound on the number of calls of CALFU
N.*/
/*    The array W will be used for working space. Its length must be at le
ast*/
/*       (NPT+5)*(NPT+N)+3*N*(N+5)/2. */

/*    SUBROUTINE CALFUN (N,X,F) has to be provided by the user. It must se
t*/
/*    F to the value of the objective function for the current values of t
he*/
/*    variables X(1),X(2),...,X(N), which are generated automatically in a
*/
/*     way that satisfies the bounds given in XL and XU. */

/*     Return if the value of NPT is unacceptable. */

    /* Parameter adjustments */
    --w;
    --xu;
    --xl;
    --x;

    /* Function Body */
    np = *n + 1;
    if (*npt < *n + 2 || *npt > (*n + 2) * np / 2) {
	*iflag = 1;
	s_wsfe(&io___39);
	e_wsfe();
	goto L40;
    }

/*    Partition the working space array, so that different parts of it can
*/
/*    be treated separately during the calculation of BOBYQB. The partitio
n*/
/*    requires the first (NPT+2)*(NPT+N)+3*N*(N+5)/2 elements of W plus th
e*/
/*    space that is taken by the last array in the argument list of BOBYQB
.*/

    ndim = *npt + *n;
    ixb = 1;
    ixp = ixb + *n;
    ifv = ixp + *n * *npt;
    ixo = ifv + *npt;
    igo = ixo + *n;
    ihq = igo + *n;
    ipq = ihq + *n * np / 2;
    ibmat = ipq + *npt;
    izmat = ibmat + ndim * *n;
    isl = izmat + *npt * (*npt - np);
    isu = isl + *n;
    ixn = isu + *n;
    ixa = ixn + *n;
    id = ixa + *n;
    ivl = id + *n;
    iw = ivl + ndim;

/*    Return if there is insufficient space between the bounds. Modify the
*/
/*    initial X if necessary in order to avoid conflicts between the bound
s*/
/*    and the construction of the first quadratic model. The lower and upp
er*/
/*     bounds on moves from the updated X are set now, in the ISL and ISU 
*/
/*    partitions of W, in order to provide useful and exact information ab
out*/
/*    components of X that become within distance RHOBEG from their bounds
.*/

    zero = 0.;
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	temp = xu[j] - xl[j];
	if (temp < *rhobeg + *rhobeg) {
	    *iflag = 2;
	    s_wsfe(&io___60);
	    e_wsfe();
	    goto L40;
	}
	jsl = isl + j - 1;
	jsu = jsl + *n;
	w[jsl] = xl[j] - x[j];
	w[jsu] = xu[j] - x[j];
	if (w[jsl] >= -(*rhobeg)) {
	    if (w[jsl] >= zero) {
		x[j] = xl[j];
		w[jsl] = zero;
		w[jsu] = temp;
	    } else {
		x[j] = xl[j] + *rhobeg;
		w[jsl] = -(*rhobeg);
/* Computing MAX */
		d__1 = xu[j] - x[j];
		w[jsu] = max(d__1,*rhobeg);
	    }
	} else if (w[jsu] <= *rhobeg) {
	    if (w[jsu] <= zero) {
		x[j] = xu[j];
		w[jsl] = -temp;
		w[jsu] = zero;
	    } else {
		x[j] = xu[j] - *rhobeg;
/* Computing MIN */
		d__1 = xl[j] - x[j], d__2 = -(*rhobeg);
		w[jsl] = min(d__1,d__2);
		w[jsu] = *rhobeg;
	    }
	}
/* L30: */
    }

/*     Make the call of BOBYQB. */

    bobyqb_((U_fp)fcn, n, npt, &x[1], &xl[1], &xu[1], rhobeg, rhoend, iprint, 
	    maxfun, &w[ixb], &w[ixp], &w[ifv], &w[ixo], &w[igo], &w[ihq], &w[
	    ipq], &w[ibmat], &w[izmat], &ndim, &w[isl], &w[isu], &w[ixn], &w[
	    ixa], &w[id], &w[ivl], &w[iw], iflag);
L40:
    return 0;
} /* bobyqa_ */

/* Subroutine */ int bobyqb_(S_fp fcn, integer *n, integer *npt, doublereal *
	x, doublereal *xl, doublereal *xu, doublereal *rhobeg, doublereal *
	rhoend, integer *iprint, integer *maxfun, doublereal *xbase, 
	doublereal *xpt, doublereal *fval, doublereal *xopt, doublereal *gopt,
	 doublereal *hq, doublereal *pq, doublereal *bmat, doublereal *zmat, 
	integer *ndim, doublereal *sl, doublereal *su, doublereal *xnew, 
	doublereal *xalt, doublereal *d, doublereal *vlag, doublereal *w, 
	integer *iflag)
{
    /* Format strings */
    static char fmt_390[] = "(/4x,\002Return from BOBYQA because FCN has b\
een\002,\002 called MAXFUN times.\002)";
    static char fmt_320[] = "(/5x,\002Return from BOBYQA because of muc\
h\002,\002 cancellation in a denominator.\002)";
    static char fmt_400[] = "(/4x,\002Function number\002,i6,\002    F =\002\
,1pd18.10,\002    The corresponding X is:\002/(2x,5d15.6))";
    static char fmt_430[] = "(/4x,\002Return from BOBYQA because a trus\
t\002,\002 region step has failed to reduce Q.\002)";
    static char fmt_690[] = "(5x)";
    static char fmt_700[] = "(/4x,\002New RHO =\002,1pd11.4,5x,\002Number o\
f\002,\002 function values =\002,i6)";
    static char fmt_710[] = "(4x,\002Least value of F =\002,1pd23.15,9x,\002\
The corresponding X is:\002/(2x,5d15.6))";
    static char fmt_740[] = "(/4x,\002At the return from BOBYQA\002,5x,\002N\
umber of function values =\002,i6)";

    /* System generated locals */
    integer xpt_dim1, xpt_offset, bmat_dim1, bmat_offset, zmat_dim1, 
	    zmat_offset, i__1, i__2, i__3;
    doublereal d__1, d__2, d__3, d__4;

    /* Builtin functions */
    integer s_wsfe(cilist *), e_wsfe();
    double sqrt(doublereal);
    integer do_fio(integer *, char *, ftnlen);

    /* Local variables */
    static doublereal diff, half, beta, gisq;
    static integer knew;
    static doublereal temp, suma, sumb, bsum, fopt;
    static integer kopt, nptm;
    static doublereal zero, curv;
    static integer ksav;
    static doublereal gqsq, dist, sumw, sumz, f;
    static integer i;
    static doublereal diffa, diffb;
    static integer j, k;
    static doublereal diffc, hdiag;
    static integer kbase;
    static doublereal alpha, delta, adelt, denom, fsave, bdtol, delsq;
    static integer nresc, nfsav;
    static doublereal ratio, dnorm, vquad, pqold, tenth;
    static integer itest;
    static doublereal sumpq;
    static integer ih, nf, jj, nh, ip, jp;
    static doublereal dx, scaden;
    static integer np;
    static doublereal errbig, cauchy, fracsq, biglsq, densav;
    extern /* Subroutine */ int update_(integer *, integer *, doublereal *, 
	    doublereal *, integer *, doublereal *, doublereal *, doublereal *,
	     integer *, doublereal *);
    static doublereal bdtest;
    extern /* Subroutine */ int rescue_(S_fp, integer *, integer *, 
	    doublereal *, doublereal *, integer *, integer *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, integer *,
	     doublereal *, doublereal *, integer *, doublereal *, integer *, 
	    doublereal *, doublereal *, doublereal *, doublereal *), prelim_(
	    S_fp, integer *, integer *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, integer *, integer *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, integer *, doublereal *,
	     doublereal *, integer *, integer *);
    static doublereal crvmin, frhosq;
    extern /* Subroutine */ int altmov_(integer *, integer *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, integer *, doublereal *,
	     doublereal *, integer *, integer *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *);
    static doublereal distsq;
    extern /* Subroutine */ int trsbox_(integer *, integer *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *);
    static integer ntrits;
    static doublereal xoptsq, den, one, ten, dsq, rho, sum, two;

    /* Fortran I/O blocks */
    static cilist io___77 = { 0, 6, 0, fmt_390, 0 };
    static cilist io___108 = { 0, 6, 0, fmt_390, 0 };
    static cilist io___120 = { 0, 6, 0, fmt_320, 0 };
    static cilist io___126 = { 0, 6, 0, fmt_320, 0 };
    static cilist io___127 = { 0, 6, 0, fmt_390, 0 };
    static cilist io___129 = { 0, 6, 0, fmt_400, 0 };
    static cilist io___133 = { 0, 6, 0, fmt_430, 0 };
    static cilist io___141 = { 0, 6, 0, fmt_690, 0 };
    static cilist io___142 = { 0, 6, 0, fmt_700, 0 };
    static cilist io___143 = { 0, 6, 0, fmt_710, 0 };
    static cilist io___144 = { 0, 6, 0, fmt_740, 0 };
    static cilist io___145 = { 0, 6, 0, fmt_710, 0 };



/*     The arguments N, NPT, X, XL, XU, RHOBEG, RHOEND, IPRINT and MAXFUN 
*/
/*      are identical to the corresponding arguments in SUBROUTINE BOBYQA.
*/
/*     XBASE holds a shift of origin that should reduce the contributions 
*/
/*      from rounding errors to values of the model and Lagrange functions
.*/
/*     XPT is a two-dimensional array that holds the coordinates of the */
/*       interpolation points relative to XBASE. */
/*     FVAL holds the values of F at the interpolation points. */
/*    XOPT is set to the displacement from XBASE of the trust region centr
e.*/
/*     GOPT holds the gradient of the quadratic model at XBASE+XOPT. */
/*     HQ holds the explicit second derivatives of the quadratic model. */
/*    PQ contains the parameters of the implicit second derivatives of the
*/
/*       quadratic model. */
/*     BMAT holds the last N columns of H. */
/*    ZMAT holds the factorization of the leading NPT by NPT submatrix of 
H,*/
/*      this factorization being ZMAT times ZMAT^T, which provides both th
e*/
/*       correct rank and positive semi-definiteness. */
/*     NDIM is the first dimension of BMAT and has the value NPT+N. */
/*     SL and SU hold the differences XL-XBASE and XU-XBASE, respectively.
 */
/*       All the components of every XOPT are going to satisfy the bounds 
*/
/*       SL(I) .LEQ. XOPT(I) .LEQ. SU(I), with appropriate equalities when
 */
/*       XOPT is on a constraint boundary. */
/*    XNEW is chosen by SUBROUTINE TRSBOX or ALTMOV. Usually XBASE+XNEW is
 the*/
/*       vector of variables for the next call of FCN. XNEW also satisfies
 */
/*      the SL and SU constraints in the way that has just been mentioned.
*/
/*    XALT is an alternative to XNEW, chosen by ALTMOV, that may replace X
NEW*/
/*       in order to increase the denominator in the updating of UPDATE. 
*/
/*    D is reserved for a trial step from XOPT, which is usually XNEW-XOPT
.*/
/*    VLAG contains the values of the Lagrange functions at a new point X.
*/
/*      They are part of a product that requires VLAG to be of length NDIM
.*/
/*    W is a one-dimensional array that is used for working space. Its len
gth*/
/*       must be at least 3*NDIM = 3*(NPT+N). */

/*     Set some constants. */

    /* Parameter adjustments */
    --w;
    --vlag;
    --d;
    --xalt;
    --xnew;
    --su;
    --sl;
    zmat_dim1 = *npt;
    zmat_offset = zmat_dim1 + 1;
    zmat -= zmat_offset;
    bmat_dim1 = *ndim;
    bmat_offset = bmat_dim1 + 1;
    bmat -= bmat_offset;
    --pq;
    --hq;
    --gopt;
    --xopt;
    --fval;
    xpt_dim1 = *npt;
    xpt_offset = xpt_dim1 + 1;
    xpt -= xpt_offset;
    --xbase;
    --xu;
    --xl;
    --x;

    /* Function Body */
    half = .5;
    one = 1.;
    ten = 10.;
    tenth = .1;
    two = 2.;
    zero = 0.;
    np = *n + 1;
    nptm = *npt - np;
    nh = *n * np / 2;

/*       Set the flag */
    *iflag = 0;
/*    The call of PRELIM sets the elements of XBASE, XPT, FVAL, GOPT, HQ, 
PQ,*/
/*    BMAT and ZMAT for the first iteration, with the corresponding values
 of*/
/*     of NF and KOPT, which are the number of calls of FCN so far and the
 */
/*    index of the interpolation point at the trust region centre. Then th
e*/
/*    initial XOPT is set too. The branch to label 720 occurs if MAXFUN is
*/
/*    less than NPT. GOPT will be updated if KOPT is different from KBASE.
*/

    prelim_((S_fp)fcn, n, npt, &x[1], &xl[1], &xu[1], rhobeg, iprint, maxfun, 
	    &xbase[1], &xpt[xpt_offset], &fval[1], &gopt[1], &hq[1], &pq[1], &
	    bmat[bmat_offset], &zmat[zmat_offset], ndim, &sl[1], &su[1], &nf, 
	    &kopt);
    xoptsq = zero;
    i__1 = *n;
    for (i = 1; i <= i__1; ++i) {
	xopt[i] = xpt[kopt + i * xpt_dim1];
/* L10: */
/* Computing 2nd power */
	d__1 = xopt[i];
	xoptsq += d__1 * d__1;
    }
    fsave = fval[1];
    if (nf < *npt) {
	*iflag = 3;
	if (*iprint > 0) {
	    s_wsfe(&io___77);
	    e_wsfe();
	}
	goto L720;
    }
    kbase = 1;

/*    Complete the settings that are required for the iterative procedure.
*/

    rho = *rhobeg;
    delta = rho;
    nresc = nf;
    ntrits = 0;
    diffa = zero;
    diffb = zero;
    itest = 0;
    nfsav = nf;

/*     Update GOPT if necessary before the first iteration and after each 
*/
/*     call of RESCUE that makes a call of FCN. */

L20:
    if (kopt != kbase) {
	ih = 0;
	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
	    i__2 = j;
	    for (i = 1; i <= i__2; ++i) {
		++ih;
		if (i < j) {
		    gopt[j] += hq[ih] * xopt[i];
		}
/* L30: */
		gopt[i] += hq[ih] * xopt[j];
	    }
	}
	if (nf > *npt) {
	    i__2 = *npt;
	    for (k = 1; k <= i__2; ++k) {
		temp = zero;
		i__1 = *n;
		for (j = 1; j <= i__1; ++j) {
/* L40: */
		    temp += xpt[k + j * xpt_dim1] * xopt[j];
		}
		temp = pq[k] * temp;
		i__1 = *n;
		for (i = 1; i <= i__1; ++i) {
/* L50: */
		    gopt[i] += temp * xpt[k + i * xpt_dim1];
		}
	    }
	}
    }

/*    Generate the next point in the trust region that provides a small va
lue*/
/*     of the quadratic model subject to the constraints on the variables.
 */
/*    The integer NTRITS is set to the number "trust region" iterations th
at*/
/*     have occurred since the last "alternative" iteration. If the length
 */
/*    of XNEW-XOPT is less than HALF*RHO, however, then there is a branch 
to*/
/*     label 650 or 680 with NTRITS=-1, instead of calculating F at XNEW. 
*/

L60:
    trsbox_(n, npt, &xpt[xpt_offset], &xopt[1], &gopt[1], &hq[1], &pq[1], &sl[
	    1], &su[1], &delta, &xnew[1], &d[1], &w[1], &w[np], &w[np + *n], &
	    w[np + (*n << 1)], &w[np + *n * 3], &dsq, &crvmin);
/* Computing MIN */
    d__1 = delta, d__2 = sqrt(dsq);
    dnorm = min(d__1,d__2);
    if (dnorm < half * rho) {
	ntrits = -1;
/* Computing 2nd power */
	d__1 = ten * rho;
	distsq = d__1 * d__1;
	if (nf <= nfsav + 2) {
	    goto L650;
	}

/*    The following choice between labels 650 and 680 depends on wheth
er or*/
/*    not our work with the current RHO seems to be complete. Either R
HO is*/
/*    decreased or termination occurs if the errors in the quadratic m
odel at*/
/*    the last three interpolation points compare favourably with pred
ictions*/
/*    of likely improvements to the model within distance HALF*RHO of 
XOPT.*/

/* Computing MAX */
	d__1 = max(diffa,diffb);
	errbig = max(d__1,diffc);
	frhosq = rho * .125 * rho;
	if (crvmin > zero && errbig > frhosq * crvmin) {
	    goto L650;
	}
	bdtol = errbig / rho;
	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
	    bdtest = bdtol;
	    if (xnew[j] == sl[j]) {
		bdtest = w[j];
	    }
	    if (xnew[j] == su[j]) {
		bdtest = -w[j];
	    }
	    if (bdtest < bdtol) {
		curv = hq[(j + j * j) / 2];
		i__2 = *npt;
		for (k = 1; k <= i__2; ++k) {
/* L70: */
/* Computing 2nd power */
		    d__1 = xpt[k + j * xpt_dim1];
		    curv += pq[k] * (d__1 * d__1);
		}
		bdtest += half * curv * rho;
		if (bdtest < bdtol) {
		    goto L650;
		}
	    }
/* L80: */
	}
	goto L680;
    }
    ++ntrits;

/*    Severe cancellation is likely to occur if XOPT is too far from XBASE
.*/
/*    If the following test holds, then XBASE is shifted so that XOPT beco
mes*/
/*     zero. The appropriate changes are made to BMAT and to the second */
/*    derivatives of the current model, beginning with the changes to BMAT
*/
/*    that do not depend on ZMAT. VLAG is used temporarily for working spa
ce.*/

L90:
    if (dsq <= xoptsq * .001) {
	fracsq = xoptsq * .25;
	sumpq = zero;
	i__1 = *npt;
	for (k = 1; k <= i__1; ++k) {
	    sumpq += pq[k];
	    sum = -half * xoptsq;
	    i__2 = *n;
	    for (i = 1; i <= i__2; ++i) {
/* L100: */
		sum += xpt[k + i * xpt_dim1] * xopt[i];
	    }
	    w[*npt + k] = sum;
	    temp = fracsq - half * sum;
	    i__2 = *n;
	    for (i = 1; i <= i__2; ++i) {
		w[i] = bmat[k + i * bmat_dim1];
		vlag[i] = sum * xpt[k + i * xpt_dim1] + temp * xopt[i];
		ip = *npt + i;
		i__3 = i;
		for (j = 1; j <= i__3; ++j) {
/* L110: */
		    bmat[ip + j * bmat_dim1] = bmat[ip + j * bmat_dim1] + w[i]
			     * vlag[j] + vlag[i] * w[j];
		}
	    }
	}

/*     Then the revisions of BMAT that depend on ZMAT are calculated. 
*/

	i__3 = nptm;
	for (jj = 1; jj <= i__3; ++jj) {
	    sumz = zero;
	    sumw = zero;
	    i__2 = *npt;
	    for (k = 1; k <= i__2; ++k) {
		sumz += zmat[k + jj * zmat_dim1];
		vlag[k] = w[*npt + k] * zmat[k + jj * zmat_dim1];
/* L120: */
		sumw += vlag[k];
	    }
	    i__2 = *n;
	    for (j = 1; j <= i__2; ++j) {
		sum = (fracsq * sumz - half * sumw) * xopt[j];
		i__1 = *npt;
		for (k = 1; k <= i__1; ++k) {
/* L130: */
		    sum += vlag[k] * xpt[k + j * xpt_dim1];
		}
		w[j] = sum;
		i__1 = *npt;
		for (k = 1; k <= i__1; ++k) {
/* L140: */
		    bmat[k + j * bmat_dim1] += sum * zmat[k + jj * zmat_dim1];
		}
	    }
	    i__1 = *n;
	    for (i = 1; i <= i__1; ++i) {
		ip = i + *npt;
		temp = w[i];
		i__2 = i;
		for (j = 1; j <= i__2; ++j) {
/* L150: */
		    bmat[ip + j * bmat_dim1] += temp * w[j];
		}
	    }
	}

/*    The following instructions complete the shift, including the cha
nges*/
/*     to the second derivative parameters of the quadratic model. */

	ih = 0;
	i__2 = *n;
	for (j = 1; j <= i__2; ++j) {
	    w[j] = -half * sumpq * xopt[j];
	    i__1 = *npt;
	    for (k = 1; k <= i__1; ++k) {
		w[j] += pq[k] * xpt[k + j * xpt_dim1];
/* L160: */
		xpt[k + j * xpt_dim1] -= xopt[j];
	    }
	    i__1 = j;
	    for (i = 1; i <= i__1; ++i) {
		++ih;
		hq[ih] = hq[ih] + w[i] * xopt[j] + xopt[i] * w[j];
/* L170: */
		bmat[*npt + i + j * bmat_dim1] = bmat[*npt + j + i * 
			bmat_dim1];
	    }
	}
	i__1 = *n;
	for (i = 1; i <= i__1; ++i) {
	    xbase[i] += xopt[i];
	    xnew[i] -= xopt[i];
	    sl[i] -= xopt[i];
	    su[i] -= xopt[i];
/* L180: */
	    xopt[i] = zero;
	}
	xoptsq = zero;
    }
    if (ntrits == 0) {
	goto L210;
    }
    goto L230;

/*    XBASE is also moved to XOPT by a call of RESCUE. This calculation is
*/
/*    more expensive than the previous shift, because new matrices BMAT an
d*/
/*    ZMAT are generated from scratch, which may include the replacement o
f*/
/*     interpolation points whose positions seem to be causing near linear
 */
/*    dependence in the interpolation conditions. Therefore RESCUE is call
ed*/
/*    only if rounding errors have reduced by at least a factor of two the
*/
/*     denominator of the formula for updating the H matrix. It provides a
 */
/*    useful safeguard, but is not invoked in most applications of BOBYQA.
*/

L190:
    nfsav = nf;
    kbase = kopt;
    rescue_((S_fp)fcn, n, npt, &xl[1], &xu[1], iprint, maxfun, &xbase[1], &
	    xpt[xpt_offset], &fval[1], &xopt[1], &gopt[1], &hq[1], &pq[1], &
	    bmat[bmat_offset], &zmat[zmat_offset], ndim, &sl[1], &su[1], &nf, 
	    &delta, &kopt, &vlag[1], &w[1], &w[*n + np], &w[*ndim + np]);

/*     XOPT is updated now in case the branch below to label 720 is taken.
 */
/*    Any updating of GOPT occurs after the branch below to label 20, whic
h*/
/*     leads to a trust region iteration as does the branch to label 60. 
*/

    xoptsq = zero;
    if (kopt != kbase) {
	i__1 = *n;
	for (i = 1; i <= i__1; ++i) {
	    xopt[i] = xpt[kopt + i * xpt_dim1];
/* L200: */
/* Computing 2nd power */
	    d__1 = xopt[i];
	    xoptsq += d__1 * d__1;
	}
    }
    if (nf < 0) {
	nf = *maxfun;
	*iflag = 3;
	if (*iprint > 0) {
	    s_wsfe(&io___108);
	    e_wsfe();
	}
	goto L720;
    }
    nresc = nf;
    if (nfsav < nf) {
	nfsav = nf;
	goto L20;
    }
    if (ntrits > 0) {
	goto L60;
    }

/*     Pick two alternative vectors of variables, relative to XBASE, that 
*/
/*     are suitable as new positions of the KNEW-th interpolation point. 
*/
/*    Firstly, XNEW is set to the point on a line through XOPT and another
*/
/*     interpolation point that minimizes the predicted value of the next 
*/
/*     denominator, subject to ||XNEW - XOPT|| .LEQ. ADELT and to the SL 
*/
/*     and SU bounds. Secondly, XALT is set to the best feasible point on 
*/
/*     a constrained version of the Cauchy step of the KNEW-th Lagrange */
/*     function, the corresponding value of the square of this function */
/*     being returned in CAUCHY. The choice between these alternatives is 
*/
/*     going to be made when the denominator is calculated. */

L210:
    altmov_(n, npt, &xpt[xpt_offset], &xopt[1], &bmat[bmat_offset], &zmat[
	    zmat_offset], ndim, &sl[1], &su[1], &kopt, &knew, &adelt, &xnew[1]
	    , &xalt[1], &alpha, &cauchy, &w[1], &w[np], &w[*ndim + 1]);
    i__1 = *n;
    for (i = 1; i <= i__1; ++i) {
/* L220: */
	d[i] = xnew[i] - xopt[i];
    }

/*     Calculate VLAG and BETA for the current choice of D. The scalar */
/*     product of D with XPT(K,.) is going to be held in W(NPT+K) for */
/*     use when VQUAD is calculated. */

L230:
    i__1 = *npt;
    for (k = 1; k <= i__1; ++k) {
	suma = zero;
	sumb = zero;
	sum = zero;
	i__2 = *n;
	for (j = 1; j <= i__2; ++j) {
	    suma += xpt[k + j * xpt_dim1] * d[j];
	    sumb += xpt[k + j * xpt_dim1] * xopt[j];
/* L240: */
	    sum += bmat[k + j * bmat_dim1] * d[j];
	}
	w[k] = suma * (half * suma + sumb);
	vlag[k] = sum;
/* L250: */
	w[*npt + k] = suma;
    }
    beta = zero;
    i__1 = nptm;
    for (jj = 1; jj <= i__1; ++jj) {
	sum = zero;
	i__2 = *npt;
	for (k = 1; k <= i__2; ++k) {
/* L260: */
	    sum += zmat[k + jj * zmat_dim1] * w[k];
	}
	beta -= sum * sum;
	i__2 = *npt;
	for (k = 1; k <= i__2; ++k) {
/* L270: */
	    vlag[k] += sum * zmat[k + jj * zmat_dim1];
	}
    }
    dsq = zero;
    bsum = zero;
    dx = zero;
    i__2 = *n;
    for (j = 1; j <= i__2; ++j) {
/* Computing 2nd power */
	d__1 = d[j];
	dsq += d__1 * d__1;
	sum = zero;
	i__1 = *npt;
	for (k = 1; k <= i__1; ++k) {
/* L280: */
	    sum += w[k] * bmat[k + j * bmat_dim1];
	}
	bsum += sum * d[j];
	jp = *npt + j;
	i__1 = *n;
	for (i = 1; i <= i__1; ++i) {
/* L290: */
	    sum += bmat[jp + i * bmat_dim1] * d[i];
	}
	vlag[jp] = sum;
	bsum += sum * d[j];
/* L300: */
	dx += d[j] * xopt[j];
    }
    beta = dx * dx + dsq * (xoptsq + dx + dx + half * dsq) + beta - bsum;
    vlag[kopt] += one;

/*     If NTRITS is zero, the denominator may be increased by replacing */
/*     the step D of ALTMOV by a Cauchy step. Then RESCUE may be called if
 */
/*     rounding errors have damaged the chosen denominator. */

    if (ntrits == 0) {
/* Computing 2nd power */
	d__1 = vlag[knew];
	denom = d__1 * d__1 + alpha * beta;
	if (denom < cauchy && cauchy > zero) {
	    i__2 = *n;
	    for (i = 1; i <= i__2; ++i) {
		xnew[i] = xalt[i];
/* L310: */
		d[i] = xnew[i] - xopt[i];
	    }
	    cauchy = zero;
	    goto L230;
	}
/* Computing 2nd power */
	d__1 = vlag[knew];
	if (denom <= half * (d__1 * d__1)) {
	    if (nf > nresc) {
		goto L190;
	    }
	    *iflag = 4;
	    if (*iprint > 0) {
		s_wsfe(&io___120);
		e_wsfe();
	    }
	    goto L720;
	}

/*     Alternatively, if NTRITS is positive, then set KNEW to the inde
x of */
/*     the next interpolation point to be deleted to make room for a t
rust */
/*    region step. Again RESCUE may be called if rounding errors have 
damaged*/
/*    the chosen denominator, which is the reason for attempting to se
lect*/
/*     KNEW before calculating the next value of the objective functio
n. */

    } else {
	delsq = delta * delta;
	scaden = zero;
	biglsq = zero;
	knew = 0;
	i__2 = *npt;
	for (k = 1; k <= i__2; ++k) {
	    if (k == kopt) {
		goto L350;
	    }
	    hdiag = zero;
	    i__1 = nptm;
	    for (jj = 1; jj <= i__1; ++jj) {
/* L330: */
/* Computing 2nd power */
		d__1 = zmat[k + jj * zmat_dim1];
		hdiag += d__1 * d__1;
	    }
/* Computing 2nd power */
	    d__1 = vlag[k];
	    den = beta * hdiag + d__1 * d__1;
	    distsq = zero;
	    i__1 = *n;
	    for (j = 1; j <= i__1; ++j) {
/* L340: */
/* Computing 2nd power */
		d__1 = xpt[k + j * xpt_dim1] - xopt[j];
		distsq += d__1 * d__1;
	    }
/* Computing MAX */
/* Computing 2nd power */
	    d__3 = distsq / delsq;
	    d__1 = one, d__2 = d__3 * d__3;
	    temp = max(d__1,d__2);
	    if (temp * den > scaden) {
		scaden = temp * den;
		knew = k;
		denom = den;
	    }
/* Computing MAX */
/* Computing 2nd power */
	    d__3 = vlag[k];
	    d__1 = biglsq, d__2 = temp * (d__3 * d__3);
	    biglsq = max(d__1,d__2);
L350:
	    ;
	}
	if (scaden <= half * biglsq) {
	    if (nf > nresc) {
		goto L190;
	    }
	    *iflag = 4;
	    if (*iprint > 0) {
		s_wsfe(&io___126);
		e_wsfe();
	    }
	    goto L720;
	}
    }

/*    Put the variables for the next calculation of the objective function
*/
/*       in XNEW, with any adjustments for the bounds. */


/*     Calculate the value of the objective function at XBASE+XNEW, unless
 */
/*       the limit on the number of calculations of F has been reached. */

L360:
    i__2 = *n;
    for (i = 1; i <= i__2; ++i) {
/* Computing MIN */
/* Computing MAX */
	d__3 = xl[i], d__4 = xbase[i] + xnew[i];
	d__1 = max(d__3,d__4), d__2 = xu[i];
	x[i] = min(d__1,d__2);
	if (xnew[i] == sl[i]) {
	    x[i] = xl[i];
	}
	if (xnew[i] == su[i]) {
	    x[i] = xu[i];
	}
/* L380: */
    }
    if (nf >= *maxfun) {
	*iflag = 3;
	if (*iprint > 0) {
	    s_wsfe(&io___127);
	    e_wsfe();
	}
	goto L720;
    }
    ++nf;
    (*fcn)(n, &x[1], &f);
    if (*iprint == 3) {
	s_wsfe(&io___129);
	do_fio(&c__1, (char *)&nf, (ftnlen)sizeof(integer));
	do_fio(&c__1, (char *)&f, (ftnlen)sizeof(doublereal));
	i__2 = *n;
	for (i = 1; i <= i__2; ++i) {
	    do_fio(&c__1, (char *)&x[i], (ftnlen)sizeof(doublereal));
	}
	e_wsfe();
    }
    if (ntrits == -1) {
	fsave = f;
	goto L720;
    }

/*    Use the quadratic model to predict the change in F due to the step D
,*/
/*       and set DIFF to the error of this prediction. */

    fopt = fval[kopt];
    vquad = zero;
    ih = 0;
    i__2 = *n;
    for (j = 1; j <= i__2; ++j) {
	vquad += d[j] * gopt[j];
	i__1 = j;
	for (i = 1; i <= i__1; ++i) {
	    ++ih;
	    temp = d[i] * d[j];
	    if (i == j) {
		temp = half * temp;
	    }
/* L410: */
	    vquad += hq[ih] * temp;
	}
    }
    i__1 = *npt;
    for (k = 1; k <= i__1; ++k) {
/* L420: */
/* Computing 2nd power */
	d__1 = w[*npt + k];
	vquad += half * pq[k] * (d__1 * d__1);
    }
    diff = f - fopt - vquad;
    diffc = diffb;
    diffb = diffa;
    diffa = abs(diff);
    if (dnorm > rho) {
	nfsav = nf;
    }

/*     Pick the next value of DELTA after a trust region step. */

    if (ntrits > 0) {
	if (vquad >= zero) {
	    *iflag = 5;
	    if (*iprint > 0) {
		s_wsfe(&io___133);
		e_wsfe();
	    }
	    goto L720;
	}
	ratio = (f - fopt) / vquad;
	if (ratio <= tenth) {
/* Computing MIN */
	    d__1 = half * delta;
	    delta = min(d__1,dnorm);
	} else if (ratio <= .7) {
/* Computing MAX */
	    d__1 = half * delta;
	    delta = max(d__1,dnorm);
	} else {
/* Computing MAX */
	    d__1 = half * delta, d__2 = dnorm + dnorm;
	    delta = max(d__1,d__2);
	}
	if (delta <= rho * 1.5) {
	    delta = rho;
	}

/*     Recalculate KNEW and DENOM if the new F is less than FOPT. */

	if (f < fopt) {
	    ksav = knew;
	    densav = denom;
	    delsq = delta * delta;
	    scaden = zero;
	    biglsq = zero;
	    knew = 0;
	    i__1 = *npt;
	    for (k = 1; k <= i__1; ++k) {
		hdiag = zero;
		i__2 = nptm;
		for (jj = 1; jj <= i__2; ++jj) {
/* L440: */
/* Computing 2nd power */
		    d__1 = zmat[k + jj * zmat_dim1];
		    hdiag += d__1 * d__1;
		}
/* Computing 2nd power */
		d__1 = vlag[k];
		den = beta * hdiag + d__1 * d__1;
		distsq = zero;
		i__2 = *n;
		for (j = 1; j <= i__2; ++j) {
/* L450: */
/* Computing 2nd power */
		    d__1 = xpt[k + j * xpt_dim1] - xnew[j];
		    distsq += d__1 * d__1;
		}
/* Computing MAX */
/* Computing 2nd power */
		d__3 = distsq / delsq;
		d__1 = one, d__2 = d__3 * d__3;
		temp = max(d__1,d__2);
		if (temp * den > scaden) {
		    scaden = temp * den;
		    knew = k;
		    denom = den;
		}
/* L460: */
/* Computing MAX */
/* Computing 2nd power */
		d__3 = vlag[k];
		d__1 = biglsq, d__2 = temp * (d__3 * d__3);
		biglsq = max(d__1,d__2);
	    }
	    if (scaden <= half * biglsq) {
		knew = ksav;
		denom = densav;
	    }
	}
    }

/*    Update BMAT and ZMAT, so that the KNEW-th interpolation point can be
*/
/*     moved. Also update the second derivative terms of the model. */

    update_(n, npt, &bmat[bmat_offset], &zmat[zmat_offset], ndim, &vlag[1], &
	    beta, &denom, &knew, &w[1]);
    ih = 0;
    pqold = pq[knew];
    pq[knew] = zero;
    i__1 = *n;
    for (i = 1; i <= i__1; ++i) {
	temp = pqold * xpt[knew + i * xpt_dim1];
	i__2 = i;
	for (j = 1; j <= i__2; ++j) {
	    ++ih;
/* L470: */
	    hq[ih] += temp * xpt[knew + j * xpt_dim1];
	}
    }
    i__2 = nptm;
    for (jj = 1; jj <= i__2; ++jj) {
	temp = diff * zmat[knew + jj * zmat_dim1];
	i__1 = *npt;
	for (k = 1; k <= i__1; ++k) {
/* L480: */
	    pq[k] += temp * zmat[k + jj * zmat_dim1];
	}
    }

/*    Include the new interpolation point, and make the changes to GOPT at
*/
/*    the old XOPT that are caused by the updating of the quadratic model.
*/

    fval[knew] = f;
    i__1 = *n;
    for (i = 1; i <= i__1; ++i) {
	xpt[knew + i * xpt_dim1] = xnew[i];
/* L490: */
	w[i] = bmat[knew + i * bmat_dim1];
    }
    i__1 = *npt;
    for (k = 1; k <= i__1; ++k) {
	suma = zero;
	i__2 = nptm;
	for (jj = 1; jj <= i__2; ++jj) {
/* L500: */
	    suma += zmat[knew + jj * zmat_dim1] * zmat[k + jj * zmat_dim1];
	}
	sumb = zero;
	i__2 = *n;
	for (j = 1; j <= i__2; ++j) {
/* L510: */
	    sumb += xpt[k + j * xpt_dim1] * xopt[j];
	}
	temp = suma * sumb;
	i__2 = *n;
	for (i = 1; i <= i__2; ++i) {
/* L520: */
	    w[i] += temp * xpt[k + i * xpt_dim1];
	}
    }
    i__2 = *n;
    for (i = 1; i <= i__2; ++i) {
/* L530: */
	gopt[i] += diff * w[i];
    }

/*    Update XOPT, GOPT and KOPT if the new calculated F is less than FOPT
.*/

    if (f < fopt) {
	kopt = knew;
	xoptsq = zero;
	ih = 0;
	i__2 = *n;
	for (j = 1; j <= i__2; ++j) {
	    xopt[j] = xnew[j];
/* Computing 2nd power */
	    d__1 = xopt[j];
	    xoptsq += d__1 * d__1;
	    i__1 = j;
	    for (i = 1; i <= i__1; ++i) {
		++ih;
		if (i < j) {
		    gopt[j] += hq[ih] * d[i];
		}
/* L540: */
		gopt[i] += hq[ih] * d[j];
	    }
	}
	i__1 = *npt;
	for (k = 1; k <= i__1; ++k) {
	    temp = zero;
	    i__2 = *n;
	    for (j = 1; j <= i__2; ++j) {
/* L550: */
		temp += xpt[k + j * xpt_dim1] * d[j];
	    }
	    temp = pq[k] * temp;
	    i__2 = *n;
	    for (i = 1; i <= i__2; ++i) {
/* L560: */
		gopt[i] += temp * xpt[k + i * xpt_dim1];
	    }
	}
    }

/*     Calculate the parameters of the least Frobenius norm interpolant to
 */
/*    the current data, the gradient of this interpolant at XOPT being put
*/
/*     into VLAG(NPT+I), I=1,2,...,N. */

    if (ntrits > 0) {
	i__2 = *npt;
	for (k = 1; k <= i__2; ++k) {
	    vlag[k] = fval[k] - fval[kopt];
/* L570: */
	    w[k] = zero;
	}
	i__2 = nptm;
	for (j = 1; j <= i__2; ++j) {
	    sum = zero;
	    i__1 = *npt;
	    for (k = 1; k <= i__1; ++k) {
/* L580: */
		sum += zmat[k + j * zmat_dim1] * vlag[k];
	    }
	    i__1 = *npt;
	    for (k = 1; k <= i__1; ++k) {
/* L590: */
		w[k] += sum * zmat[k + j * zmat_dim1];
	    }
	}
	i__1 = *npt;
	for (k = 1; k <= i__1; ++k) {
	    sum = zero;
	    i__2 = *n;
	    for (j = 1; j <= i__2; ++j) {
/* L600: */
		sum += xpt[k + j * xpt_dim1] * xopt[j];
	    }
	    w[k + *npt] = w[k];
/* L610: */
	    w[k] = sum * w[k];
	}
	gqsq = zero;
	gisq = zero;
	i__1 = *n;
	for (i = 1; i <= i__1; ++i) {
	    sum = zero;
	    i__2 = *npt;
	    for (k = 1; k <= i__2; ++k) {
/* L620: */
		sum = sum + bmat[k + i * bmat_dim1] * vlag[k] + xpt[k + i * 
			xpt_dim1] * w[k];
	    }
	    if (xopt[i] == sl[i]) {
/* Computing MIN */
		d__2 = zero, d__3 = gopt[i];
/* Computing 2nd power */
		d__1 = min(d__2,d__3);
		gqsq += d__1 * d__1;
/* Computing 2nd power */
		d__1 = min(zero,sum);
		gisq += d__1 * d__1;
	    } else if (xopt[i] == su[i]) {
/* Computing MAX */
		d__2 = zero, d__3 = gopt[i];
/* Computing 2nd power */
		d__1 = max(d__2,d__3);
		gqsq += d__1 * d__1;
/* Computing 2nd power */
		d__1 = max(zero,sum);
		gisq += d__1 * d__1;
	    } else {
/* Computing 2nd power */
		d__1 = gopt[i];
		gqsq += d__1 * d__1;
		gisq += sum * sum;
	    }
/* L630: */
	    vlag[*npt + i] = sum;
	}

/*    Test whether to replace the new quadratic model by the least Fro
benius*/
/*     norm interpolant, making the replacement if the test is satisfi
ed. */

	++itest;
	if (gqsq < ten * gisq) {
	    itest = 0;
	}
	if (itest >= 3) {
	    i__1 = max(*npt,nh);
	    for (i = 1; i <= i__1; ++i) {
		if (i <= *n) {
		    gopt[i] = vlag[*npt + i];
		}
		if (i <= *npt) {
		    pq[i] = w[*npt + i];
		}
		if (i <= nh) {
		    hq[i] = zero;
		}
		itest = 0;
/* L640: */
	    }
	}
    }

/*    If a trust region step has provided a sufficient decrease in F, then
*/
/*    branch for another trust region calculation. The case NTRITS=0 occur
s*/
/*    when the new interpolation point was reached by an alternative step.
*/

    if (ntrits == 0) {
	goto L60;
    }
    if (f <= fopt + tenth * vquad) {
	goto L60;
    }

/*    Alternatively, find out if the interpolation points are close enough
*/
/*       to the best point so far. */

/* Computing MAX */
/* Computing 2nd power */
    d__3 = two * delta;
/* Computing 2nd power */
    d__4 = ten * rho;
    d__1 = d__3 * d__3, d__2 = d__4 * d__4;
    distsq = max(d__1,d__2);
L650:
    knew = 0;
    i__1 = *npt;
    for (k = 1; k <= i__1; ++k) {
	sum = zero;
	i__2 = *n;
	for (j = 1; j <= i__2; ++j) {
/* L660: */
/* Computing 2nd power */
	    d__1 = xpt[k + j * xpt_dim1] - xopt[j];
	    sum += d__1 * d__1;
	}
	if (sum > distsq) {
	    knew = k;
	    distsq = sum;
	}
/* L670: */
    }

/*    If KNEW is positive, then ALTMOV finds alternative new positions for
*/
/*    the KNEW-th interpolation point within distance ADELT of XOPT. It is
*/
/*     reached via label 90. Otherwise, there is a branch to label 60 for 
*/
/*     another trust region iteration, unless the calculations with the */
/*     current RHO are complete. */

    if (knew > 0) {
	dist = sqrt(distsq);
	if (ntrits == -1) {
/* Computing MIN */
	    d__1 = tenth * delta, d__2 = half * dist;
	    delta = min(d__1,d__2);
	    if (delta <= rho * 1.5) {
		delta = rho;
	    }
	}
	ntrits = 0;
/* Computing MAX */
/* Computing MIN */
	d__2 = tenth * dist;
	d__1 = min(d__2,delta);
	adelt = max(d__1,rho);
	dsq = adelt * adelt;
	goto L90;
    }
    if (ntrits == -1) {
	goto L680;
    }
    if (ratio > zero) {
	goto L60;
    }
    if (max(delta,dnorm) > rho) {
	goto L60;
    }

/*    The calculations with the current value of RHO are complete. Pick th
e*/
/*       next values of RHO and DELTA. */

L680:
    if (rho > *rhoend) {
	delta = half * rho;
	ratio = rho / *rhoend;
	if (ratio <= 16.) {
	    rho = *rhoend;
	} else if (ratio <= 250.) {
	    rho = sqrt(ratio) * *rhoend;
	} else {
	    rho = tenth * rho;
	}
	delta = max(delta,rho);
	if (*iprint >= 2) {
	    if (*iprint >= 3) {
		s_wsfe(&io___141);
		e_wsfe();
	    }
	    s_wsfe(&io___142);
	    do_fio(&c__1, (char *)&rho, (ftnlen)sizeof(doublereal));
	    do_fio(&c__1, (char *)&nf, (ftnlen)sizeof(integer));
	    e_wsfe();
	    s_wsfe(&io___143);
	    do_fio(&c__1, (char *)&fval[kopt], (ftnlen)sizeof(doublereal));
	    i__1 = *n;
	    for (i = 1; i <= i__1; ++i) {
		d__1 = xbase[i] + xopt[i];
		do_fio(&c__1, (char *)&d__1, (ftnlen)sizeof(doublereal));
	    }
	    e_wsfe();
	}
	ntrits = 0;
	nfsav = nf;
	goto L60;
    }

/*     Return from the calculation, after another Newton-Raphson step, if 
*/
/*       it is too short to have been tried before. */

    if (ntrits == -1) {
	goto L360;
    }
L720:
    if (fval[kopt] <= fsave) {
	i__1 = *n;
	for (i = 1; i <= i__1; ++i) {
/* Computing MIN */
/* Computing MAX */
	    d__3 = xl[i], d__4 = xbase[i] + xopt[i];
	    d__1 = max(d__3,d__4), d__2 = xu[i];
	    x[i] = min(d__1,d__2);
	    if (xopt[i] == sl[i]) {
		x[i] = xl[i];
	    }
	    if (xopt[i] == su[i]) {
		x[i] = xu[i];
	    }
/* L730: */
	}
	f = fval[kopt];
    }
    if (*iprint >= 1) {
	s_wsfe(&io___144);
	do_fio(&c__1, (char *)&nf, (ftnlen)sizeof(integer));
	e_wsfe();
	s_wsfe(&io___145);
	do_fio(&c__1, (char *)&f, (ftnlen)sizeof(doublereal));
	i__1 = *n;
	for (i = 1; i <= i__1; ++i) {
	    do_fio(&c__1, (char *)&x[i], (ftnlen)sizeof(doublereal));
	}
	e_wsfe();
    }
    return 0;
} /* bobyqb_ */

/* Subroutine */ int calfun_(integer *n, doublereal *x, doublereal *f)
{
    /* System generated locals */
    integer i__1, i__2;
    doublereal d__1, d__2;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    static doublereal temp;
    static integer i, j;

    /* Parameter adjustments */
    --x;

    /* Function Body */
    *f = 0.;
    i__1 = *n;
    for (i = 4; i <= i__1; i += 2) {
	i__2 = i - 2;
	for (j = 2; j <= i__2; j += 2) {
/* Computing 2nd power */
	    d__1 = x[i - 1] - x[j - 1];
/* Computing 2nd power */
	    d__2 = x[i] - x[j];
	    temp = d__1 * d__1 + d__2 * d__2;
	    temp = max(temp,1e-6);
/* L10: */
	    *f += 1. / sqrt(temp);
	}
    }
    return 0;
} /* calfun_ */

/* Subroutine */ int prelim_(S_fp fcn, integer *n, integer *npt, doublereal *
	x, doublereal *xl, doublereal *xu, doublereal *rhobeg, integer *
	iprint, integer *maxfun, doublereal *xbase, doublereal *xpt, 
	doublereal *fval, doublereal *gopt, doublereal *hq, doublereal *pq, 
	doublereal *bmat, doublereal *zmat, integer *ndim, doublereal *sl, 
	doublereal *su, integer *nf, integer *kopt)
{
    /* Format strings */
    static char fmt_70[] = "(/4x,\002Function number\002,i6,\002    F =\002,\
1pd18.10,\002    The corresponding X is:\002/(2x,5d15.6))";

    /* System generated locals */
    integer xpt_dim1, xpt_offset, bmat_dim1, bmat_offset, zmat_dim1, 
	    zmat_offset, i__1, i__2;
    doublereal d__1, d__2, d__3, d__4;

    /* Builtin functions */
    integer s_wsfe(cilist *), do_fio(integer *, char *, ftnlen), e_wsfe();
    double sqrt(doublereal);

    /* Local variables */
    static doublereal fbeg, diff, half, temp, zero, f;
    static integer i, j, k;
    static doublereal recip, stepa, stepb;
    static integer itemp;
    static doublereal rhosq;
    static integer ih, np, nfm;
    static doublereal one;
    static integer nfx, ipt, jpt;
    static doublereal two;

    /* Fortran I/O blocks */
    static cilist io___168 = { 0, 6, 0, fmt_70, 0 };



/*     The arguments N, NPT, X, XL, XU, RHOBEG, IPRINT and MAXFUN are the 
*/
/*       same as the corresponding arguments in SUBROUTINE BOBYQA. */
/*     The arguments XBASE, XPT, FVAL, HQ, PQ, BMAT, ZMAT, NDIM, SL and SU
 */
/*      are the same as the corresponding arguments in BOBYQB, the element
s*/
/*       of SL and SU being set in BOBYQA. */
/*    GOPT is usually the gradient of the quadratic model at XOPT+XBASE, b
ut*/
/*      it is set by PRELIM to the gradient of the quadratic model at XBAS
E.*/
/*      If XOPT is nonzero, BOBYQB will change it to its usual value later
.*/
/*     NF is maintaned as the number of calls of FCN so far. */
/*     KOPT will be such that the least calculated value of F so far is at
 */
/*       the point XPT(KOPT,.)+XBASE in the space of the variables. */

/*    SUBROUTINE PRELIM sets the elements of XBASE, XPT, FVAL, GOPT, HQ, P
Q,*/
/*    BMAT and ZMAT for the first iteration, and it maintains the values o
f*/
/*     NF and KOPT. The vector X is also changed by PRELIM. */

/*     Set some constants. */

    /* Parameter adjustments */
    --su;
    --sl;
    zmat_dim1 = *npt;
    zmat_offset = zmat_dim1 + 1;
    zmat -= zmat_offset;
    bmat_dim1 = *ndim;
    bmat_offset = bmat_dim1 + 1;
    bmat -= bmat_offset;
    --pq;
    --hq;
    --gopt;
    --fval;
    xpt_dim1 = *npt;
    xpt_offset = xpt_dim1 + 1;
    xpt -= xpt_offset;
    --xbase;
    --xu;
    --xl;
    --x;

    /* Function Body */
    half = .5;
    one = 1.;
    two = 2.;
    zero = 0.;
    rhosq = *rhobeg * *rhobeg;
    recip = one / rhosq;
    np = *n + 1;

/*     Set XBASE to the initial vector of variables, and set the initial 
*/
/*     elements of XPT, BMAT, HQ, PQ and ZMAT to zero. */

    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	xbase[j] = x[j];
	i__2 = *npt;
	for (k = 1; k <= i__2; ++k) {
/* L10: */
	    xpt[k + j * xpt_dim1] = zero;
	}
	i__2 = *ndim;
	for (i = 1; i <= i__2; ++i) {
/* L20: */
	    bmat[i + j * bmat_dim1] = zero;
	}
    }
    i__2 = *n * np / 2;
    for (ih = 1; ih <= i__2; ++ih) {
/* L30: */
	hq[ih] = zero;
    }
    i__2 = *npt;
    for (k = 1; k <= i__2; ++k) {
	pq[k] = zero;
	i__1 = *npt - np;
	for (j = 1; j <= i__1; ++j) {
/* L40: */
	    zmat[k + j * zmat_dim1] = zero;
	}
    }

/*    Begin the initialization procedure. NF becomes one more than the num
ber*/
/*    of function values so far. The coordinates of the displacement of th
e*/
/*     next initial interpolation point from XBASE are set in XPT(NF+1,.).
 */

    *nf = 0;
L50:
    nfm = *nf;
    nfx = *nf - *n;
    ++(*nf);
    if (nfm <= *n << 1) {
	if (nfm >= 1 && nfm <= *n) {
	    stepa = *rhobeg;
	    if (su[nfm] == zero) {
		stepa = -stepa;
	    }
	    xpt[*nf + nfm * xpt_dim1] = stepa;
	} else if (nfm > *n) {
	    stepa = xpt[*nf - *n + nfx * xpt_dim1];
	    stepb = -(*rhobeg);
	    if (sl[nfx] == zero) {
/* Computing MIN */
		d__1 = two * *rhobeg, d__2 = su[nfx];
		stepb = min(d__1,d__2);
	    }
	    if (su[nfx] == zero) {
/* Computing MAX */
		d__1 = -two * *rhobeg, d__2 = sl[nfx];
		stepb = max(d__1,d__2);
	    }
	    xpt[*nf + nfx * xpt_dim1] = stepb;
	}
    } else {
	itemp = (nfm - np) / *n;
	jpt = nfm - itemp * *n - *n;
	ipt = jpt + itemp;
	if (ipt > *n) {
	    itemp = jpt;
	    jpt = ipt - *n;
	    ipt = itemp;
	}
	xpt[*nf + ipt * xpt_dim1] = xpt[ipt + 1 + ipt * xpt_dim1];
	xpt[*nf + jpt * xpt_dim1] = xpt[jpt + 1 + jpt * xpt_dim1];
    }

/*     Calculate the next value of F. The least function value so far and 
*/
/*     its index are required. */

    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
/* Computing MIN */
/* Computing MAX */
	d__3 = xl[j], d__4 = xbase[j] + xpt[*nf + j * xpt_dim1];
	d__1 = max(d__3,d__4), d__2 = xu[j];
	x[j] = min(d__1,d__2);
	if (xpt[*nf + j * xpt_dim1] == sl[j]) {
	    x[j] = xl[j];
	}
	if (xpt[*nf + j * xpt_dim1] == su[j]) {
	    x[j] = xu[j];
	}
/* L60: */
    }
    (*fcn)(n, &x[1], &f);
    if (*iprint == 3) {
	s_wsfe(&io___168);
	do_fio(&c__1, (char *)&(*nf), (ftnlen)sizeof(integer));
	do_fio(&c__1, (char *)&f, (ftnlen)sizeof(doublereal));
	i__1 = *n;
	for (i = 1; i <= i__1; ++i) {
	    do_fio(&c__1, (char *)&x[i], (ftnlen)sizeof(doublereal));
	}
	e_wsfe();
    }
    fval[*nf] = f;
    if (*nf == 1) {
	fbeg = f;
	*kopt = 1;
    } else if (f < fval[*kopt]) {
	*kopt = *nf;
    }

/*    Set the nonzero initial elements of BMAT and the quadratic model in 
the*/
/*    cases when NF is at most 2*N+1. If NF exceeds N+1, then the position
s*/
/*     of the NF-th and (NF-N)-th interpolation points may be switched, in
 */
/*    order that the function value at the first of them contributes to th
e*/
/*    off-diagonal second derivative terms of the initial quadratic model.
*/

    if (*nf <= (*n << 1) + 1) {
	if (*nf >= 2 && *nf <= *n + 1) {
	    gopt[nfm] = (f - fbeg) / stepa;
	    if (*npt < *nf + *n) {
		bmat[nfm * bmat_dim1 + 1] = -one / stepa;
		bmat[*nf + nfm * bmat_dim1] = one / stepa;
		bmat[*npt + nfm + nfm * bmat_dim1] = -half * rhosq;
	    }
	} else if (*nf >= *n + 2) {
	    ih = nfx * (nfx + 1) / 2;
	    temp = (f - fbeg) / stepb;
	    diff = stepb - stepa;
	    hq[ih] = two * (temp - gopt[nfx]) / diff;
	    gopt[nfx] = (gopt[nfx] * stepb - temp * stepa) / diff;
	    if (stepa * stepb < zero) {
		if (f < fval[*nf - *n]) {
		    fval[*nf] = fval[*nf - *n];
		    fval[*nf - *n] = f;
		    if (*kopt == *nf) {
			*kopt = *nf - *n;
		    }
		    xpt[*nf - *n + nfx * xpt_dim1] = stepb;
		    xpt[*nf + nfx * xpt_dim1] = stepa;
		}
	    }
	    bmat[nfx * bmat_dim1 + 1] = -(stepa + stepb) / (stepa * stepb);
	    bmat[*nf + nfx * bmat_dim1] = -half / xpt[*nf - *n + nfx * 
		    xpt_dim1];
	    bmat[*nf - *n + nfx * bmat_dim1] = -bmat[nfx * bmat_dim1 + 1] - 
		    bmat[*nf + nfx * bmat_dim1];
	    zmat[nfx * zmat_dim1 + 1] = sqrt(two) / (stepa * stepb);
	    zmat[*nf + nfx * zmat_dim1] = sqrt(half) / rhosq;
	    zmat[*nf - *n + nfx * zmat_dim1] = -zmat[nfx * zmat_dim1 + 1] - 
		    zmat[*nf + nfx * zmat_dim1];
	}

/*    Set the off-diagonal second derivatives of the Lagrange function
s and*/
/*     the initial quadratic model. */

    } else {
	ih = ipt * (ipt - 1) / 2 + jpt;
	zmat[nfx * zmat_dim1 + 1] = recip;
	zmat[*nf + nfx * zmat_dim1] = recip;
	zmat[ipt + 1 + nfx * zmat_dim1] = -recip;
	zmat[jpt + 1 + nfx * zmat_dim1] = -recip;
	temp = xpt[*nf + ipt * xpt_dim1] * xpt[*nf + jpt * xpt_dim1];
	hq[ih] = (fbeg - fval[ipt + 1] - fval[jpt + 1] + f) / temp;
    }
    if (*nf < *npt && *nf < *maxfun) {
	goto L50;
    }
    return 0;
} /* prelim_ */

/* Subroutine */ int rescue_(S_fp fcn, integer *n, integer *npt, doublereal *
	xl, doublereal *xu, integer *iprint, integer *maxfun, doublereal *
	xbase, doublereal *xpt, doublereal *fval, doublereal *xopt, 
	doublereal *gopt, doublereal *hq, doublereal *pq, doublereal *bmat, 
	doublereal *zmat, integer *ndim, doublereal *sl, doublereal *su, 
	integer *nf, doublereal *delta, integer *kopt, doublereal *vlag, 
	doublereal *ptsaux, doublereal *ptsid, doublereal *w)
{
    /* Format strings */
    static char fmt_300[] = "(/4x,\002Function number\002,i6,\002    F =\002\
,1pd18.10,\002    The corresponding X is:\002/(2x,5d15.6))";

    /* System generated locals */
    integer xpt_dim1, xpt_offset, bmat_dim1, bmat_offset, zmat_dim1, 
	    zmat_offset, i__1, i__2, i__3;
    doublereal d__1, d__2, d__3, d__4;

    /* Builtin functions */
    double sqrt(doublereal);
    integer s_wsfe(cilist *), do_fio(integer *, char *, ftnlen), e_wsfe();

    /* Local variables */
    static doublereal diff, half, beta;
    static integer kold;
    static doublereal winc;
    static integer nrem, knew;
    static doublereal temp, bsum;
    static integer nptm;
    static doublereal zero, f;
    static integer i, j, k;
    static doublereal hdiag, fbase, sfrac, denom, vquad, sumpq;
    static integer ih, jp, ip, iq, np, iw;
    static doublereal xp, xq;
    extern /* Subroutine */ int update_(integer *, integer *, doublereal *, 
	    doublereal *, integer *, doublereal *, doublereal *, doublereal *,
	     integer *, doublereal *);
    static doublereal dsqmin, distsq, vlmxsq, den;
    static integer ihp;
    static doublereal one;
    static integer ihq, jpn, kpt;
    static doublereal sum;

    /* Fortran I/O blocks */
    static cilist io___210 = { 0, 6, 0, fmt_300, 0 };



/*    The arguments N, NPT, XL, XU, IPRINT, MAXFUN, XBASE, XPT, FVAL, XOPT
,*/
/*      GOPT, HQ, PQ, BMAT, ZMAT, NDIM, SL and SU have the same meanings a
s*/
/*       the corresponding arguments of BOBYQB on the entry to RESCUE. */
/*    NF is maintained as the number of calls of CALFUN so far, except tha
t*/
/*       NF is set to -1 if the value of MAXFUN prevents further progress.
 */
/*    KOPT is maintained so that FVAL(KOPT) is the least calculated functi
on*/
/*      value. Its correct value must be given on entry. It is updated if 
a*/
/*      new least function value is found, but the corresponding changes t
o*/
/*       XOPT and GOPT have to be made later by the calling program. */
/*     DELTA is the current trust region radius. */
/*    VLAG is a working space vector that will be used for the values of t
he*/
/*      provisional Lagrange functions at each of the interpolation points
.*/
/*      They are part of a product that requires VLAG to be of length NDIM
.*/
/*    PTSAUX is also a working space array. For J=1,2,...,N, PTSAUX(1,J) a
nd*/
/*      PTSAUX(2,J) specify the two positions of provisional interpolation
*/
/*      points when a nonzero step is taken along e_J (the J-th coordinate
*/
/*       direction) through XBASE+XOPT, as specified below. Usually these 
*/
/*      steps have length DELTA, but other lengths are chosen if necessary
*/
/*       in order to satisfy the given bounds on the variables. */
/*    PTSID is also a working space array. It has NPT components that deno
te*/
/*      provisional new positions of the original interpolation points, in
*/
/*       case changes are needed to restore the linear independence of the
 */
/*      interpolation conditions. The K-th point is a candidate for change
*/
/*      if and only if PTSID(K) is nonzero. In this case let p and q be th
e*/
/*      integer parts of PTSID(K) and (PTSID(K)-p) multiplied by N+1. If p
*/
/*       and q are both positive, the step from XBASE+XOPT to the new K-th
 */
/*      interpolation point is PTSAUX(1,p)*e_p + PTSAUX(1,q)*e_q. Otherwis
e*/
/*      the step is PTSAUX(1,p)*e_p or PTSAUX(2,q)*e_q in the cases q=0 or
*/
/*       p=0, respectively. */
/*    The first NDIM+NPT elements of the array W are used for working spac
e.*/
/*    The final elements of BMAT and ZMAT are set in a well-conditioned wa
y*/
/*      to the values that are appropriate for the new interpolation point
s.*/
/*    The elements of GOPT, HQ and PQ are also revised to the values that 
are*/
/*       appropriate to the final quadratic model. */

/*     Set some constants. */

    /* Parameter adjustments */
    --w;
    --ptsid;
    ptsaux -= 3;
    --vlag;
    --su;
    --sl;
    zmat_dim1 = *npt;
    zmat_offset = zmat_dim1 + 1;
    zmat -= zmat_offset;
    bmat_dim1 = *ndim;
    bmat_offset = bmat_dim1 + 1;
    bmat -= bmat_offset;
    --pq;
    --hq;
    --gopt;
    --xopt;
    --fval;
    xpt_dim1 = *npt;
    xpt_offset = xpt_dim1 + 1;
    xpt -= xpt_offset;
    --xbase;
    --xu;
    --xl;

    /* Function Body */
    half = .5;
    one = 1.;
    zero = 0.;
    np = *n + 1;
    sfrac = half / (doublereal) np;
    nptm = *npt - np;

/*    Shift the interpolation points so that XOPT becomes the origin, and 
set*/
/*     the elements of ZMAT to zero. The value of SUMPQ is required in the
 */
/*     updating of HQ below. The squares of the distances from XOPT to the
 */
/*    other interpolation points are set at the end of W. Increments of WI
NC*/
/*     may be added later to these squares to balance the consideration of
 */
/*     the choice of point that is going to become current. */

    sumpq = zero;
    winc = zero;
    i__1 = *npt;
    for (k = 1; k <= i__1; ++k) {
	distsq = zero;
	i__2 = *n;
	for (j = 1; j <= i__2; ++j) {
	    xpt[k + j * xpt_dim1] -= xopt[j];
/* L10: */
/* Computing 2nd power */
	    d__1 = xpt[k + j * xpt_dim1];
	    distsq += d__1 * d__1;
	}
	sumpq += pq[k];
	w[*ndim + k] = distsq;
	winc = max(winc,distsq);
	i__2 = nptm;
	for (j = 1; j <= i__2; ++j) {
/* L20: */
	    zmat[k + j * zmat_dim1] = zero;
	}
    }

/*    Update HQ so that HQ and PQ define the second derivatives of the mod
el*/
/*     after XBASE has been shifted to the trust region centre. */

    ih = 0;
    i__2 = *n;
    for (j = 1; j <= i__2; ++j) {
	w[j] = half * sumpq * xopt[j];
	i__1 = *npt;
	for (k = 1; k <= i__1; ++k) {
/* L30: */
	    w[j] += pq[k] * xpt[k + j * xpt_dim1];
	}
	i__1 = j;
	for (i = 1; i <= i__1; ++i) {
	    ++ih;
/* L40: */
	    hq[ih] = hq[ih] + w[i] * xopt[j] + w[j] * xopt[i];
	}
    }

/*     Shift XBASE, SL, SU and XOPT. Set the elements of BMAT to zero, and
 */
/*     also set the elements of PTSAUX. */

    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	xbase[j] += xopt[j];
	sl[j] -= xopt[j];
	su[j] -= xopt[j];
	xopt[j] = zero;
/* Computing MIN */
	d__1 = *delta, d__2 = su[j];
	ptsaux[(j << 1) + 1] = min(d__1,d__2);
/* Computing MAX */
	d__1 = -(*delta), d__2 = sl[j];
	ptsaux[(j << 1) + 2] = max(d__1,d__2);
	if (ptsaux[(j << 1) + 1] + ptsaux[(j << 1) + 2] < zero) {
	    temp = ptsaux[(j << 1) + 1];
	    ptsaux[(j << 1) + 1] = ptsaux[(j << 1) + 2];
	    ptsaux[(j << 1) + 2] = temp;
	}
	if ((d__1 = ptsaux[(j << 1) + 2], abs(d__1)) < half * (d__2 = ptsaux[(
		j << 1) + 1], abs(d__2))) {
	    ptsaux[(j << 1) + 2] = half * ptsaux[(j << 1) + 1];
	}
	i__2 = *ndim;
	for (i = 1; i <= i__2; ++i) {
/* L50: */
	    bmat[i + j * bmat_dim1] = zero;
	}
    }
    fbase = fval[*kopt];

/*     Set the identifiers of the artificial interpolation points that are
 */
/*     along a coordinate direction from XOPT, and set the corresponding 
*/
/*     nonzero elements of BMAT and ZMAT. */

    ptsid[1] = sfrac;
    i__2 = *n;
    for (j = 1; j <= i__2; ++j) {
	jp = j + 1;
	jpn = jp + *n;
	ptsid[jp] = (doublereal) j + sfrac;
	if (jpn <= *npt) {
	    ptsid[jpn] = (doublereal) j / (doublereal) np + sfrac;
	    temp = one / (ptsaux[(j << 1) + 1] - ptsaux[(j << 1) + 2]);
	    bmat[jp + j * bmat_dim1] = -temp + one / ptsaux[(j << 1) + 1];
	    bmat[jpn + j * bmat_dim1] = temp + one / ptsaux[(j << 1) + 2];
	    bmat[j * bmat_dim1 + 1] = -bmat[jp + j * bmat_dim1] - bmat[jpn + 
		    j * bmat_dim1];
	    zmat[j * zmat_dim1 + 1] = sqrt(2.) / (d__1 = ptsaux[(j << 1) + 1] 
		    * ptsaux[(j << 1) + 2], abs(d__1));
	    zmat[jp + j * zmat_dim1] = zmat[j * zmat_dim1 + 1] * ptsaux[(j << 
		    1) + 2] * temp;
	    zmat[jpn + j * zmat_dim1] = -zmat[j * zmat_dim1 + 1] * ptsaux[(j 
		    << 1) + 1] * temp;
	} else {
	    bmat[j * bmat_dim1 + 1] = -one / ptsaux[(j << 1) + 1];
	    bmat[jp + j * bmat_dim1] = one / ptsaux[(j << 1) + 1];
/* Computing 2nd power */
	    d__1 = ptsaux[(j << 1) + 1];
	    bmat[j + *npt + j * bmat_dim1] = -half * (d__1 * d__1);
	}
/* L60: */
    }

/*     Set any remaining identifiers with their nonzero elements of ZMAT. 
*/

    if (*npt >= *n + np) {
	i__2 = *npt;
	for (k = np << 1; k <= i__2; ++k) {
	    iw = (integer) (((doublereal) (k - np) - half) / (doublereal) (*n)
		    );
	    ip = k - np - iw * *n;
	    iq = ip + iw;
	    if (iq > *n) {
		iq -= *n;
	    }
	    ptsid[k] = (doublereal) ip + (doublereal) iq / (doublereal) np + 
		    sfrac;
	    temp = one / (ptsaux[(ip << 1) + 1] * ptsaux[(iq << 1) + 1]);
	    zmat[(k - np) * zmat_dim1 + 1] = temp;
	    zmat[ip + 1 + (k - np) * zmat_dim1] = -temp;
	    zmat[iq + 1 + (k - np) * zmat_dim1] = -temp;
/* L70: */
	    zmat[k + (k - np) * zmat_dim1] = temp;
	}
    }
    nrem = *npt;
    kold = 1;
    knew = *kopt;

/*    Reorder the provisional points in the way that exchanges PTSID(KOLD)
*/
/*     with PTSID(KNEW). */

L80:
    i__2 = *n;
    for (j = 1; j <= i__2; ++j) {
	temp = bmat[kold + j * bmat_dim1];
	bmat[kold + j * bmat_dim1] = bmat[knew + j * bmat_dim1];
/* L90: */
	bmat[knew + j * bmat_dim1] = temp;
    }
    i__2 = nptm;
    for (j = 1; j <= i__2; ++j) {
	temp = zmat[kold + j * zmat_dim1];
	zmat[kold + j * zmat_dim1] = zmat[knew + j * zmat_dim1];
/* L100: */
	zmat[knew + j * zmat_dim1] = temp;
    }
    ptsid[kold] = ptsid[knew];
    ptsid[knew] = zero;
    w[*ndim + knew] = zero;
    --nrem;
    if (knew != *kopt) {
	temp = vlag[kold];
	vlag[kold] = vlag[knew];
	vlag[knew] = temp;

/*     Update the BMAT and ZMAT matrices so that the status of the KNE
W-th */
/*    interpolation point can be changed from provisional to original.
 The*/
/*    branch to label 350 occurs if all the original points are reinst
ated.*/
/*    The nonnegative values of W(NDIM+K) are required in the search b
elow.*/

	update_(n, npt, &bmat[bmat_offset], &zmat[zmat_offset], ndim, &vlag[1]
		, &beta, &denom, &knew, &w[1]);
	if (nrem == 0) {
	    goto L350;
	}
	i__2 = *npt;
	for (k = 1; k <= i__2; ++k) {
/* L110: */
	    w[*ndim + k] = (d__1 = w[*ndim + k], abs(d__1));
	}
    }

/*     Pick the index KNEW of an original interpolation point that has not
 */
/*     yet replaced one of the provisional interpolation points, giving */
/*     attention to the closeness to XOPT and to previous tries with KNEW.
 */

L120:
    dsqmin = zero;
    i__2 = *npt;
    for (k = 1; k <= i__2; ++k) {
	if (w[*ndim + k] > zero) {
	    if (dsqmin == zero || w[*ndim + k] < dsqmin) {
		knew = k;
		dsqmin = w[*ndim + k];
	    }
	}
/* L130: */
    }
    if (dsqmin == zero) {
	goto L260;
    }

/*     Form the W-vector of the chosen original interpolation point. */

    i__2 = *n;
    for (j = 1; j <= i__2; ++j) {
/* L140: */
	w[*npt + j] = xpt[knew + j * xpt_dim1];
    }
    i__2 = *npt;
    for (k = 1; k <= i__2; ++k) {
	sum = zero;
	if (k == *kopt) {
	} else if (ptsid[k] == zero) {
	    i__1 = *n;
	    for (j = 1; j <= i__1; ++j) {
/* L150: */
		sum += w[*npt + j] * xpt[k + j * xpt_dim1];
	    }
	} else {
	    ip = (integer) ptsid[k];
	    if (ip > 0) {
		sum = w[*npt + ip] * ptsaux[(ip << 1) + 1];
	    }
	    iq = (integer) ((doublereal) np * ptsid[k] - (doublereal) (ip * 
		    np));
	    if (iq > 0) {
		iw = 1;
		if (ip == 0) {
		    iw = 2;
		}
		sum += w[*npt + iq] * ptsaux[iw + (iq << 1)];
	    }
	}
/* L160: */
	w[k] = half * sum * sum;
    }

/*    Calculate VLAG and BETA for the required updating of the H matrix if
*/
/*     XPT(KNEW,.) is reinstated in the set of interpolation points. */

    i__2 = *npt;
    for (k = 1; k <= i__2; ++k) {
	sum = zero;
	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
/* L170: */
	    sum += bmat[k + j * bmat_dim1] * w[*npt + j];
	}
/* L180: */
	vlag[k] = sum;
    }
    beta = zero;
    i__2 = nptm;
    for (j = 1; j <= i__2; ++j) {
	sum = zero;
	i__1 = *npt;
	for (k = 1; k <= i__1; ++k) {
/* L190: */
	    sum += zmat[k + j * zmat_dim1] * w[k];
	}
	beta -= sum * sum;
	i__1 = *npt;
	for (k = 1; k <= i__1; ++k) {
/* L200: */
	    vlag[k] += sum * zmat[k + j * zmat_dim1];
	}
    }
    bsum = zero;
    distsq = zero;
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	sum = zero;
	i__2 = *npt;
	for (k = 1; k <= i__2; ++k) {
/* L210: */
	    sum += bmat[k + j * bmat_dim1] * w[k];
	}
	jp = j + *npt;
	bsum += sum * w[jp];
	i__2 = *ndim;
	for (ip = *npt + 1; ip <= i__2; ++ip) {
/* L220: */
	    sum += bmat[ip + j * bmat_dim1] * w[ip];
	}
	bsum += sum * w[jp];
	vlag[jp] = sum;
/* L230: */
/* Computing 2nd power */
	d__1 = xpt[knew + j * xpt_dim1];
	distsq += d__1 * d__1;
    }
    beta = half * distsq * distsq + beta - bsum;
    vlag[*kopt] += one;

/*    KOLD is set to the index of the provisional interpolation point that
 is*/
/*    going to be deleted to make way for the KNEW-th original interpolati
on*/
/*    point. The choice of KOLD is governed by the avoidance of a small va
lue*/
/*     of the denominator in the updating calculation of UPDATE. */

    denom = zero;
    vlmxsq = zero;
    i__1 = *npt;
    for (k = 1; k <= i__1; ++k) {
	if (ptsid[k] != zero) {
	    hdiag = zero;
	    i__2 = nptm;
	    for (j = 1; j <= i__2; ++j) {
/* L240: */
/* Computing 2nd power */
		d__1 = zmat[k + j * zmat_dim1];
		hdiag += d__1 * d__1;
	    }
/* Computing 2nd power */
	    d__1 = vlag[k];
	    den = beta * hdiag + d__1 * d__1;
	    if (den > denom) {
		kold = k;
		denom = den;
	    }
	}
/* L250: */
/* Computing MAX */
/* Computing 2nd power */
	d__3 = vlag[k];
	d__1 = vlmxsq, d__2 = d__3 * d__3;
	vlmxsq = max(d__1,d__2);
    }
    if (denom <= vlmxsq * .01) {
	w[*ndim + knew] = -w[*ndim + knew] - winc;
	goto L120;
    }
    goto L80;

/*    When label 260 is reached, all the final positions of the interpolat
ion*/
/*    points have been chosen although any changes have not been included 
yet*/
/*    in XPT. Also the final BMAT and ZMAT matrices are complete, but, apa
rt*/
/*    from the shift of XBASE, the updating of the quadratic model remains
 to*/
/*    be done. The following cycle through the new interpolation points be
gins*/
/*    by putting the new point in XPT(KPT,.) and by setting PQ(KPT) to zer
o,*/
/*     except that a RETURN occurs if MAXFUN prohibits another value of F.
 */

L260:
    i__1 = *npt;
    for (kpt = 1; kpt <= i__1; ++kpt) {
	if (ptsid[kpt] == zero) {
	    goto L340;
	}
	if (*nf >= *maxfun) {
	    *nf = -1;
	    goto L350;
	}
	ih = 0;
	i__2 = *n;
	for (j = 1; j <= i__2; ++j) {
	    w[j] = xpt[kpt + j * xpt_dim1];
	    xpt[kpt + j * xpt_dim1] = zero;
	    temp = pq[kpt] * w[j];
	    i__3 = j;
	    for (i = 1; i <= i__3; ++i) {
		++ih;
/* L270: */
		hq[ih] += temp * w[i];
	    }
	}
	pq[kpt] = zero;
	ip = (integer) ptsid[kpt];
	iq = (integer) ((doublereal) np * ptsid[kpt] - (doublereal) (ip * np))
		;
	if (ip > 0) {
	    xp = ptsaux[(ip << 1) + 1];
	    xpt[kpt + ip * xpt_dim1] = xp;
	}
	if (iq > 0) {
	    xq = ptsaux[(iq << 1) + 1];
	    if (ip == 0) {
		xq = ptsaux[(iq << 1) + 2];
	    }
	    xpt[kpt + iq * xpt_dim1] = xq;
	}

/*     Set VQUAD to the value of the current model at the new point. 
*/

	vquad = fbase;
	if (ip > 0) {
	    ihp = (ip + ip * ip) / 2;
	    vquad += xp * (gopt[ip] + half * xp * hq[ihp]);
	}
	if (iq > 0) {
	    ihq = (iq + iq * iq) / 2;
	    vquad += xq * (gopt[iq] + half * xq * hq[ihq]);
	    if (ip > 0) {
		iw = max(ihp,ihq) - (i__3 = ip - iq, abs(i__3));
		vquad += xp * xq * hq[iw];
	    }
	}
	i__3 = *npt;
	for (k = 1; k <= i__3; ++k) {
	    temp = zero;
	    if (ip > 0) {
		temp += xp * xpt[k + ip * xpt_dim1];
	    }
	    if (iq > 0) {
		temp += xq * xpt[k + iq * xpt_dim1];
	    }
/* L280: */
	    vquad += half * pq[k] * temp * temp;
	}

/*    Calculate F at the new interpolation point, and set DIFF to the 
factor*/
/*    that is going to multiply the KPT-th Lagrange function when the 
model*/
/*     is updated to provide interpolation to the new function value. 
*/

	i__3 = *n;
	for (i = 1; i <= i__3; ++i) {
/* Computing MIN */
/* Computing MAX */
	    d__3 = xl[i], d__4 = xbase[i] + xpt[kpt + i * xpt_dim1];
	    d__1 = max(d__3,d__4), d__2 = xu[i];
	    w[i] = min(d__1,d__2);
	    if (xpt[kpt + i * xpt_dim1] == sl[i]) {
		w[i] = xl[i];
	    }
	    if (xpt[kpt + i * xpt_dim1] == su[i]) {
		w[i] = xu[i];
	    }
/* L290: */
	}
	++(*nf);
	(*fcn)(n, &w[1], &f);
	if (*iprint == 3) {
	    s_wsfe(&io___210);
	    do_fio(&c__1, (char *)&(*nf), (ftnlen)sizeof(integer));
	    do_fio(&c__1, (char *)&f, (ftnlen)sizeof(doublereal));
	    i__3 = *n;
	    for (i = 1; i <= i__3; ++i) {
		do_fio(&c__1, (char *)&w[i], (ftnlen)sizeof(doublereal));
	    }
	    e_wsfe();
	}
	fval[kpt] = f;
	if (f < fval[*kopt]) {
	    *kopt = kpt;
	}
	diff = f - vquad;

/*    Update the quadratic model. The RETURN from the subroutine occur
s when*/
/*     all the new interpolation points are included in the model. */

	i__3 = *n;
	for (i = 1; i <= i__3; ++i) {
/* L310: */
	    gopt[i] += diff * bmat[kpt + i * bmat_dim1];
	}
	i__3 = *npt;
	for (k = 1; k <= i__3; ++k) {
	    sum = zero;
	    i__2 = nptm;
	    for (j = 1; j <= i__2; ++j) {
/* L320: */
		sum += zmat[k + j * zmat_dim1] * zmat[kpt + j * zmat_dim1];
	    }
	    temp = diff * sum;
	    if (ptsid[k] == zero) {
		pq[k] += temp;
	    } else {
		ip = (integer) ptsid[k];
		iq = (integer) ((doublereal) np * ptsid[k] - (doublereal) (ip 
			* np));
		ihq = (iq * iq + iq) / 2;
		if (ip == 0) {
/* Computing 2nd power */
		    d__1 = ptsaux[(iq << 1) + 2];
		    hq[ihq] += temp * (d__1 * d__1);
		} else {
		    ihp = (ip * ip + ip) / 2;
/* Computing 2nd power */
		    d__1 = ptsaux[(ip << 1) + 1];
		    hq[ihp] += temp * (d__1 * d__1);
		    if (iq > 0) {
/* Computing 2nd power */
			d__1 = ptsaux[(iq << 1) + 1];
			hq[ihq] += temp * (d__1 * d__1);
			iw = max(ihp,ihq) - (i__2 = iq - ip, abs(i__2));
			hq[iw] += temp * ptsaux[(ip << 1) + 1] * ptsaux[(iq <<
				 1) + 1];
		    }
		}
	    }
/* L330: */
	}
	ptsid[kpt] = zero;
L340:
	;
    }
L350:
    return 0;
} /* rescue_ */

/* Subroutine */ int trsbox_(integer *n, integer *npt, doublereal *xpt, 
	doublereal *xopt, doublereal *gopt, doublereal *hq, doublereal *pq, 
	doublereal *sl, doublereal *su, doublereal *delta, doublereal *xnew, 
	doublereal *d, doublereal *gnew, doublereal *xbdi, doublereal *s, 
	doublereal *hs, doublereal *hred, doublereal *dsq, doublereal *crvmin)
{
    /* System generated locals */
    integer xpt_dim1, xpt_offset, i__1, i__2;
    doublereal d__1, d__2, d__3, d__4;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    static doublereal half, beta, sdec, blen;
    static integer iact, nact;
    static doublereal angt, qred;
    static integer isav;
    static doublereal temp, zero, xsav, xsum;
    static integer i, j;
    static doublereal angbd;
    static integer k;
    static doublereal dredg, sredg;
    static integer iterc;
    static doublereal resid, delsq, ggsav, tempa, tempb, ratio, sqstp;
    static integer ih;
    static doublereal ds;
    static integer iu;
    static doublereal redmax, dredsq, redsav, onemin, gredsq, rednew;
    static integer itcsav;
    static doublereal rdprev, rdnext, stplen, stepsq, dhd, dhs, cth, one, shs,
	     sth, ssq;
    static integer itermax;


/*    The arguments N, NPT, XPT, XOPT, GOPT, HQ, PQ, SL and SU have the sa
me*/
/*       meanings as the corresponding arguments of BOBYQB. */
/*     DELTA is the trust region radius for the present calculation, which
 */
/*      seeks a small value of the quadratic model within distance DELTA o
f*/
/*       XOPT subject to the bounds on the variables. */
/*     XNEW will be set to a new vector of variables that is approximately
 */
/*      the one that minimizes the quadratic model within the trust region
*/
/*      subject to the SL and SU constraints on the variables. It satisfie
s*/
/*      as equations the bounds that become active during the calculation.
*/
/*    D is the calculated trial step from XOPT, generated iteratively from
 an*/
/*      initial value of zero. Thus XNEW is XOPT+D after the final iterati
on.*/
/*    GNEW holds the gradient of the quadratic model at XOPT+D. It is upda
ted*/
/*       when D is updated. */
/*    XBDI is a working space vector. For I=1,2,...,N, the element XBDI(I)
 is*/
/*      set to -1.0, 0.0, or 1.0, the value being nonzero if and only if t
he*/
/*      I-th variable has become fixed at a bound, the bound being SL(I) o
r*/
/*       SU(I) in the case XBDI(I)=-1.0 or XBDI(I)=1.0, respectively. This
 */
/*       information is accumulated during the construction of XNEW. */
/*    The arrays S, HS and HRED are also used for working space. They hold
 the*/
/*      current search direction, and the changes in the gradient of Q alo
ng S*/
/*      and the reduced D, respectively, where the reduced D is the same a
s D,*/
/*       except that the components of the fixed variables are zero. */
/*     DSQ will be set to the square of the length of XNEW-XOPT. */
/*    CRVMIN is set to zero if D reaches the trust region boundary. Otherw
ise*/
/*      it is set to the least curvature of H that occurs in the conjugate
*/
/*       gradient searches that are not restricted by any constraints. The
 */
/*       value CRVMIN=-1.0D0 is set, however, if all of these searches are
 */
/*       constrained. */

/*     A version of the truncated conjugate gradient is applied. If a line
 */
/*    search is restricted by a constraint, then the procedure is restarte
d,*/
/*    the values of the variables that are at their bounds being fixed. If
*/
/*    the trust region boundary is reached, then further changes may be ma
de*/
/*     to D, each one being in the two dimensional space that is spanned 
*/
/*    by the current D and the gradient of Q at XOPT+D, staying on the tru
st*/
/*    region boundary. Termination occurs when the reduction in Q seems to
*/
/*     be close to the greatest reduction that can be achieved. */

/*     Set some constants. */

    /* Parameter adjustments */
    --hred;
    --hs;
    --s;
    --xbdi;
    --gnew;
    --d;
    --xnew;
    --su;
    --sl;
    --pq;
    --hq;
    --gopt;
    --xopt;
    xpt_dim1 = *npt;
    xpt_offset = xpt_dim1 + 1;
    xpt -= xpt_offset;

    /* Function Body */
    half = .5;
    one = 1.;
    onemin = -1.;
    zero = 0.;

/*    The sign of GOPT(I) gives the sign of the change to the I-th variabl
e*/
/*    that will reduce Q from its value at XOPT. Thus XBDI(I) shows whethe
r*/
/*    or not to fix the I-th variable at one of its bounds initially, with
*/
/*    NACT being set to the number of fixed variables. D and GNEW are also
*/
/*     set for the first iteration. DELSQ is the upper bound on the sum of
 */
/*     squares of the free variables. QRED is the reduction in Q so far. 
*/

    iterc = 0;
    nact = 0;
    sqstp = zero;
    i__1 = *n;
    for (i = 1; i <= i__1; ++i) {
	xbdi[i] = zero;
	if (xopt[i] <= sl[i]) {
	    if (gopt[i] >= zero) {
		xbdi[i] = onemin;
	    }
	} else if (xopt[i] >= su[i]) {
	    if (gopt[i] <= zero) {
		xbdi[i] = one;
	    }
	}
	if (xbdi[i] != zero) {
	    ++nact;
	}
	d[i] = zero;
/* L10: */
	gnew[i] = gopt[i];
    }
    delsq = *delta * *delta;
    qred = zero;
    *crvmin = onemin;

/*    Set the next search direction of the conjugate gradient method. It i
s*/
/*    the steepest descent direction initially and when the iterations are
*/
/*     restarted because a variable has just been fixed by a bound, and of
 */
/*    course the components of the fixed variables are zero. ITERMAX is an
*/
/*     upper bound on the indices of the conjugate gradient iterations. */

L20:
    beta = zero;
L30:
    stepsq = zero;
    i__1 = *n;
    for (i = 1; i <= i__1; ++i) {
	if (xbdi[i] != zero) {
	    s[i] = zero;
	} else if (beta == zero) {
	    s[i] = -gnew[i];
	} else {
	    s[i] = beta * s[i] - gnew[i];
	}
/* L40: */
/* Computing 2nd power */
	d__1 = s[i];
	stepsq += d__1 * d__1;
    }
    if (stepsq == zero) {
	goto L190;
    }
    if (beta == zero) {
	gredsq = stepsq;
	itermax = iterc + *n - nact;
    }
    if (gredsq * delsq <= qred * 1e-4 * qred) {
	goto L190;
    }

/*    Multiply the search direction by the second derivative matrix of Q a
nd*/
/*    calculate some scalars for the choice of steplength. Then set BLEN t
o*/
/*    the length of the the step to the trust region boundary and STPLEN t
o*/
/*     the steplength, ignoring the simple bounds. */

    goto L210;
L50:
    resid = delsq;
    ds = zero;
    shs = zero;
    i__1 = *n;
    for (i = 1; i <= i__1; ++i) {
	if (xbdi[i] == zero) {
/* Computing 2nd power */
	    d__1 = d[i];
	    resid -= d__1 * d__1;
	    ds += s[i] * d[i];
	    shs += s[i] * hs[i];
	}
/* L60: */
    }
    if (resid <= zero) {
	goto L90;
    }
    temp = sqrt(stepsq * resid + ds * ds);
    if (ds < zero) {
	blen = (temp - ds) / stepsq;
    } else {
	blen = resid / (temp + ds);
    }
    stplen = blen;
    if (shs > zero) {
/* Computing MIN */
	d__1 = blen, d__2 = gredsq / shs;
	stplen = min(d__1,d__2);
    }

/*     Reduce STPLEN if necessary in order to preserve the simple bounds, 
*/
/*     letting IACT be the index of the new constrained variable. */

    iact = 0;
    i__1 = *n;
    for (i = 1; i <= i__1; ++i) {
	if (s[i] != zero) {
	    xsum = xopt[i] + d[i];
	    if (s[i] > zero) {
		temp = (su[i] - xsum) / s[i];
	    } else {
		temp = (sl[i] - xsum) / s[i];
	    }
	    if (temp < stplen) {
		stplen = temp;
		iact = i;
	    }
	}
/* L70: */
    }

/*    Update CRVMIN, GNEW and D. Set SDEC to the decrease that occurs in Q
.*/

    sdec = zero;
    if (stplen > zero) {
	++iterc;
	temp = shs / stepsq;
	if (iact == 0 && temp > zero) {
	    *crvmin = min(*crvmin,temp);
	    if (*crvmin == onemin) {
		*crvmin = temp;
	    }
	}
	ggsav = gredsq;
	gredsq = zero;
	i__1 = *n;
	for (i = 1; i <= i__1; ++i) {
	    gnew[i] += stplen * hs[i];
	    if (xbdi[i] == zero) {
/* Computing 2nd power */
		d__1 = gnew[i];
		gredsq += d__1 * d__1;
	    }
/* L80: */
	    d[i] += stplen * s[i];
	}
/* Computing MAX */
	d__1 = stplen * (ggsav - half * stplen * shs);
	sdec = max(d__1,zero);
	qred += sdec;
    }

/*     Restart the conjugate gradient method if it has hit a new bound. */

    if (iact > 0) {
	++nact;
	xbdi[iact] = one;
	if (s[iact] < zero) {
	    xbdi[iact] = onemin;
	}
/* Computing 2nd power */
	d__1 = d[iact];
	delsq -= d__1 * d__1;
	if (delsq <= zero) {
	    goto L90;
	}
	goto L20;
    }

/*     If STPLEN is less than BLEN, then either apply another conjugate */
/*     gradient iteration or RETURN. */

    if (stplen < blen) {
	if (iterc == itermax) {
	    goto L190;
	}
	if (sdec <= qred * .01) {
	    goto L190;
	}
	beta = gredsq / ggsav;
	goto L30;
    }
L90:
    *crvmin = zero;

/*     Prepare for the alternative iteration by calculating some scalars 
*/
/*     and by multiplying the reduced D by the second derivative matrix of
 */
/*     Q, where S holds the reduced D in the call of GGMULT. */

L100:
    if (nact >= *n - 1) {
	goto L190;
    }
    dredsq = zero;
    dredg = zero;
    gredsq = zero;
    i__1 = *n;
    for (i = 1; i <= i__1; ++i) {
	if (xbdi[i] == zero) {
/* Computing 2nd power */
	    d__1 = d[i];
	    dredsq += d__1 * d__1;
	    dredg += d[i] * gnew[i];
/* Computing 2nd power */
	    d__1 = gnew[i];
	    gredsq += d__1 * d__1;
	    s[i] = d[i];
	} else {
	    s[i] = zero;
	}
/* L110: */
    }
    itcsav = iterc;
    goto L210;

/*     Let the search direction S be a linear combination of the reduced D
 */
/*     and the reduced G that is orthogonal to the reduced D. */

L120:
    ++iterc;
    temp = gredsq * dredsq - dredg * dredg;
    if (temp <= qred * 1e-4 * qred) {
	goto L190;
    }
    temp = sqrt(temp);
    i__1 = *n;
    for (i = 1; i <= i__1; ++i) {
	if (xbdi[i] == zero) {
	    s[i] = (dredg * d[i] - dredsq * gnew[i]) / temp;
	} else {
	    s[i] = zero;
	}
/* L130: */
    }
    sredg = -temp;

/*    By considering the simple bounds on the variables, calculate an uppe
r*/
/*    bound on the tangent of half the angle of the alternative iteration,
*/
/*     namely ANGBD, except that, if already a free variable has reached a
 */
/*    bound, there is a branch back to label 100 after fixing that variabl
e.*/

    angbd = one;
    iact = 0;
    i__1 = *n;
    for (i = 1; i <= i__1; ++i) {
	if (xbdi[i] == zero) {
	    tempa = xopt[i] + d[i] - sl[i];
	    tempb = su[i] - xopt[i] - d[i];
	    if (tempa <= zero) {
		++nact;
		xbdi[i] = onemin;
		goto L100;
	    } else if (tempb <= zero) {
		++nact;
		xbdi[i] = one;
		goto L100;
	    }
	    ratio = one;
/* Computing 2nd power */
	    d__1 = d[i];
/* Computing 2nd power */
	    d__2 = s[i];
	    ssq = d__1 * d__1 + d__2 * d__2;
/* Computing 2nd power */
	    d__1 = xopt[i] - sl[i];
	    temp = ssq - d__1 * d__1;
	    if (temp > zero) {
		temp = sqrt(temp) - s[i];
		if (angbd * temp > tempa) {
		    angbd = tempa / temp;
		    iact = i;
		    xsav = onemin;
		}
	    }
/* Computing 2nd power */
	    d__1 = su[i] - xopt[i];
	    temp = ssq - d__1 * d__1;
	    if (temp > zero) {
		temp = sqrt(temp) + s[i];
		if (angbd * temp > tempb) {
		    angbd = tempb / temp;
		    iact = i;
		    xsav = one;
		}
	    }
	}
/* L140: */
    }

/*     Calculate HHD and some curvatures for the alternative iteration. */

    goto L210;
L150:
    shs = zero;
    dhs = zero;
    dhd = zero;
    i__1 = *n;
    for (i = 1; i <= i__1; ++i) {
	if (xbdi[i] == zero) {
	    shs += s[i] * hs[i];
	    dhs += d[i] * hs[i];
	    dhd += d[i] * hred[i];
	}
/* L160: */
    }

/*    Seek the greatest reduction in Q for a range of equally spaced value
s*/
/*    of ANGT in [0,ANGBD], where ANGT is the tangent of half the angle of
*/
/*     the alternative iteration. */

    redmax = zero;
    isav = 0;
    redsav = zero;
    iu = (integer) (angbd * 17. + 3.1);
    i__1 = iu;
    for (i = 1; i <= i__1; ++i) {
	angt = angbd * (doublereal) i / (doublereal) iu;
	sth = (angt + angt) / (one + angt * angt);
	temp = shs + angt * (angt * dhd - dhs - dhs);
	rednew = sth * (angt * dredg - sredg - half * sth * temp);
	if (rednew > redmax) {
	    redmax = rednew;
	    isav = i;
	    rdprev = redsav;
	} else if (i == isav + 1) {
	    rdnext = rednew;
	}
/* L170: */
	redsav = rednew;
    }

/*     Return if the reduction is zero. Otherwise, set the sine and cosine
 */
/*     of the angle of the alternative iteration, and calculate SDEC. */

    if (isav == 0) {
	goto L190;
    }
    if (isav < iu) {
	temp = (rdnext - rdprev) / (redmax + redmax - rdprev - rdnext);
	angt = angbd * ((doublereal) isav + half * temp) / (doublereal) iu;
    }
    cth = (one - angt * angt) / (one + angt * angt);
    sth = (angt + angt) / (one + angt * angt);
    temp = shs + angt * (angt * dhd - dhs - dhs);
    sdec = sth * (angt * dredg - sredg - half * sth * temp);
    if (sdec <= zero) {
	goto L190;
    }

/*     Update GNEW, D and HRED. If the angle of the alternative iteration 
*/
/*     is restricted by a bound on a free variable, that variable is fixed
 */
/*     at the bound. */

    dredg = zero;
    gredsq = zero;
    i__1 = *n;
    for (i = 1; i <= i__1; ++i) {
	gnew[i] = gnew[i] + (cth - one) * hred[i] + sth * hs[i];
	if (xbdi[i] == zero) {
	    d[i] = cth * d[i] + sth * s[i];
	    dredg += d[i] * gnew[i];
/* Computing 2nd power */
	    d__1 = gnew[i];
	    gredsq += d__1 * d__1;
	}
/* L180: */
	hred[i] = cth * hred[i] + sth * hs[i];
    }
    qred += sdec;
    if (iact > 0 && isav == iu) {
	++nact;
	xbdi[iact] = xsav;
	goto L100;
    }

/*     If SDEC is sufficiently small, then RETURN after setting XNEW to */
/*     XOPT+D, giving careful attention to the bounds. */

    if (sdec > qred * .01) {
	goto L120;
    }
L190:
    *dsq = zero;
    i__1 = *n;
    for (i = 1; i <= i__1; ++i) {
/* Computing MAX */
/* Computing MIN */
	d__3 = xopt[i] + d[i], d__4 = su[i];
	d__1 = min(d__3,d__4), d__2 = sl[i];
	xnew[i] = max(d__1,d__2);
	if (xbdi[i] == onemin) {
	    xnew[i] = sl[i];
	}
	if (xbdi[i] == one) {
	    xnew[i] = su[i];
	}
	d[i] = xnew[i] - xopt[i];
/* L200: */
/* Computing 2nd power */
	d__1 = d[i];
	*dsq += d__1 * d__1;
    }
    return 0;
/*    The following instructions multiply the current S-vector by the seco
nd*/
/*    derivative matrix of the quadratic model, putting the product in HS.
*/
/*    They are reached from three different parts of the software above an
d*/
/*     they can be regarded as an external subroutine. */

L210:
    ih = 0;
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	hs[j] = zero;
	i__2 = j;
	for (i = 1; i <= i__2; ++i) {
	    ++ih;
	    if (i < j) {
		hs[j] += hq[ih] * s[i];
	    }
/* L220: */
	    hs[i] += hq[ih] * s[j];
	}
    }
    i__2 = *npt;
    for (k = 1; k <= i__2; ++k) {
	if (pq[k] != zero) {
	    temp = zero;
	    i__1 = *n;
	    for (j = 1; j <= i__1; ++j) {
/* L230: */
		temp += xpt[k + j * xpt_dim1] * s[j];
	    }
	    temp *= pq[k];
	    i__1 = *n;
	    for (i = 1; i <= i__1; ++i) {
/* L240: */
		hs[i] += temp * xpt[k + i * xpt_dim1];
	    }
	}
/* L250: */
    }
    if (*crvmin != zero) {
	goto L50;
    }
    if (iterc > itcsav) {
	goto L150;
    }
    i__2 = *n;
    for (i = 1; i <= i__2; ++i) {
/* L260: */
	hred[i] = hs[i];
    }
    goto L120;
} /* trsbox_ */

/* Subroutine */ int update_(integer *n, integer *npt, doublereal *bmat, 
	doublereal *zmat, integer *ndim, doublereal *vlag, doublereal *beta, 
	doublereal *denom, integer *knew, doublereal *w)
{
    /* System generated locals */
    integer bmat_dim1, bmat_offset, zmat_dim1, zmat_offset, i__1, i__2;
    doublereal d__1, d__2, d__3;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    static doublereal temp;
    static integer nptm;
    static doublereal zero;
    static integer i, j, k;
    static doublereal alpha, tempa, tempb, ztest;
    static integer jl, jp;
    static doublereal one, tau;


/*    The arrays BMAT and ZMAT are updated, as required by the new positio
n*/
/*    of the interpolation point that has the index KNEW. The vector VLAG 
has*/
/*    N+NPT components, set on entry to the first NPT and last N component
s*/
/*     of the product Hw in equation (4.11) of the Powell (2006) paper on 
*/
/*     NEWUOA. Further, BETA is set on entry to the value of the parameter
 */
/*     with that name, and DENOM is set to the denominator of the updating
 */
/*    formula. Elements of ZMAT may be treated as zero if their moduli are
*/
/*    at most ZTEST. The first NDIM elements of W are used for working spa
ce.*/

/*     Set some constants. */

    /* Parameter adjustments */
    --w;
    --vlag;
    zmat_dim1 = *npt;
    zmat_offset = zmat_dim1 + 1;
    zmat -= zmat_offset;
    bmat_dim1 = *ndim;
    bmat_offset = bmat_dim1 + 1;
    bmat -= bmat_offset;

    /* Function Body */
    one = 1.;
    zero = 0.;
    nptm = *npt - *n - 1;
    ztest = zero;
    i__1 = *npt;
    for (k = 1; k <= i__1; ++k) {
	i__2 = nptm;
	for (j = 1; j <= i__2; ++j) {
/* L10: */
/* Computing MAX */
	    d__2 = ztest, d__3 = (d__1 = zmat[k + j * zmat_dim1], abs(d__1));
	    ztest = max(d__2,d__3);
	}
    }
    ztest *= 1e-20;

/*     Apply the rotations that put zeros in the KNEW-th row of ZMAT. */

    jl = 1;
    i__2 = nptm;
    for (j = 2; j <= i__2; ++j) {
	if ((d__1 = zmat[*knew + j * zmat_dim1], abs(d__1)) > ztest) {
/* Computing 2nd power */
	    d__1 = zmat[*knew + zmat_dim1];
/* Computing 2nd power */
	    d__2 = zmat[*knew + j * zmat_dim1];
	    temp = sqrt(d__1 * d__1 + d__2 * d__2);
	    tempa = zmat[*knew + zmat_dim1] / temp;
	    tempb = zmat[*knew + j * zmat_dim1] / temp;
	    i__1 = *npt;
	    for (i = 1; i <= i__1; ++i) {
		temp = tempa * zmat[i + zmat_dim1] + tempb * zmat[i + j * 
			zmat_dim1];
		zmat[i + j * zmat_dim1] = tempa * zmat[i + j * zmat_dim1] - 
			tempb * zmat[i + zmat_dim1];
/* L20: */
		zmat[i + zmat_dim1] = temp;
	    }
	}
	zmat[*knew + j * zmat_dim1] = zero;
/* L30: */
    }

/*     Put the first NPT components of the KNEW-th column of HLAG into W, 
*/
/*     and calculate the parameters of the updating formula. */

    i__2 = *npt;
    for (i = 1; i <= i__2; ++i) {
	w[i] = zmat[*knew + zmat_dim1] * zmat[i + zmat_dim1];
/* L40: */
    }
    alpha = w[*knew];
    tau = vlag[*knew];
    vlag[*knew] -= one;

/*     Complete the updating of ZMAT. */

    temp = sqrt(*denom);
    tempb = zmat[*knew + zmat_dim1] / temp;
    tempa = tau / temp;
    i__2 = *npt;
    for (i = 1; i <= i__2; ++i) {
/* L50: */
	zmat[i + zmat_dim1] = tempa * zmat[i + zmat_dim1] - tempb * vlag[i];
    }

/*     Finally, update the matrix BMAT. */

    i__2 = *n;
    for (j = 1; j <= i__2; ++j) {
	jp = *npt + j;
	w[jp] = bmat[*knew + j * bmat_dim1];
	tempa = (alpha * vlag[jp] - tau * w[jp]) / *denom;
	tempb = (-(*beta) * w[jp] - tau * vlag[jp]) / *denom;
	i__1 = jp;
	for (i = 1; i <= i__1; ++i) {
	    bmat[i + j * bmat_dim1] = bmat[i + j * bmat_dim1] + tempa * vlag[
		    i] + tempb * w[i];
	    if (i > *npt) {
		bmat[jp + (i - *npt) * bmat_dim1] = bmat[i + j * bmat_dim1];
	    }
/* L60: */
	}
    }
    return 0;
} /* update_ */

#ifdef __cplusplus
	}
#endif
