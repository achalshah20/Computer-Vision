% Author: Lijiang Guo
% Date: Feb 10, 2017
%%
clc; % Clears the Command Window
clear all;% remove all variables
%close all % Closes all open windows

%% Step 1: Import a image
% load in data matrix
X = imread('./SRSC.png','png');
% check if original image type is uint8
if class(X) == 'uint8'
    % if uint8, convert to double
    X = im2double(X);
end
% if rgb, separate into three channels
if length(size(X)) == 3
    X_R = X(:,:,1);
    X_G = X(:,:,2);
    X_B = X(:,:,3);
end
% if rgb, convert to gray scale
X_GRAY = rgb2gray(X); 
imshow(X_GRAY);
%
% figure;
% subplot(2,2,1);
% imshow(X_R);
% subplot(2,2,2);
% imshow(X_G);
% subplot(2,2,3);
% imshow(X_B);
% subplot(2,2,4);
% imshow(X);

%% Step: Pre-processing

%% Step: Edge detection: Sobel operator
tic
[X_edge, X_edge_x, X_edge_y] = sobel(X_GRAY,16,4);
toc
close all;
figure;
imshow(X_edge);
title('Image w edge');
saveas(gcf,'./SRSC_edge.jpg');
close(gcf);
% How to select threshold for solbel operator for unseen image?

%% Step: Thining

%% Step: Box detection using Hogue transform

%% Step: Car detection: box-in-box

%% Step: SIFT

%% Step 2: Frequency domain
%FX  = fftshift(fft2(X));
%figure;






