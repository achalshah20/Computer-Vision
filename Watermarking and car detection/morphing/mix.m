% Author: Lijiang Guo
% Date: Jan 29, 2017
%%
clear % remove all variables
p = genpath(pwd); % find all folders in our current folder
addpath(p); % add them to the path (addpath(genpath(pwd)) would do too)

%%
% load in data matrix
X = imread('./images/aj.png','png'); % This is a greyscale image
%image(X_raw); % This function plots RBG image
% rgb image can be converted to grayscale using rgb2gray()
% original image type is uint8
class(X)
% convert to double
X = im2double(X);
% convert to gray scale
X = rgb2gray(X); 

[X_low, X_high] = filter(X,15,0.9,10);

figure;
subplot(1,3,1)
imshow(X);
title('Original');
subplot(1,3,2)
imshow(X_low);
title('Low pass');
subplot(1,3,3)
imshow(X_high);
title('High pass');

%%
% load in data matrix
Y = imread('./images/tiger.png','png'); % This is a greyscale image
%image(X_raw); % This function plots RBG image
% rgb image can be converted to grayscale using rgb2gray()
% original image type is uint8
class(Y)
% convert to double
X = im2double(Y);
% convert to gray scale
Y = rgb2gray(Y); 

[Y_low, Y_high] = filter(Y,22,0.005,50);

figure;
subplot(1,3,1)
imshow(Y);
title('Original');
subplot(1,3,2)
imshow(Y_low);
title('Low pass');
subplot(1,3,3)
imshow(Y_high);
title('High pass');

%%
Z = X_low + Y_high;
figure;
imshow(Z);
title('Duplex');

%%
function [low_pass, high_pass] = filter(X, radius, lf, hf)
    % transform to frequency fomain
    FX  = fftshift(fft2(X));
    figure
    subplot(1,3,1)
    % plot power spectrogram
    imagesc(log(abs(FX)));
    axis equal tight;
    colormap jet;
    title('Power spectogram');

    % masking the frequency with a disc
    [dimX, dimY] = size(FX);
    
    center = [dimX/2,dimY/2];
    T = FX; % mask
    for i = 1:dimX
        for j = 1:dimY
            if (center(1)-i)^2 + (center(2)-j)^2 < radius^2
                T(i,j) = 0*T(i,j);
            end
        end
    end
    FX_high = T*hf; % high-pass
    FX_low = (FX - T)*lf; % low-pass

    % plot power spectrogram
    subplot(1,3,2)
    imshow(log(abs(FX_high)));
    title('Power specogram after high filtering');
    subplot(1,3,3)
    imshow(log(abs(FX_low)));
    title('Power specogram after low filtering');

    % invert tranform to spatial domain
    X_high = ifft2(ifftshift(FX_high));
    X_low = ifft2(ifftshift(FX_low));

    % convert back to uint8
    high_pass = im2uint8(X_high);
    low_pass = im2uint8(X_low);
end





