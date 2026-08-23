clc;
clear;
close all;

out = sim('simulink8');

x = out.x.Data;
y = out.y.Data;

m  = str2double(get_param('simulink8/Subsystem','m'));
l  = str2double(get_param('simulink8/Subsystem','l'));

xi = str2num(get_param('simulink8/Subsystem','xi'));
yi = str2num(get_param('simulink8/Subsystem','yi'));
ai = str2num(get_param('simulink8/Subsystem','ai'));
di = str2num(get_param('simulink8/Subsystem','di'));

figure;

for i = 1:2:length(x)

    a = sqrt(x(i)^2 + y(i)^2);
    b = sqrt(l^2 - a^2);
    z = l - b;

    plot3(x(i), y(i), z, 'b.', 'MarkerSize', 20*m);
    hold on;

    plot3([0 x(i)], [0 y(i)], [l z], 'b', 'LineWidth', 2);

    plot3(xi, yi, -di, 'r.', 'MarkerSize', 20);

    hold off;
    grid on;
    axis([-l l -l l -1 l+1]);
    xlabel('x');
    ylabel('y');
    zlabel('z');
    
    pause(0.01);
end