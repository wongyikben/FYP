function [] = compare(inpu,bitch)

a=zeros(55,3);



for i=1:165
   %figure
   %subplot(2,1,1);
   %plot(2:50,inpu(i,2:50));
   %subplot(2,1,2);
   %plot(1:49,abs((fft(inpu(i,2:50)))));
   q=((fft(inpu(i,2:50))));
   temp = q(4)+q(47);
   a(ceil(i/3),mod(i,3)+1)=abs(temp)/10;
   %bitch(i)
   %prompt = 'next? ';
   %x = input(prompt); 
   %close 
end

std(a)

return


for i=1:3
figure
plot(2:50,inpu(i,2:50));
hold on 
temp = conv([1 1 1 1]/4,inpu(i,2:50));
plot(4:49,temp(4:49));
order = sort(temp(4:49));

mat(i) = order(45)-order(2)
prompt = 'next? ';
x = input(prompt); 
close
end

plot(inpu(2:20,51)');
std(inpu(2:20,51)')
hold on 
plot(mat(2:20))
std(mat(2:20))
