clear
close all

out = sim('simulink6.slx');
start = str2num(get_param('simulink6/Subsystem','r0'));
x = out.x.Data;
y = out.y.Data;
t = out.x.Time;

r = sqrt(x.^2 + y.^2);
  


xP = start/2;
yP = 0;

xL = -start/2;    
yL = 0;

lmax = 5;

figure

for i = 1:1:length(t)

 
    x1 = xP - x(i);
    y1 = yP + y(i);

    x2 = xL;
    y2 = yL - r(i);

    cla
    hold on
    
    % gorna
    plot([xL xP], [yL yP], 'k', 'LineWidth', 2)
    % lewa
    plot([xL x2], [yL y2], 'k', 'LineWidth', 2)
    % prawa
    plot([xP x1], [yP y1], 'r', 'LineWidth', 2)

   % laczniki poziomej
    plot(xL, yL, 'ko', 'MarkerFaceColor', 'k', 'MarkerSize', 10)
    plot(xP, yP, 'ko', 'MarkerFaceColor', 'k', 'MarkerSize', 10)

    % kwadrat
    rectangle('Position', [x2-0.15 y2-0.15 0.3 0.3],'FaceColor', 'b')
    % kulka
    plot(x1, y1, 'ro', 'MarkerFaceColor', 'r', 'MarkerSize', 10)

    axis equal
    axis([-lmax lmax -lmax lmax])
    grid on
    drawnow
end