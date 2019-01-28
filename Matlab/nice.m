function [] = nice(input)
% t=(1:100);
% y = sin(2*pi*(0.05)*t);
% thre=0.1;
% for i = 1:100
%    if y(i) <thre
% 
%        y(i)=thre;
%    end
% end
% 
% x = conv([1 -1],y);
% x = x(3:100);
% (max(x)-min(x))/(2*pi*0.05)
% plot(x)
% figure
% plot(y)



leng = length(input);

x=[max(input)-mean(input) 270 mean(input)  2*pi]';
iter = 1000;
delta = 0.001;
t = (1:leng);

for i=1:iter
    temp = x(1)*sin(x(4)*t+x(2))+x(3);
    
    d1 = ((x(1)+delta)*sin(x(4)*t+x(2))+x(3)-temp)/delta;
    d2 = (x(1)*sin(x(4)*t+x(2)+delta)+x(3)-temp)/delta;
    d3 = (x(1)*sin(x(4)*t+x(2))+x(3)+delta-temp)/delta;
    d4 = (x(1)*sin((x(4)+delta)*t+x(2))+x(3)-temp)/delta;

    D = ([d1' d2' d3' d4']);
    
    
    invD = inv(D'*D)*D';
     x = x-invD*(temp-input)';   
     
     
 
end
pk2pk = x(1)
%x(2) = mod(x(2),pi);
phase_shift = x(2)
offset = x(3)
% x(4) = mod(x(4),2*pi);
% if x(4)>pi
%     x(4)=2*pi-x(4);
%     
% end
% x(1)=x(1)+20;
%x(1)=-x(1);
frequency = x(4)
plot(input)
hold on 
plot(x(1)*sin(x(4)*t+x(2))+x(3))