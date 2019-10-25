function [ fft_data ] = plot_fft( data, FS )
%PLOT_FFT plot FFT of data
L = length(data);
fft_data = abs(fft(data)/L);
fft_data = fft_data(1:L/2+1);
fft_data(2:end-1) = 2*fft_data(2:end-1);
f = FS*(0:(L/2))/L;
plot(f,fft_data);
end

