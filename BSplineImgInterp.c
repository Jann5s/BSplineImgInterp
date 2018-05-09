/*
 Matlab usage:
 g = ImgInterpBSpline(f,pos,SplineDegree,MirrorFlag,Tol)
 
 Inputs:
 f             : input image (single)
 pos           : [N x 2] list of interpolation coordinates (double)
 SplineDegree* : default=3, one of [2,3,4,5,6,7,8,9] (double)
 MirrorFlag*   : default=true, extrapolate outside of the image by mirroring
 Tol*          : default=1e-9, tolerance used when computing the coefficients
 
 Outputs:
 g            : interpolated values of f on pos (single)
 c*            : BSpline coefficient image (single)
 
 * in/outputs marked with an asterisk are optional
 
 *----------------------------------------------------------------------------
 Written by Jan Neggers, 2018 Paris
 *----------------------------------------------------------------------------
 This file is merely a wrapper around code provided by P. Thevenaz, hosted on:
 http://bigwww.epfl.ch/thevenaz/interpolation/interpolation.html
 with the permision of the author, provided that the following message remains
 with this code:
 *----------------------------------------------------------------------------
 * This C program is based on the following paper:
 *        P. Thevenaz, T. Blu, M. Unser, "Interpolation Revisited,"
 *        IEEE Transactions on Medical Imaging,
 *        vol. 19, no. 7, pp. 739-758, July 2000.
 * EPFL makes no warranties of any kind on this software and shall in no event
 * be liable for damages of any kind in connection with the use and
 * exploitation of this technology.
 *----------------------------------------------------------------------------
 */

// included for the mex code
#include    "mex.h"
#include    <string.h>

// included for the bspline code
#include    "lib/coeff.c"
#include    "lib/interpol.c"


// Main function (called by matlab)
// ---------------------------------------------------------------------------
void mexFunction( int nlhs, mxArray *plhs[],
                 int nrhs, const mxArray *prhs[])
{
    int i,j,k,o,p;
    // zero the Weights
    double xWeight[10]; for (k=0;k<10;k++) xWeight[k]=0;
    double yWeight[10]; for (k=0;k<10;k++) yWeight[k]=0;
    double zWeight[10]; for (k=0;k<10;k++) zWeight[k]=0;

    // Defaults
    // ------------------------------------
    int s = 3;
    double Tol = 1e-9;
    bool MirrorFlag = true;

    // Processing intputs
    // ------------------------------------
    if(nrhs == 2) {
    } else if(nrhs == 3){
        s = (int) mxGetScalar( prhs[ 2 ] );
    } else if(nrhs == 4){
        s = (int) mxGetScalar( prhs[ 2 ] );
        MirrorFlag = (bool) mxGetScalar( prhs[ 3 ] );
    } else if(nrhs == 5){
        s = (int) mxGetScalar( prhs[ 2 ] );
        MirrorFlag = (bool) mxGetScalar( prhs[ 3 ] );
        Tol = (double) mxGetScalar( prhs[ 4 ] );
    } else {
        mexErrMsgIdAndTxt("ImgInterp:BSpline:nrhs","Incorrect number of inputs inputs.");
    }
    
    // initialize inputs
    if (!mxIsClass(prhs[ 0 ],"single")){
        mexErrMsgIdAndTxt("ImgInterp:BSpline:nrhs","The input image (f) must be single precision.");
    }
    float  *f = (float *)mxGetPr( prhs[ 0 ] );
    
    const mwSize ndims = mxGetNumberOfDimensions(prhs[0]);
    const mwSize *siz = mxGetDimensions(prhs[0]);
    long n = siz[0];
    long m = siz[1];
    if (ndims != 2) {
        mexErrMsgIdAndTxt("ImgInterp:BSpline:prhs","only 2D images are supported.");
    }
    
    if (!mxIsClass(prhs[ 1 ],"double")){
        mexErrMsgIdAndTxt("ImgInterp:BSpline:nrhs","The positions (pos) must be double precision.");
    }
    double *pos = (double *)mxGetPr( prhs[ 1 ] );
    const mwSize *pos_siz = mxGetDimensions(prhs[1]);
    long Np = pos_siz[0];
    long Nc = pos_siz[1];
    if (Nc != 2L){
        mexErrMsgIdAndTxt("ImgInterp:BSpline:prhs","pos must be an array of size [Nx2]");
    }
    
    if (s < 2 | s > 9){
        mexErrMsgIdAndTxt("ImgInterp:BSpline:prhs","SplineDegree out of allowable range: 2 <= s <= 9");
    }

    // Processing outputs
    // ------------------------------------

    // Create the coefficient image
    if (nlhs>2) {
        mexErrMsgIdAndTxt("ImgInterp:BSpline:nlhs","One or Two outputs are allowed.");
    }
    plhs[1] = mxCreateNumericArray(2,siz,mxSINGLE_CLASS,mxREAL);
    float *c = (float *)mxGetData(plhs[1]);

    // Create an empty output array (for the coefficients)
    const mwSize Csiz[2] = {Np,1L};
    plhs[0] = mxCreateNumericArray(2,Csiz,mxSINGLE_CLASS,mxREAL);
    float *F = (float *)mxGetData(plhs[0]);
    

    // Copy the input image to c (ML doesn't allow inputs to be modified)
    memcpy(c, f,  sizeof(float)*n*m);
    
    // Compute the Coefficients
    // ------------------------------------
    int Error = SamplesToCoefficients(c, n, m, s, Tol);
    
    // Loop over the interpolation positions
    // ------------------------------------
    double x, y;
    for (long p=0;p<Np;p++){
        
        // Get one coordinate
        x = pos[p] - 1; // convert from ML index
        y = pos[p+Np] - 1;  // convert from ML index
        
        if ((MirrorFlag == false) & (x < 0 | x >= m | y < 0 | y >= n)){
            // don't mirror, force points outside of the image to zero
            F[p] = 0;
        } else {
            // interpolate the value at pos
            F[p] = (float)InterpolatedValue(c, n, m, y, x, s);
        }
    }
} // End of Main
