% Zadanie 1
x=-3:0.5:3;
y=[-0.,-0.3,0.6,1,0.3,-0.3,0,0.3,-0.4,-1,-0.6,0.3,0.2];
figure;
hold on;
scatter(x,y)
plot(x,y);
hold off;
%Zadanie 2
x1=0.25;
y1=0.2;

yn=interp1(x,y,x1,'nearest');
yl=interp1(x,y,x1,'linear');
ys=interp1(x,y,x1,'spline');
yc=interp1(x,y,x1,'cubic');
score1=abs(yn-y1);
score2=abs(yl-y1);
score3=abs(ys-y1);
score4=abs(yc-y1);
disp([score1,score2,score3,score4])
x1=-2.75;
y1=-0.3;
yn=interp1(x,y,x1,'nearest');
yl=interp1(x,y,x1,'linear');
ys=interp1(x,y,x1,'spline');
yc=interp1(x,y,x1,'cubic');
score1=abs(yn-y1);
score2=abs(yl-y1);
score3=abs(ys-y1);
score4=abs(yc-y1);
disp([score1,score2,score3,score4])
% Najlepsza metoda cubic
% Zadanie 3
xx=linspace(-3,3,1000);
yn=interp1(x,y,xx,'nearest');
yl=interp1(x,y,xx,'linear');
ys=interp1(x,y,xx,'spline');
yc=interp1(x,y,xx,'cubic');
figure;
subplot(2,2,1)
hold on;
scatter(x,y)
plot(xx,yn)
hold off;
title('Nearest')
subplot(2,2,2)
hold on;
scatter(x,y)
plot(xx,yl)
hold off;
title('Linear')
subplot(2,2,3)
hold on;
scatter(x,y)
plot(xx,ys)
hold off;
title('Spline')
subplot(2,2,4)
hold on;
scatter(x,y)
plot(xx,yc)
title('Cubic')
hold off;

%Cubic te wartosci sa blizej prawdziwych ale spline też dobrze i gładszy
%wykres
% Zadanie 4

p=polyfit(x,y,9);
yp=polyval(p,xx);
figure;
subplot(2,2,1)
hold on;
scatter(x,y)
plot(xx,yp);
plot(x,y);
title('Stopien 9')
hold off;
subplot(2,2,2)
p=polyfit(x,y,10);
yp=polyval(p,xx);
hold on;
scatter(x,y)
plot(xx,yp);
plot(x,y);
title('Stopien 10')
hold off;
subplot(2,2,3)
p=polyfit(x,y,11);
yp=polyval(p,xx);
hold on;
scatter(x,y)
plot(xx,yp);
plot(x,y);
title('Stopien 11')
hold off;
subplot(2,2,4)
p=polyfit(x,y,12);
yp=polyval(p,xx);
hold on;
scatter(x,y)
plot(xx,yp);
plot(x,y);
title('Stopien 12')
hold off;

% Stopien Wielomianu 9 jest najlepszy 10,11 wartosci brzegowe zaczynaja
% uciekac
p1=polyfit(x,y,9);
yp=polyval(p1,0.25);
score4=abs(0.2-yp);
disp(score4);
yp=polyval(p1,-2.75);
score4=abs(-.3-yp);
disp(score4);

