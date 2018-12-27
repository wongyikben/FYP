function [x]=shit_solver(input)

input = input;

offset = [input(1) input(3) input(5)]';
x= [6000 6000 6000]';
iter = 3;
delta = 1;
for i=0:iter
    d1 = (funct(x(1)+delta,x(2),x(3),0)-funct(x(1),x(2),x(3),0))/delta;
    d2 = (funct(x(1),x(2)+delta,x(3),0)-funct(x(1),x(2),x(3),0))/delta;
    d3 = (funct(x(1),x(2),x(3)+delta,0)-funct(x(1),x(2),x(3),0))/delta;
    D = [d1 d2 d3];
    x = x-inv(D)*(funct(x(1),x(2),x(3),0)-offset);
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
       break 
    end
end

funct(x(1),x(2),x(3),0);
a = x;
offset = [input(2) input(4) input(6)]';
for i=0:iter
    d1 = (funct(x(1)+delta,x(2),x(3),1)-funct(x(1),x(2),x(3),1))/delta;
    d2 = (funct(x(1),x(2)+delta,x(3),1)-funct(x(1),x(2),x(3),1))/delta;
    d3 = (funct(x(1),x(2),x(3)+delta,1)-funct(x(1),x(2),x(3),1))/delta;
    D = [d1 d2 d3];
    x = x-inv(D)*(funct(x(1),x(2),x(3),1)-offset);
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
       break 
    end
end

funct(x(1),x(2),x(3),1);

a = [a;x];
x = a;



