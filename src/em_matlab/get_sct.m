function [ result ] = get_sct( xs, Sigma, mu, weights )
%GET_SCT Computes the sCT values
%   Computes all sCT values
%   xs      = x*15 matrix of MR values.
%   Sigma   = 16*16*20 matrixof gaussian sigmas.
%   mu      = 20*16 matrix of gaussian means.
%   weights = 1*20 matrix of gaussian weights.
%   
%   result = x*1 matrix of sCT values.
    
    % Pre-allocate what we need.
    result = zeros(length(xs),1);
    smusigma = zeros(1,15,20);
    constant = zeros(20);
    invsigmai22 = zeros(15,15,20);

    % Cut and translate
    sigmai12 = Sigma(1,2:16,:);
    sigmai22 = Sigma(2:16,2:16,:);
    mui1 = mu(:, 1);
    mui2 = mu(:, 2:16)';
    xs = xs(:,2:16)';
    
    % Compute all constants used inside the parfor-loop
    for k=1:20
        constant(k) = 1/((2*pi)^8 * det(sigmai22(:,:,k))^.5);
        invsigmai22(:,:,k) = inv(sigmai22(:,:,k));
        smusigma(:,:,k) = sigmai12(:,:,k) * invsigmai22(:, :, k);
    end
    
    % Reshape the arrays for faster indexing.
    smusigma = reshape(mat2cell(smusigma, 1, 15, ones(1,20)), [20, 1]);
    invsigmai22 = reshape(mat2cell(invsigmai22, 15, 15, ones(1,20)), [20, 1]);
    
    % Execute in parallel
    parfor j=1:length(xs)
        % Find the x we are working with
        x = xs(:, j);
        
        % Place for intermediate results
        upper = 0.0;
        lower = 0.0;
        
        % calculate each gaussian
        for i=1:20
            % Find mu and weights for this gaussian
            m2 = mui2(:, i);
            a = weights(i);
            
            % Calculate sample_mean_i(x) and h_i(x)
            smu = samplemu(x, mui1(i),m2 , smusigma{i});
            hi = h(x, invsigmai22{i}, m2, constant(i));
            
            % Add the results of this gaussian
            upper = upper + smu * a *hi;
            lower = lower + a * hi;
        end
        
        % Compute the CT intensity
        result(j) = upper/lower;
    end
end

