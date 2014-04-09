function [ output ] = h(x2, invsigmai22, mui2, constant )
%H Summary of this function goes here
%   Detailed explanation goes here

    output =  constant * exp(-.5 * ((x2-mui2).' * invsigmai22 * (x2-mui2)));

end

