close all;
X = [340, 420, 440, 600, 520, 640, 760, 840, 840, 700, 680, 840, 880, 900, 960, 920, 900, 960, 980, 980, 980, 1020, 1000, 1020, 1060, 1080, 1080, 1120, 1320, 1560, 1540, 1420, 1260, 1200, 1200, 1220, 1180, 1180, 1140, 1060, 1040, 980, 940, 900, 880, 740, 700, 520, 720, 700, 660, 300, 540, 580, 560, 780, 520, 720, 760, 860, 820, 800, 720, 800, 820, 940, 920, 920, 920, 960, 960, 980, 1000, 1000, 1000, 1040, 1080, 1080, 1080, 1140, 1340, 1560, 1540, 1400, 1240, 1240, 1180, 1200, 1180, 1160, 1060, 1100, 1080, 980, 920, 900, 820, 740, 700, 560, 560, 580, 720, 460, 600, 580, 500, 740, 640, 780, 740, 840, 760, 820, 800, 880, 900, 900, 940, 940, 960, 980, 1020, 1040, 1020, 1020, 1060, 1080, 1120, 1120, 1240, 1480, 1540, 1500, 1320, 1280, 1220, 1240, 1240, 1140, 1140, 1100, 1080, 1000, 940, 900, 820, 780, 700, 660, 740, 700, 400, 660, 420, 600, 560, 480, 540, 760, 760, 860, 760, 760, 760, 820, 860, 920, 900, 920, 920, 940, 1040, 1020, 1040, 1020, 1040, 1060, 1080, 1080, 1100, 1200, 1420, 1540, 1540, 1380, 1180, 1100, 1140, 1220, 1340, 1220, 1120, 1040, 1000, 940, 920, 800, 760, 760, 680, 640, 820, 600, 720, 800, 480, 680, 480, 520, 420, 740, 780, 860, 780, 720, 740, 800, 860, 960, 900, 880, 900, 940, 980, 980, 980, 1000, 980, 1020, 1080, 1080, 1080, 1160, 1380, 1540, 1560, 1380, 1240, 1200, 1220, 1240, 1240, 1160, 1060, 1040, 1020, 980, 920, 880, 840, 780, 700, 660, 740, 720]';

% % Write signal and magFFT to file to be copied to MPlab source file
% xInput = Y(idxStart:idxEnd);
% fileID = fopen('C:\Users\engs1602\research\meetings\smallGroup\20161104ManandharAnalysisCodeUpdate\data\xInput2.txt','w');
% fprintf(fileID,'%d, ',xInput);
% fclose(fileID);

% Moving average filter coeff
% Ad-hoc: Currently, fixed at 4th order!
filterOrder = 4;
a = 1;
% b = [1/4 1/4 1/4 1/4];
b = [1 1 1 1];
% AM Modified 25 Oct 2016: Add delay to take care of phase shift
% Take care of phase delay by appending zeros to original signal
% X_ma = filter(b,a,X);
D = filterOrder/2; % D = filter order/2
xZeroApp = cat(1,X,zeros(D,1));
X_ma = nan(length(X),1);
iY = 1;
for iX=D+1:length(xZeroApp)
    if (iX<filterOrder)
        X_ma(iY) = b(1:iX)*xZeroApp(1:iX);
    else
        X_ma(iY) = b*xZeroApp(iX-filterOrder+1:iX);
    end
    iY = iY+1;
end   
X_hp = 4*X-X_ma;    

    timeStamp = (1:length(X))';
    yMax = max(cat(1,4*X,X_ma,X_hp))+200;
    yMin = min(cat(1,4*X,X_ma,X_hp))-200;  

    % Plot signal and low-passed
    figure(1);
    subplot(4,1,1);
    plot(timeStamp,4*X,'LineWidth',1.5);
    grid on; 
    axis([0 length(X) yMin yMax]);
    title('X (Scaled * 1000)');  
    
    subplot(4,1,2);           
    plot(timeStamp,X_hp,'LineWidth',1.5);    
    axis([0 length(X) -1000 1000]);    
    title('X High-passed (Scaled  * 4)');    
    grid on;     
    
% Read MPLAB's output
% X_hpMplab
    figure(1);
    subplot(4,1,2);    
    hold on;      
    jitter = 50;
%     % Simulation
%     plot(timeStamp,X_hpMplab(:,3)+jitter*randn(length(timeStamp),1),'r','LineWidth',1.5);            
    % Hardware
    plot(timeStamp,X_hpMplab(:,2)+jitter*randn(length(timeStamp),1),'r','LineWidth',1.5);            

% FFT
NFFT = 256;
% Apply FFT per window to the highpassed Y signal   
X_fft = fft(X_hp, NFFT);       
% save this up to the nyquist limit. 
P2 = abs(X_fft/NFFT);
P1 = P2(1:NFFT/2);
% P1(2:end-1) = 2*P1(2:end-1);
% Define the frequency domain f and plot the single-sided amplitude spectrum P1. The amplitudes are not exactly at 0.7 and 1, as expected, because of the added noise. On average, longer signals produce better frequency approximations.
Fs = 46;
% f = Fs*(0:(NFFT/2))/NFFT;
f = Fs*(1:(NFFT/2))/NFFT;

    figure(1);
    subplot(4,1,3);
    plot(f,P1,'LineWidth',1.5);
    title('Single-Sided Amplitude Spectrum of X(t)');
    xlabel('f (Hz)');
    ylabel('|FFT|');             
    subplot(4,1,4);
    plot(f,10*log10(P1),'LineWidth',1.5);
    xlabel('f (Hz)');
    ylabel('|FFT| (dB)');

% Read MPLAB's output
% X_fftMplab
    figure(1);
    subplot(4,1,3);hold on;
    plot(f,X_fftMplab(:,2),'r','LineWidth',1.5);hold off;
    grid on;

    subplot(4,1,4);hold on;
    plot(f,10*log10(X_fftMplab(:,2)),'r','LineWidth',1.5);hold off;
    legend('MATLAB','PIC Simulation');
    legend('MATLAB','PIC Hardware');
    grid on;