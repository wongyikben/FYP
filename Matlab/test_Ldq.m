function[y] = test_Ldq(input)
x = zeros(6,72);
for i=1:72
    x(1:6,i)=shit_solver(input(1:6,i));

end

y(1,1:72) = x(1,1:72) + x(4,1:72);
y(2,1:72) = x(2,1:72) + x(5,1:72);
y(3,1:72) = x(3,1:72) + x(6,1:72);

% single_nice(x(1,1:72))
% single_nice(x(2,1:72))
% single_nice(x(3,1:72))
% single_nice(x(4,1:72))
% single_nice(x(5,1:72))
% single_nice(x(6,1:72))

%  single_nice(y(1,1:72))
%  single_nice(y(2,1:72))
%  single_nice(y(3,1:72)

figure
plot(1:72,y);
figure
plot(1:72,x);
return 


theta = (1:72)*pi/72;
v=0.9281*0.9;
Ls = 0.001;
Lm = 0.00012;
r = 10;

Ld = 7.1139*10^-6;
Lq =  4.0799*10^-6;
freq = 150000;
freq=freq*2*pi;

La = (Ld+Lq)/2+(Ld-Lq)*cos(-2*theta)/2;
Lb = (Ld+Lq)/2+(Ld-Lq)*cos(2*(2*pi/3-theta))/2;
Lc = (Ld+Lq)/2+(Ld-Lq)*cos(-2*(-2*pi/3-theta))/2;

a=0.001;
b=0.0005;

% La = a+b*cos(-2*theta);
% Lb = a+b*cos(2*(2*pi/3-theta));
% Lc = a+b*cos(-2*(-2*pi/3-theta));


x = zeros(6,length(theta));



x(1,1:length(theta)) = (v*Lb.*1i*freq)./(r+La.*1i*freq+Lb.*1i*freq);
x(2,1:length(theta)) = (v*Lc.*1i*freq)./(r+La.*1i*freq+Lc.*1i*freq);
x(3,1:length(theta)) = (v*La.*1i*freq)./(r+Lb.*1i*freq+La.*1i*freq);
x(4,1:length(theta)) = (v*Lc.*1i*freq)./(r+Lb.*1i*freq+Lc.*1i*freq);
x(5,1:length(theta)) = (v*La.*1i*freq)./(r+Lc.*1i*freq+La.*1i*freq);
x(6,1:length(theta)) = (v*Lb.*1i*freq)./(r+Lc.*1i*freq+Lb.*1i*freq);
figure
plot(1:length(theta),abs(x)*4096/3.3)
return
input = abs(x);
 x = zeros(6,72);
 for i=1:72
     x(1:6,i)=shit_solver(input(1:6,i));

end

y(1,1:72) = x(1,1:72) + x(4,1:72);
y(2,1:72) = x(2,1:72) + x(5,1:72);
y(3,1:72) = x(3,1:72) + x(6,1:72);




plot(1:72,y);
return 



%plot(La);
%hold on 
%plot(Lb);
%plot(Lc);



