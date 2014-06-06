function [ cumm_diff_v ]  = cumm_diff( patient )
%UNTITLED7 Summary of this function goes here
%   Detailed explanation goes here

startup;

ct = load_nii(strcat(patient, '/', 'ct_new.nii'));
sct = load_nii(strcat(patient, '/', 'loosctt2_new.nii'));
mask = load_nii(strcat(patient, '/', 'mask_air_inv.nii'));
mask_list = mask.img(:);


diff = ct.img(mask_list > 0) - sct.img(mask_list > 0);

lng = length(diff);

diff_middle = [0];

for k = -500 : 1 : 500
    diff_middle = [diff_middle, (length(diff(diff(diff>=k-1) < k)) / lng) + diff_middle(end)];
end

cumm_diff_v = vertcat(-501 : 1 : 500, diff_middle)';

end