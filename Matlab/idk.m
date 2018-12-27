function[result] = idk(input,pos)
corr = [-1,1,1,1,2,2,2,2,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,-1,-1,-1,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-1,-1,-1,0,0,1,1,1,2,2,2,3,3,3,3,2,2,1,1,0,-1,-1,-2,-3,-3,-3,-3,-3,-3,-2,-1,-1,0,1,1,0,];
%corr1 = [0,0,0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,0,1,1,0];
x = zeros(1,6);

s=size(input);
s=s(1)/6;


for i=0:5
   % x(i+1) = input(i*s+uint8(rand(1)*(s-1)+1),pos);
    x(i+1) = input((i+1),pos);
end

offset = mean(x);

% x(1) = x(1) - 458;
% x(1) = x(1)/124;
% x(2) = x(2) - 468;
% x(2) = x(2)/131;
% x(3) = x(3) - 479;
% x(3) = x(3)/135;
% x(4) = x(4) - 467;
% x(4) = x(4)/127;
% x(5) = x(5) - 458;
% x(5) = x(5)/127;
% x(6) = x(6) - 477;
% x(6) = x(6)/130;
for i=1:6
    x(i)=x(i)-offset;
end


x = [x(1:2) x(4) x(3) x(5:6)];


ksin = ((x(1)-x(4))+(x(2)+x(6))-(x(3)+x(5)))/4;
kcos = ((x(2)+x(3))-(x(5)+x(6)))/(4*sin(pi/3));
k = sqrt(ksin^2+kcos^2);

%x = x-offset;

result = given_x(x);



x = [x(2:6) x(1)];


result1 = given_x(x);
result1 = result1-pi/3;

if(result1<=-pi)
    result1 = result1+2*pi;
end
%result2



%result
x = [x(2:6) x(1)];
result2 = given_x(x);
result2 = result2-2*pi/3;
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

result = median([result;result1;result2]);
x = [x(2:6) x(1)];


% x = x/k;
% for i=-pi:0.01:pi
%    
%     y=[sin(i) sin(i+1*pi/6) sin(i+2*pi/6) sin(i+3*pi/6) sin(i+4*pi/6) sin(i+5*pi/6) ];
%     a = norm(y-x);
%     if a<mini
%        result = i ;
%        mini = a;
%     end
% 
% end
%mini;
result;
%plot(result);
%return

% if(max(result)-min(result)>pi)
%    sort_re = sort(result);
%    if(sort_re>0)
% %      result = max(result); 
%    else
% %      result = min(result); 
%    end
%     
% end


result = int32((result/(2*pi))*73);
result = result ;
if result < 0
   result = int32(73+result); 
end

if result > 73
   result = int32(result-73); 
end
return 
result = result + corr(result+1);
if result < 0
   result = int32(73+result); 
end


