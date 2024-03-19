genml %1d;
masm /Mx %1d;
masm /Mx /Dlarge %1i;
cl -c -AL -Ox %1.c
link /NOE /NOI /DOSSEG %1+%1i+%1d+modc,%1;
