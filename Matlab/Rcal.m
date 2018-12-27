function [R_up,R_low] = Rcal(upper,lower)

current = (1152-upper)/10;

R_up=(upper-lower)./current;

R_low=lower./current;
