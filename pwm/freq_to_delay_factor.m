clear
clc

% Calculate frequency
Ns = 100; %Samples per period
D = 624; %Delay factor
f = ( (5e-9)*2*(D+1)*Ns )^-1

% Calculate delay factor
Ns = 1000; %Samples per second
f = 1600; % frequency
D = ( (5e-9)*2*f*Ns )^-1 - 2