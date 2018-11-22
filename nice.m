function result = nice(input)
input = zero_filter(input);
order = sort(input);

s = length(input);


pk2pk = order(s-14)-order(14);

offset = median(input);
t = 1:s;

min = [9999999,0];
for i=1:75
    output = norm((pk2pk*sin((2*pi*14*(t+i))/(s))/2+offset)-input);
    if(output<min(1))
        min(1)= output;
        min(2) = i;
    end
end
phase = min(2);
min = [9999999,0];
for i=1:400
    output = norm((i*sin((2*pi*14*(t+phase))/(s))/2+offset)-input);
    if(output<min(1))
        min(1)= output;
        min(2) = i;
    end
end
pk2pk = min(2);
min = [9999999,0];
for i=offset-50:offset+50
    output = norm((pk2pk*sin((2*pi*14*(t+phase))/(s))/2+i)-input);
    if(output<min(1))
        min(1)= output;
        min(2) = i;
    end
end
offset=min(2);
%result = [offset,pk2pk,phase];
%y = pk2pk*sin((2*pi*14*(t+phase))/(s))/2+offset;
result = [offset , pk2pk , phase];
little_sin(offset,pk2pk,phase,input,1)

