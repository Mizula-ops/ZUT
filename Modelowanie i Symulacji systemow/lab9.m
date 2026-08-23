clc;
clear;
close all;

out = sim('simulink9');

h1 = out.x.Data;
h2 = out.y.Data;



S1 = str2num(get_param('simulink9/Subsystem/Subsystem','S1'));
S2 = str2num(get_param('simulink9/Subsystem/Subsystem','S2'));
Swy1 = str2num(get_param('simulink9/Subsystem/Subsystem','Swy1'));
Swy2 = str2num(get_param('simulink9/Subsystem/Subsystem','Swy2'));

h1max = max(h1) + 1;
h2max = max(h2) + 1;
