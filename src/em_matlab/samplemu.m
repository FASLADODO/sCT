function [ smu ] = samplemu( x2, mu1, mu2, sigma )
%SAMPLEMU Summary of this function goes here
%   Detailed explanation goes here

    smu = mu1 + sigma * (x2 - mu2);

end

