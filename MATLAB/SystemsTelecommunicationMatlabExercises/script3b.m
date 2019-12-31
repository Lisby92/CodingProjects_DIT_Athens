% EROTIMA 3
number = [1 1 1 5 2 0 1 2 0 0 1 0 6];
dt = 1;
nd = 0.2;
np = 0.08;
call = ss_dtmf(number, dt, nd, np);
sound(call);