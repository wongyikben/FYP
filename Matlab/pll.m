function[] = pll(input,noise)
output = input+noise(1:length(input));
size = 5;
for i=size:length(input)
    window = output(i-(size-1):i);
    var = std(window);
    %output(i) = output(i) - ((3.0369)/var)*(output(i)-mean(window));
    output(i) = output(i) - ((std(noise(i-(size-1):i)/var)))*(output(i)-mean(window));
end

figure 
hold on 
plot(input,'blue')
plot(input+noise(1:length(input)),'black')
plot(output,'red')

rms(input-output)