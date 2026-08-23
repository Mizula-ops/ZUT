clc 
clear all
close all
load('lab11_data2.mat')
Z= data2(:,3:6)';
X=data2(:,1:2)';
Rs=X*Z';
Ps=Z*Z';
C=Rs*Ps^-1;

Z=[data2(:,3:6)'; data2(:,3:6)'.^2];
X=data2(:,1:2)';

Rs=X*Z';
Ps=Z*Z';
C2=Rs*Ps^-1;
load('lab11_data1.mat');
xpp = data1(:,3:end)*C(1,:)';
ypp=data1(:,3:end)*C(2,:)';
xpp_true=data1(:,1);
ypp_true=data1(:,2);

mse_x = mean((xpp_true-xpp).^2);
mse_y=mean((ypp_true-ypp).^2);

mse_lin=(mse_x+mse_y)/2;

Ztest = [data1(:,3:6) data1(:,3:6).^2];
xpp2 = Ztest * C2(1,:)';
ypp2 = Ztest * C2(2,:)';
mse_x2 = mean((xpp_true - xpp2).^2);
mse_y2 = mean((ypp_true - ypp2).^2);

mse_nonlin = (mse_x2 + mse_y2)/2;
h = 2^-8;
t = 500;
N = round(t/h);

xp = data1(1,3);
yp = data1(1,4);
x  = data1(1,5);
y  = data1(1,6);

x_sim = zeros(1,N);
y_sim = zeros(1,N);

for k = 1:N
    Z = [xp; yp; x; y; xp^2; yp^2; x^2; y^2];

    xpp = C2(1,:) * Z;
    ypp = C2(2,:) * Z;

    xp = xp + h*xpp;
    yp = yp + h*ypp;

    x = x + h*xp;
    y = y + h*yp;

    x_sim(k) = x;
    y_sim(k) = y;
end

figure

plot(data1(:,5), data1(:,6))

title('Trajektoria z data1')

xlabel('x')

ylabel('y')

grid on

axis equal
axis([-60 60 -60 60])
figure

plot(x_sim, y_sim,'r')

title('Trajektoria z modelu Eulera')

xlabel('x')

ylabel('y')

grid on

axis equal  
axis([-60 60 -60 60])

