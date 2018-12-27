function[output] = funct(a,b,c,flag)
v = 1152;


R = 12412;
output=[0 0 0]';

if flag == 0
output(1) = v*b/sqrt(R^2+(a+b)^2);
output(2) = v*c/sqrt(R^2+(c+b)^2);
output(3) = v*a/sqrt(R^2+(a+c)^2);
else 
output(1) = v*c/sqrt(R^2+(a+c)^2);
output(2) = v*a/sqrt(R^2+(a+b)^2);
output(3) = v*b/sqrt(R^2+(b+c)^2);    
end