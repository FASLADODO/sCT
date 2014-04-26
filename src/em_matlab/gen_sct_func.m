function [ ct ] = gen_sct_func( nii_list, mask )
%GEN_SCT_FUNC Summary of this function goes here
%   Detailed explanation goes here
startup;

% Compute the sCT values
load('model0123.mat');

load(mask);

tic;
sct_val = get_sct(nii_list, model0123.Sigma, model0123.mu, model0123.PComponents);
toc;

ct = nii_list(:,1);

% Insert new sCT values in old CT image to preserve the header
count = 1;
for i=1:192
    for j=1:192
        for k=1:192
            if mask.img(i,j,k) == 255
               ct.img(i,j,k) = sct_val(count, 1);
               count = count + 1;
            end
        end
    end
end


end

