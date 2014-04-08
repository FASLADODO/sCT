function [ result ] = apply( xs, GMModel  )
%APPLY Summary of this function goes here
%   Detailed explanation goes here

    sigmai12 = GMModel.Sigma(1,2:16,:);
    sigmai22 = GMModel.Sigma(2:16,2:16,:);
    mui1 = GMModel.mu(:, 1);
    mui2 = GMModel.mu(:, 2:16);
    
    lxs = length(xs);
    result = zeros(lxs,1);
    
    smusigma = [];
    
    constant = [];
    
    invsigmai22 = [];
    
    for k=1:20,
        smusigma(:,:,k) = sigmai12(:,:,k) * inv(sigmai22(:,:,k));
    end
    
    for k=1:20,
        constant(k) = 1/((2*pi)^8 * det(sigmai22(:,:,k))^.5);
    end
    
    for k=1:20,
        invsigmai22(:, :, k) = inv(sigmai22(:,:,k));
    end
    
    
    parfor j=1:lxs,
        upper = 0.0;
        lower = 0.0;
        
        x = xs(j, 2:16);

        for i=1:20,
            m2 = mui2(i, :);
            a = GMModel.PComponents(i);
            smu = samplemu(x, mui1(i,:),m2 , smusigma(:,:,i));
            hi = h(x, invsigmai22(:, :, i), m2, constant(i));
            upper = upper + smu * a *hi;
            lower = lower + a * hi;
        end
        result(j) = upper/lower;
    end
end

