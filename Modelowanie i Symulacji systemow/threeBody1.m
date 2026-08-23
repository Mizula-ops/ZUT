clear
close all
clc

G = 1;

m1 = 1;
m2 = 1;
m3 = 1;

y0 = [
     0.97000436
     0.24308753
     0.466203685
    -0.43236573

     0
     0
    -0.93240737
     0.86473146

    -0.97000436
    -0.24308753
     0.466203685
    -0.43236573
];

tspan = [0 20];

[t,y] = ode45(@(t,y) threebody(t,y,G,m1,m2,m3), tspan, y0);

x1 = y(:,1);
y1 = y(:,2);

x2 = y(:,5);
y2 = y(:,6);

x3 = y(:,9);
y3 = y(:,10);

figure
hold on
axis equal
grid on

plot(x1,y1,'r','LineWidth',2)
plot(x2,y2,'g','LineWidth',2)
plot(x3,y3,'b','LineWidth',2)

legend('Ciało 1','Ciało 2','Ciało 3')
title('Problem trzech ciał - ode45')