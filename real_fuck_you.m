function[] = real_fuck_you(input)

result = zeros(1,1024);
ideal = zeros(1,1024);

for i=1:1024
 ideal(i)=mod(i,73)+1;
 if(input(i)>73/2) input(i)= input(i)-73; end
 if(input(i)<-73/2) input(i)=input(i)+73; end
 result(i) = input(i)+ideal(i);   
end
mean(abs(input))
plot(1:1024,ideal)
hold on 
plot((1:1024),result)