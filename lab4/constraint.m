function [c, ceq] = constraint(z)
    N = 40;
    mx = 6;
    beta = 20;
    lambda_t = 2*pi/3;
    alpha = 0.2;
    ceq = [];
    c = zeros(N, 1);
    for k = 0:N-1
        c(k+1) = alpha*exp(-beta*(z*(k*mx+1)-lambda_t)^2)-z(k*mx+5);
    end
end
