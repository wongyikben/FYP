function pk = pk2pk(fil,input)

fil = fil./sum(fil);

fil_size = size(fil);
fil_size = fil_size(2);

input_size = size(input);
input_size = input_size(2);

after = conv(fil,input);
after_size = size(after);
after_size = after_size(2);
after = after(fil_size:after_size-fil_size);
after_size = size(after);
after_size = after_size(2);
min = [];
max = [];
for i=2:after_size-1 
    if(after(i-1)>after(i)&&after(i+1)>after(i))
        min = [min after(i)];
    end
    if(after(i-1)<after(i)&&after(i+1)<after(i))
        max = [max after(i)];
    end
    
    
end

pk = median(max)-median(min);
plot((1:after_size),after);
