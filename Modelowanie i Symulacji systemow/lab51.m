

out = sim("lab5.slx");

r = out.r.Data;
th = out.y.Data;
t=out.r.Time;
l = str2double(get_param('lab5/Subsystem', 'l'));
m = str2double(get_param('lab5/Subsystem', 'm'));

lmax = l + max(r);
figure



for i = 1:3:length(t)
    ls = l + r(i);


    xi = -ls*sin(th(i));
    yi = -ls*cos(th(i));
    
 
    % start, start,kulka,kulka,zwoje,dlugosc,promien
    [xs, ys] = spring(0, 0, xi, yi, 10, 1, 0.5);

    cla
    hold on

    plot(xs, ys, 'r', 'LineWidth', 2)
    plot(xi, yi, 'bo', 'MarkerFaceColor', 'b', 'MarkerSize', 5*m)
    plot(0, 0, 'ko', 'MarkerFaceColor', 'k', 'MarkerSize', 8)

    axis equal
    axis([-1.1*lmax 1.1*lmax -1.1*lmax 1.1*lmax])
    grid on

    drawnow

end