function result = single_nice(input)

input = zero_filter(input);
order = sort(input);

pk2pk = order(73-2)-order(2);

offset = median(input);
t = 1:72;

min = [9999999,0];
for i=1:72
    output = norm((pk2pk*sin((2*pi*(t+i))/(73))/2+offset)-input);
    if(output<min(1))
        min(1)= output;
        min(2) = i;
    end
end
phase = min(2);
min = [9999999,0];
for i=1:400
    output = norm((i*sin((2*pi*(t+phase))/(73))/2+offset)-input);
    if(output<min(1))
        min(1)= output;
        min(2) = i;
    end
end
pk2pk = min(2);
min = [9999999,0];
for i=offset-5:0.01:offset+5
    output = norm((pk2pk*sin((2*pi*(t+phase))/(73))/2+i)-input);
    if(output<min(1))
        min(1)= output;
        min(2) = i;
    end
end
offset=min(2);
%result = [offset,pk2pk,phase];
%y = pk2pk*sin((2*pi*14*(t+phase))/(1024))/2+offset;
result = [offset , pk2pk , phase];
t = 1:72;
output = pk2pk*sin((2*pi*(t+phase))/(73))/2+offset;

figure
plot(1:72,output);
hold on
plot(1:72,input);