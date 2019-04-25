function [] = angle_test()
t = 0:0.01:2*pi;
a = cos(t);
b = cos(t+2*pi/3);
c = cos(t-2*pi/3);

ksin = (a-b-c)./2;
kcos = (b-c)./(2*sin(pi/3));
kang = mod(atan2(ksin,kcos)-pi/2,2*pi);

plot(kang);
kkang = zeros(1,length(t));
time = zeros(1,length(t));
%temp  = c-a;
%temp1 = b-a

%temp = a-b;
%temp1 = c-b;

%temp = a-c;
%temp1 = b-c;
for i=1:length(t)
   if temp(i)>0 && temp1(i)>0
       time(i) = 1;
   end
   % for GND A
   %kkang(i) = -atan2((-(temp(i)+temp1(i))/(cos(pi/6))),(temp(i)-temp1(i))/(sin(pi/6)))+pi/2;

   % for GND B
   kkang(i) = -atan2((temp(i)-sin(pi/6)*temp1(i))/(cos(pi/6)),temp1(i))+pi/2;

   % for GND C
   % kkang(i) = -atan2((temp(i)-sin(pi/6)*temp1(i))/(sqrt(3)*cos(pi/6)),temp1(i)/-sqrt(3))+pi/2;

end

figure
plot(kkang)
hold on 
plot(time)
plot(temp)
plot(temp1)
%xlim([258 368])




