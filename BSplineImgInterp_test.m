clear; close all;

% Compile the mex file
% ------------------------
cfile = 'BSplineImgInterp.c';
mex(cfile,'-largeArrayDims','-DNOMINMAX')

% BSpline options
% ------------------------
BSplineOrder = 9;
Tol = 1e-5;
MirrorFlag = 0;

% Read the image
% ------------------------
n = 256;
m = 256;
filename = 'lena.img';
fid = fopen(filename,'r+');
f = fread(fid,inf,'uint8');
f = single(reshape(f,n,m).');


% Rotation Test
% ------------------------

% Pixel coordinates
[Y, X] = ndgrid(1:n,1:m);

% number of rotation steps
N = 7;

% rotation center
xc = (1+m)./2;
yc = (1+n)./2;
R = (min(m,n)-2)./2;

% write pixel coordiantes as complex numbers
Z = complex(X-xc,Y-yc);

% rotation angle per step
beta = 2*pi/N;

% rotate the complex coordinates
Zk = Z*exp(-1i*beta);
Xk = real(Zk) + xc;
Yk = imag(Zk) + yc;

% the set of coordinates wher eto interpolate on
pos = [Xk(:), Yk(:)];

g = f;
for k = 1:N
    [g, c] = BSplineImgInterp(g,pos,BSplineOrder,Tol,MirrorFlag);
    g = reshape(g,n,m);
end

% residual image
r = f - g;

% mask the residual outside of the circle
mask = (X-xc).^2 + (Y-yc).^2 > R.^2;
r(mask) = 0;

figure('Position',[50 250 1200 400]);
colormap(gray);
ha(1) = axes('Position',[0/3 0 1/3 1]);
imagesc(f);
ht(1) = text(0.05,0.05,'f','units','normalized');
colorbar

ha(2) = axes('Position',[1/3 0 1/3 1]);
imagesc(g);
ht(2) = text(0.05,0.05,'g','units','normalized');
colorbar

ha(3) = axes('Position',[2/3 0 1/3 1]);
imagesc(r);
ht(3) = text(0.05,0.05,'f-g','units','normalized');
colorbar

set(ha,'DataAspectRatio',[1 1 1]);
set(ht,'color','c','FontSize',28)
set(ha(1:2),'CLim',[0 255]);

set(ha,'XColor','none','YColor','none');

