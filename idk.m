function[result] = idk(input,pos)
corr = [0,1,1,2,2,2,2,2,2,2,1,1,1,1,1,0,1,1,0,1,1,0,1,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,0,0,1,1,1,2,2,2,3,3,3,3,3,2,2,1,0,0,-1,-2,-2,-2,-2,-2,-2,-2,-2,-1,-1,0,1,1,0,];
%corr1 = [0,0,0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,0,1,1,0];
x = zeros(1,6);

s=size(input);
s=s(1)/6;


for i=0:5
    x(i+1) = input(i*s+uint8(rand(1)*(s-1)+1),pos);
end



x = [x(1:2) x(4) x(3) x(5:6)];

offset = mean(x);
x = x-offset;

ksin = ((x(1)-x(4))+(x(2)+x(6))-(x(3)+x(5)))/4;
kcos = ((x(2)+x(3))-(x(5)+x(6)))/(4*sin(pi/3));
result = -atan2(ksin,kcos);


%k = sqrt((ksin^2)+(kcos^2));
%sine = ksin/k;
%cose = kcos/k;
%arcsin = asin(sine);
%arccos = acos(cose);
%if sine>0
%    if cose>0
        % I
%    else
        %II
%        arcsin = pi-arcsin;
%    end
%else
%   if cose<0
        %III
%        arcsin = -arcsin+pi;
%        arccos = 2*pi-arccos;
%   else
        %IV
%        arcsin = arcsin+2*pi;
%        arccos = 2*pi-arccos;
%    end   
%end

%result = (arcsin+arccos)/2;
%result = (atan2(ksin,kcos));


x = [x(2:6) x(1)];
ksin = ((x(1)-x(4))+(x(2)+x(6))-(x(3)+x(5)))/4;
kcos = ((x(2)+x(3))-(x(5)+x(6)))/(4*sin(pi/3));
result1 = atan2(ksin,kcos)-pi/3;
result1 = -result1;
if(result1<=-pi)
    result1 = result1+2*pi;
end
%result2



%result
x = [x(2:6) x(1)];
ksin = ((x(1)-x(4))+(x(2)+x(6))-(x(3)+x(5)))/4;
kcos = ((x(2)+x(3))-(x(5)+x(6)))/(4*sin(pi/3));
result2 = atan2(ksin,kcos)-2*pi/3;
result2 = -result2;
if(result2<=-pi)
    result2 = result2+2*pi;
end
%result2


%result = 0.6*result + 0.2*result1 +0.2*result2;
if(result<-pi/2)%||result>pi/2)
  %  result = result2;
else
  %  result = (result*0.9+result2*0.1);
end

%if(result>1.4632)
%   result = result + 0.6685*sin((result-1.4632)/2.4099);    
%end

result = median([-result;-result1;-result2]);

result = int32((result/(2*pi))*73);
result = result -3;
if result < 0
   result = int32(73+result); 
end
result = result + corr(result+1);
if result < 0
   result = int32(73+result); 
end


