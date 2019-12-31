function [PL, PLdB] = walfisch_Ikegami(f, hMs, hBs, d, hRoof, phi, b, flagLOS, flagCity)
  if ((f>=800 && f<=2000) && (hBs >= 4 && hBs <= 50) && (hMs >=1 && hMs <= 3) && (d>=0.02 && d <= 5) && (flagLOS == 1 || flagLOS == 0) && (flagCity == 1 || flagCity == 0) && (phi >= 0 && phi <= 90) && (b >= 20 && b <= 50))
    w = b/2;
    if(flagLOS == 0)%LOS
      PLdB = 42.6 + 26*log10(d) + 20*log10(f);
    elseif(flagLOS == 1)%NLOS
      if(hBs > hRoof)
        Lbsh = -18*log10(1 + (hBs - hRoof));
      else
        Lbsh = 0;
      end
      
      if(hBs > hRoof)
        ka = 54;
      elseif(hBs<= hRoof && d >= 0.5)
        ka = 54 - 0.8*(hBs - hRoof);
      else
        ka = 54 - 0.8*(hBs - hRoof)*d*2;
      end
      
      if(hBs > hRoof)
        kd = 18;
      else
        kd = 18 - (15 *(hBs - hRoof)/hRoof);
      end
      
      if(flagCity == 0)%small cities
        kf = -4 + 0.7*(f/925 - 1);
      else%metropolies
        kf = -4 + 1.5*(f/925 - 1);
      end
      
      if(phi < 35)
        LORI = -10 + 0.354*phi;
      elseif(phi < 55)
        LORI = 2.5 + 0.075*(phi-35);
      else
        LORI = 4.0 - 0.114*(phi-55);
      end
      
      LRTS = -8.2 - 10*log10(w) + log10(f) + 20*log10(hRoof-hMs) + LORI;
      
      LFS = 32.45 + 20 * log10(f) + 20*log10(d);
      
      LMSD = Lbsh + ka + kd*log10(d) + kf*log10(f) - 9*log10(b);
      
      if((LRTS + LMSD) >= 0)
        PLdB = LFS + LRTS + LMSD;
      else
        PLdB = LFS;
      end
    end
    PL=10^(PLdB/10);
  else%input variables out of model's range
    PL=-1;
    PLdB=100000000000;
  end
end