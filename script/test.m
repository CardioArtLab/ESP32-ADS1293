%% Load 
clear all;
load dataset.mat;

%% Plot
clc;  close all;
data = double(ch1);
plot(data);

%% 
plot_fft(data, 320)

%% Sgolay
f1 = smooth(data, 21, 'sgolay', 5);
plot((f1/hex2dec('c35000')-0.5)*12/35);

%% 
plot_fft(f1/hex2dec('800000')-0.5, 320);