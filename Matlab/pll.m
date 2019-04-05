function[] = pll(input)
output = input;
output(1) = input(1);
ratio = 0.8;
for i=2:length(input)
    output(i) = output(i-1)*ratio+input(i)*(1-ratio); 
end
x=0:8;
 gg = flip(gaussmf(x,[2.4 0]))
 
 round(flip(gg)*100)
 sum(round(flip(gg)*100))

%gg = [0.986207116743916,0.945959468906765,0.882496902584596,0.800737402916808,0.706648277857716,0.606530659712633,0.506335616648101,0.411112290507187,0.324652467358350,0.249352208777296];
output1= conv(input,gg);
output1=output1/sum(gg);
figure 
hold on 
plot(input,'blue')
%plot(output,'red')
%size(output1)
plot(output1(10:502),'red')


%rms(input-output)