function []=new_fuck_you(input)

result = zeros(1,length(input));
for i=1:length(input)
   x = input(1:3,i); 
   offset = mean(x);
   x = x-offset;
   sine = (x(1)-x(2)-x(3))/2;
   cose = (x(2)-x(3))/(2*sin(2*pi/3));
   result(i)=(-atan2(sine,cose)-pi/2)-pi/2;
   angle = atan2(sine,cose);
%   result(i) = mod(result(i)+pi,2*pi)-pi;
    result(i) = int32(result(i)*146/(2*pi));
%    result(i)=result(i)+93;
     if result(i)<0
         result(i) = result(i)+146;
     end
%    if result(i)>72
%       result(i) = result(i)-146;
%    end
   x = input(1:3,i); 
   Va=x(1);
   Vb=x(2);
   Vc=x(3);
   V=(2/3)*(Va+Vb*exp(j*2*pi/3)+Vc*exp(-j*2*pi/3)); % voltage vector
   Valpha(i)=real(V);
   Vbeta(i)=imag(V);
   
   
Vd(i)=Valpha(i)*cos(-(angle-pi/2))+Vbeta(i)*sin(-(angle-pi/2));
Vq(i)=-Valpha(i)*sin(-(angle-pi/2))+Vbeta(i)*cos(-(angle-pi/2));

   
end
% plot(Valpha)
% hold on 
% plot(Vbeta)
% figure
% mean(Vd)
% plot(Vd)
% hold on 
% plot(Vq)




plot(result);
%hold on 
%plot(1:72,1:72);
%error = (0:71)-result;




return
% for i=1:72
%    if error(i)<-72/2
%        error(i) = error(i)+72;
%    end
%    if error(i)> 72/2
%        error(i) = error(i)-72;
%    end   
% 
% end
% 
% max(error)
% min(error)
% rms(error)