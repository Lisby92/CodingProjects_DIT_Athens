% EROTIMA 1
syms x(t)
x(t) = piecewise(0 <= t < 1, t, ...
                 1 <= t < 3, 1 / (2 * (3 - 2*t)), ...
                 0);
timeDomain = [-3:0.05:6];

hold on;
subplot(311);
x1 = x(timeDomain);
plot(timeDomain, x1, 'r'), grid on;
xlabel('t sec');
ylabel('x(t)');

subplot(312);
x2 = x(timeDomain .* 2);
plot(timeDomain, x2, 'k'), grid on;
xlabel('t sec');
ylabel('x(2*t)');

subplot(313);
x3 = x(timeDomain ./ 2);
plot(timeDomain, x3, 'b'), grid on;
xlabel('t sec');
ylabel('x(t/2)');
hold off;