A = conteosmetroE1{:, 2};
[Up, ~] = envelope(A, 4 ,'peak');
for I = 1:length(Up)
    if Up(I) < 0
        Up(I) = 0;
    end
end
plot(Up)
conteosmetroE2{:, 2} = Up;

writetable(conteosmetroE2, 'Datos.txt');