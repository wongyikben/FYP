function [] = haha(y)

y=y';
%back EMF
% 1 0 2 10
% 2 1 0 - 13 
% 0 1 2 -37


%high frequency injection
% 0 2 4 0
% 4 0 2 -26
% 2 4 0 +22

% 0 2 4 0
% 4 0 2 -26
% 2 4 0 28

% 0 4
% 1 -9
% 2 -19
a = y(1,1:length(y));%37
b = y(2,1:length(y));

lol = (conv([-1 0 1],b));
count=0;


%a = WTF(a);
figure
plot(a)
hold on 
plot(b)

error = a-b;

vara = 146;

for i=1:length(error)
   if error(i)>vara/2
       error(i)=error(i)-vara;
   end
   if error(i)<-vara/2
       error(i)=error(i)+vara;
   end
end
figure 
plot(error)
figure
hist(error)
A = sort(error);
min_error=A(20)
max_error=A(length(y)-10)
rms((error))

