function [ GMModel ] = train( data )
%TRAIN Summary of this function goes here
%   Detailed explanation goes here
    tic;
    startup;
    
    kmeansoptions = statset('UseParallel',true,'MaxIter',1000);
    
    IDX = kmeans(data,20,'Options',kmeansoptions);
    
    toc;
    
    options = statset('MaxIter',1000,'Display','final');

    GMModel = fitgmdist(data, 20,'Options',options, 'Regularize',0.01,'Start',IDX);
    
    toc;
end

