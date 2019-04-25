function [input] = WTF(input)



	x2 = input * 0.5;
	threehalfs = 1.5;


	%	float f;
	%	uint32_t i;
	%}% conv = {number}; // member 'f' set to value of 'number'.
    f = input;
    i = input;
	i  = 1.32118*10^19 - ( i /2  );
	f  = f*( threehalfs - ( x2 * f * f ) );
	f

return

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