function[output] = funct(a,b,c,flag)
v = 1152;


R = 12412;
Rm = 5*4096/3.3;
output=[0 0 0]';

if flag == 0
 output(1)=round((v^2)*(b^2)/(((R)^2+(a+b)^2)));
 output(2)=round((v^2)*(c^2)/(((R)^2+(b+c)^2)));
 output(3)=round((v^2)*(a^2)/(((R)^2+(c+a)^2)));
    
 end
 return

if flag == 0
output(1) = v*b*128/sqrt((R+2*Rm)^2+(a+b)^2);
output(2) = v*c*128/sqrt((R+2*Rm)^2+(c+b)^2);
output(3) = v*a*128/sqrt((R+2*Rm)^2+(a+c)^2);
else 
output(1) = v*c*128/sqrt(R^2+(a+c)^2);
output(2) = v*a*128/sqrt(R^2+(a+b)^2);
output(3) = v*b*128/sqrt(R^2+(b+c)^2);    
end

output=round(output);