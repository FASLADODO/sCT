function [ GMModel ] = train( data )
%TRAIN Summary of this function goes here
%   Detailed explanation goes here
    
    startup;
    
    
    options = statset('MaxIter',1000,'Display','final');

    GMModel = fitgmdist(data, 20,'Options',options, 'Regularize',0.01)

    
end

