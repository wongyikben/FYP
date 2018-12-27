function []=new_fuck_you(input)

result = zeros(1,length(input));
for i=1:length(input)
   x = input(1:3,i); 
   offset = mean(x);
   x = x-offset;
   sine = (x(1)-x(2)-x(3))/2;
   cose = (x(2)-x(3))/(2*sin(2*pi/3));
   result(i)=atan2(sine,cose);
   result(i) = int32(result(i)*72/(2*pi));
   result(i)=result(i)+30;
   if result(i)<0
       result(i) = result(i)+72;
   end
   if result(i)>72
      result(i) = result(i)-72;
   end
end




plot(1:72,result);
hold on 
plot(1:72,1:72);
error = (0:71)-result;
max(error)
min(error)
mean(abs(error))