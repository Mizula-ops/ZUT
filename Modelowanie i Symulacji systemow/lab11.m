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

for i=1:10:length(h1)
    fill([0,S1,S1,0], [0, 0, h1(i), h1(i)], [135/255, 205/255, 235/255],'EDGECOLOR', 'None');
    hold on;
    fill([S1,S1+1,S1+1,S1], [0, 0, Swy1, Swy1], [135/255, 205/255, 235/255],'EDGECOLOR', 'None');
    hold on;
    fill([S1+S2+1,S1+S2+2,S1+S2+2,S1+S2+1], [0, 0, Swy2, Swy2], [135/255, 205/255, 235/255],'EDGECOLOR', 'None');
    hold on;
    fill([S1+1,S1+S2+1,S1+S2+1,S1+1], [0, 0, h2(i), h2(i)], [135/255, 205/255, 235/255],'EDGECOLOR', 'None');
    hold on;
    
    plot([0 0], [0, h1max], 'k');
    hold on;
    plot([0 S1+S2+2], [0, 0], 'k')
    hold on;
    plot([S1 S1], [Swy1, h1max], 'k')
    hold on;
    plot([S1 S1+1], [Swy1, Swy1], 'k')
    hold on;
    plot([S1+S2+1 S1+S2+2], [Swy2, Swy2], 'k')
    hold on;
    plot([S1+1 S1+1], [Swy1, h2max], 'k')
    hold on;
    plot([S1+S2+1 S1+S2+1], [Swy1, h2max], 'k')

    hold off;
    axis([-1, S1+S2+3, -1, h1max+1]);
    pause(0.05);
end
