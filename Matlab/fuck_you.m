function[error] = fuck_you(input)
result = zeros(1,72);
ideal = zeros(1,72);
error = zeros(1,72);

err = zeros(1,73);

%data = zeros(100,1024);

%for k=1:100
for i= 1:72
   result(i)=idk(input,i); 
   ideal(i)=i-1;
   error(i) = ideal(i)-result(i);
   if error(i)>73/2
    error(i) = (73-error(i));
   end
   if error(i)<-73/2
    error(i) = -(error(i)+73); 
   end
    
end


%lazy([result;ideal])
%mean(abs(error))
figure
plot(1:72,result);

hold on
plot(1:72,ideal);
figure
%xlim([363 582])

%figure
%lazy(error)
%figure
%error = conv(error,[1 1 1 1 1 1]/6);
%plot(1:length(error),error);
%xlim([454 492])
%histogram(error)
%data(k,1:1024)=error;

%plot(1:1024,ideal)
%hold on
%plot(1:1024,result)
%mean(error)
%err(k)=mean(abs(error));

%mean(error)
%end
%histogram(data)
%y=std(data);
%figure
%plot(1:length(y),y);
%figure
%k = mean(data);
%plot(1:length(k),k)
%error = mean(data);
plot(error)
mean(abs(error))
return
for i=0:1023
    err(mod(i,73)+1)=err(mod(i,73)+1)+error(i+1);
end

err = err/14;

%for i=1:73
%    if(abs(err(i))>=1)
%       err(i)=1; 
%    else
%        err(i)=0;
%    end
%end
figure
%plot((1:73)*2*pi/73-pi,err*2*pi/73);
hold on
a = 'corr1 = [';
for i=1:73
    a=[a,int2str(err(i)),','];
end
a = [a,'];'];

%disp(a)







