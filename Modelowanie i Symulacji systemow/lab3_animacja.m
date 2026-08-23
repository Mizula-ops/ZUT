l = 3;
theta = out.y.Data;
t = out.y.Time;

figure;

for i = 1:length(t)
    x = -l * sin(theta(i));
    y = -l * cos(theta(i));

    cla;
    line([0 x], [0 y], 'LineWidth', 1);
    hold on
    plot(x, y, 'o', 'MarkerSize', 10, 'MarkerFaceColor', 'b');
    hold off

    xlim([-4 4]);
    ylim([-4 1]);

    drawnow;
    % pause(0.05);
end


-(M*G*u(2))/((u(1)^2+u(2)^2)^(3/2))