function M_out = rescale(M,a,b)
    M_min = min(min(M));
    M_max = max(max(M));
    
    M_out = (M - M_min)/(M_max-M_min)*(b-a) + a;
end