function [ GMModel ] = train( data )
%TRAIN Summary of this function goes here
%   Detailed explanation goes here
    
    startup;
    
    IDX = kmeans(data,20);
        
    options = statset('MaxIter',1000,'Display','final');

    GMModel = fitgmdist(data, 20,'Options',options, 'Regularize',0.01,'Start',IDX);

    
end

