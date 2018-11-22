function[] = pll(input)
fff = zeros(1,1024);
hhh = zeros(4,1024);
for pos = 1:1024
x = zeros(1,6);
for i=0:5
    x(i+1) = input(i*5+uint8(rand(1)*4+1),pos);
end
i = pos;
offset = mean(x);
x = x-offset;

ksin = ((x(1)-x(4))+(x(2)+x(6))-(x(3)+x(5)))/4;
kcos = ((x(2)+x(3))-(x(5)+x(6)))/(4*sin(pi/3));
%theta = atan2(ksin,kcos);
theta = (mod(i,73)/73)*2*pi;
    
DQ_M = (2/3)*[cos(theta) cos(theta-2*pi/3) cos(theta+2*pi/3);-sin(theta) -sin(theta-2*pi/3) -sin(theta+2*pi/3)];
a = DQ_M*[x(1)+x(2);x(3)+x(4);x(5)+x(6)] ;
%b =DQ_M*[x(2);x(4);x(6)] ;

hhh(1,i)=a(1)+a(2);
%hhh(2,i)=a(2);
%hhh(3,i)=b(1);
%hhh(4,i)=b(2);

end
plot(1:1024,hhh(1,1:1024))