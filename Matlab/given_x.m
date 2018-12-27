function [theta] = given_x(x)

%ksin = ((x(1)-x(4))+(x(2)+x(6))-(x(3)+x(5)))/4;
%kcos = ((x(2)+x(3))-(x(5)+x(6)))/(4*sin(pi/3));
%result = atan2(ksin,kcos);

%[l,MAX] = sort(x);
%MAX(1);


%if(mod(MAX(1),3)==0)

%    y = [x(1)-x(4) x(2)-x(5) ]'/2;
%    M = [1 0;0.5 sin(pi/3)];
%    gg = M\y;
    
%    ksin = gg(1);
%    kcos = gg(2);
    
%result = atan2(ksin,kcos);
%end

%if(mod(MAX(1),3)==1)
%    y = [x(3)-x(6) x(2)-x(5) ]'/2;
%    M = [-0.5 sin(pi/3);0.5 sin(pi/3)];
%    gg = M\y;
    
%    ksin = gg(1);
%    kcos = gg(2);
%result = atan2(ksin,kcos);
%end

%if(mod(MAX(1),3)==2)
%    y = [x(1)-x(4) x(3)-x(6)]'/2;
%    M = [1 0;-0.5 sin(pi/3)];
%    gg = M\y;
    
%    ksin = gg(1);
%    kcos = gg(2);
%result = atan2(ksin,kcos);
%end




pk2pk = max(x) - min(x);
offset = 0;
Min = 999999;
phase = 0;
t=(1:6)*pi/3;
for i = (-50:50)*pi/50
   sine =  pk2pk*sin(t+i+pi*(-16)/50)+offset;
   temp = norm(sine-x);
   if(temp<Min)
    Min =temp;
    phase = i;
   end
end





ksin = ((x(1)-x(4))+(x(2)+x(6))-(x(3)+x(5)))/4;
kcos = ((x(2)+x(3))-(x(5)+x(6)))/(4*sin(pi/3));
angle = atan2(ksin,kcos);

result = (angle + phase)/2;

if(abs(phase-angle)>=(pi-0.0001))
   if(phase<angle)
      phase = phase + 2*pi; 
   else
      angle = angle + 2*pi;
   end
   
   result = (angle + phase)/2;
   if(result>pi)
      result = result - 2*pi; 
   end
   if(result<-pi)
      result = result + 2*pi;
   end 
end




theta = result;