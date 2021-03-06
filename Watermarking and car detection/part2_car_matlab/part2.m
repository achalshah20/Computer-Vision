% Author: Lijiang Guo
% Date: Feb 10, 2017
%%
clc; % Clears the Command Window
clear all;% remove all variables
close all % Closes all open windows

%% Step 1: Import a image
% load in data matrix
X = imread('./SRSC.png','png');
X = imread('./Informatics.png','png');
X = imread('./Plaza.png','png');
% check if original image type is uint8
if class(X) == 'uint8'
    % if uint8, convert to double
    X = im2double(X);
end
% if rgb, separate into three channels
% if length(size(X)) == 3
%     X_R = X(:,:,1);
%     X_G = X(:,:,2);
%     X_B = X(:,:,3);
% end
% if rgb, convert to gray scale
X_gray = rgb2gray(X); 
imshow(X_gray);
colorbar;

%% Step: Pre-processing: margin expansion by mirrowing
X_GRAY = margin_expansion(X_gray,1,1);
figure
imshow(X_GRAY);

%% Step: Edge detection: Sobel operator
[X_sob, X_sob_x, X_sob_y, X_sob_dir] = sobel(X_GRAY,16,4);
close all;

figure;
imshow(X_sob);
colorbar
title('Image w edge');

saveas(gcf,'./SRSC_sob_edge.jpg');
saveas(gcf,'./Plaza_sob_edge.jpg');
close(gcf);
% How to select threshold for solbel operator for unseen image?

%% Step: Edge detection: Laplacian operator
X_lap = laplacian_edge(X_GRAY,1,3);
%X_lap = laplacian_edge(X_GRAY,2,1);
saveas(gcf,'./Plaza_lap_edge.jpg');
close(gcf);
close all;

%% Step: Edge detection: Canny (matlab vision tool box)
% No time to implement in my codes.
% Just try Matlab functions to see the effect. Will not use for this homework.
X_canny = edge(X_GRAY,'Canny');
figure;
imshow(X_canny);
colorbar;

%% Step: Edge thinning (nonmaximum supression)

%% Step: Post-processing of edge points before shape detection/template matching
% assign edge points to value 1, others to value 0.

% Sobel
X_edge = double(X_sob > 0.7);   
% sobel operator gives rather messy edge points. But with a general
% similarity measure such as inverse MSE, it is good for SRSC image

% Canny
%X_edge = X_canny;

% Laplacian
%X_edge = double(X_lap > 0.4);   % good for informatics image
% Laplacian detects cleaner edges. But with less edge points detected,
% it is better to user Champer distance.

figure;
imshow(X_edge);
colorbar
title('Image w edge');

saveas(gcf,'./Plaza_edge.jpg');
%saveas(gcf,'./Informatics_edge.jpg');
close(gcf);

%% Step: Hough shape
% Hough tranfrom for ranctagle 

%% Step: Car detection: box-in-box

%% Step: Scale detection: DoG
% not necessary here as most cars are similiar sizes.

%% Step: shift and rotation invariant features: SIFT
% not necessary here as most cars are similarlly posed.
% 'plaza' data needs three kinds of templates to compensate for rotations.

%% Step: Template matching using Chamfer distance
%% Find template
% Display edge image (renormalized to [0, 255])
figure;
X_canny_255 = X_canny*255;
image(X_canny_255);
colormap gray
colorbar
axis equal tight

% Display template
%x_a = 294; x_b = 330; y_a = 190; y_b = 210; % SRSC template
%x_a = 80; x_b = 122; y_a = 110; y_b = 130;  % Informatics template  
x_a = 318; x_b = 340; y_a = 235; y_b = 277; % Plaza template
template_1 = X_canny_255(x_a:x_b,y_a:y_b);
size(template_1);
figure
image(template_1)
colormap gray
colorbar
axis equal tight

%% Template matching part 1: Distance transform
% Reference 1: https://www.cs.cornell.edu/courses/cs664/2008sp/handouts/cs664-7-dtrans.pdf
% Reference 2: Burger and Burger, core algorithms, chapter 11.
% Reference 3: http://www.cs.bc.edu/~hjiang/c342/lectures/matching/fit-I.pdf
Z = X_edge*255;
% Chamfer distance transform (Reference 3)
dist = bwdist(Z,'euclidean'); % matlab function
% Compute Chamfer similarity between template and image
% chamfer_dist = imfilter(dist, template_1,'corr', 'same'); % matlab
% function
chamfer_dist = cross_correlation(dist,template_1);

