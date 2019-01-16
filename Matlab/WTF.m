function [input] = WTF(input)

for i=1:length(input)
   if(input(i)<1)
       input(i)=input(i)+71;
   end
   if(input(i)>72)
       input(i)=input(i)-71;
   end

end

for i=1:length(input)
   if(input(i)<1)
       input(i)=input(i)+71;
   end
   if(input(i)>72)
       input(i)=input(i)-71;
   end

end