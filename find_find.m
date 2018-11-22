function [result] = find_find(pos,input)

input_size = size(input);
input_size = input_size(1);
result = [];
for i =1:input_size
   if  pos == input(i,1)
      result = [result input(i,2)]; 
   end
end