close all;
% Xhw
X = Xhw(:,2);

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
    yMax = max(cat(1,4*X,X_hp))+200;
    yMin = min(cat(1,4*X,X_hp))-200;  

    % Plot signal and low-passed
    figure(1);
    subplot(4,1,1);
    plot(timeStamp,4*X,'LineWidth',1.5);
    grid on; 
    axis([0 length(X) yMin yMax]);
    title('X (Scaled)');  
    
    subplot(4,1,2);           
    plot(timeStamp,X_hp,'LineWidth',1.5);    
    axis([0 length(X) -1500 1500]);    
    title('X High-passed (Scaled  * 4)');    
    grid on;     
    
% Read MPLAB's output
% X_hphw
    figure(1);
    subplot(4,1,2);    
    hold on;      
    jitter = 50;
%     % Simulation
%     plot(timeStamp,X_hpMplab(:,3)+jitter*randn(length(timeStamp),1),'r','LineWidth',1.5);            
    % Hardware
    plot(timeStamp,X_hphw(:,2)+jitter*randn(length(timeStamp),1),'r','LineWidth',1.5);            

% FFT
NFFT = 256;
% Apply FFT per window to the highpassed Y signal   
X_fft = fft(X_hp, NFFT);       
% save this up to the nyquist limit. 
P2 = abs(X_fft/NFFT);
P1 = P2(1:NFFT/2);
% P1(2:end-1) = 2*P1(2:end-1);
% Define the frequency domain f and plot the single-sided amplitude spectrum P1. The amplitudes are not exactly at 0.7 and 1, as expected, because of the added noise. On average, longer signals produce better frequency approximations.
Fs = 96;
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
% X_ffthw
    figure(1);
    subplot(4,1,3);hold on;
    plot(f,X_ffthw(:,2),'r','LineWidth',1.5);hold off;
    grid on;

    subplot(4,1,4);hold on;
    plot(f,10*log10(X_ffthw(:,2)),'r','LineWidth',1.5);hold off;
    legend('MATLAB','PIC Simulation');
    legend('MATLAB','PIC Hardware');
    grid on;