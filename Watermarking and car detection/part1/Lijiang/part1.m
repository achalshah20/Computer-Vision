% Author: Lijiang Guo
% Date: Feb 4, 2017
%%
clear % remove all variables
%% Step 1: generate random number
% Given a seed number N, generate a sequence of numbers.
N = 47405; % set seed number
rng(N); % set random number generator with seed
v = randi([1,1000], 360,1);

%% Step 2: Fourier transform of a image
% load in data matrix
X = imread('./noise1.png','png'); % This is a greyscale image
%image(X_raw); % This function plots RBG image
% rgb image can be converted to grayscale using rgb2gray()
% check if original image type is uint8
if class(X) == 'uint8'
    % if uint8, convert to double
    X = im2double(X);
end
% if rgb, convert to gray scale
% X = rgb2gray(X); 
FX  = fftshift(fft2(X));
figure;
% plot power spectrogram
c_min = min(min(log(abs(FX)))) - 5;
c_max = max(max(log(abs(FX)))) + 10;
clims = [c_min, c_max];
subplot(1,2,1);
imagesc(log(abs(FX)), clims);
axis equal tight;
colormap jet;
title('Power (log) spectogram');
colorbar;

%% Step 3: Add watermark
% Modify the real portion of the frequency representation of
% a image.
center = size(FX)/2;
r = floor(center(1)/2);
alpha = 10^(c_max); % This number will dominate R(x,y) 
                        % such that the correlation between v and c
                        % is large.
FX_wm = FX;
points = zeros(360,2);
for t = 1:360
    point = center + floor([cos(t/360*2*pi)*r, sin(t/360*2*pi)*r]);
    FX_wm(point(1), point(2)) = FX(point(1), point(2)) + ...
        alpha*abs(real(FX(point(1), point(2))))*v(t);
    %FX_wm(point(1), point(2)) = 1e30;
    points(t,:) = point;
end

subplot(1,2,2);
imagesc(log(abs(FX_wm)), clims);
axis equal tight;
colormap jet;
title('Power (log) spectogram w watermark');
colorbar;

saveas(gcf,'./spec_water_cmp.jpg');
close(gcf);

%% Step 4: Test watermark
t = 0.05
N = 47405; % set seed number
rng(N); % set random number generator with seed
v = randi([1,1000], 360,1);

% recover v from FX_wm
X_wm = im2uint8(ifft2(ifftshift(FX_wm)));
FX_test = fftshift(fft2(im2double(X_wm)));
c = zeros(360,1);
for t = 1:360
    point = center + floor([cos(t/360*2*pi)*r, sin(t/360*2*pi)*r]);
    c(t) = real(FX_test(point(1),point(2)));
end

mean_v = sum(v)/360;
mean_c = sum(c)/360;
sd_v = sqrt(sum((v - mean_v).^2)/360);
sd_c = sqrt(sum((c - mean_c).^2)/360);
corr_vc = sum((v - mean_v).*(c - mean_c))/360/sd_v/sd_c;

corr_vc

%% Remove "HI' from image
FX  = fftshift(fft2(X));
figure;
subplot(1,2,1);
imagesc(log(abs(FX)), clims);
axis equal tight;
colormap jet;
title('Power (log) spectogram');

FX_m_HI = FX;
for j = [157, 160, 163]
   for i = 157:161
       FX_m_HI(i,j) = FX(156, 159);
   end
end
FX_m_HI(159, 157:163) = FX(156, 159);

for j = [351, 354, 357]
   for i = 353:357
       FX_m_HI(i,j) = FX(355, 358);
   end
end
FX_m_HI(355, 351:357) = FX(355, 358);

subplot(1,2,2);
imagesc(log(abs(FX_m_HI)), clims);
axis equal tight;
colormap jet;
title('Power (log) spectogram');

saveas(gcf,'./spec_HI_cmp.jpg');
close(gcf);

X_m_HI = im2uint8(ifft2(ifftshift(FX_m_HI)));
figure;
subplot(1,2,1);
imshow(X)
axis equal tight;
title('Original image');
subplot(1,2,2);
imshow(X_m_HI);
axis equal tight;
title('Image wo/ HI');
saveas(gcf,'./image_HI_cmp.jpg');
close(gcf);





