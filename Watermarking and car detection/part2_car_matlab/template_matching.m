% Author: Lijiang Guo
% Date: Feb 10, 2017
% This function measures similarity between template to a image. The
% similarity measure used is inverse MSE.
% Input: 
%   img: greyscale image with edges, 
%   t: template. The x and y dimensions of t should be odd numbers.
%
% Output: img_out: Image with edges.

%%
function img_out = template_matching(img, t)
%img = Z;
%t = template_1;

[m,n] = size(img);
[x,y] = size(t);

mar_x = floor(x/2);
mar_y = floor(y/2);

X = zeros(x*y,(m-2*mar_y)*(n-2*mar_x));
count = 1;
for i= 1+mar_y : m-mar_y
    for j = 1+mar_x : n-mar_x
        X(:,count) = reshape(img(i-mar_y:i+mar_y,j-mar_x:j+mar_x), x*y,1);
        count = count + 1;
    end
end

%% template matching using cosine similarity
%t_vec = reshape(t, x*y, 1);
%X_2norm = sqrt((X.*X)'*repmat(1,x*y,1));
%t_vec_2norm = sqrt(t_vec'*t_vec);
%sim = reshape(X'*t_vec./X_2norm./t_vec_2norm, n-2*mar_x, m-2*mar_y)';

%% template matching using inverse mse
t_vec = reshape(t, x*y, 1);
t_mat = repmat(t_vec, 1, (m-2*mar_y)*(n-2*mar_x));
sim = reshape(1./(1 + sum((X - t_mat).^2, 1)./(x*y)), n-2*mar_x, m-2*mar_y)';

figure;
imagesc(sim);
title('similarity map');
colormap gray
axis equal tight
colorbar;

img_out = sim;
end