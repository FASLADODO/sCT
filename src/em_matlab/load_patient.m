function [ nii_list ] = load_patient( patient, ute1,ute2,ute3,ute4,t1 )
%UNTITLED3 Summary of this function goes here
%   Detailed explanation goes here

startup;



% Load all the images
nii1 = load_nii(strcat(patient,'/coreg/ct.nii'));
nii2 = load_nii(strcat(patient,'/coreg/',ute1));
nii3 = load_nii(strcat(patient,'/coreg/',ute2));
nii4 = load_nii(strcat(patient,'/coreg/',ute3));
nii5 = load_nii(strcat(patient,'/coreg/',ute4));
nii6 = load_nii(strcat(patient,'/coreg/',t1));
nii7 = load_nii(strcat(patient,'/coreg/mean_',ute1));
nii8 = load_nii(strcat(patient,'/coreg/mean_',ute2));
nii9 = load_nii(strcat(patient,'/coreg/mean_',ute3));
nii10 = load_nii(strcat(patient,'/coreg/mean_',ute4));
nii11 = load_nii(strcat(patient,'/coreg/mean_',t1));
nii12 = load_nii(strcat(patient,'/coreg/variance_',ute1));
nii13 = load_nii(strcat(patient,'/coreg/variance_',ute2));
nii14 = load_nii(strcat(patient,'/coreg/variance_',ute3));
nii15 = load_nii(strcat(patient,'/coreg/variance_',ute4));
nii16 = load_nii(strcat(patient,'/coreg/variance_',t1));

% Load the mask
mask = load_nii(strcat(patient,'/coreg/mask.nii'));

% Pre-allocate list for voxel values
nii_list = zeros(192*192*192, 16);
count = 1;

% Extract the voxel values
for i=1:192,
    for j=1:192,
        for k=1:192,
            if mask.img(i,j,k) == 255,
               nii_list(count, 1) = nii1.img(i,j,k);
               nii_list(count, 2) = nii2.img(i,j,k);
               nii_list(count, 3) = nii3.img(i,j,k);
               nii_list(count, 4) = nii4.img(i,j,k);
               nii_list(count, 5) = nii5.img(i,j,k);
               nii_list(count, 6) = nii6.img(i,j,k);
               nii_list(count, 7) = nii7.img(i,j,k);
               nii_list(count, 8) = nii8.img(i,j,k);
               nii_list(count, 9) = nii9.img(i,j,k);
               nii_list(count, 10) = nii10.img(i,j,k);
               nii_list(count, 11) = nii11.img(i,j,k);
               nii_list(count, 12) = nii12.img(i,j,k);
               nii_list(count, 13) = nii13.img(i,j,k);
               nii_list(count, 14) = nii14.img(i,j,k);
               nii_list(count, 15) = nii15.img(i,j,k);
               nii_list(count, 16) = nii16.img(i,j,k);
               count = count + 1;
            end
        end
    end
end

% Shrink the list to fit the exact number of voxels
nii_list = nii_list(1:count-1, :);



end

