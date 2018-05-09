% g = ImgInterpBSpline(f,pos), interpolate the image f on the coordinates
% defined by pos using 3rd degree BSpline interpolation. f must be single
% precision and pos must be double precision.
%
% g = ImgInterpBSpline(f,pos,s), 2 <= s <= 9, indicate the degree of the
% splines from 2nd to 9th degree
%
% g = ImgInterpBSpline(f,pos,s,mirror), mirror = true by default, set it to
% false to cause points outside of the image to be set to zero instead of
% using image mirroring.
%
% g = ImgInterpBSpline(f,pos,s,mirror,tol), tol = 1e-9 by default. This
% tolerance is used when computing the initial guess for the causal filter
% used to compute the bspline coefficients.
%
% [g, c] = ImgInterpBSpline(f,pos), also return the bspline coefficient
% image c.
