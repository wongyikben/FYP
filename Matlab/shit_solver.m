function [x]=shit_solver(input,stfu)

input = input;

offset = [input(1) input(3) input(5)]'*128;
x= stfu';
iter = 6;
delta = 1;
for i=1:iter
    

    d1 = (funct(x(1)+delta,x(2),x(3),0)-funct(x(1),x(2),x(3),0))/delta;
    d2 = (funct(x(1),x(2)+delta,x(3),0)-funct(x(1),x(2),x(3),0))/delta;
    d3 = (funct(x(1),x(2),x(3)+delta,0)-funct(x(1),x(2),x(3),0))/delta;
    D = ([d1 d2 d3]);
    
    invD = round(inv(D)*128);
    
    x = x-round((invD)*(funct(x(1),x(2),x(3),0)-offset)/128);
%    hi(i) = norm(funct(x(1),x(2),x(3),0)-offset);
   % x = x-floor((inv(D)*128)*floor(funct(x(1),x(2),x(3),0)-offset));
%     if(x(1)<0)
%        x(1)=1; 
%     end
%     if(x(2)<0)
%        x(2)=1; 
%     end
%     if(x(3)<0)
%        x(3)=1; 
%     end

    
end
x(4)= norm(funct(x(1),x(2),x(3),0)-offset);

%plot(hi)
%(norm(funct(x(1),x(2),x(3),0)-offset))

return 
funct(x(1),x(2),x(3),0);
a = x;
x=[1 1 1]';
offset = [input(2) input(4) input(6)]';
for i=1:iter
    d1 = (funct(x(1)+delta,x(2),x(3),0)-funct(x(1),x(2),x(3),0))/delta;
    d2 = (funct(x(1),x(2)+delta,x(3),0)-funct(x(1),x(2),x(3),0))/delta;
    d3 = (funct(x(1),x(2),x(3)+delta,0)-funct(x(1),x(2),x(3),0))/delta;
    D = floor([d1 d2 d3]);
    %x = x-0.01*(inv(D)*128)*(funct(x(1),x(2),x(3),0)-offset);
    x = x-floor(0.01*(inv(D)*128)*floor(funct(x(1),x(2),x(3),0)-offset));
    if(x(1)<0)
       x(1)=1; 
    end
    if(x(2)<0)
       x(2)=1; 
    end
    if(x(3)<0)
       x(3)=1; 
    end
    if(norm(funct(x(1),x(2),x(3),0)-offset)<delta)
     %  break 
    end
end

funct(x(1),x(2),x(3),1);

a = [a;x];
x = a;



