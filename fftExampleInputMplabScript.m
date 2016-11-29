clear;
close all;

Fs = 1000;            % Sampling frequency
T = 1/Fs;             % Sampling period
L = 256;             % Length of signal
t = (0:L-1)*T;        % Time vector
% Form a signal containing a 50 Hz sinusoid of amplitude 0.7 and a 120 Hz sinusoid of amplitude 1.

S = 0.7*sin(2*pi*50*t) + sin(2*pi*120*t);
% Corrupt the signal with zero-mean white noise with a variance of 4.

X = S + .5*randn(size(t));
X = round(X*100);
% Plot the noisy signal in the time domain. It is difficult to identify the frequency components by looking at the signal X(t).

figure(1);subplot(2,1,1);
plot(1000*t,X,'LineWidth',2);
title('Signal'); % Corrupted with Zero-Mean Random Noise')
xlabel('t (milliseconds)')
ylabel('X(t)')

Y = fft(X);
% Compute the two-sided spectrum P2. Then compute the single-sided spectrum P1 based on P2 and the even-valued signal length L.

P2 = abs(Y/L);
P1 = P2(1:L/2+1);
P1(2:end-1) = P1(2:end-1); %2*P1(2:end-1);
% Define the frequency domain f and plot the single-sided amplitude spectrum P1. The amplitudes are not exactly at 0.7 and 1, as expected, because of the added noise. On average, longer signals produce better frequency approximations.

f = Fs*(0:(L/2))/L;
figure(1);subplot(2,1,2);
plot(f,P1,'LineWidth',2);
title('Single-Sided Amplitude Spectrum of X(t)')
xlabel('f (Hz)')
ylabel('|P1(f)|')

X
round(P1)

% Write signal and magFFT to file to be copied to MPlab source file
realNum = X;
fileID = fopen('C:\Users\engs1602\research\data\MPLABXProjects\inputData\realNum.txt','w');
fprintf(fileID,'%d, ',realNum);
fclose(fileID);
imagNum = zeros(1,L);
fileID = fopen('C:\Users\engs1602\research\data\MPLABXProjects\inputData\imagNum.txt','w');
fprintf(fileID,'%d, ',imagNum);
fclose(fileID);

% Plot MPlab's output
figure(2);subplot(2,1,1);
plot(1000*t,mplabOut(:,2))
title('Signal'); % Corrupted with Zero-Mean Random Noise')
xlabel('t (milliseconds)')
ylabel('X(t)')

figure(2);subplot(2,1,2);
plot(f(1:end-1),mplabOut(:,2),'LineWidth',2);
title('Single-Sided Amplitude Spectrum of X(t)')
xlabel('f (Hz)')
ylabel('|P1(f)|')

%% Single plot
figure(1);
subplot(2,1,2)
hold on
plot(f(1:end-1),mplabOut(:,2),'LineWidth',2);
ylabel('|FFT|');
suptitle('Comparison of FFT MATLAB vs. PIC18');

    if (saveFigureOption)
        % Save plot
        pathName = fullfile('C:\Users\engs1602\research\meetings\water\20161019OnPumpUnicef\plots');
        plotName = fullfile(pathName,sprintf('plotSignalFFTMatlabVsPic18'));
        fig = gcf;
        fig.PaperPositionMode = 'auto';    
        print(plotName,'-dpng','-r0');
        plotName = fullfile(pathName,sprintf('plotSignalFFTMatlabVsPic18'));
        savefig(plotName);
    end
    
%%
% Write signal and magFFT to file to be copied to MPlab source file
xInput = X(1:256);
fileID = fopen('C:\Users\engs1602\research\meetings\smallGroup\20161025ManandharAnalysisCodeUpdate\data\xInput.txt','w');
fprintf(fileID,'%d, ',xInput);
fclose(fileID);

figure(1001);
subplot(2,1,1);
plot(X(1:256),'b');
title('Signal');
grid on;

subplot(2,1,2);
plot(X_hp(1:256),'b');hold on;
plot(mplabOut(:,3)*1.1,'r');hold off;
grid on;
legend('MATLAB','C-code');
title('Compaison of filter implementation');
ylabel('High-pass filtered');
xlabel('time');
    if (saveFigureOption)
        % Save plot
        pathName = fullfile('C:\Users\engs1602\research\meetings\smallGroup\20161025ManandharAnalysisCodeUpdate\plots\verifyAMmodified28102016');
        plotName = fullfile(pathName,sprintf('plotFilterMatlabVsC'));
        fig = gcf;
        fig.PaperPositionMode = 'auto';    
        print(plotName,'-dpng','-r0');
        plotName = fullfile(pathName,sprintf('plotFilterMatlabVsC'));
        savefig(plotName);
    end
    
