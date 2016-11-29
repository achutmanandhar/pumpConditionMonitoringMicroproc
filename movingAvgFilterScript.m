clear
close all

% MATLAB's data
% load count.dat
% x = count(:,1);

% Hanpump data
x = [340, 420, 440, 600, 520, 640, 760, 840, 840, 700, 680, 840, 880, 900, 960, 920, 900, 960, 980, 980, 980, 1020, 1000, 1020, 1060, 1080, 1080, 1120, 1320, 1560, 1540, 1420, 1260, 1200, 1200, 1220, 1180, 1180, 1140, 1060, 1040, 980, 940, 900, 880, 740, 700, 520, 720, 700, 660, 300, 540, 580, 560, 780, 520, 720, 760, 860, 820, 800, 720, 800, 820, 940, 920, 920, 920, 960, 960, 980, 1000, 1000, 1000, 1040, 1080, 1080, 1080, 1140, 1340, 1560, 1540, 1400, 1240, 1240, 1180, 1200, 1180, 1160, 1060, 1100, 1080, 980, 920, 900, 820, 740, 700, 560, 560, 580, 720, 460, 600, 580, 500, 740, 640, 780, 740, 840, 760, 820, 800, 880, 900, 900, 940, 940, 960, 980, 1020, 1040, 1020, 1020, 1060, 1080, 1120, 1120, 1240, 1480, 1540, 1500, 1320, 1280, 1220, 1240, 1240, 1140, 1140, 1100, 1080, 1000, 940, 900, 820, 780, 700, 660, 740, 700, 400, 660, 420, 600, 560, 480, 540, 760, 760, 860, 760, 760, 760, 820, 860, 920, 900, 920, 920, 940, 1040, 1020, 1040, 1020, 1040, 1060, 1080, 1080, 1100, 1200, 1420, 1540, 1540, 1380, 1180, 1100, 1140, 1220, 1340, 1220, 1120, 1040, 1000, 940, 920, 800, 760, 760, 680, 640, 820, 600, 720, 800, 480, 680, 480, 520, 420, 740, 780, 860, 780, 720, 740, 800, 860, 960, 900, 880, 900, 940, 980, 980, 980, 1000, 980, 1020, 1080, 1080, 1080, 1160, 1380, 1540, 1560, 1380, 1240, 1200, 1220, 1240, 1240, 1160, 1060, 1040, 1020, 980, 920, 880, 840, 780, 700, 660, 740, 720]';

% MATLAB
% Create the filter coefficient vectors.

a = 1;
b = [1/4 1/4 1/4 1/4]; % [b0 bn-1 bn-2 bn-3]
filterOrder = length(b);
% Compute the 4-hour moving average of the data, and plot both the original data and the filtered data.

y = filter(b,a,x);

t = 1:length(x);
figure(10);
plot(t,x,'--',t,y,'-')
legend('Original Data','Filtered Data')

% Take care of phase delay by appending zeros to original signal
D = filterOrder/2; % D = filter order/2
y = filter(b,a,cat(1,x,zeros(D,1))); % Append D zeros to the input data
y = y(D+1:end);
figure(11);
plot(t,x,'--',t,y,'-')
legend('Original Data','Filtered Data')

% AM
% The following is almost the same as MATLAB's moving average except the
% first N-1 samples, where N = filter order
% Take care of phase delay by appending zeros to original signal
D = filterOrder/2; % D = filter order/2
xZeroApp = cat(1,x,zeros(D,1));
yAM = nan(length(x),1);
iY = 1;
for iX=D+1:length(xZeroApp)
    if (iX<filterOrder)
        yAM(iY) = b(1:iX)*xZeroApp(1:iX);
    else
        yAM(iY) = b*xZeroApp(iX-filterOrder+1:iX);
    end
    iY = iY+1;
end
figure(12);
plot(t,x,'--',t,yAM,'-');
legend('Original Data','Filtered Data');

% % HPF = Signal - LPF(moving average with phase shift corrected)
% yhp = x-yAM;  