clc;
clear;
close all;
out=sim('simulink7');
th1=out.fi1.Data;
th2=out.fi2.Data;
t = out.fi1.Time;
l1=str2num(get_param('simulink7/Subsystem','l1'));
l2=str2num(get_param('simulink7/Subsystem','l2'));
m2=str2num(get_param('simulink7/Subsystem','m2'));
m1=str2num(get_param('simulink7/Subsystem','m1'));
lmax=(l1+l2)*1.1;
for i = 1:2:length(t)
    x1 = -l1*sin(th1(i));
    y1 = -l1*cos(th1(i));
    x2 = -l2 * sin(th2(i)) + x1;
    y2 = -l2 * cos(th2(i)) + y1;

    plot([0 x1],[0 y1],'Color','r','LineWidth',2);
    hold on;
    plot(x1, y1, 'b.','MarkerSize',15*m1);
    
    plot([x1 x2],[y1 y2],'Color','r','LineWidth',2);
    plot(x2, y2, 'b.','MarkerSize',15*m2);

    hold off;
    axis equal
    axis([-lmax lmax -lmax lmax])
    grid on
 
    drawnow


end