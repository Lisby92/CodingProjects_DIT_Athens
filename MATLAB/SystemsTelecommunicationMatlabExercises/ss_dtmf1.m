function x = ss_dtmf1(key, t, Ts)
  if key == 1
    f1 = 697;
    f2 = 1209;
  elseif key == 2
    f1 = 697;
    f2 = 1336;
  elseif key == 3
    f1 = 697;
    f2 = 1477;
  elseif key == 4
    f1 = 770;
    f2 = 1209;
  elseif key == 5
    f1 = 770;
    f2 = 1336;
  elseif key == 6
    f1 = 770;
    f2 = 1477;
  elseif key == 7
    f1 = 852;
    f2 = 1209;
  elseif key == 8
    f1 = 852;
    f2 = 1336;
  elseif key == 9
    f1 = 852;
    f2 = 1477;
  elseif key == 0
    f1 = 941;
    f2 = 1336;
  elseif key == 10
    f1 = 941;
    f2 = 1209;
  elseif key == 11
    f1 = 941;
    f2 = 1477;
  elseif key == 12
    f1 = 0;
    f2 = 0;
  end
  time = 0:Ts:t;
  x = sin(2 * pi * f1 * time) + sin(2 * pi * f2 * time);
end