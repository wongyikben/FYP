function []= DispSine()
size = 64;
X = ['const u16 sin1[7][',int2str(size),']= {'];
for j = 1:7
t = (0:1:size-1);
sine = sin(t*2*pi/size);
sine = sine*128+128;
sine = uint16(sine*(j+0.5)+700);
for i= 1:size
    if i == size
        X = [X,int2str(sine(i)),'},'];
    elseif i==1
        X = [X,'{',int2str(sine(i)),','];
    else
        X = [X,int2str(sine(i)),','];
    end
end
max(sine)-min(sine)
plot(sine)
hold on 
end
X = [X,'};'];
%disp(X);
return
size = 32;
X = ['const u16 sin2[7][',int2str(size),']= {'];
for j = 1:7
t = (0:1:size-1);
sine = sin(t*2*pi/size);
sine = sine*128+128;
sine = uint16(sine*(j+0.5)+150);
for i= 1:size
    if i == size
        X = [X,int2str(sine(i)),'},'];
    elseif i==1
        X = [X,'{',int2str(sine(i)),','];
    else
        X = [X,int2str(sine(i)),','];
    end
end
end
X = [X,'};'];
disp(X);


size = 16;
X = ['const u16 sin3[7][',int2str(size),']= {'];
for j = 1:7
t = (0:1:size-1);
sine = sin(t*2*pi/size);
sine = sine*128+128;
sine = uint16(sine*(j+0.5)+150);
for i= 1:size
    if i == size
        X = [X,int2str(sine(i)),'},'];
    elseif i==1
        X = [X,'{',int2str(sine(i)),','];
    else
        X = [X,int2str(sine(i)),','];
    end
end
end
X = [X,'};'];
disp(X);