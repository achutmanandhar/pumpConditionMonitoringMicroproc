% Run wiimoteRecorindgAnalysisScript.m
% Keyboard in wiimoteRecordingsFftFixedlen.m
% Pump 32 data

% clear;
close all;

Fs = 96;            % Sampling frequency
T = 1/Fs;             % Sampling period
L = 256;             % Length of signal
t = (0:L-1)'*T;        % Time vector

X = round(1000*Y(idxStart:idxEnd));
X_hp = round(1000*Y_hp(idxStart:idxEnd));

figure(1);subplot(3,1,1);
plot(t,X,'LineWidth',2);
hold on;plot(t,1000*Y(idxStart:idxEnd),':r');hold off;
title('Signal'); % Corrupted with Zero-Mean Random Noise')
xlabel('t (seconds)')
ylabel('X(t)')

figure(1);subplot(3,1,2);
plot(t,X_hp,'LineWidth',2);
hold on;plot(t,1000*Y_hp(idxStart:idxEnd),':r');hold off;
title('Signal'); % Corrupted with Zero-Mean Random Noise')
xlabel('t (seconds)')
ylabel('X(t)')

% MATLAB FFT
Ymatlab = fft(X_hp);
% Compute the two-sided spectrum P2. Then compute the single-sided spectrum P1 based on P2 and the even-valued signal length L.
P2 = abs(Ymatlab/L);
P1 = P2(1:L/2+1);
P1(2:end-1) = P1(2:end-1); %2*P1(2:end-1);
% Define the frequency domain f and plot the single-sided amplitude spectrum P1. The amplitudes are not exactly at 0.7 and 1, as expected, because of the added noise. On average, longer signals produce better frequency approximations.
f = Fs*(0:(L/2))/L;
% f = 1:NFFT/a2+1;
figure(1);subplot(3,1,3);
plot(f,P1,'LineWidth',2);
title('Single-Sided Amplitude Spectrum of X(t)')
xlabel('f (Hz)')
ylabel('|FFT|')

% Write signal and magFFT to file to be copied to MPlab source file
realNum = X_hp;
fileID = fopen('C:\Users\engs1602\research\codes\mplabPic2\MPLABXProjects\inputDataPump32\realNum.txt','w');
fprintf(fileID,'%d, ',realNum);
fclose(fileID);
imagNum = zeros(1,L);
fileID = fopen('C:\Users\engs1602\research\codes\mplabPic2\MPLABXProjects\inputDataPump32\imagNum.txt','w');
fprintf(fileID,'%d, ',imagNum);
fclose(fileID);

%% Single plot
figure(1);
subplot(3,1,3)
hold on
plot(f(1:end-1),mplabOut(:,2),'LineWidth',2);
ylabel('|FFT|');
suptitle('Comparison of FFT MATLAB vs. PIC18');
legend('MATLAB fft','PIC18 C-code fft');

    if (saveFigureOption)
        % Save plot
        pathName = fullfile('C:\Users\engs1602\research\meetings\water\20161019OnPumpUnicef\plots');
        plotName = fullfile(pathName,sprintf('plotSignalFFTpumpMatlabVsPic18'));
        fig = gcf;
        fig.PaperPositionMode = 'auto';    
        print(plotName,'-dpng','-r0');
        plotName = fullfile(pathName,sprintf('plotSignalFFTpumpMatlabVsPic18'));
        savefig(plotName);
    end