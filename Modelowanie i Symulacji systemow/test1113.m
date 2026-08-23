clear
close all
clc

out = sim('threeBodyProblem.slx');

x1 = out.x1.Data; 
y1 = out.y1.Data;
x2 = out.x2.Data; 
y2 = out.y2.Data;
x3 = out.x3.Data; 
y3 = out.y3.Data;
t  = out.x1.Time;

allx = [x1; x2; x3];
ally = [y1; y2; y3];

xmin = min(allx);
xmax = max(allx);
ymin = min(ally);
ymax = max(ally);

margines = 0.1 * max(xmax-xmin, ymax-ymin);

figure

for i = 1:1:length(t)

    clf
    hold on
    grid on
    axis equal

    plot(x1(1:i), y1(1:i), 'r')
    plot(x2(1:i), y2(1:i), 'g')
    plot(x3(1:i), y3(1:i), 'b')

    plot(x1(i), y1(i), 'ro', 'MarkerFaceColor','r', 'MarkerSize',12)
    plot(x2(i), y2(i), 'go', 'MarkerFaceColor','g', 'MarkerSize',12)
    plot(x3(i), y3(i), 'bo', 'MarkerFaceColor','b', 'MarkerSize',12)

    % xlim([xmin-margines xmax+margines])
    % ylim([ymin-margines ymax+margines])
    xlim([-5 5])
    ylim([-5 5])
    title(['t = ' num2str(t(i),'%.2f') ' s'])

    drawnow
end