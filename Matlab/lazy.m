function[] = lazy()

R = 10;
Vin = (1152/4096)*3.3;
Rm = 0:0.1:20;
La = 1:10:1000;
Lb = 1:10:1000;
count  = 1;
for a = 1:length(Rm)
    for b = 1:length(La)
        for c = 1:length(Lb)
            result(count) = abs(abs(Vin/(1+(R+Rm(a)+La(b)*1i)/(Lb(c)*1i+Rm(a))))-(Vin/sqrt(((2*Rm(a)+R)^2+(La(b)+Lb(c))^2)/(Rm(a)^2+Lb(c)^2))));
            count = count + 1;
            result2(count) = sqrt(((2*Rm(a)+R)^2+(La(b)+Lb(c))^2)/(Rm(a)^2+Lb(c)^2));
        end
    end
end
max(result)

plot(result2)