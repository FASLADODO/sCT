function [ smu ] = samplemu( x, mu1, mu2, sigma )
%SAMPLEMU Computes the sample mean.
%   Detailed explanation goes here

    smu = mu1 + (sigma * (x - mu2));
end

