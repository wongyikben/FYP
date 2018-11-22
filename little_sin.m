function [] = little_sin(offset,pk2pk,phase,input,input_bool) 

s = length(input);

t = 1:s;
output = pk2pk*sin((2*pi*14*(t+phase))/(s))/2+offset;


plot(1:s,output);

hold on
if(input_bool == 1)
    plot(1:s,input);
end