genml %1d;
masm /Mx %1d;
masm /Mx /Dmedium %1i;
cl -c -AM -Ox %1.c
link /NOE /NOI /DOSSEG %1+%1i+%1d+modc,%1;
