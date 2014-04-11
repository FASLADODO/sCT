function [ output ] = h(x, invsigma, mu, constant )
%H Computes our gaussian.
%   Detailed explanation goes here
    
    output =  constant * exp(-.5 * ((x-mu).' * invsigma * (x-mu)));
end

