genmlp %1d;
masm /Mx %1d;
masm /Mx /Dprotect /Dsmall %1i,%1h;
cl -Dprotect -c -AS -Ox -G2 -Zp -Fo%1p %1.c
link /NOE /NOI /DOSSEG %1h+%1p+%1d+modcp,%1.dll,,doscalls,modc.def;
