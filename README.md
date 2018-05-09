# BSplineImgInterp
Image Interpolation mex file for matlab allowing 2nd to 9th order BSplines.

This is a simple wrapper use the excellent image interpolation code provided by P. Thevenaz on http://bigwww.epfl.ch/thevenaz/interpolation/interpolation.html. The file BSplineImgInterp.c can be compiled using the matlab mex function to create the matlab function BSplineImgInterp:

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
 
  \* in/outputs marked with an asterisk are optional

The original author provides three versions of the code, one for MAC, Unix and PC. I've only tested the MAC version. BSplineImgInterp.c includes the code from the lib directory. This way, different versions can easily be interchanged by renaming lib_MAC, lib_UNIX or lib_PC to lib.

 
 Written by Jan Neggers, 2018 Paris

 This file is merely a wrapper around code provided by P. Thevenaz, hosted on:
 http://bigwww.epfl.ch/thevenaz/interpolation/interpolation.html
 with the permision of the author, provided that the following message remains
 with this code:

 This C program is based on the following paper:
        P. Thevenaz, T. Blu, M. Unser, "Interpolation Revisited,"
        IEEE Transactions on Medical Imaging,
        vol. 19, no. 7, pp. 739-758, July 2000.
 EPFL makes no warranties of any kind on this software and shall in no event
 be liable for damages of any kind in connection with the use and
 exploitation of this technology.
 
