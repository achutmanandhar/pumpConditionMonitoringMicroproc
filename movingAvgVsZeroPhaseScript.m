% Compare filtfilt vs. HPF=signal-movingAverage

% Moving average filter coeff
a = 1;
b = [1/4 1/4 1/4 1/4];
% Compute the 4-hour moving average of the data, and plot both the original data and the filtered data.

% Zero-phase HPF 
Fs = 96; % Hz % Sampling Frequency
Fpass = 4; % 2*Fpass(Hz)/sampling f(Hz) in normalized freq
Fstop = 2; % 2*Fstop(Hz)/sampling f(Hz) in normalized freq
Highpass = designfilt('highpassfir', ...
    'PassbandFrequency',2*Fpass/Fs,'StopbandFrequency',2*Fstop/Fs, ...
    'PassbandRipple',1,'StopbandAttenuation',60, ...
    'DesignMethod','equiripple');

% Load accelerometry data
% load('Z:\netshares\bspprojects6\SmartWater\Kenya LT Logger Data\dotMatfiles\Mangawani3-3');

% Select x/y/z = 1/2/3
signal = 3;

idxStart = 10000;
idxEnd = idxStart+512-1;
while idxEnd<size(vect,1)
    x = vect(idxStart:idxEnd,signal);
    y = filter(b,a,x);

    t = 1:length(x);
    % figure(2);
    % plot(t,x,'--',t,y,'-')
    % legend('Original Data','Filtered Data')

    % HPF = signal - FIR LPF (moving average filter)
    yHP = x-y;
    figure(3);
    plot(t,x,'--',t,yHP,'-')
    legend('Original Data','Filtered Data')

    % HPF = zero-phase FIR
    yHPzp = filtfilt(Highpass, x); 
    figure(3);hold on;
    plot(t,yHPzp,'-k')
    axis([1 512 -8000 8000 ]);
    legend('Original Data','Filtered Data')
    hold off;
    
    drawnow;
    pause(.1);
    
    idxStart = idxEnd+1;
    idxEnd = idxStart+512-1;
end