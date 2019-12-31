for key = 0:12 %play all the key sounds and plot the graphs each second
  tone = ss_dtmf1(key,0.025,1/8000);
  plot(0:1/8000:0.025, tone), grid on;
  xlabel('t msec');
  ylabel(['key ', num2str(key), ' signal']);
  sound(tone);
  pause(1);
end