% Find threshold value for similarity
chamfer_dist_v = reshape(chamfer_dist, prod(size(chamfer_dist)),1);
histogram(chamfer_dist_v);
%threshold = 40000;
threshold = quantile(chamfer_dist_v,0.02)
% Find true position: replace Chamfer similarity with min in a window
size(template_1)
chamfer_dist_min = ordfilt2(chamfer_dist, 1, true(30,15)); %SRSC
chamfer_dist_min = ordfilt2(chamfer_dist, 1, true(43,21)); %Informatics
chamfer_dist_min = ordfilt2(chamfer_dist, 1, true(23,40)); %Plaza
%smap = ordfilt2(map, (x_b-x_a+1)*(y_b-y_a+1), true(x_b-x_a+1,y_b-y_a+1));
%threshold = median(median(map));
[r, c] = find((chamfer_dist == chamfer_dist_min) & (chamfer_dist < threshold));
%
% Plot car locations
figure
imagesc(X)
colormap gray
colorbar
axis equal tight
hold on
scatter(c,r, 50, '.', 'red');
hold off
hold on
[height, width] = size(template_1);
for i = 1:length(r)
   rectangle('Position',[c(i)-width/2,r(i)-height/2,width,height],...
       'EdgeColor','red','LineWidth',1);
end
title('Image detected car');
saveas(gcf,'./Informatics_detected.jpg');
saveas(gcf,'./SRSC_detected.jpg');
saveas(gcf,'./Plaza_detected.jpg');
close(gcf);
%
% figure
% subplot(2,1,1)
% imagesc(chamfer_dist)
% colormap gray
% colorbar
% axis equal tight
% subplot(2,1,2)
% imagesc(Z)
% colormap gray
% colorbar
% axis equal tight

%% Step: Template matching using inverse mean-squared-error

% Display edge image (renormalized to [0, 255])
figure;
Z = X_edge*255;
image(X_edge*255);
colormap gray
colorbar
axis equal tight

% Display template
x_a = 294; x_b = 330; y_a = 190; y_b = 210; % SRSC template
%x_a = 80; x_b = 122; y_a = 110; y_b = 130;  % Informatics template  
template_1 = Z(x_a:x_b,y_a:y_b);
size(template_1);
figure
image(template_1)
colormap gray
colorbar
axis equal tight

% First template matching
sim = template_matching(Z,template_1);
sim_ex = margin_expansion(sim,(x_b-x_a)/2,(y_b-y_a)/2);

figure
subplot(2,1,1)
imagesc(Z)
colormap gray
colorbar
axis equal tight
subplot(2,1,2)
imagesc(sim_ex)
colormap gray
colorbar
axis equal tight

% second template matching
template_2 = true(x_b-x_a+1, y_b-y_a+1);
sim2 = template_matching(sim_ex,template_2);
sim2_ex = margin_expansion(sim2,(x_b-x_a)/2,(y_b-y_a)/2);

threshold = 0.6*255;
figure
subplot(3,1,1)
imagesc(Z)
colormap gray
colorbar
axis equal tight
title('Image w edge');
subplot(3,1,2)
imagesc(sim2_ex)
colormap gray
colorbar
axis equal tight
title('Template matching');
subplot(3,1,3)
imagesc(rescale(sim2_ex,0,255)>threshold)
colormap gray
colorbar
axis equal tight
title('Template matching threshoulded');

saveas(gcf,'./SRSC_temp_match.jpg');
%




%% History

%
figure
imagesc(sim_ex)
colormap gray
colorbar
axis equal tight

figure
imagesc(Z.* (Z > 30))
colormap gray
colorbar
axis equal tight
% Overlay display template matching with edge image
imshow(Z.* (Z > 30));
hold on
h = imshow(sim_ex*10^3.5)
hold off;
set(h, 'AlphaData', h)
%
Y = X_edge/w;
Y = Y > 0.18;

figure;
imshow(Y);
colorbar
axis equal tight


%% Step: Frequency domain
%FX  = fftshift(fft2(X));
%figure;






