function walfisch_Ikegami_plot()
  %default values to be used below
  f = 1400;%MHz
  hBs = 25;%m
  hMs = 2;%m
  d = 2.51;%km
  phi = 45;%degrees
  b = 35;%m
  hRoof = 21;%m
  flagLOS = 1;%no line of sight
  flagCity = 1;%big city
  if ((f>=800 && f<=2000) && (hBs >= 4 && hBs <= 50) && (hMs >=1 && hMs <= 3) && (d>=0.02 && d <= 5) && (flagLOS == 1 || flagLOS == 0) && (flagCity == 1 || flagCity == 0) && (phi >= 0 && phi <= 90) && (b >= 20 && b <= 50))
    %experiment1
    D = 0.02:0.02:5;
    LdMetroNLOS = zeros(1,length(D));%distance in meters
    LdSCityNLOS = zeros(1, length(D));
    LdLOS = zeros(1,length(D));%distance in meters
    for i = 1:length(D)
      [x ,LdMetroNLOS(1,i)] = walfisch_Ikegami(f, hMs, hBs, D(i), 24, phi, 25, 1, 1);
      [x ,LdSCityNLOS(1,i)] = walfisch_Ikegami(f, hMs, hBs, D(i), 18, phi, 45, 1, 0);
      [x ,LdLOS(1,i)] = walfisch_Ikegami(f, hMs, hBs, D(i), hRoof, phi, b, 0, 1);
    end
    figure(1);
    plot(D, LdMetroNLOS, D, LdSCityNLOS, D, LdLOS), grid on;
    xlabel('Distance(km)');
    ylabel('Path Loss(dB)');
    legend('Metropolis NLOS', 'Small City NLOS', 'LOS', 'Location', 'southeast');
    %experiment2
    F = 800:25:2000;
    LdFLOS = zeros(1, length(F));
    LdFNLOSMetro = zeros(1, length(F));
    LdFNLOSSmallCity = zeros(1, length(F));
    for i = 1:length(F)
      [x, LdFLOS(1,i)] = walfisch_Ikegami(F(i), hMs, hBs, d, hRoof, phi, b, 0, 0);
      [x, LdFNLOSMetro(1,i)] = walfisch_Ikegami(F(i), hMs, hBs, d, hRoof, phi, 25, 1, 1);
      [x, LdFNLOSSmallCity(1,i)] = walfisch_Ikegami(F(i), hMs, hBs, d, hRoof, phi, 45, 1, 0);
    end
    figure(2);
    plot(F, LdFLOS, F, LdFNLOSMetro, F, LdFNLOSSmallCity), grid on;
    xlabel('Frequency(MHz)');
    ylabel('Path Loss(dB)');
    legend('LOS', 'NLOS Metro', 'NLOS Small City','Location', 'southeast');
    %experiment3
    f1 = 800;
    f2 = 1400;
    f3 = 2000;
    LdF1LOS = zeros(1, length(D));
    LdF1NLOSMetro = zeros(1, length(D));
    LdF1NLOSSmallCity = zeros(1, length(D));
    LdF2LOS = zeros(1, length(D));
    LdF2NLOSMetro = zeros(1, length(D));
    LdF2NLOSSmallCity = zeros(1, length(D));
    LdF3LOS = zeros(1, length(D));
    LdF3NLOSMetro = zeros(1, length(D));
    LdF3NLOSSmallCity = zeros(1, length(D));
    for i = 1:length(D)
      [x, LdF1LOS(1,i)] = walfisch_Ikegami(f1, hMs, hBs, D(i), hRoof, phi, b, 0, 0);
      [x, LdF2LOS(1,i)] = walfisch_Ikegami(f2, hMs, hBs, D(i), hRoof, phi, b, 0, 0);
      [x, LdF3LOS(1,i)] = walfisch_Ikegami(f3, hMs, hBs, D(i), hRoof, phi, b, 0, 0);
      [x, LdF1NLOSSmallCity(1,i)] = walfisch_Ikegami(f1, hMs, hBs, D(i), 18, phi, 45, 1, 0);
      [x, LdF2NLOSSmallCity(1,i)] = walfisch_Ikegami(f2, hMs, hBs, D(i), 18, phi, 45, 1, 0);
      [x, LdF3NLOSSmallCity(1,i)] = walfisch_Ikegami(f3, hMs, hBs, D(i), 18, phi, 45, 1, 0);
      [x, LdF1NLOSMetro(1,i)] = walfisch_Ikegami(f1, hMs, hBs, D(i), 24, phi, 25, 1, 1);
      [x, LdF2NLOSMetro(1,i)] = walfisch_Ikegami(f2, hMs, hBs, D(i), 24, phi, 25, 1, 1);
      [x, LdF3NLOSMetro(1,i)] = walfisch_Ikegami(f3, hMs, hBs, D(i), 24, phi, 25, 1, 1);
    end
    figure(3);
    plot(D, LdF1LOS, 'r', D, LdF2LOS ,'r--', D, LdF3LOS, 'r-.', D, LdF1NLOSSmallCity, 'm', D, LdF2NLOSSmallCity, 'm--', D, LdF3NLOSSmallCity, 'm-.', D, LdF1NLOSMetro, 'b', D, LdF2NLOSMetro, 'b--', D, LdF3NLOSMetro, 'b-.'), grid on;
    xlabel('Distance(km)');
    ylabel('Path Loss(dB)');
    legend('800Mhz LOS', '1400Mhz LOS', '2000Mhz LOS', '800Mhz NLOS Small City', '1400Mhz NLOS Small City', '2000Mhz NLOS Small City', '800Mhz NLOS Metro', '1400Mhz NLOS Metro', '2000Mhz NLOS Metro', 'Location', 'southeast');
    %experiment4
    d1 = 0.5;
    d2 = 2.5;
    d3 = 5;
    LdD1LOS = zeros(1, length(F));
    LdD1NLOSMetro = zeros(1, length(F));
    LdD1NLOSSmallCity = zeros(1, length(F));
    LdD2LOS = zeros(1, length(F));
    LdD2NLOSMetro = zeros(1, length(F));
    LdD2NLOSSmallCity = zeros(1, length(F));
    LdD3LOS = zeros(1, length(F));
    LdD3NLOSMetro = zeros(1, length(F));
    LdD3NLOSSmallCity = zeros(1, length(F));
    for i = 1:length(F)
      [x, LdD1LOS(1,i)] = walfisch_Ikegami(F(i), hMs, hBs, d1, hRoof, phi, b, 0, 0);
      [x, LdD2LOS(1,i)] = walfisch_Ikegami(F(i), hMs, hBs, d2, hRoof, phi, b, 0, 0);
      [x, LdD3LOS(1,i)] = walfisch_Ikegami(F(i), hMs, hBs, d3, hRoof, phi, b, 0, 0);
      [x, LdD1NLOSSmallCity(1,i)] = walfisch_Ikegami(F(i), hMs, hBs, d1, 18, phi, 45, 1, 0);
      [x, LdD2NLOSSmallCity(1,i)] = walfisch_Ikegami(F(i), hMs, hBs, d2, 18, phi, 45, 1, 0);
      [x, LdD3NLOSSmallCity(1,i)] = walfisch_Ikegami(F(i), hMs, hBs, d3, 18, phi, 45, 1, 0);
      [x, LdD1NLOSMetro(1,i)] = walfisch_Ikegami(F(i), hMs, hBs, d1, 24, phi, 25, 1, 1);
      [x, LdD2NLOSMetro(1,i)] = walfisch_Ikegami(F(i), hMs, hBs, d2, 24, phi, 25, 1, 1);
      [x, LdD3NLOSMetro(1,i)] = walfisch_Ikegami(F(i), hMs, hBs, d3, 24, phi, 25, 1, 1);
    end
    figure(4);
    plot(F, LdD1LOS, 'r', F, LdD2LOS ,'r--', F, LdD3LOS, 'r-.', F, LdD1NLOSSmallCity, 'm', F, LdD2NLOSSmallCity, 'm--', F, LdD3NLOSSmallCity, 'm-.', F, LdD1NLOSMetro, 'b', F, LdD2NLOSMetro, 'b--', F, LdD3NLOSMetro, 'b-.'), grid on;
    xlabel('Frequency(MHz)');
    ylabel('Path Loss(dB)');
    legend('0.5km LOS', '2.5km LOS', '5km LOS', '0.5km NLOS Small City', '2.5km NLOS Small City', '5km NLOS Small City', '0.5km NLOS Metro', '2.5km NLOS Metro', '5km NLOS Metro', 'Location','northeastoutside');
    %experiment 5
    HBS = 4:1:50;
    LdD1F2NLOSMetro = zeros(1, length(HBS));
    LdD3F2NLOSMetro = zeros(1, length(HBS));
    LdF1D2NLOSMetro = zeros(1, length(HBS));
    LdF2D2NLOSMetro = zeros(1, length(HBS));
    LdF3D2NLOSMetro = zeros(1, length(HBS));
    LdD1F2NLOSSmallCity = zeros(1, length(HBS));
    LdD3F2NLOSSmallCity = zeros(1, length(HBS));
    LdF1D2NLOSSmallCity = zeros(1, length(HBS));
    LdF2D2NLOSSmallCity = zeros(1, length(HBS));
    LdF3D2NLOSSmallCity = zeros(1, length(HBS));
    for i = 1:length(HBS)
      [x, LdD1F2NLOSMetro(1,i)] = walfisch_Ikegami(f2, hMs, HBS(i), d1, 24, phi, 25, 1, 1);
      [x, LdD3F2NLOSMetro(1,i)] = walfisch_Ikegami(f2, hMs, HBS(i), d3, 24, phi, 25, 1, 1);
      [x, LdF1D2NLOSMetro(1,i)] = walfisch_Ikegami(f1, hMs, HBS(i), d2, 24, phi, 25, 1, 1);
      [x, LdF2D2NLOSMetro(1,i)] = walfisch_Ikegami(f2, hMs, HBS(i), d2, 24, phi, 25, 1, 1);
      [x, LdF3D2NLOSMetro(1,i)] = walfisch_Ikegami(f3, hMs, HBS(i), d2, 24, phi, 25, 1, 1);
      [x, LdD1F2NLOSSmallCity(1,i)] = walfisch_Ikegami(f2, hMs, HBS(i), d1, 18, phi, 45, 1, 0);
      [x, LdD3F2NLOSSmallCity(1,i)] = walfisch_Ikegami(f2, hMs, HBS(i), d3, 18, phi, 45, 1, 0);
      [x, LdF1D2NLOSSmallCity(1,i)] = walfisch_Ikegami(f1, hMs, HBS(i), d2, 18, phi, 45, 1, 0);
      [x, LdF2D2NLOSSmallCity(1,i)] = walfisch_Ikegami(f2, hMs, HBS(i), d2, 18, phi, 45, 1, 0);
      [x, LdF3D2NLOSSmallCity(1,i)] = walfisch_Ikegami(f3, hMs, HBS(i), d2, 18, phi, 45, 1, 0);
    end
    figure(5);
    plot(HBS, LdD1F2NLOSMetro, 'r', HBS, LdD3F2NLOSMetro, 'r-.', HBS, LdF1D2NLOSMetro, 'b', HBS, LdF2D2NLOSMetro, 'b--', HBS, LdF3D2NLOSMetro, 'b-.'), grid on;
    xlabel('Metropolis Base Station Height(m)');
    ylabel('NLOS Path Loss(dB)');
    legend('d = 0.5km, f = 1400Mhz', 'd = 5km, f = 1400Mhz', 'd = 2.5km, f = 800Mhz', 'd = 2.5km, f = 1400Mhz', 'd = 2.5km, f = 2000Mhz');
    figure(6);
    plot(HBS, LdD1F2NLOSSmallCity, 'r', HBS, LdD3F2NLOSSmallCity, 'r-.', HBS, LdF1D2NLOSSmallCity, 'b', HBS, LdF2D2NLOSSmallCity, 'b--', HBS, LdF3D2NLOSSmallCity, 'b-.'), grid on;
    xlabel('Small City Base Station Height(m)');
    ylabel('NLOS Path Loss(dB)');
    legend('d = 0.5km, f = 1400Mhz', 'd = 5km, f = 1400Mhz', 'd = 2.5km, f = 800Mhz', 'd = 2.5km, f = 1400Mhz', 'd = 2.5km, f = 2000Mhz');
    %experiment 6
    HROOF = 3:0.5:42;
    LdD1F2NLOSMetroHr = zeros(1, length(HROOF));
    LdD3F2NLOSMetroHr = zeros(1, length(HROOF));
    LdF1D2NLOSMetroHr = zeros(1, length(HROOF));
    LdF2D2NLOSMetroHr = zeros(1, length(HROOF));
    LdF3D2NLOSMetroHr = zeros(1, length(HROOF));
    LdD1F2NLOSSmallCityHr = zeros(1, length(HROOF));
    LdD3F2NLOSSmallCityHr = zeros(1, length(HROOF));
    LdF1D2NLOSSmallCityHr = zeros(1, length(HROOF));
    LdF2D2NLOSSmallCityHr = zeros(1, length(HROOF));
    LdF3D2NLOSSmallCityHr = zeros(1, length(HROOF));
    for i = 1:length(HROOF)
      [x, LdD1F2NLOSMetroHr(1,i)] = walfisch_Ikegami(f2, hMs, hBs, d1, HROOF(i), phi, 25, 1, 1);
      [x, LdD3F2NLOSMetroHr(1,i)] = walfisch_Ikegami(f2, hMs, hBs, d3, HROOF(i), phi, 25, 1, 1);
      [x, LdF1D2NLOSMetroHr(1,i)] = walfisch_Ikegami(f1, hMs, hBs, d2, HROOF(i), phi, 25, 1, 1);
      [x, LdF2D2NLOSMetroHr(1,i)] = walfisch_Ikegami(f2, hMs, hBs, d2, HROOF(i), phi, 25, 1, 1);
      [x, LdF3D2NLOSMetroHr(1,i)] = walfisch_Ikegami(f3, hMs, hBs, d2, HROOF(i), phi, 25, 1, 1);
      [x, LdD1F2NLOSSmallCityHr(1,i)] = walfisch_Ikegami(f2, hMs, hBs, d1, HROOF(i), phi, 45, 1, 0);
      [x, LdD3F2NLOSSmallCityHr(1,i)] = walfisch_Ikegami(f2, hMs, hBs, d3, HROOF(i), phi, 45, 1, 0);
      [x, LdF1D2NLOSSmallCityHr(1,i)] = walfisch_Ikegami(f1, hMs, hBs, d2, HROOF(i), phi, 45, 1, 0);
      [x, LdF2D2NLOSSmallCityHr(1,i)] = walfisch_Ikegami(f2, hMs, hBs, d2, HROOF(i), phi, 45, 1, 0);
      [x, LdF3D2NLOSSmallCityHr(1,i)] = walfisch_Ikegami(f3, hMs, hBs, d2, HROOF(i), phi, 45, 1, 0);
    end
    figure(7);
    plot(HROOF, LdD1F2NLOSMetroHr, 'r', HROOF, LdD3F2NLOSMetroHr, 'r-.', HROOF, LdF1D2NLOSMetroHr, 'b', HROOF, LdF2D2NLOSMetroHr, 'b--', HROOF, LdF3D2NLOSMetroHr, 'b-.'), grid on;
    xlabel('Metropolis Roof Height(m)');
    ylabel('NLOS Path Loss(dB)');
    legend('d = 0.5km, f = 1400Mhz', 'd = 5km, f = 1400Mhz', 'd = 2.5km, f = 800Mhz', 'd = 2.5km, f = 1400Mhz', 'd = 2.5km, f = 2000Mhz', 'Location', 'northwest');
    figure(8);
    plot(HROOF, LdD1F2NLOSSmallCityHr, 'r', HROOF, LdD3F2NLOSSmallCityHr, 'r-.', HROOF, LdF1D2NLOSSmallCityHr, 'b', HROOF, LdF2D2NLOSSmallCityHr, 'b--', HROOF, LdF3D2NLOSSmallCityHr, 'b-.'), grid on;
    xlabel('Small City Roof Height(m)');
    ylabel('NLOS Path Loss(dB)');
    legend('d = 0.5km, f = 1400Mhz', 'd = 5km, f = 1400Mhz', 'd = 2.5km, f = 800Mhz', 'd = 2.5km, f = 1400Mhz', 'd = 2.5km, f = 2000Mhz', 'Location', 'northwest');
    %experiment 7
    PHI = 0:0.25:90;
    LdD1F2NLOSMetroPHI = zeros(1, length(PHI));
    LdD3F2NLOSMetroPHI = zeros(1, length(PHI));
    LdF1D2NLOSMetroPHI = zeros(1, length(PHI));
    LdF2D2NLOSMetroPHI = zeros(1, length(PHI));
    LdF3D2NLOSMetroPHI = zeros(1, length(PHI));
    LdD1F2NLOSSmallCityPHI = zeros(1, length(PHI));
    LdD3F2NLOSSmallCityPHI = zeros(1, length(PHI));
    LdF1D2NLOSSmallCityPHI = zeros(1, length(PHI));
    LdF2D2NLOSSmallCityPHI = zeros(1, length(PHI));
    LdF3D2NLOSSmallCityPHI = zeros(1, length(PHI));
    for i = 1:length(PHI)
      [x, LdD1F2NLOSMetroPHI(1,i)] = walfisch_Ikegami(f2, hMs, hBs, d1, 24, PHI(i), 25, 1, 1);
      [x, LdD3F2NLOSMetroPHI(1,i)] = walfisch_Ikegami(f2, hMs, hBs, d3, 24, PHI(i), 25, 1, 1);
      [x, LdF1D2NLOSMetroPHI(1,i)] = walfisch_Ikegami(f1, hMs, hBs, d2, 24, PHI(i), 25, 1, 1);
      [x, LdF2D2NLOSMetroPHI(1,i)] = walfisch_Ikegami(f2, hMs, hBs, d2, 24, PHI(i), 25, 1, 1);
      [x, LdF3D2NLOSMetroPHI(1,i)] = walfisch_Ikegami(f3, hMs, hBs, d2, 24, PHI(i), 25, 1, 1);
      [x, LdD1F2NLOSSmallCityPHI(1,i)] = walfisch_Ikegami(f2, hMs, hBs, d1, 18, PHI(i), 45, 1, 0);
      [x, LdD3F2NLOSSmallCityPHI(1,i)] = walfisch_Ikegami(f2, hMs, hBs, d3, 18, PHI(i), 45, 1, 0);
      [x, LdF1D2NLOSSmallCityPHI(1,i)] = walfisch_Ikegami(f1, hMs, hBs, d2, 18, PHI(i), 45, 1, 0);
      [x, LdF2D2NLOSSmallCityPHI(1,i)] = walfisch_Ikegami(f2, hMs, hBs, d2, 18, PHI(i), 45, 1, 0);
      [x, LdF3D2NLOSSmallCityPHI(1,i)] = walfisch_Ikegami(f3, hMs, hBs, d2, 18, PHI(i), 45, 1, 0);
    end
    figure(9);
    plot(PHI, LdD1F2NLOSMetroPHI, 'r', PHI, LdD3F2NLOSMetroPHI, 'r-.', PHI, LdF1D2NLOSMetroPHI, 'b', PHI, LdF2D2NLOSMetroPHI, 'b--', PHI, LdF3D2NLOSMetroPHI, 'b-.'), grid on;
    xlabel('Metropolis phi angle(degrees)');
    ylabel('NLOS Path Loss(dB)');
    legend('d = 0.5km, f = 1400Mhz', 'd = 5km, f = 1400Mhz', 'd = 2.5km, f = 800Mhz', 'd = 2.5km, f = 1400Mhz', 'd = 2.5km, f = 2000Mhz', 'Location', 'northeastoutside');
    figure(10);
    plot(PHI, LdD1F2NLOSSmallCityPHI, 'r', PHI, LdD3F2NLOSSmallCityPHI, 'r-.', PHI, LdF1D2NLOSSmallCityPHI, 'b', PHI, LdF2D2NLOSSmallCityPHI, 'b--', PHI, LdF3D2NLOSSmallCityPHI, 'b-.'), grid on;
    xlabel('Small City phi angle(degrees)');
    ylabel('NLOS Path Loss(dB)');
    legend('d = 0.5km, f = 1400Mhz', 'd = 5km, f = 1400Mhz', 'd = 2.5km, f = 800Mhz', 'd = 2.5km, f = 1400Mhz', 'd = 2.5km, f = 2000Mhz', 'Location', 'northeastoutside');
    %final experiment/conclusion plot
    ff1 = 1000;
    ff2 = 1800;
    ff = 1400;
    fphi1 = 30;
    fphi2 = 60;
    fphi = 45;
    fhroof1 = 12;
    fhroof2 = 24;
    fhroof = 18;
    fhroofhigh = 26;
    FLdF1LOS = zeros(1,length(D));
    FLdF2LOS = zeros(1,length(D));
    FLdF1NLOSSmall = zeros(1,length(D));
    FLdF2NLOSSmall = zeros(1,length(D));
    FLdF1NLOSMetro = zeros(1,length(D));
    FLdF2NLOSMetro = zeros(1,length(D));
    FLdFNLOSSmallPhi1 = zeros(1,length(D));
    FLdFNLOSSmallPhi2 = zeros(1,length(D));
    FLdFNLOSMetroPhi1 = zeros(1,length(D));
    FLdFNLOSMetroPhi2 = zeros(1,length(D));
    FLdFNLOSSmallPhiHroof1 = zeros(1,length(D));
    FLdFNLOSSmallPhiHroof2 = zeros(1,length(D));
    FLdFNLOSMetroPhiHroof1 = zeros(1,length(D));
    FLdFNLOSMetroPhiHroof2 = zeros(1,length(D));
    FLdFNLOSSmallPhiHroofHBSlow = zeros(1,length(D));
    FLdFNLOSMetroPhiHroofHBSlow = zeros(1,length(D));
    for i = 1:length(D)
      [x, FLdF1LOS(1,i)] = walfisch_Ikegami(ff1, hMs, hBs, D(i), fhroof, fphi, b, 0, 0);
      [x, FLdF2LOS(1,i)] = walfisch_Ikegami(ff2, hMs, hBs, D(i), fhroof, fphi, b, 0, 0);
      [x, FLdF1NLOSSmall(1,i)] = walfisch_Ikegami(ff1, hMs, hBs, D(i), fhroof, fphi, 45, 1, 0);
      [x, FLdF2NLOSSmall(1,i)] = walfisch_Ikegami(ff2, hMs, hBs, D(i), fhroof, fphi, 45, 1, 0);
      [x, FLdF1NLOSMetro(1,i)] = walfisch_Ikegami(ff1, hMs, hBs, D(i), fhroof, fphi, 25, 1, 1);
      [x, FLdF2NLOSMetro(1,i)] = walfisch_Ikegami(ff2, hMs, hBs, D(i), fhroof, fphi, 25, 1, 1);
      [x, FLdFNLOSSmallPhi1(1,i)] = walfisch_Ikegami(ff, hMs, hBs, D(i), fhroof, fphi1, 45, 1, 0);
      [x, FLdFNLOSSmallPhi2(1,i)] = walfisch_Ikegami(ff, hMs, hBs, D(i), fhroof, fphi2, 45, 1, 0);
      [x, FLdFNLOSMetroPhi1(1,i)] = walfisch_Ikegami(ff, hMs, hBs, D(i), fhroof, fphi1, 25, 1, 1);
      [x, FLdFNLOSMetroPhi2(1,i)] = walfisch_Ikegami(ff, hMs, hBs, D(i), fhroof, fphi2, 25, 1, 1);
      [x, FLdFNLOSSmallPhiHroof1(1,i)] = walfisch_Ikegami(ff, hMs, hBs, D(i), fhroof1, fphi, 45, 1, 0);
      [x, FLdFNLOSSmallPhiHroof2(1,i)] = walfisch_Ikegami(ff, hMs, hBs, D(i), fhroof2, fphi, 45, 1, 0);
      [x, FLdFNLOSMetroPhiHroof1(1,i)] = walfisch_Ikegami(ff, hMs, hBs, D(i), fhroof1, fphi, 25, 1, 1);
      [x, FLdFNLOSMetroPhiHroof2(1,i)] = walfisch_Ikegami(ff, hMs, hBs, D(i), fhroof2, fphi, 25, 1, 1);
      [x, FLdFNLOSSmallPhiHroofHBSlow(1,i)] = walfisch_Ikegami(ff, hMs, hBs, D(i), fhroofhigh, fphi, 45, 1, 0);
      [x, FLdFNLOSMetroPhiHroofHBSlow(1,i)] = walfisch_Ikegami(ff, hMs, hBs, D(i), fhroofhigh, fphi, 25, 1, 1);
    end
    figure(11);
    plot(D, FLdF1LOS, 'c', D, FLdF2LOS, 'c--', D, FLdF1NLOSSmall, 'b', D, FLdF2NLOSSmall, 'b--', D, FLdF1NLOSMetro, 'b:', D, FLdF2NLOSMetro, 'b-.', ...
         D, FLdFNLOSSmallPhi1, 'm', D, FLdFNLOSSmallPhi2, 'm--', D, FLdFNLOSMetroPhi1, 'm:', D, FLdFNLOSMetroPhi2, 'm-.', ...
         D, FLdFNLOSSmallPhiHroof1, 'r', D, FLdFNLOSSmallPhiHroof2, 'r--', D, FLdFNLOSMetroPhiHroof1, 'r:', D, FLdFNLOSMetroPhiHroof2, 'r-.', ...
         D, FLdFNLOSSmallPhiHroofHBSlow, 'k', D, FLdFNLOSMetroPhiHroofHBSlow, 'k--'), grid on;
    xlabel('Distance(km)');
    ylabel('Path Loss(dB)');
    legend('LOS 1000MHz', 'LOS 1800MHz', ['Small City 1000MHz, 45' char(176) 'roof 18m'], ['Small City 1800MHz, 45' char(176) 'roof 18m'], ['Metropolis 1000MHz, 45' char(176) 'roof 18m'], ['Metropolis 1800MHz, 45' char(176) 'roof 18m'], ...
           ['Small City 1400MHz, 30' char(176)  'roof 18m'], ['Small City 1400MHz, 60' char(176) 'roof 18m'], ['Metropolis 1400MHz, 30' char(176) 'roof 18m'], ['Metropolis 1400MHz, 60' char(176) 'roof 18m'], ...
           ['Small City 1400MHz, 45' char(176) ', roof 12m'], ['Small City 1400MHz, 45' char(176) ', roof 24m'], ['Metropolis 1400MHz, 45' char(176) ', roof 12m'], ['Metropolis 1400MHz, 45' char(176) ', roof 24m'], ...
           ['Small City 1400MHz, 45' char(176) ', roof 26m'], ['Metropolis 1400MHz, 45' char(176) ', roof 26m'] ,'Location', 'southeast');
  end
end