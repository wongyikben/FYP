function [] = haha(y)

y=y';

a = y(1,1:length(y))+1;
b = y(2,1:length(y));

lol = (conv([-1 0 1],b));
count=0;
for i=1:length(lol)
    if lol(i)<-56
       count = count+1; 
    end 
end
freq = count/4

a = WTF(a);

plot(a)
hold on 
plot(b)

error = a-b;

for i=1:length(error)
   if error(i)>36
       error(i)=error(i)-73;
   end
   if error(i)<-36
       error(i)=error(i)+73;
   end
end
figure 
plot(error)
figure
hist(error)
A = sort(error);
min_error=A(20)
max_error=A(1980)
rms((error))

