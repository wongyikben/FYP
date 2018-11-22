function [result] = zero_filter(input)
s = size(input);
s = s(1);
result = input;
for j=1:s
if(result(j,1)==0)
   result(j,2) = result(j,1); 
end

for i=2:length(input)-1
    if(result(j,i)==0)
       result(j,i)= (result(j,i-1)+result(j,i+1))/2;
    end
end
end