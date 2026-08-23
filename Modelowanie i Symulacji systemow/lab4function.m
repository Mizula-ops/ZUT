out = sim('lab4');

data = out.orbita.Data;
time = out.orbita.Time;

x = data(:,1);
y = data(:,2);

R = str2double(get_param('lab4/Subsystem','R'));
Rs = str2double(get_param('lab4/Subsystem','Rs'));

figure;
grid on;

for i = 1:length(time)

    cla;

    rectangle('Position',[-R -R 2*R 2*R],'Curvature',[1 1], 'FaceColor','g');
    hold on;

    plot(x(1:i), y(1:i), 'r');

    rectangle('Position',[x(i)-Rs, y(i)-Rs, 2*Rs, 2*Rs],'Curvature',[1 1], 'FaceColor','b');

    hold off;

    axis equal;
    xlim([-10 10]);
    ylim([-10 10]);

    drawnow;
end