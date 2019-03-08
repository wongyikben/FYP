function result = single_nice(input,phase)

input = zero_filter(input);
order = sort(input);

pk2pk = order(length(input)-2)-order(2);

offset = median(input);
t = (1:length(input))/length(input);

min = [9999999,0];
for i=0:0.001:10
    output = norm(pk2pk*sin((2*pi*(t+i))/2+offset)-input);
    if(output<min(1))
        min(1)= output;
        min(2) = i;
    end
end



% min = [9999999,0];
% for i=offset-20:1:offset+20
%     output = norm((pk2pk*sin((2*pi*(t+phase)))/2+i)-input);
%     if(output<min(1))
%         min(1)= output;
%         min(2) = i;
%     end
% end
% offset=min(2);
% 
% min = [9999999,0];
% for i=pk2pk-100:pk2pk+100
%     output = norm((i*sin((2*pi*(t+phase)))/2+offset)-input);
%     if(output<min(1))
%         min(1)= output;
%         min(2) = i;
%     end
% end
% pk2pk = min(2);




%result = [offset,pk2pk,phase];
%y = pk2pk*sin((2*pi*14*(t+phase))/(1024))/2+offset;
result = [offset , pk2pk , phase];

output = pk2pk*sin((2*pi*(t+phase)))/2+offset;

figure
plot(1:length(input),output);
hold on
plot(1:length(input),input);