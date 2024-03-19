genmlp %1d;
masm %1d;
masm /Dprotect %1,%1p;
link %1p+%1d+modmlp,%1.dll,,\os2\doscalls,modml.def;
