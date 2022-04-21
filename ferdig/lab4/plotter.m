


travel = plot3.ans(2,:)*pi/180;
t = 0:delta_t:delta_t*(length(u)-1);
t1 = plot3.ans(1,:);

plot1 = load('pref.mat');


travel1 = plot1.ans(2, :)*pi/180;
tq1 = plot1.ans(1, :);

travel2 = plot2.ans(2, :)*pi/180;
tq2 = plot2.ans(1, :);

travel3 = plot3.ans(2, :)*pi/180;
tq3 = plot3.ans(1, :);


figure(3)
title('Predicted vs. measured travel comparison')
subplot(511)
stairs(t,u, 'b'),grid
ylabel('u')
ylim([-1 1])
subplot(512)
plot(t,x1, 'b', t1,travel),grid
legend('predicted', 'measured')
ylabel('Travel [rad]')
subplot(513)
plot(t,x2,'m',t,x2', 'b'),grid
ylabel('r')
subplot(514)
plot(t,x3,'m',t,x3, 'b'),grid
ylabel('p')
subplot(515)
plot(t,x4,'m',t,x4', 'b'),grid
xlabel('Time [s]'),ylabel('pdot')

figure(4)
plot(tq1, travel1);
hold on
plot(tq2, travel2);
plot(tq3, travel3);
hold off
xlim([0 17])
ylabel('Travel [rad]')
xlabel('Time [s]')
title('Travel with different q-weight comparison')
legend('q=0.1', 'q=1', 'q=10');
grid

