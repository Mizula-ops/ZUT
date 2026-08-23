function dy = threebody(~,y,G,m1,m2,m3)

x1 = y(1);
y1 = y(2);
vx1 = y(3);
vy1 = y(4);

x2 = y(5);
y2 = y(6);
vx2 = y(7);
vy2 = y(8);

x3 = y(9);
y3 = y(10);
vx3 = y(11);
vy3 = y(12);

r12 = ((x2-x1)^2 + (y2-y1)^2)^(3/2);
r13 = ((x3-x1)^2 + (y3-y1)^2)^(3/2);
r23 = ((x3-x2)^2 + (y3-y2)^2)^(3/2);

ax1 = G*m2*(x2-x1)/r12 + G*m3*(x3-x1)/r13;
ay1 = G*m2*(y2-y1)/r12 + G*m3*(y3-y1)/r13;

ax2 = G*m1*(x1-x2)/r12 + G*m3*(x3-x2)/r23;
ay2 = G*m1*(y1-y2)/r12 + G*m3*(y3-y2)/r23;

ax3 = G*m1*(x1-x3)/r13 + G*m2*(x2-x3)/r23;
ay3 = G*m1*(y1-y3)/r13 + G*m2*(y2-y3)/r23;

dy = [
vx1
vy1
ax1
ay1

vx2
vy2
ax2
ay2

vx3
vy3
ax3
ay3
];
end