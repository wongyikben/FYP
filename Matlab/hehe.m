function [] = hehe(L)

L = L';

a = [sin(0),cos(0)];
b = [sin(2*pi/3),cos(2*pi/3)];
c = [sin(-2*pi/3),cos(-2*pi/3)];

rec_a = 0;
rec_b = 0;
rec_c = 0;

count = 1;
for i=0:0.01:2*pi
    temp1 = abs([cos(i),-sin(i);sin(i),cos(i)]*[0 L(1)]');
    temp2 = abs([cos(i),-sin(i);sin(i),cos(i)]*[L(2) 0]');
    
    
    
    rec_a(count) = (abs(dot(temp1,a))+abs(dot(temp2,a)));
    rec_b(count) = (dot(temp1,b)+dot(temp2,b));
    rec_c(count) = (dot(temp1,c)+dot(temp2,c));
    count = count + 1;
end
plot(rec_a)
%hold on 
%plot(rec_b)
%plot(rec_c)





