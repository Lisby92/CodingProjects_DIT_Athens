function y = ss_dtmf(number, dt, nd, np)
  y = [];
  Td = nd * dt;
  Tp = np * dt;
  for key = 1:length(number)
    tone = ss_dtmf1(number(key), Td, 1/8000);
    y = cat(2, y, tone); 
    pause = ss_dtmf1(12, Tp, 1/8000);
    y = cat(2, y, pause);
  end
end