function [ result ] = apply( xs, Sigma, mu, weights )
%APPLY Summary of this function goes here
%   Detailed explanation goes here

    sigmai12 = Sigma(1,2:16,:);
    sigmai22 = Sigma(2:16,2:16,:);
    mui1 = mu(:, 1);
    mui2 = mu(:, 2:16);
    
    lxs = length(xs);
    result = zeros(lxs,1);
    
    smusigma = zeros(1,15,20);
    
    constant = zeros(20);
    
    invsigmai22 = zeros(15,15,20);
    
    for k=1:20,
        constant(k) = 1/((2*pi)^8 * det(sigmai22(:,:,k))^.5);
        invsigmai22(:,:,k) = inv(sigmai22(:,:,k));
        smusigma(:,:,k) = sigmai12(:,:,k) * invsigmai22(:, :, k);
    end
    
    smusigma = mat2cell(smusigma, 1, 15, ones(1,20));
    invsigmai22 = mat2cell(invsigmai22, 15, 15, ones(1,20));
    
    tmpmu = mui2';
    xs = xs(:,2:16)';
    
    parfor j=1:lxs,
        upper = 0.0;
        lower = 0.0;
        
        x = xs(:, j);

        for i=1:20,
            m2 = tmpmu(:, i);
            a = weights(i);
            smu = samplemu(x, mui1(i),m2 , smusigma{i});
            hi = h(x, invsigmai22{i}, m2, constant(i));
            upper = upper + smu * a *hi;
            lower = lower + a * hi;
        end
        result(j) = upper/lower;
    end
end

