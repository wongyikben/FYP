function [] = lazy(input)

y = size(input);
y = y(1);

array = zeros(1,73);
output = zeros(1,73);


for i=0:1023
    for j=1:y
    if input(j,i+1)>30
        input(j,i+1) = input(j,i+1)-73;
    end
   array(j+idivide(int32(i+1),73, 'floor')*8,mod(i,73)+1) =  input(j,i+1);
    
    end
end
plot(1:73,array);
figure
s = size(array);
s = s(1);
med = zeros(1,73);
for i =1 :73
    temp = [];
   for j=1:s
      if array(j,i)~=0
          temp = [temp;array(j,i)];
      end
   end
   med(i)=mean(temp);
end
plot(0:2*pi/73:2*pi-2*pi/73,med*2*pi/73)
%single_nice(med)
%figure
%plot(1:73,med);
return 
figure
sd = std(array);


med = median(sd);
%sd = conv([1 2 4 8 4 2 1]/22,sd);
plot(1:length(sd),sd);
figure

for i=1:73
   if sd(i)>med
      output(i) = 1; 
   end
end
plot(1:length(output),output);
