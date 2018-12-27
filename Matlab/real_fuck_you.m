function[] = real_fuck_you(input)

result = zeros(1,1024);
ideal = zeros(1,1024);

err = zeros(1,73);

for i=1:1024
 ideal(i)=mod(i,73)+1;
 if(input(i)>73/2) input(i)= input(i)-73; end
 if(input(i)<-73/2) input(i)=input(i)+73; end
 result(i) = input(i)+ideal(i);  
 err(mod(i-1,73)+1)=err(mod(i-1,73)+1)+input(i);
end


err = err/14;
mean(abs(input))
plot(1:1024,ideal)
hold on 
plot((1:1024),result)

figure
plot(1:73,err)