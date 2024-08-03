C0 = 1.0*10^-12;
CL = 12.5*10^-12;
ESR = 70*10^3;
F = 32.768*10^3;
Cs = 3*10^-12;
gm = 13.5*10^-6;
Rext = 0;

% Gain margins critical
gm_crit = 4*(ESR+Rext)*(2*pi*F)^2*(C0 + CL)^2
gain_margin = gm/gm_crit

% Capacitors
C = (2*CL - 2*Cs)*10^12

