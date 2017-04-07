% Author: Lijiang Guo
% Date: Feb 10, 2017
% This function implements the Laplacian Operator for edge detection in images.
% Input: 
%   img: greyscale image, 
%   a: threshold for x dimension; high value is more sensitive to edges;
%   b: threshold for y dimension; high value is more sensitive to edges;
%   dx: filter size in x dimension, 
%   dy: filter size in y dimension,
%
% Output: 
%   img_out: Image with edges.

%%
function [img_out] = laplacian_edge(img, kernel, a)
[m,n] = size(img);
if kernel == 1
    d_lap = a*[0 1 0; 1 -4 1; 0 1 0];
elseif kernel == 2
    d_lap = a*[1 1 1; 1 -8 1; 1 1 1];
end
    
    
dlap = reshape(d_lap,9,1);

%% Construct local patch matrix
mar = 1;
X = zeros(9,(m-2*mar)*(n-2*mar));
count = 1;
for i= 1+mar : m-mar
    for j = 1 + mar : n-mar
        X(:,count) = reshape(img(i-mar:i+mar,j-mar:j+mar),9,1);
        count = count + 1;
    end
end

%% Running laplacian operator
img_out = reshape(X'*dlap, n-2*mar, m-2*mar)';

figure;
imshow(img_out);
title('x direction');
colorbar;

end

