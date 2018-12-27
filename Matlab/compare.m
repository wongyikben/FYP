function [] = compare(input)

sample_size = size(input);
input_size = sample_size(2);
sample_size = sample_size(1);

no_fil = [];
mean2 = [];
mean3 = [];
mean4 = [];
gau = [];
gau1 = [];
gau2 = [];
gau3 = [];

for i=1:sample_size
   no_fil = [no_fil pk2pk([1],input(i,1:input_size))]; 
   mean2 = [mean2 pk2pk([1 1],input(i,1:input_size))];
   mean3 = [mean3 pk2pk([1 1 1],input(i,1:input_size))];
   mean4 = [mean4 pk2pk([1 1 1 1],input(i,1:input_size))];
   gau = [gau pk2pk([1 2 1],input(i,1:input_size))];
   gau1 = [gau1 pk2pk([1 2 4 2 1],input(i,1:input_size))]; 
   gau2 = [gau2 pk2pk([1 2 4 16 4 2 1],input(i,1:input_size))]; 
   gau3 = [gau3 pk2pk([1 2 2 2 1],input(i,1:input_size))]; 
end
                     %150 kHz   
no_fil = std(no_fil) %2.3926
mean2 = std(mean2) %2.2291
mean3 = std(mean3) %2.6996
mean4 = std(mean4) %1.8727
gau = std(gau)     %2.3639
gau1 = std(gau1)   %1.8175
gau2 = std(gau2)   %1.6350
gau3 = std(gau3)   %1.9410

