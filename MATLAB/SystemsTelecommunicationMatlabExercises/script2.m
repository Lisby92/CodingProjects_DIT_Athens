% EROTIMA 2
syms x(t);
x(t) = piecewise(0 <= t <= 1, t, ...
                 0);
syms v(t);
v(t) = x(-t);
syms y(t);
y(t) = v(t-2);
syms w(t);
w(t) = x(t-2);
syms z(t);
z(t) = w(-t);

timeDomain = [-4:0.05:4];

hold on;
subplot(211);
y1 = y(timeDomain);
plot(timeDomain, y1, 'b'), grid on;
xlabel('t sec');
ylabel('y(t)');

subplot(212);
z1 = z(timeDomain);
plot(timeDomain, z1, 'r'), grid on;
xlabel('t sec');
ylabel('z(t)');
hold off;