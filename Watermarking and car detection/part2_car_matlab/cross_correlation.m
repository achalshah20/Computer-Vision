function M_out = cross_correlation(M,T)
M_out = zeros(size(M));
mar_x = floor(size(T,2)/2);
mar_y = floor(size(T,1)/2);
M_ex = margin_expansion(M,mar_x,mar_y);
[a,b] = size(M);

M_patchs = zeros(prod(size(T)),prod(size(M)));
count = 1;
for r = 1+mar_y:a+mar_y
    for c = 1+mar_x:b+mar_x
       M_patchs(:,count) = reshape(M_ex(r-mar_y:r+mar_y,c-mar_x:c+mar_x),...
           prod(size(T)),1);
       count = count+1;
    end
end

[c,d] = size(M);
M_out = reshape(M_patchs'*reshape(T,prod(size(T)),1), d, c)';
end